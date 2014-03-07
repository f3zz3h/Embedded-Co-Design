
/* Standard includes. */
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "emu.c"
#include "keypad.c"
#include "lcd.c"
#include "pwm.c"

#include "main.h"


//DUMMY FUCNTIONS UNTIL READ AND WRITE IS PROPERLY WRITTEN
/* int Read_Keypad()
{
	int keypad_val = 1;

	return keypad_val;
}
void Write_PWM(int output_servo, int value)
{
	printf("Servo to write to: %d, Servo movement value %d", output_servo, value);

}
*/

 
/*********************************************************
 * MAIN 
 * ******************************************************* */
int main( void )
{
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

	if (argc < 2)
	{
		printf("Error: No Value, Enter a PWM value\n");
		return 0;
	}

	xE = XE_START;   //X position relative to centre of robot base
	yE = YE_START;    //Y position relative to centre of robot base
	zE = ZE_START;   //Z position relative to centre of robot base

	int key_val;
	int angle4in = 0;   
	int servo_val;

	/* Call of Inverse Kinematic function to set the start position of the EMU arm using the previously defined angle settings */   
 	ikrun(xE,yE,zE,angle4in);
  
	/* Sets the elbow to its zero start position based on the Inverse Kinematics */

	servo_val = SERVO_MID;
	Write_PWM(ELBOW, servo_val);

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
			case 2 : zE = zE - 3;
				ikrun(xE,yE,zE,angle4in);
					break;
			case 3 : Write_PWM(SHOULDER, SERVO_MIN);
					break;
			case 4 : 
					break;
			//Row 2 [ 5 - 8 ]
			case 5 : yE = yE + 4;
				ikrun(xE,yE,zE,angle4in);
					break;
			case 6 : xE = 29;   //X position relative to centre of 				     // robot base
				yE = 0;    //Y position relative to centre of 				     //robot base
				zE = 15;   //Z
				Write_PWM(ELBOW, SERVO_MID);
				ikrun(xE,yE,zE,angle4in);
					break;
			case 7 : yE = yE - 4;
				ikrun(xE,yE,zE,angle4in);  
					break;
			case 8 : 
					break;
			//Row 3 [ 9 - 12 ]
			case 9 : 
					break;
			case 10 : zE = zE + 3;
				ikrun(xE,yE,zE,angle4in);
					break;
			case 11 : 
					break;
			case 12 : 
					break;
			//Row 4 [ 13 - 16]
			case 13 : angle4in = 0;
				ikrun(xE,yE,zE,angle4in);
					break;
			case 14 : 
					break;
			case 15 : angle4in = 1;
				ikrun(xE,yE,zE,angle4in);
					break;
			case 16 : 
					break;
					
		}
	}
}

