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
	int shoulder_angle_int, base_angle_int, elbow_angle_int;
	float cq1, c3,  k1, k2;
	float elbow_step_1, elbow_step_2;
	float q3a;
	float elbow_angle, base_angle, shoulder_angle;    //2nd possible angle of rotation at shoulder
	float x4,z4;  //wrist X,Y positions
	float ref_dist;

	printf("X: %f- Y: %f- Z: %f\n\n",xyz_pos[X],xyz_pos[Y],xyz_pos[Z]);

	/* base_angle - base */
	base_angle = atan2(xyz_pos[Y],xyz_pos[X]);
	cq1 = cos(base_angle);
	base_angle  = base_angle *(180/M_PI);

	printf("CQ1:%f\n", cq1);

	//angle 3 - Elbow
	x4 = (xyz_pos[X] - (l4*cq1))/cq1;  //wrist positions  T14
	z4 = xyz_pos[Z] - l1;

	printf("X4: %f, Z4: %f\n", x4, z4);
	c3 = (square(x4) + square(z4) - square(l2) - square(l3)) / (2*l2*l3);
	elbow_step_1 = sqrt(1-(square(c3)));
	elbow_step_2 = -sqrt(1-(square(c3)));

	printf("C3: %f, ElStep1: %f,ElStep2: %f\n", c3, elbow_step_1, elbow_step_2);

	q3a = atan2(elbow_step_1, c3);
	elbow_angle = q3a * (180/M_PI);

	printf("q3a: %f, elbow_angle: %f\n", q3a, elbow_angle);
	/* angle 2 - shoulder */
	k1 = l2+l3*c3;
	k2 = l3*elbow_step_2;

	printf("k1: %f, k2: %f\n", k1, k2);
	ref_dist = sqrt(square(k1) + square(k2));

	printf("ref_dist: %f\n", ref_dist);
	shoulder_angle = (atan2((z4/ref_dist),(x4/ref_dist)))-(atan2((-1*k2),k1));
	shoulder_angle = shoulder_angle/(M_PI/180);

	/* Make int versions */
	shoulder_angle_int = (int)floor(shoulder_angle);
	base_angle_int = (int)floor(base_angle);
	elbow_angle_int = (int)floor(elbow_angle);

	sleep(2);

	printf("\nBase angle: %d\nShoulder Angle:%d\nEblow Angle: %d\n\n",base_angle_int, shoulder_angle_int,elbow_angle_int);
	/* Before sending the move command to the EMU arm confirm it is within the operating envelope of the servos */
	if((base_angle_int <= MAX_ANGLE) && (base_angle_int > MIN_ANGLE))
	{
		printf("Move B:\n");
		servo_vals[BASE] = emu_map(base_angle_int);
	}
	if ((shoulder_angle_int <= MAX_ANGLE) && (shoulder_angle_int >= MIN_ANGLE))
	{
		printf("Move S:\n");
		servo_vals[SHOULDER] = emu_map(shoulder_angle_int);
	}
	if ((elbow_angle_int <= MAX_ANGLE) && (elbow_angle_int >= MIN_ANGLE))
	{
		printf("Move E:\n");
		servo_vals[ELBOW] = emu_map(elbow_angle_int);
	}
}

/****************************************************
 * Map
 ***************************************************/
int emu_map(int x)
{
	return ((x - MIN_ANGLE) * (SERVO_MAX - SERVO_MIN) / (MAX_ANGLE - MIN_ANGLE) + SERVO_MIN);
}

/* 1 == Increment
 * 0 == Decrement
 */
void ik_update_xyz(float* xyz_pos,int which,int change)
{
	//step default value
	float step, max, min;
	int i;

	printf("Update XYZ\n");

	for (i = 0; i < 3;i++)
	{
		printf("\n -- xyz[%d] - %f",i,xyz_pos[i]);
	}


	if (which == X)
	{

		max = MAX_X;
		min = MIN_X;
		step = (MAX_X - MIN_X) / NUM_OF_STEPS;
		printf("Update X by: %f\n", step);
	}
	else if( which == Y)
	{
		max = MAX_Y;
		min = MIN_Y;
		step = (MAX_Y - MIN_Y) / NUM_OF_STEPS;
		printf("Update Y by: %f\n", step);
	}
	else if( which == Z)
	{
		max = MAX_Z;
		min = MIN_Z;
		step = (MAX_Z - MIN_Z) / NUM_OF_STEPS;
		printf("Update Z by: %f\n", step);
	}
	else return;


	if ((change == INCREMENT) && (xyz_pos[which] < max))
	{
		xyz_pos[which] += step;
		if (xyz_pos[which] > max)
		{
			xyz_pos[which] = max;
		}
	}
	else if ((change == DECREMENT) && (xyz_pos[which] + step > min))
	{
		xyz_pos[which] -= step;
		if (xyz_pos[which] < min)
		{
			xyz_pos[which] = min;
		}
	}

	for (i = 0; i < 3;i++)
	{
		printf("\n -- xyz[%d] - %f",i,xyz_pos[i]);
	}

}
/* Invert the gripper status */
void invert_gripper(int* sVals)
{
	if ( sVals[GRIPPER] == SERVO_MIN)
	{
		sVals[GRIPPER] = SERVO_MAX;
	}
	else
	{
		sVals[GRIPPER]= SERVO_MIN;
	}
}

void emu_intialize(float* xyz_pos, int* sVals)
{
	xyz_pos[X] = X_START;   //X position relative to centre of robot base //ToDo: On return this value if lost
	xyz_pos[Y] = Y_START;    //Y position relative to centre of robot base
	xyz_pos[Z] = Z_START;   //Z position relative to centre of robot base
	sVals[GRIPPER] = SERVO_MAX;
}
