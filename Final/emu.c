#include "global.h"
#include "emu.h"
/* WHOLE DOCUMENT - ToDo: Refactor, Rename and comment all of the code
 */
/* Inverse Kinematics function - Kerrim Morris, Luke Hart, Joe Ellis, Lukasz Matchzak
 *
 * ToDo: Serious scope potential issues here, the parameters x,y,zE are all globals
 */
int square(int x)
{
   return x * x;
}
/* xyz pointer is an array of [x,y,z] and current[x,y,z] is the previous values ..
 * grabber angle is the grabber opening amount i believe?
 *
 */
void emu_ikrun(float* xyz_pos, float* current_xyz, int grabber_angle)
{
	int i;
	int base = 0, shoulder = 0, gripper = 0 ; // elbow, NOT USED
	float cq1 = 0.0, c3 = 0.0, s3a = 0.0, s3b = 0.0, k1 = 0.0;
	float k2 = 0.0;
	float xyz_pos_meters[3];

	double base_angle;     //angle of rotation at the base
	double angle2 = 0.0;    //2nd possible angle of rotation at shoulder

	double x4;  //wrist X positions
	double z4;  //wrist Z positions

	double r;

	/* change to meters */
	for (i = 0; i < 3 ; i++)
	{
		xyz_pos_meters[i] = xyz_pos[i] / 100;
	}

	/* base_angle - base */
	base_angle = atan2(xyz_pos_meters[Y],xyz_pos_meters[X]);
	cq1 = cos(base_angle);
	base_angle  = base_angle *(180/M_PI);

	x4 = (xyz_pos_meters[X] - (l4*cq1))/cq1;  //wrist positions  T14
	z4 = xyz_pos_meters[Z] - l1;

	//angle 3 - Elbow
	c3 = (square(x4) + square(z4) - square(l2) - square(l3)) / (2*l2*l3);
	s3a = (1-(square(c3)));

	/* 2 positions for elbow??  */
	if (s3a < 0)
	{
		s3b = 0;
	}

	/* angle 2 - shoulder */
	k1 = l2+l3*c3;
	k2 = l3*s3b;
	r = sqrt(square(k1) + square(k2));

	angle2 = (atan2((z4/r),(x4/r)))-(atan2((-1*k2),k1));
	angle2 = angle2/(M_PI/180);
	angle2 = (int)floor(angle2);

	base_angle = (int)floor(base_angle);

	/* Before sending the move command to the EMU arm confirm it is within the operating envelope of the servos */
	if((base_angle <= 45 && base_angle >= -45) && (angle2 <= 45 && angle2 >= -45) )
  	{
		base = emu_map(base_angle);
		shoulder = emu_map(angle2);
		gripper = emu_map(grabber_angle);/* ToDo: 0 - 1 min and max not the angles soo need to fix this */

		/* ToDo: Why 5 ? ? ? */
		for (i = 0; i < 3 ; i++)
		{
			xyz_pos[i] -= 5;
		}
	}

	/* Send the move commands to each of the servos, Base, Shoulder, Gripper. The Elbow is currently controlled
	 * directly at the function call of the correct keypad press - only moves to two static positions*/
	Write_PWM(BASE, base);
	Write_PWM(SHOULDER, shoulder);
	Write_PWM(GRIPPER, gripper);
}

/****************************************************
 * Map
 ***************************************************/
int emu_map(int x)
{
	return (x - MIN_ANGLE) * (SERVO_MAX - SERVO_MIN) / (MAX_ANGLE - MIN_ANGLE) + SERVO_MIN;
}

void emu_intialize(float* xyz_pos)
{
	xyz_pos[X] = X_START;   //X position relative to centre of robot base
	xyz_pos[Y] = Y_START;    //Y position relative to centre of robot base
	xyz_pos[Z] = Z_START;   //Z position relative to centre of robot base
}
