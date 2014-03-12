#ifndef GLOBAL_H
#define GLOBAL_H

#define BASE 0
#define SHOULDER 1
#define ELBOW 2
#define GRIPPER 3

#define X 0
#define Y 1
#define Z 2

int fd;        /* /dev/mem file descriptor  */

extern volatile unsigned int *mem_addr;
extern unsigned int mem_phys; /*base for opencore reg */

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

/* Function decelerations */
void Write_PWM(int output_servo, int value);
int Read_Keypad();
int emu_map(int x);
void emu_ikrun(float* xyz_pos, float* current_xyz, int grabber_angle);
void emu_intialize();

#endif
