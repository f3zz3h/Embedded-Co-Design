/* ********************************************************
 * Project Title: EMU Pick & Place
 * File: keypad.c
 * Description: Home to the Read_Keypad function that runs
 * in its own thread and is used to detect keypad presses for
 * use elsewhere in the system.
 * Authors: Luke Hart, Joe Ellis, Kerrim Morris & Lukasz Matczak
 * Last edited: 14/04/2014
 * ******************************************************* */
#include "global.h"
#include "keypad.h"
/****************************************************************
 * void* Read_Keypad();
 * Determines which key has been pressed by writing a walking 0
 * to the row bit pattern and checking to see if a column has been 
 * pulled low.
 * (Runs in its' own thread, hence void* declaration)
 * Last edited: 14/04/2014
****************************************************************/
void* Read_Keypad()
{
	volatile unsigned int *vga_reg;      /* only bits 14-0 valid */
	volatile unsigned int *enable_vga;   /* only bits 14-0 valid */
	/* Walking 0 bit pattern for keypad */
	unsigned int vga_reg_vals[4] = {0x000001C0,
									0x000001A0,
									0x00000160,
									0x000000E0};
	
	/* Set up the pointers to the registers that have been created in the FPGA. Then, make all configurable pins outputs.*/
	/* IO registers */
	vga_reg = mem_addr + 0x00;
	enable_vga = mem_addr + 0x01;
	
	/* DIO direction configuration*/
	/* The hardware will ignore attempts to modify reserved I/O pins to make life easier for the programmer.
	   Just remember to mask out invalid bits on a register read.*/
	*enable_vga  = 0x000001E0;  /* bits 5-8*/

	int flag = 0;
	int keyval = 0;
	int temp_key = 0;
	int i = 0;

	while(1)
	{

		for (i = 0; i < 4; i++)
		{
			*vga_reg = vga_reg_vals[i]; // bitshift row 1 five bits to the left

			usleep(1000);

			temp_key = (*vga_reg & 0x000001eF); // Mask the shifted bit pattern to determine if a column has been pulled low.

			if ((keyval != temp_key) && ((temp_key & 0x0000000F) < 0xF))
			{
				keyval = temp_key;
				flag = 1;

				/* breaking here will take only the first button pressed */
				break;
			}
		}


		if (flag)
		{
			pthread_mutex_lock(&keypad_mutex);
			switch (keyval)
			{
				case 0x1C7 :
					key_val = KEY_1;
					break;
				case 0x1A7 :
					key_val = KEY_2;
					break;
				case 0x167 :
					key_val = KEY_3;
					break;
				case 0xE7 :
					key_val = KEY_4;
					break;
				case 0x1CB :
					key_val =  KEY_5;
					break;
				case 0x1AB :
					key_val =  KEY_6;
					break;
				case 0x16B :
					key_val =  KEY_7;
					break;
				case 0xEB :
					key_val =  KEY_8;
					break;
				case 0x1CD :
					key_val =  KEY_9;
					break;
				case 0x1AD :
					key_val =  KEY_10;
					break;
				case 0x16D :
					key_val =  KEY_11;
					break;
				case 0xED :
					key_val =  KEY_12;
					break;
				case 0x1CE :
					key_val =  KEY_13;
					break;
				case 0x1AE :
					key_val =  KEY_14;
					break;
				case 0x16E :
					key_val =  KEY_15;
					break;
				case 0xEE :
					key_val =  KEY_16;
					break;
				default :
					key_val = 0;
			}
			pthread_mutex_unlock(&keypad_mutex);
			keyval = 0;
		}
	}
}

