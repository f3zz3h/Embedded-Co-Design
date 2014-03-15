/* Standard includes. */
#include "global.h"

volatile unsigned int *mem_addr  = NULL;
unsigned int mem_phys  = 0x72A00000; /*base for opencore reg */
pthread_mutex_t keypad_mutex = PTHREAD_MUTEX_INITIALIZER;

/* ********************************************************
 * Program entry point, opens file pointed to memory address for
 * TS-7300 to access wishbone (FPGA side) of the platform.
 * Contains primary infinite loop for program and switch statement
 * based on key press to send corresponding values to drive the arm.
 * ******************************************************* */
int main( void )
{
	float xyz_pos[3];
	int page_size = getpagesize();
	int i, grabber_val = 0;
	int temp_key = 0;
	pthread_t keypad_thread;

	/* Open a page at the FPGA base address */
	fd = open("/dev/mem", O_RDWR | O_SYNC);

	if (fd < 0)
	{
		printf("Open /dev/mem failed - %s %d\n",
			strerror(errno),errno);
		return -1;
	}

	mem_addr = (unsigned int *)mmap(
		0,
		page_size,
		PROT_READ|PROT_WRITE,
		MAP_SHARED,
		fd,
		mem_phys);

	if (mem_addr == (unsigned int *)MAP_FAILED)
	{
		printf("Error: mmap failed\n");
		close(fd);
		return -1;
	}

	//Intialize the emu array
	//emu_intialize(xyz_pos);

	/* Read the keypad and switch over its return value */
	pthread_create(&keypad_thread, NULL, Read_Keypad, NULL);

	int sVal = SERVO_MID-3, eVal = SERVO_MID, bVal = SERVO_MID, gVal = SERVO_MID;

	while(1)
	{

		/* Run the inverse kinematics */
		Write_PWM(ELBOW, eVal);
		Write_PWM(SHOULDER, sVal );
		Write_PWM(BASE, bVal);
		Write_PWM(GRIPPER, gVal);
		//emu_ikrun(xyz_pos,grabber_val);

		switch (temp_key)
		{
			//Row 1 [ 1 - 4 ]
			case 1 : sVal = SERVO_MID-3;
					 eVal = SERVO_MID;
					 bVal = SERVO_MID;
					 gVal = SERVO_MID;
					//emu_intialize(xyz_pos);
					break;
			case 2 : if (sVal < SERVO_MAX) sVal++;
					if (eVal > SERVO_MIIN) eVal--;
					//xyz_pos[Y] = xyz_pos[Y] - 5;

					break;
			case 3 : //GRABBER open or close
					if ( gVal == SERVO_MIN)
					{
						gVal = SERVO_MAX;
					}
					else
					{
						gVal = SERVO_MIN;
					}
					break;
			case 4 : xyz_pos[Z] = xyz_pos[Z] + 5;

					break;
			//Row 2 [ 5 - 8 ]
			case 5 : if (bVal < SERVO_MAX) bVal++;

					//move left
					//xyz_pos[X] = xyz_pos[X] + 5;

					break;
			case 6 : //RECORD HERE!?
					break;
			case 7 : //Move right.
				 	 if (bVal > SERVO_MIN) bVal--;
					//xyz_pos[X] = xyz_pos[X] + 5;
					break;
			case 8 : //MOVE DOWN!?
				xyz_pos[Z] = xyz_pos[Z] - 5;
					break;
			//Row 3 [ 9 - 12 ]
			case 9 : //PLAYBACK
					break;
			case 10 : if (sVal > SERVO_MIN) sVal--;
					if (eVal < SERVO_MAX) eVal++;
				//xyz_pos[Y] = xyz_pos[Y] + 5;

					break;
			case 11 : //MASTER STOP!!!
					break;
			case 12 : //MENU STUFF
					break;
			//Row 4 [ 13 - 16]
			case 13 :
					break;
			case 14 : 
					break;
			case 15 :
					break;
			case 16 ://MENU STUFF (FOR NOW JUST PRINT)

					for (i = 0; i < 3 ; i++)
					{
						printf("XYZ[%d]: %f\n\n", i, xyz_pos[i]);
					}
					break;
			default : printf("Key val not defined = %x\n", temp_key);
					
		}

		if (temp_key != 0)
		{
			printf("KeyVal:%d\n", temp_key);
		}
		temp_key = 0;

		if (key_val != 0)
		{
			pthread_mutex_lock(&keypad_mutex);
			temp_key = key_val;
			key_val = 0;
			pthread_mutex_unlock(&keypad_mutex);
		}
	}

}
