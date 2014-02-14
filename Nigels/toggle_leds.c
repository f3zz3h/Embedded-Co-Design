/* modified from toggle_dio2.c by NJG.
   Altered to use unsigned int pointers instead of unsigned char
   as byte writes weren't working correctly.

   Toggle rate not yet tested but the 2 printfs don't half slow things 
   down :)

   LEDs mapped to bits 2 & 3 of misc_reg (counting from zero)
  
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
	volatile unsigned int *dio2_reg;     /* only bits 15..10,8..0 valid */
	volatile unsigned int *enable_dio2;  /* ditto */
	volatile unsigned int *misc_reg;     /* bits 3 ..0 only */
	volatile unsigned int *enable_misc;  /* ditto */
	
	/* Open a page at the FPGA base address */
	fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (fd < 0) {
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
	if (mem_addr == (unsigned int *)MAP_FAILED) {
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
	dio2_reg = mem_addr + 0x02;
	enable_dio2 = mem_addr + 0x03;
	misc_reg = mem_addr + 0x04;
	enable_misc = mem_addr + 0x05;

	
	/* ******************************
	 *
	 * DIO direction configuration next.
	 * The hardware will ignore attempts to modify
	 * reserved I/O pins to make life easier for the programmer.
	 * Just remember to mask out invalid bits on a register read.
	 ***************
	 */

	*enable_vga  = 0x00007FFF;  /* All 15 outputs */
	*enable_dio2 = 0x0000FDFF;  /* all 15 outputs */
        *enable_misc = 0x0000000F;   /* all 4 outputs */
	/* Toggle the pins */
	printf("Toggling XDIO#1, use Ctrl-C to exit\n");
	toggle_io(misc_reg);

	/* Ctrl-C exits the application */
	
	return 0;
}

void toggle_io(volatile unsigned int *port)
{

	while (1) {
	  /* both on, one one, swap, both off*/
	    *port = 0x00000000;
	    printf("reg val = %x  \n",*port);
	    *port = 0x00000004;
	    printf("reg val = %x  \n",*port);
	    *port = 0x00000008;	  
	    printf("reg val = %x  \n",*port);
	    *port = 0x0000000C;
	    printf("reg val = %x  \n",*port);
	}
}

