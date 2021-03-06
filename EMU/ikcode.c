/*******************************************************************
 * This example has been heavily modified from the original to model the 
 * examples in the RTOS manual 
 * NJG 02/12/09 
 * 
 * Extended to combine the LEDs worksheet, queues and mutex for the print to 
 * console functions. 14/11/11
 * 
 * 
 * */

/* Standard includes. */
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "emu.h"


//DUMMY FUCNTIONS UNTIL READ AND WRITE IS PROPERLY WRITTEN
int Read_Keypad()
{
	int keypad_val = 1;

	return keypad_val;
}
void Write_PWM(int output_servo, int value)
{
	printf("Servo to write to: %d, Servo movement value %d", output_servo, value);

}


//Inverse Kinematics function
void ikrun(double xE, double yE, double zE, int angle4in)
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
		base = map(angle1int,-45,45,SERVO_MIN,SERVO_MAX);
		shoulder= map(angle2bint,45,-45,SERVO_MIN,SERVO_MAX);
		gripper = map(angle4int,0,1,SERVO_MIN,SERVO_MAX);
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
int map(int x, int in_min, int in_max, int out_min, int out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
 
/*********************************************************
 * MAIN 
 * ******************************************************* */
int main( void )
{
	xE = XE_START;   //X position relative to centre of robot base
	yE = YE_START;    //Y position relative to centre of robot base
	zE = ZE_START;   //Z position relative to centre of robot base

	int key_val;
	int angle4in = 0;   
	int servo_val;

	/* Call of Inverse Kinematic function to set the start position of the EMU arm using the previously defined angle settings */   
 	ikrun(xE,yE,zE,angle4in);
  
	/* Sets the elbow to its zero start position based on the Inverse Kinematics */

	servo_val = SERVO_MID;
	Write_PWM(ELBOW, servo_val);

	/////////////////////////////////
	while(1)
	{
		/* Read the keypad and switch over its return value */
		key_val = Read_Keypad();
		
		switch (key_val)
		{
			//Row 1 [ 1 - 4 ]
			case 1 : Write_PWM(SHOULDER, SERVO_MAX);
					break;
			case 2 : zE = zE - 3;
				ikrun(xE,yE,zE,angle4in);
					break;
			case 3 : Write_PWM(SHOULDER, SERVO_MIN);
					break;
			case 4 : 
					break;
			//Row 2 [ 5 - 8 ]
			case 5 : yE = yE + 4;
				ikrun(xE,yE,zE,angle4in);
					break;
			case 6 : xE = 29;   //X position relative to centre of 				     // robot base
				yE = 0;    //Y position relative to centre of 				     //robot base
				zE = 15;   //Z
				Write_PWM(ELBOW, SERVO_MID);
				ikrun(xE,yE,zE,angle4in);
					break;
			case 7 : yE = yE - 4;
				ikrun(xE,yE,zE,angle4in);  
					break;
			case 8 : 
					break;
			//Row 3 [ 9 - 12 ]
			case 9 : 
					break;
			case 10 : zE = zE + 3;
				ikrun(xE,yE,zE,angle4in);
					break;
			case 11 : 
					break;
			case 12 : 
					break;
			//Row 4 [ 13 - 16]
			case 13 : angle4in = 0;
				ikrun(xE,yE,zE,angle4in);
					break;
			case 14 : 
					break;
			case 15 : angle4in = 1;
				ikrun(xE,yE,zE,angle4in);
					break;
			case 16 : 
					break;
					
		}
	}
}

