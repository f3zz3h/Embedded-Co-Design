#include "emu.h"
#include "global.h"
//#include "keypad.c"
//#include "pwm.c"

//Inverse Kinematics function
void emu_ikrun(double xE, double yE, double zE, int angle4in)
{

	xE = xE / 100;   //convert to meters
	yE = yE / 100;
	zE = zE / 100;
	xEik = xE;
	yEik = yE;
	zEik = zE;


	//angle1
	angle1 = atan2(yEik,xEik);
	cq1 = cos(angle1);
	sq1 = sin(angle1);
	angle1  = angle1 *(180/M_PI);

	x4 = (xE - (l4*cq1))/cq1;  //wrist positions  T14
	y4 = 0;
	z4 = zE - l1;

	x4ik = x4;
	y4ik = y4;
	z4ik = z4;

	//angle 3
	c3 = ((x4ik*x4ik) + (z4ik*z4ik) - (l2*l2) - (l3*l3))/(2*l2*l3);
	s3a = (1-(c3*c3));

	if (s3a < 0)
	{
	    	s3a = 0;
		s3a = sqrt(s3a);
		s3b = -1* s3a;
		angle3a = (atan2(s3a,c3))/(M_PI/180);
		angle3b = (atan2(s3b,c3))/(M_PI/180);
	}


	//angle 2
	cq3 = cos((angle3a)*(M_PI/180));
	sq3 = sin((angle3a)*(M_PI/180));
	k1 = l2+l3*c3;
	k2 = l3*s3b;
	r = sqrt((k1*k1) + (k2*k2));
	val5 = (z4ik/r)/(x4ik/r);
	val6 = (k2/k1);
	angle2a = (atan2((z4ik/r),(x4ik/r)))-(atan2(k2,k1));
	angle2a = angle2a/(M_PI/180);
	val7 = (z4ik/r)/(x4ik/r);
	val8 = ((-1*k2)/k1);
	angle2b = (atan2((z4ik/r),(x4ik/r)))-(atan2((-1*k2),k1));
	angle2b = angle2b/(M_PI/180);

	angle1int = (int)floor(angle1);
	angle2bint = (int)floor(angle2b);
	angle3aint = (int)floor(angle3a);
	angle4int = angle4in;

	int angle2aint = (int)floor(angle2a);
	int angle3bint = (int)floor(angle3b);

	/*Before sending the move command to the EMU arm confirm it is within the operating envelope of the servos*/

	if((angle1int <= 45 && angle1int >= -45) && (angle2bint <= 45 && angle2bint >= -45) )
  	{
		base = emu_map(angle1int,-45,45,SERVO_MIN,SERVO_MAX);
		shoulder= emu_map(angle2bint,45,-45,SERVO_MIN,SERVO_MAX);
		gripper = emu_map(angle4int,0,1,SERVO_MIN,SERVO_MAX);
		xEholder = (xE * 100) -5;
		yEholder = (yE * 100) -5;
		zEholder = (zE * 100) -5;
	}

	/*If the move command is outside of the operating envelope of the servos make the Inverse Kinematic coordinates for xE, yE and zE equal to the last in-range values as dictated by XXHolder*/

	if(angle1int > 45 || angle1int < -45 || angle2bint > 45 || angle2bint < -45 )
	{
		xE = xEholder;
		yE = yEholder;
		zE = zEholder;
	}

/*Send the move commands to each of the servos, Base, Shoulder,Gripper. The Elbow is currently controlled directly at the function call of the correct keypad press - only moves to two static positions*/

	Write_PWM(BASE, base);
	Write_PWM(SHOULDER, shoulder);
	Write_PWM(GRIPPER, gripper);
}

/*MapM_PIng function from Arduino Library*/

/****************************************************
 * Map
 ***************************************************/
int emu_map(int x, int in_min, int in_max, int out_min, int out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void emu_intialize()
{
	xE = XE_START;   //X position relative to centre of robot base
	yE = YE_START;    //Y position relative to centre of robot base
	zE = ZE_START;   //Z position relative to centre of robot base



	/* Call of Inverse Kinematic function to set the start position of the EMU arm using the previously defined angle settings */
 	emu_ikrun(xE,yE,zE,angle4in);
}
