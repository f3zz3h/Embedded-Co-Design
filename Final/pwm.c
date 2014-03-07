/* Based from Nigel's toggle_leds.c */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "global.h"
#include "pwm.h"

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
