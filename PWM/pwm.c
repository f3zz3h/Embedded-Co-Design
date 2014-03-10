/* Based from Nigel's toggle_leds.c

Todo: Same output to all 4 PWMs
 */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int fd;        /* /dev/mem file descriptor  */

volatile unsigned int *mem_addr  = NULL;
unsigned int mem_phys  = 0x72A00000; /*base for opencore reg */

#define ENABLE 0x1
#define DISABLE 0x0

#define DATA_A 0x06
#define DATA_B 0x07
#define DATA_C 0x08
#define DATA_D 0x09

#define PWM_A_ENABLE_REG 0x0A
#define PWM_B_ENABLE_REG 0x0B
#define PWM_C_ENABLE_REG 0x0C
#define PWM_D_ENABLE_REG 0x0D
#define RESET_REG 0x0E

void Write_PWM(int output_servo, int value);

void Write_PWM(int output_servo, int value)
{
	volatile unsigned int *pwm;
	volatile unsigned int *pwm_enable;
	volatile unsigned int *pwm_reset_n;
	
	/* Choose output reg, assign correct address' */
	switch (output_servo)
	{
		case 0 : pwm = mem_addr + DATA_A;
			 pwm_enable = mem_addr + PWM_A_ENABLE_REG;
				break;
		case 1 : pwm = mem_addr + DATA_B;
			 pwm_enable = mem_addr + PWM_B_ENABLE_REG;
				break;
		case 2 : pwm = mem_addr + DATA_C;
			 pwm_enable = mem_addr + PWM_C_ENABLE_REG;
				break;
		case 3 : pwm = mem_addr + DATA_C;
			 pwm_enable = mem_addr + PWM_C_ENABLE_REG;
				break;
		default : printf("Attempted to use invalid PWM");
			return;
	}

	/* Enable PWM, reset and write value to reg*/
	*pwm_enable = ENABLE;
	pwm_reset_n = mem_addr + RESET_REG;
	*pwm = value;

	/* Print to see whats happening */	
	printf("Writing to pwm %d with value %d", (int)*pwm, value);

	/* Give it time to process request */
	usleep(50000);

	/* Disable PWM */
	*pwm_enable = DISABLE;
}
int main(int argc, char *argv[])
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

	Write_PWM(0,35);

	return 0;
}
