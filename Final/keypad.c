#include "global.h"
#include "keypad.h"


/****************************************************************
 * Read_Keypad:
 * Writes a walking 0 to bits 5 - 8 then tests the key value to
 * see which column button is pressed.
 * E = 1110, D = 1101, B = 1011, 7 = 0111 then shift left 5.
 *
 * 	ToDo: Check return values of 0 as errors and do somethign about it
****************************************************************/
void* Read_Keypad()
{
	volatile unsigned int *vga_reg;      /* only bits 14-0 valid */
	volatile unsigned int *enable_vga;   /* only bits 14-0 valid */
	
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

	while(1)
	{
		*vga_reg = 0x0000000E<<5; // bitshift column 1 five bits to the left

		usleep(1000); //ToDo: check if we actually need this
		temp_key = (*vga_reg & 0x000001eF); // mask column (bits 5-8) and row (bits 0-3)

		if ((keyval != temp_key) && ((temp_key & 0x0000000F) < 0xF))
		{
			keyval = temp_key;
			flag = 1;
		}

		*vga_reg = 0x0000000D<<5;
		usleep(1000);
		temp_key = (*vga_reg & 0x000001eF);

		if ((keyval != temp_key) && ((temp_key & 0x0000000F) < 0xF))
		{
			keyval = temp_key;
			flag = 1;
		}

		*vga_reg = 0x0000000B<<5;
		usleep(1000);
		temp_key = (*vga_reg & 0x000001eF);

		if ((keyval != temp_key) && ((temp_key & 0x0000000F) < 0xF))
		{
			keyval = temp_key;
			flag = 1;
		}

		*vga_reg = 0x00000007<<5;
		usleep(1);
		temp_key = (*vga_reg & 0x000001eF);

		if ((keyval != temp_key) && ((temp_key & 0x0000000F) < 0xF))
		{
			keyval = temp_key;
			flag = 1;
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
			}
			pthread_mutex_unlock(&keypad_mutex);
			keyval = 0;
		}
	}
}

