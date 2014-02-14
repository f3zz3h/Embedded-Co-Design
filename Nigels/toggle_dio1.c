/* toggle_dio1.c
 
  Toggle an I/O pin on the DIO1 or LCD header as fast as 
  possible to determine the maximum toggle speed.
 
  Compile using optimization to get the fastest code, i.e.,

  gcc -O2 -Wall -o toggle_dio1 toggle_dio1.c

  Then use objdump to disassemble the code, i.e.,

  objdump -d toggle_dio1

  For example, if the toggle function is as simple as;
  
  void toggle_io(volatile unsigned int *port)
  {
	while (1) {
		*port = 0;
		*port = 0xFF;
	}
  }
  
  the toggle I/O function reduces to;
  
0000899c <toggle_io>:
    899c:	e3a02000 	mov	r2, #0	; 0x0
    89a0:	e3a030ff 	mov	r3, #255	; 0xff
    89a4:	e5802000 	str	r2, [r0]
    89a8:	e5803000 	str	r3, [r0]
    89ac:	eafffffc 	b	89a4 <toggle_io+0x8>

	i.e., two store operations and a branch back to the start of
	the first store operation.
	
	The store operations can be changed to store-byte by using
	a volatile unsigned char *, rather than volatile unsigned int *.

	The output frequency on EGPIO[15] on DIO.15 was 7.14MHz 
	(140ns period). For a 200MHz processor, this is 28 clock cycles.
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

/* Put the toggle loop in a separate function so that the
 * disassembly of the code is easier to interpret.
 */
void toggle_io(volatile unsigned int *port);

static void show_usage()
{
	printf("\nUsage: toggle_io -p <port>\n"\
		 "  -h            Help (this message)\n"\
		 "  -p <port>     Port; A, B, C, F, H\n\n");
}

int main(int argc, char *argv[])
{
	int opt;       /* getopt processing         */
	int fd;        /* /dev/mem file descriptor  */
	int page_size = getpagesize();
	char        *mem_addr  = NULL;
	unsigned int mem_phys  = 0x80840000;
	unsigned int *port;
	unsigned int *port_dir;
	unsigned int port_mask;
	char port_name = '?';
	unsigned int syscfg_phys  = 0x80930000;
	char *syscfg_addr;
	unsigned int *devcfg;
	unsigned int *swlock;
	
	/* Command line parsing */
	while ((opt = getopt(argc, argv, "hp:")) != -1) {
		switch (opt) {
			case 'h':
				show_usage();
				return -1;
			case 'p':
				port_name = toupper(optarg[0]);
				break;
			default:
				show_usage();
				return -1;
		}
	}
	/* Check the port name */
	switch (port_name) {
		case 'A':
		case 'B':
		case 'C':
		case 'F':
		case 'H':
			break;
		default:
			printf("\nPlease provide a valid port name.\n");
			show_usage();
			return -1;
	}
	
	/* Open a page at the SYSCFG base address */
	fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (fd < 0) {
		printf("Open /dev/mem failed - %s\n", 
			strerror(errno));
		return -1;
	}
	syscfg_addr = (char *)mmap(
		0,
		page_size,
		PROT_READ|PROT_WRITE,
		MAP_SHARED,
		fd,
		syscfg_phys);
	if (syscfg_addr == (char *)MAP_FAILED) {
		printf("Error: syscfg mmap failed\n");
		close(fd);
		return -1;
	}
	devcfg = (unsigned int *)(syscfg_addr + 0x80);
	swlock = (unsigned int *)(syscfg_addr + 0xC0);
	if (*devcfg & 0x60000000) {
		printf("DEVCFG = %.8X: DMA on EGPIO was enabled, disabling ...\n",
				*devcfg);
		*swlock = 0xAA;
		*devcfg &= ~0x60000000;
		printf("DEVCFG = %.8X\n", *devcfg);
	}
	munmap((void *)syscfg_addr, page_size);
	close(fd);
	
	/* Open a page at the GPIO base address */
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
	if (mem_addr == (char *)MAP_FAILED) {
		printf("Error: mmap failed\n");
		close(fd);
		return -1;
	}

	/* Port tests
	 *
	 * Port bits    Location
	 * ---------    --------
	 * A[6:0]       LCD header
	 * B[7:3, 1:0]  DIO1 header
	 * C[0]         LCD header
	 * F[1]         DIO1 header
	 * H[5:3]       LCD header
	 */
	switch (port_name) {
		case 'A':
			port      = (unsigned int *)(mem_addr + 0x00);	
			port_dir  = (unsigned int *)(mem_addr + 0x10);	
			port_mask = 0x7F; 
			break;
		case 'B':
			port      = (unsigned int *)(mem_addr + 0x04);	
			port_dir  = (unsigned int *)(mem_addr + 0x14);	
			port_mask = 0xFB; 
			break;
		case 'C':
			port      = (unsigned int *)(mem_addr + 0x08);	
			port_dir  = (unsigned int *)(mem_addr + 0x18);	
			port_mask = 0x01;
			break;
		case 'F':
			port      = (unsigned int *)(mem_addr + 0x30);	
			port_dir  = (unsigned int *)(mem_addr + 0x34);	
			port_mask = 0x02;
			break;
		case 'H':
			port      = (unsigned int *)(mem_addr + 0x40);	
			port_dir  = (unsigned int *)(mem_addr + 0x44);	
			port_mask = 0x38;
			break;
		default:
			return -1;
	}
	/* Set the direction bits */
	*port_dir = port_mask;
	
	/* Toggle the pins */
	printf("Toggling port %c, use Ctrl-C to exit\n", port_name);
	toggle_io(port);
	
	/* Ctrl-C exits the application */
	
	return 0;
}

void toggle_io(volatile unsigned int *port)
{
	while (1) {
		*port = 0;
		*port = 0xFF;
	}
}

