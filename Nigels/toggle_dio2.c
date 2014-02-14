/* toggle_dio2.c
 
  Toggle one of the XDIO ports on DIO2 as fast as possible 
  to determine the toggle speed.

  The port toggled at roughly 3.5MHz, 285ns period, or about
  57 clocks at 200MHz.
  
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
void toggle_io(volatile unsigned char *port);

int main(int argc, char *argv[])
{
	int fd;        /* /dev/mem file descriptor  */
	int page_size = getpagesize();
	unsigned char *mem_addr  = NULL;
	unsigned int mem_phys  = 0x72000000;
	volatile unsigned char *port_ctrl;
	volatile unsigned char *port;
	volatile unsigned char *port_dir;
	
	/* Open a page at the FPGA base address */
	fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (fd < 0) {
		printf("Open /dev/mem failed - %s\n", 
			strerror(errno));
		return -1;
	}
	mem_addr = (char *)mmap(
		0,
		page_size,
		PROT_READ|PROT_WRITE,
		MAP_SHARED,
		fd,
		mem_phys);
	if (mem_addr == (unsigned char *)MAP_FAILED) {
		printf("Error: mmap failed\n");
		close(fd);
		return -1;
	}

	/* XDIO registers */
	port_ctrl = mem_addr + 0x40;
	port_dir  = mem_addr + 0x41;	
	port      = mem_addr + 0x42;	
	
	/* XDIO configuration */
	*port_ctrl = 0;    /* IO mode */
	*port_dir  = 0xFF; /* All outputs */
	
	/* Toggle the pins */
	printf("Toggling XDIO#1, use Ctrl-C to exit\n");
	toggle_io(port);

	/* Ctrl-C exits the application */
	
	return 0;
}

void toggle_io(volatile unsigned char *port)
{
	while (1) {
		*port = 0x00;
		*port = 0xFF;
	}
}

