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
