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
int Read_Keypad()
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

	*vga_reg = 0x0000000E<<5; // bitshift column 1 five bits to the left
	usleep(1000);
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
		switch (keyval) 
		{
			case 0x1C7 : 
				return KEY_1;
			case 0x1A7 :
				return KEY_2;
			case 0x167 :
				return KEY_3;
			case 0xE7 : 
				return KEY_4;
			case 0x1CB :
				return KEY_5;
			case 0x1AB :
				return KEY_6;
			case 0x16B :
				return KEY_7;
			case 0xEB : 
				return KEY_8;
			case 0x1CD :
				return KEY_9; 
			case 0x1AD : 
				return KEY_10;
			case 0x16D :
				return KEY_11; 
			case 0xED : 
				return KEY_12;
			case 0x1CE :
				return KEY_13;
			case 0x1AE : 
				return KEY_14;
			case 0x16E : 
				return KEY_15;
			case 0xEE : 
				return KEY_16;
			default : 
				/* ToDO: add cases for multiple key presses for arm movements */
				printf("Keyval not defined = %x", keyval);
				return keyval;
		}
	}
	return 0;
}

