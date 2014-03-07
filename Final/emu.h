
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

#define _USE_MATH_DEFINES

/* functions */
int map(int x, int in_min, int in_max, int out_min, int out_max);
void ikrun(double xE, double yE, double zE,int angle4in);

 
/* *********************************************************
 * GLOBAL VARIABLES
 * ********************************************************* */
/////////////////////////ANGLES///////////////////////////
double angle1 = 0;     //angle of rotation at the base
double angle2a = 0;    //1st possible angle of rotation at 					  //shoulder 
double angle2b = 0;    //2nd possible angle of rotation at 					  //shoulder 
double angle3a = 0;    //1st possible angle of rotation at 					  //elbow  - matches 2b
double angle3b = 0;    //2nd possible angle of rotation at 					  //elbow - matches 2a
double angle4a = 0;    //wrist angle - place holder, never 					  //used

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

int angle1int, angle2bint, angle3aint, angle4int;

double xE;    //X position relative to centre of robot base
double yE;    //Y position relative to centre of robot base
double zE;    //Z position relative to centre of robot base

int base = 0, shoulder = 0, elbow = 0, gripper = 0;
