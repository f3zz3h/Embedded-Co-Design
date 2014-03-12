/* Standard includes. */
#include "global.h"

volatile unsigned int *mem_addr  = NULL;
unsigned int mem_phys  = 0x72A00000; /*base for opencore reg */

/* ********************************************************
 * Program entry point, opens file pointed to memory address for
 * TS-7300 to access wishbone (FPGA side) of the platform.
 * Contains primary infinite loop for program and switch statement
 * based on key press to send corresponding values to drive the arm.
 * ******************************************************* */
int main( void )
{
	float xyz_pos[3];
	float current_xyz_pos[3];
	int key_val, grabber_val = 0;
	int page_size = getpagesize();

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

	emu_intialize(xyz_pos);
	Write_PWM(ELBOW, SERVO_MID);
	emu_ikrun(xyz_pos,current_xyz_pos,grabber_val);

	/////////////////////////////////
	while(1)
	{
		/* Read the keypad and switch over its return value */
		key_val = Read_Keypad();
		
		switch (key_val)
		{
			//Row 1 [ 1 - 4 ]
			case 1 : Write_PWM(SHOULDER, SERVO_MAX);
					break;
			case 2 : current_xyz_pos[Z] = current_xyz_pos[Z] - 3;
					emu_ikrun(xyz_pos,current_xyz_pos,grabber_val);
					break;
			case 3 : Write_PWM(SHOULDER, SERVO_MIN);
					break;
			case 4 : 
					break;
			//Row 2 [ 5 - 8 ]
			case 5 : current_xyz_pos[Y] = current_xyz_pos[Y] + 4;
				emu_ikrun(xyz_pos,current_xyz_pos,grabber_val);
					break;
			case 6 : current_xyz_pos[X] = 29;   //X position relative to centre of 				     // robot base
					current_xyz_pos[Y] = 0;    //Y position relative to centre of 				     //robot base
					current_xyz_pos[Z] = 15;   //Z
					Write_PWM(ELBOW, SERVO_MID);
					emu_ikrun(xyz_pos,current_xyz_pos,grabber_val);
					break;
			case 7 : current_xyz_pos[Y] = current_xyz_pos[Y] - 4;
				emu_ikrun(xyz_pos,current_xyz_pos,grabber_val);
					break;
			case 8 : 
					break;
			//Row 3 [ 9 - 12 ]
			case 9 : 
					break;
			case 10 : current_xyz_pos[Z] = current_xyz_pos[Z] + 3;
				emu_ikrun(xyz_pos,current_xyz_pos,grabber_val);
					break;
			case 11 : 
					break;
			case 12 : 
					break;
			//Row 4 [ 13 - 16]
			case 13 : grabber_val = 0; //grabber_val angle
					emu_ikrun(xyz_pos,current_xyz_pos,grabber_val);
					break;
			case 14 : 
					break;
			case 15 : grabber_val = 1;
					emu_ikrun(xyz_pos,current_xyz_pos,grabber_val);
					break;
			case 16 : 
					break;
					
		}
	}
}
