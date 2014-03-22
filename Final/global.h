#ifndef GLOBAL_H
#define GLOBAL_H

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
#include <pthread.h>


#define BASE 0
#define SHOULDER 1
#define ELBOW 2
#define GRIPPER 3

#define X 0
#define Y 1
#define Z 2

#define SERVO_MIN 14
#define SERVO_MAX 25
#define SERVO_MID 20

int fd;        /* /dev/mem file descriptor  */

extern volatile unsigned int *mem_addr;
extern unsigned int mem_phys; /* base for opencore reg */
extern pthread_mutex_t keypad_mutex;

int key_val;

/* Function decelerations */
void Write_PWM(int output_servo, int value);
void* Read_Keypad();
int emu_map(int x);
void emu_ikrun(float xyz_pos[3], int* servo_vals);
void emu_intialize(float* xyz_pos, int* servo_vals);
int Read_PWM(int servo);

#endif
