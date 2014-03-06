/*
    FreeRTOS V6.0.1 - Copyright (C) 2009 Real Time Engineers Ltd.

    ***************************************************************************
    *                                                                         *
    * If you are:                                                             *
    *                                                                         *
    *    + New to FreeRTOS,                                                   *
    *    + Wanting to learn FreeRTOS or multitasking in general quickly       *
    *    + Looking for basic training,                                        *
    *    + Wanting to improve your FreeRTOS skills and productivity           *
    *                   
    *                                                       *
    * then take a look at the FreeRTOS eBook                                  *
    *                                                                         *
    *        "Using the FreeRTOS Real Time Kernel - a Practical Guide"        *
    *                  http://www.FreeRTOS.org/Documentation                  *
    *                                                                         *
    * A pdf reference manual is also available.  Both are usually delivered   *
    * to your inbox within 20 minutes to two hours when purchased between 8am *
    * and 8pm GMT (although please allow up to 24 hours in case of            *
    * exceptional circumstances).  Thank you for your support!                *
    *                                                                         *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    ***NOTE*** The exception to the GPL is included to allow you to distribute
    a combined work that includes FreeRTOS without being obliged to provide the
    source code for proprietary components outside of the FreeRTOS kernel.
    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public 
    License and the FreeRTOS license exception along with FreeRTOS; if not it 
    can be viewed here: http://www.freertos.org/a00114.html and also obtained 
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

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


/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* Altera specific includes*/
/* probably shouldn't mix FreeRTOS and Altera type defs. :( */
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "alt_types.h"

/* Altera HAL includes for timestamp timer */
#include "sys/alt_timestamp.h"
/* use alt_timestamp_start, alt_timestamp,alt_timestamp_freq */

#include "pwm.h"

/* Globals and type definitions */

typedef struct
{
	short sTaskID;
	short sLEDdata; 
} xData;

xSemaphoreHandle xMutex;
xQueueHandle xQueue;
static portCHAR cBuffer[2000];

//Inverse Kinematic Specific Definitions

#define QUEUE_LENGTH 3
#define  QUEUE_ITEM_SIZE  sizeof(xData)
#define l1  0.15    //lenght of link1 - between floor and robot base
#define l2  0.1     //lenght of link2
#define l3  0.12    //lenght of link3
#define l4  0.07    //lenght of link4
#define pi  3.1415926535897

/* Task and Function Declarations */

void ikrun(double xE, double yE, double zE,int angle4in);

/* functions */

void vConfigureTimerForRunTimeStats(void);
int map(int x, int in_min, int in_max, int out_min, int out_max);
/*********************************************************
 * MAIN 
 * *******************************************************/
 
/////////////////////////ANGLES///////////////////////////
double angle1 = 0;     //angle of rotation at the base
double angle2a = 0;    //1st possible angle of rotation at 					  //shoulder 
double angle2b = 0;    //2nd possible angle of rotation at 					  //shoulder 
double angle3a = 0;    //1st possible angle of rotation at 					  //elbow  - matches 2b
double angle3b = 0;    //2nd possible angle of rotation at 					  //elbow - matches 2a
double angle4a = 0;    //wrist angle - place holder, never 					  //used

////////Place holder variables for calculations////////////

double xEik = 0 ;    
double yEik = 0;
double zEik = 0;

double x4ik = 0;
double y4ik = 0;
double z4ik = 0;
double cq1 = 0;
double sq1 = 0;
double x4 = 0;  //wrist X positions 
double y4 = 0;  //wrist Y positions 
double z4 = 0;  //wrist Z positions 

double c3 = 0;
double s3a = 0;
double s3b = 0;

double cq3 = 0;
double sq3 = 0;
double k1 = 0;
double k2 = 0;
double r = 0;
double val5 = 0;
double val6 = 0;
double val7 = 0;
double val8 = 0;

double xEholder;
double yEholder;
double zEholder;

int angle1int, angle2bint, angle3aint, angle4int, base, elbow, shoulder, gripper;

double xE;     //X position relative to centre of robot base
double yE;    //Y position relative to centre of robot base
double zE;   //Z position relative to centre of robot base

int main( void )
{
	xE = 29;     //X position relative to centre of robot base
	yE = 0;    //Y position relative to centre of robot base
	zE = 15;   //Z position relative to centre of robot base

/*The rows below relate to the keypad used so may have to be changed for us dependent how we end up defining our key presses*/

	int rows1 = 7;
	int rows2 = 11;
	int rows3 = 13;
	int rows4 = 14;
	int colsread;

/*PWM Definitions with respect to Nigels PWM module taken from the DE0 Board. Four copies made, one for each of the servos in the EMU arm*/

	IOWR_ALTERA_AVALON_PWM_CLK_DIV(MY_PWM_0_BASE, 100);
	IOWR_ALTERA_AVALON_PWM_PERIOD(MY_PWM_0_BASE, 1000);
	IOWR_ALTERA_AVALON_PWM_CONTROL(MY_PWM_0_BASE, 1); 

	IOWR_ALTERA_AVALON_PWM_CLK_DIV(MY_PWM_1_BASE, 100);
	IOWR_ALTERA_AVALON_PWM_PERIOD(MY_PWM_1_BASE, 1000);
	IOWR_ALTERA_AVALON_PWM_CONTROL(MY_PWM_1_BASE, 1); 

	IOWR_ALTERA_AVALON_PWM_CLK_DIV(MY_PWM_2_BASE, 100);
	IOWR_ALTERA_AVALON_PWM_PERIOD(MY_PWM_2_BASE, 1000);
	IOWR_ALTERA_AVALON_PWM_CONTROL(MY_PWM_2_BASE, 1); 

	IOWR_ALTERA_AVALON_PWM_CLK_DIV(MY_PWM_3_BASE, 100);
	IOWR_ALTERA_AVALON_PWM_PERIOD(MY_PWM_3_BASE, 1000);
	IOWR_ALTERA_AVALON_PWM_CONTROL(MY_PWM_3_BASE, 1);  

	int angle4in = 0;   

	/*Call of Inverse Kinematic function to set the start position of the EMU arm using the previously defined angle settings*/   
 	ikrun(xE,yE,zE,angle4in);
  
	////ADDED BELOW - 2 LINES
	/*Sets the elbow to its zero start position based on the Inverse Kinematics*/

	elbow = 82;
	IOWR_ALTERA_AVALON_PWM_DUTY(MY_PWM_2_BASE, elbow);

	/////////////////////////////////
	while(1)
	{

		/* The Rows Relate to original kepypad - may need to change the if statements on each call*/
		///////////////////////////////ROW1 ////////////////////////       
		IOWR_ALTERA_AVALON_PIO_DATA(ROWS_BASE, rows1); 
		colsread = IORD_ALTERA_AVALON_PIO_DATA(COLLS_BASE);       
		
		if(colsread == 7)   //possible 7,11,13,14
		{
			elbow = 100;
			//elbow = elbow - 5;
			IOWR_ALTERA_AVALON_PWM_DUTY(MY_PWM_2_BASE, elbow);
		}

		if(colsread == 11)
		{  
			zE = zE - 3;
			ikrun(xE,yE,zE,angle4in);
		}

		if(colsread == 13)
		{
			elbow = 50;
			//elbow = elbow + 5;
			IOWR_ALTERA_AVALON_PWM_DUTY(MY_PWM_2_BASE, elbow);
		}
 
		///////////////////////////////ROW2/////////////////////////////   
		IOWR_ALTERA_AVALON_PIO_DATA(ROWS_BASE, rows2 ); 
		colsread = IORD_ALTERA_AVALON_PIO_DATA
		(COLLS_BASE);       
		
		if(colsread == 7)   //possible 7,11,13,14
		{
			yE = yE + 4;
			ikrun(xE,yE,zE,angle4in);
		}

		if(colsread == 11)
		{  
			xE = 29;   //X position relative to centre of 				     // robot base
			yE = 0;    //Y position relative to centre of 				     //robot base
			zE = 15;   //Z
			elbow = 82;
			IOWR_ALTERA_AVALON_PWM_DUTY(MY_PWM_2_BASE, elbow);
			ikrun(xE,yE,zE,angle4in);
		}

		if(colsread == 13)
		{
			yE = yE - 4;
			ikrun(xE,yE,zE,angle4in);  
		} 
		
		///////////////////////////////ROW3///////////////////////////// 
		IOWR_ALTERA_AVALON_PIO_DATA(ROWS_BASE, rows3 ); 
		colsread = IORD_ALTERA_AVALON_PIO_DATA 	   		        (COLLS_BASE);       
		
		if(colsread == 7)   //possible 7,11,13,14
		{

		}

		if(colsread == 11)
		{  
			zE = zE + 3;
			ikrun(xE,yE,zE,angle4in);
		}

		if(colsread == 13)
		{
		}
 
		///////////////////////////////ROW4///////////////////////////// 
		IOWR_ALTERA_AVALON_PIO_DATA(ROWS_BASE, rows4 ); 
		colsread = IORD_ALTERA_AVALON_PIO_DATA				   (COLLS_BASE);       
		
		if(colsread == 7)   //possible 7,11,13,14
		{
			angle4in = 0;
			ikrun(xE,yE,zE,angle4in);
		}

		if(colsread == 11)
		{  
		}

		if(colsread == 13)
		{
			angle4in = 1;
			ikrun(xE,yE,zE,angle4in);
		} 
	}
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
	angle1  = angle1 *(180/pi);  

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
		angle3a = (atan2(s3a,c3))/(pi/180);
		angle3b = (atan2(s3b,c3))/(pi/180);
	}
  

	//angle 2
	cq3 = cos((angle3a)*(pi/180));
	sq3 = sin((angle3a)*(pi/180));
	k1 = l2+l3*c3;
	k2 = l3*s3b;
	r = sqrt((k1*k1) + (k2*k2));
	val5 = (z4ik/r)/(x4ik/r);
	val6 = (k2/k1);
	angle2a = (atan2((z4ik/r),(x4ik/r)))-(atan2(k2,k1));
	angle2a = angle2a/(pi/180);
	val7 = (z4ik/r)/(x4ik/r);
	val8 = ((-1*k2)/k1);
	angle2b = (atan2((z4ik/r),(x4ik/r)))-(atan2((-1*k2),k1));
	angle2b = angle2b/(pi/180);
    
	angle1int = (int)floor(angle1);
	angle2bint = (int)floor(angle2b);
	angle3aint = (int)floor(angle3a);
	angle4int = angle4in;  
            
	int angle2aint = (int)floor(angle2a);
	int angle3bint = (int)floor(angle3b);

	/*Before sending the move command to the EMU arm confirm it is within the operating envelope of the servos*/
            
	if((angle1int <= 45 && angle1int >= -45) && (angle2bint <= 45 && angle2bint >= -45) )
  	{
		base = map(angle1int,-45,45,50,100);
		shoulder= map(angle2bint,45,-45,50,100);
		gripper = map(angle4int,0,1,50,100);
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
                       
	IOWR_ALTERA_AVALON_PWM_DUTY(MY_PWM_0_BASE, base);
	IOWR_ALTERA_AVALON_PWM_DUTY(MY_PWM_1_BASE, shoulder);
	IOWR_ALTERA_AVALON_PWM_DUTY(MY_PWM_3_BASE, gripper);             
}

/*Mapping function from Arduino Library*/    
 
/****************************************************
 * Map
 ***************************************************/
int map(int x, int in_min, int in_max, int out_min, int out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/*Some random function that is required by the PWM module*/
/*************************************************
 * Send Task
 ************************************************/

void vConfigureTimerForRunTimeStats(void)
{
     /* use alt_timestamp_start, alt_timestamp,alt_timestamp_freq */    
     alt_timestamp_start();
 }
 
