/* 	$Id: keypad.c,v 1.2 2008/01/09 16:23:38 ngunton Exp ngunton $	 */

#ifndef lint
static char vcid[] = "$Id: keypad.c,v 1.2 2008/01/09 16:23:38 ngunton Exp ngunton $";
#endif /* lint */


/* modified from toggle_dio2.c by NJG.
   Altered to use unsigned int pointers instead of unsigned char
   as byte writes weren't working correctly.
 */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>   /* toupper */
#include <signal.h>

/* Put the toggle loop in a separate function so that the
 * disassembly of the code is easier to interpret.
 */
void toggle_io(volatile unsigned int *port); /* modded from unsigned char */

int main(int argc, char *argv[])
{
	int fd;        /* /dev/mem file descriptor  */
	int page_size = getpagesize();
	unsigned int *mem_addr  = NULL;
	unsigned int mem_phys  = 0x72A00000; /*base for opencore reg */

	volatile unsigned int *vga_reg;      /* only bits 14 .. 0 valid */
	volatile unsigned int *enable_vga;   /* ditto */
	
	/* Open a page at the FPGA base address */
	fd = open("/dev/mem", O_RDWR | O_SYNC);

	if (fd < 0) 
	{
		printf("Open /dev/mem failed - %s %d\n", 
			strerror(errno),errno);
		return -1;
	}

	mem_addr = (int *)mmap(
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

	printf("%x: %x \n",(int)mem_addr,*(mem_addr));

	/**********************************************
	 * set up the pointers to the registers that 
	 * have been created in the fpga. Then make
	 * all configurable pins outputs. 
	 */
	 
	/* IO registers */
	vga_reg = mem_addr + 0x00;
	enable_vga = mem_addr + 0x01;

	
	/* ******************************
	 *
	 * DIO direction configuration next.
	 * The hardware will ignore attempts to modify
	 * reserved I/O pins to make life easier for the programmer.
	 * Just remember to mask out invalid bits on a register read.
	 ***************
	 */

	*enable_vga  = 0x000001E0;  /* bits 5 .. 8*/

	toggle_io(vga_reg);

	/* Ctrl-C exits the application */
	
	return 0;
}

/***************
 * write a walking zero to bits 5 ..8 then test the read
 * value to see which column button is pressed.
 * E = 1110, D = 1101, B = 1011, 7 = 0111 then shift left 5.
 * 
 * mask out the write bits when doing a read
 */
void toggle_io(volatile unsigned int *port)
{
	int flag = 0;

	while (1) 
	{	
		int keyval = 0;
		int temp_key = 0;
	
		*port = 0x0000000E<<5;
		usleep(1000);

		temp_key = (*port & 0x000001eF);

		printf("Temp key: %d \n", temp_key);
		printf("Keyval: %d \n", keyval);

		if ((keyval != temp_key) && ((temp_key & 0x0000000F) < 0xF))
		{
			keyval = temp_key;
			flag = 1;
		}

		*port = 0x0000000D<<5;	  

		usleep(1000);

		temp_key = (*port & 0x000001eF);

		if ((keyval != temp_key) && ((temp_key &0x0000000F) < 0xF))
		{
			keyval = temp_key;
			flag = 1;
		}


		*port = 0x0000000B<<5;

		usleep(1000);

		temp_key = (*port & 0x000001eF);

		if ((keyval != temp_key) && ((temp_key &0x0000000F) < 0xF))
		{
			keyval = temp_key;
			flag = 1;
		}

		*port = 0x00000007<<5;
		usleep(1);
		temp_key = (*port & 0x000001eF);

		if ((keyval != temp_key) && ((temp_key &0x0000000F) < 0xF))
		{
			keyval = temp_key;
			flag = 1;
		}

		printf("flag = %d\n",flag);

		if(flag)
		{
			switch (keyval) 
			{
				case 0x1CE : printf("-> %d\n",keyval);
					break;
				case 0x1CD : printf("<- %d\n",keyval);
					break;
				case 0x1CB : printf("blank %d\n",keyval);
					break;
				case 0x1C7 : printf(". %d\n",keyval);
					break;
				case 0x1AE : printf("9 %d\n",keyval);
					break;	
				case 0x1AD : printf("6 %d\n",keyval);
					break;
				case 0x1AB : printf("3 %d\n",keyval);
					break;
				case 0x1A7 : printf("ent %d\n",keyval);
					break;
				case 0x16E : printf("8 %d\n",keyval);
					break;
				case 0x16D : printf("5 %d\n",keyval);
					break;
				case 0x16B : printf("2 %d\n",keyval);
					break;
				case 0x167 : printf("0 %d\n",keyval);
					break;
				case 0xEE : printf("7 %d\n",keyval);
					break;
				case 0xED : printf("4 %d\n",keyval);
					break;
				case 0xEB : printf("1 %d\n",keyval);
					break;
				case 0xE7 : printf("clr %d\n",keyval);
					break;
				default : printf("keyval %d\n", keyval);
			}
		}

	}
}

