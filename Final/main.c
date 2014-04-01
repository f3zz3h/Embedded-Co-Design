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
	int sVals[4]; //Servo int write values
	int page_size = getpagesize();
	int i;
	int temp_key = 0;
	pthread_t keypad_thread;
	pthread_t lcd_thread;

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

	#ifdef IKRUN
	/* Intialize the emu array */
	emu_intialize(xyz_pos, sVals);
	#endif
	#ifndef IKRUN
	for (i = 0; i < 4; i++)
	{
		//printf("sVals - %d xyz_pos - %f\n", sVals[i], xyz_pos[i]);
		sVals[i] = SERVO_MIN;
	}
	#endif

	/* Initialize the LCD screen */
	//lcdinit();

	/* Read the keypad and switch over its return value */
	pthread_create(&keypad_thread, NULL, Read_Keypad, NULL);

	while(1)
	{
		#ifdef IKRUN
		/* Read the keypad and switch over its return value */
		//pthread_create(&lcd_thread, NULL, writechars, NULL);
		emu_ikrun(xyz_pos,sVals);
		#endif

		//printf("sVals - %d xyz_pos - %f\n", sVals[i], xyz_pos[i]);
		Write_PWM(sVals);


		#ifdef IKRUN
		switch (temp_key)
		{
			//Row 1 [ 1 - 4 ]
			//Reset Posotion
			case 1 : emu_intialize(xyz_pos, sVals);
			//lcd_message(RESETMSG) ;
					break;
			//Forward
			case 2 : ik_update_xyz(xyz_pos,Z,INCREMENT);
			//lcd_message(FWDMSG);
					break;
			//Grab
			case 3 :

			//Up
			case 4 : ik_update_xyz(xyz_pos,X,INCREMENT);
			//lcd_message(UPMSG);

					break;
			//Row 2 [ 5 - 8 ]
			//Left
			case 5 : ik_update_xyz(xyz_pos,Y,DECREMENT);
					break;
			//Start recording
			case 6 :
					break;
			//Right
			case 7 : ik_update_xyz(xyz_pos,Y,INCREMENT);
					break;
			//Down
			case 8 : ik_update_xyz(xyz_pos,X,DECREMENT);
					break;
			//Row 3 [ 9 - 12 ]
			//Playback
			case 9 :
					break;
			//Back
			case 10 : ik_update_xyz(xyz_pos,Z,DECREMENT);

					break;
			//Master Stop
			case 11 :
					printf ("You've chosen to kill me!");
					////WILL SEND OFF TO SERVOS AND EXIT
					break;
			//Enter
			case 12 :
					break;
			//Row 4 [ 13 - 16]
			//Undefined
			case 13 :
					break;
			//Undefined
			case 14 : increment_gripper(sVals);
					break;
			//Undefined
			case 15 : decrement_gripper(sVals);
					break;
			//Return
			case 16 ://MENU STUFF (FOR NOW JUST PRINT)

					for (i = 0; i < 4 ; i++)
					{
						//printf("XYZ[%d]: %f\n\n", i, xyz_pos[i]);
						printf("Servo:%d: %d\n\n", i, Read_PWM(i));
					}
					sleep(1);
					break;
			default : break; printf("Key val not defined = %x\n", temp_key);
					
		}
		#endif
		#ifndef IKRUN
		 switch (temp_key)
				{
					//Row 1 [ 1 - 4 ]
					case 1 : sVals[SHOULDER] = SERVO_MID-3;
							 sVals[ELBOW] = SERVO_MID;
							 sVals[BASE] = SERVO_MID;
							 sVals[GRIPPER] = SERVO_MAX;
							break;
					case 2 : if (sVals[SHOULDER] < SERVO_MAX) sVals[SHOULDER]++;
							if (sVals[ELBOW] > SERVO_MIN) sVals[ELBOW]--;
							break;
					case 3 : //invert_gripper(sVals);
							break;
					case 4 : xyz_pos[Z] = xyz_pos[Z] + 5;

							break;
					//Row 2 [ 5 - 8 ]
					case 5 : if (sVals[BASE] < SERVO_MAX) sVals[BASE]++;
							break;
					case 6 : //RECORD HERE!?
							break;
					case 7 : //Move right.
							  if (sVals[BASE] > SERVO_MIN) sVals[BASE]--;
							break;
					case 8 : //MOVE DOWN!?

							break;
					//Row 3 [ 9 - 12 ]
					case 9 : //PLAYBACK
							break;
					case 10 : if (sVals[SHOULDER]  > SERVO_MIN) sVals[SHOULDER] --;
							if (sVals[ELBOW] < SERVO_MAX) sVals[ELBOW]++;
							break;
					case 11 : //MASTER STOP!!!
							break;
					case 12 : //MENU STUFF
							break;
					//Row 4 [ 13 - 16]
					case 13 :
							break;
					//Undefined
					case 14 : increment_gripper(sVals);
							break;
					//Undefined
					case 15 : decrement_gripper(sVals);
							break;
					case 16 ://MENU STUFF (FOR NOW JUST PRINT)
							for (i = 0; i < 4; i++)
							{
								//printf("sVals = %d xyz_pos - %f\n", sVals[i], xyz_pos[i]);
								printf("Servo:%d: %d\n\n", i,sVals[i]);
							}
							for (i = 0; i < 3 ; i++)
							{
								printf("Read PWM:%d: %d\n\n", i, Read_PWM(i));
							}
							break;
					default : break; printf("Key val not defined = %x\n", temp_key);

				}
#endif

		//if(lcdMsg)
		//{
		//	free(lcdMsg);
		//}

		/* todo: Functionalize this as it is effectively our keypad debounce */
		if (temp_key != 0)
		{
			printf("KeyVal:%d\n", temp_key);
		}
		temp_key = 0;

		if (key_val != 0)
		{
			/*TODO: IF KEYPAD VAL BEING UPDATE THIS MAY NOT WORK.. NEED TO CHECK THIS OUT */
			pthread_mutex_lock(&keypad_mutex);
			temp_key = key_val;
			key_val = 0;
			pthread_mutex_unlock(&keypad_mutex);
		}


	}

}
