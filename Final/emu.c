#include "global.h"
#include "emu.h"
/* WHOLE DOCUMENT - ToDo: Refactor, Rename and comment all of the code
 */
/* Inverse Kinematics function - Kerrim Morris, Luke Hart, Joe Ellis, Lukasz Matchzak
 *
 * ToDo: Serious scope potential issues here, the parameters x,y,zE are all globals
 */
float square(float x)
{
   return x * x;
}
/* xyz pointer is an array of [x,y,z] and current[x,y,z] is the previous values ..
 * grabber angle is the grabber opening amount i believe?
 *
 */
void emu_ikrun(float xyz_pos[3], int* servo_vals)
{
	int i;
	int shoulder_angle_int, base_angle_int, elbow_angle_int;
	float cq1 = 0.0, c3 = 0.0, elbow_step_1 = 0.0, k1 = 0.0;
	float k2 = 0.0;
	float xyz_pos_meters[3];
	float q3a;
	float elbow_angle, base_angle, shoulder_angle;    //2nd possible angle of rotation at shoulder

	float x4,z4;  //wrist X,Y positions
	float ref_dist;

	/* change to meters */
	for (i = 0; i < 3 ; i++)
	{
		/* These values checked and are correct */
		xyz_pos_meters[i] = xyz_pos[i];// / 100;
	}

	/* base_angle - base */
	base_angle = atan2(xyz_pos_meters[Y],xyz_pos_meters[X]);
	cq1 = cos(base_angle);
	base_angle  = base_angle *(180/M_PI);

	x4 = (xyz_pos_meters[X] - (l4*cq1))/cq1;  //wrist positions  T14
	z4 = xyz_pos_meters[Z] - l1;

	//angle 3 - Elbow
	c3 = (square(x4) + square(z4) - square(l2) - square(l3)) / (2*l2*l3);
	elbow_step_1 = sqrt(1-(square(c3)));
	float elbow_step_2 = -sqrt(1-(square(c3)));

	q3a = atan2(elbow_step_1, c3);
	//q3b = atan2(elbow_step_2, c3);

	elbow_angle = q3a * (180/M_PI);
	/* angle 2 - shoulder */
	k1 = l2+l3*c3;
	k2 = l3*elbow_step_2;

	ref_dist = sqrt(square(k1) + square(k2));

	shoulder_angle = (atan2((z4/ref_dist),(x4/ref_dist)))-(atan2((-1*k2),k1));
	shoulder_angle = shoulder_angle/(M_PI/180);

	shoulder_angle_int = (int)floor(shoulder_angle);
	base_angle_int = (int)floor(base_angle);
	elbow_angle_int = (int)floor(elbow_angle);

	printf("\nBase angle: %u\nShoulder Angle:%u\nEblow Angle: %u\n\n",base_angle_int, shoulder_angle_int,elbow_angle_int);

	/* Before sending the move command to the EMU arm confirm it is within the operating envelope of the servos */
	if(((base_angle_int <= MAX_ANGLE)   &&(base_angle_int > MIN_ANGLE)) &&
			((shoulder_angle_int <= MAX_ANGLE) && (shoulder_angle_int >= MIN_ANGLE)))
  	{
		servo_vals[BASE] = emu_map(base_angle_int);
		servo_vals[SHOULDER] = emu_map(shoulder_angle_int);
		servo_vals[ELBOW] = emu_map(elbow_angle_int);
	}

}

/****************************************************
 * Map
 ***************************************************/
int emu_map(int x)
{
	return (x - MIN_ANGLE) * (SERVO_MAX - SERVO_MIN) / (MAX_ANGLE - MIN_ANGLE) + SERVO_MIN;
}

void emu_intialize(float* xyz_pos, int* sVals)
{
	xyz_pos[X] = X_START;   //X position relative to centre of robot base //ToDo: On return this value if lost
	xyz_pos[Y] = Y_START;    //Y position relative to centre of robot base
	xyz_pos[Z] = Z_START;   //Z position relative to centre of robot base
	sVals[GRIPPER] = SERVO_MAX;

}
