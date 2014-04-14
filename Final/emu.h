/* ********************************************************
 * Project Title: EMU Pick & Place
 * File: ******
 * Description:
 *
 *
 * Authors: Luke Hart, Joe Ellis, Kerrim Morris & Lukasz Matczak
 * Last edited: 14/04/2014
 * ******************************************************* */

#ifndef EMU_H
#define EMU_H

//Inverse Kinematic Specific Definitions
#define l1  0.15    //link1 - between floor and robot base
#define l2  0.1     //link2
#define l3  0.12    //link3
#define l4  0.07    //link4

#define	X_START 0.29
#define Y_START 0.0
#define	Z_START 0.15

#define MIN_X 0.0995
#define MIN_Y -0.2051
#define MIN_Z 0.0400

#define MAX_X 0.29
#define MAX_Y 0.2051
#define MAX_Z 0.3407

#define MAX_ANGLE 45
#define MIN_ANGLE -45

#define NUM_OF_STEPS 100

#define _USE_MATH_DEFINES

float square(float x);

#endif
