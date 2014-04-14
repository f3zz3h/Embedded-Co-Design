/* ********************************************************
 * Project Title: EMU Pick & Place
 * File: pwm.c
 * Description: PWM Read write functions
 *
 * Authors: Luke Hart, Joe Ellis, Kerrim Morris & Lukasz Matczak
 * Last edited: 14/04/2014
 *
 * Based from Nigel's toggle_leds.c
 * ******************************************************* */
#include "global.h"
#include "pwm.h"

/* ********************************************************
 * Description: Write values from params to pwm modules
 * Params: sVals
 * Return: Void
 * Last edited: 14/04/2014
 * ******************************************************* */
void Write_PWM(int* value)
{
	int i;

	for (i = 0; i < 4; i++)
	{
		if (( value[i] > SERVO_MAX) || (value[i] < SERVO_MIN ))
		{
			printf("Value %d out of servos range\n", value[i]);
			return;
		}
	}

	volatile unsigned int *pwm_A;
	volatile unsigned int *pwm_B;
	volatile unsigned int *pwm_C;
	volatile unsigned int *pwm_D;
	volatile unsigned int *pwm_enable_A = mem_addr + PWM_A_ENABLE_REG;
	volatile unsigned int *pwm_enable_B = mem_addr + PWM_B_ENABLE_REG;
	volatile unsigned int *pwm_enable_C = mem_addr + PWM_C_ENABLE_REG;
	volatile unsigned int *pwm_enable_D = mem_addr + PWM_D_ENABLE_REG;
	volatile unsigned int *pwm_reset_n;
	
	/* Choose output reg, assign correct address' */
	pwm_A = mem_addr + DATA_A;
	pwm_B = mem_addr + DATA_B;
	pwm_C = mem_addr + DATA_C;
	pwm_D = mem_addr + DATA_D;

	//pwm_reset_n = mem_addr + RESET_REG;

	*pwm_A = value[BASE];
	*pwm_B = value[SHOULDER];
	*pwm_C = value[ELBOW];
	*pwm_D = value[GRIPPER];

	/* Enable PWM, reset and write value to reg*/
	*pwm_enable_A = ENABLE;
	*pwm_enable_B = ENABLE;
	*pwm_enable_C = ENABLE;
	*pwm_enable_D = ENABLE;

	usleep(30000);

	*pwm_enable_A = DISABLE;
	*pwm_enable_B = DISABLE;
	*pwm_enable_C = DISABLE;
	*pwm_enable_D = DISABLE;
}
/* ********************************************************
 * Description: Read current PWM values from registers
 * params: Servo number to read based on defines
 * return: int value stored in PWM
 * Last edited: 14/04/2014
 * ******************************************************* */
int Read_PWM(int servo)
{
	volatile unsigned int *pwm;
//	volatile unsigned int *pwm_enable;

	/* Choose output reg, assign correct address' */
	switch (servo)
	{
		case BASE : pwm = mem_addr + DATA_A;
			// pwm_enable = mem_addr + PWM_A_ENABLE_REG;
				break;
		case SHOULDER : pwm = mem_addr + DATA_B;
			// pwm_enable = mem_addr + PWM_B_ENABLE_REG;
				break;
		case ELBOW : pwm = mem_addr + DATA_C;
			// pwm_enable = mem_addr + PWM_C_ENABLE_REG;
				break;
		case GRIPPER : pwm = mem_addr + DATA_D;
			// pwm_enable = mem_addr + PWM_D_ENABLE_REG;
				break;
		default : printf("Attempted to use invalid PWM");
			return 0;
	}

	/* Enable PWM, read value from reg*/
	//*pwm_enable = ENABLE;
	return *pwm;
}
