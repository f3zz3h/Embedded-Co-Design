#ifndef EMU_H
#define EMU_H

//Inverse Kinematic Specific Definitions
#define l1  0.15    //link1 - between floor and robot base
#define l2  0.1     //link2
#define l3  0.12    //link3
#define l4  0.07    //link4

#define SERVO_MIN 21
#define SERVO_MAX 34
#define SERVO_MID 28

#define	XE_START 29
#define YE_START 0 
#define	ZE_START 15

#define BASE 0
#define SHOULDER 1
#define ELBOW 2
#define GRIPPER 3

/* ToDO: This should allow us to us M_PI but it doesn't seem to work with some compilers
 * so for now we are defining it ourselves
 */
#define _USE_MATH_DEFINES
//#define M_PI 3.14
 
/* *********************************************************
 * GLOBAL VARIABLES -- ToDo: Shouldn't really be doing assignments in headers.
 * 			We should check that these are always initialised before use in code and then
 * 			remove assignments here
 * ********************************************************* */
/////////////////////////ANGLES///////////////////////////
double angle1;     //angle of rotation at the base
double angle2a;    //1st possible angle of rotation at 					  //shoulder 
double angle2b;    //2nd possible angle of rotation at 					  //shoulder 
double angle3a;    //1st possible angle of rotation at 					  //elbow  - matches 2b
double angle3b;    //2nd possible angle of rotation at 					  //elbow - matches 2a
double angle4a;    //wrist angle - place holder, never 					  //used

double xEik;
double yEik;
double zEik;

double x4ik;
double y4ik;
double z4ik;
double cq1;
double sq1;
double x4;  //wrist X positions 
double y4;  //wrist Y positions 
double z4;  //wrist Z positions 

double c3;
double s3a;
double s3b;

double cq3;
double sq3;
double k1;
double k2;
double r;
double val5;
double val6;
double val7;
double val8;

double xEholder;
double yEholder;
double zEholder;

int angle1int, angle2bint, angle3aint, angle4int;

double xE;    //X position relative to centre of robot base
double yE;    //Y position relative to centre of robot base
double zE;    //Z position relative to centre of robot base

int base, shoulder, elbow, gripper;
int angle4in;

#endif
