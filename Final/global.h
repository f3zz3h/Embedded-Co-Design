/* ********************************************************
 * Project Title: EMU Pick & Place
 * File: global.h
 * Description: Global variables and Function defintions
 * for all parts of the application.
 *
 * Authors: Luke Hart, Joe Ellis, Kerrim Morris & Lukasz Matczak
 * Last edited: 14/04/2014
 * ******************************************************* */
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

#define INCREMENT 1
#define DECREMENT 0

#define UPMSG "UP"
#define FWDMSG "FORWARD"
#define DOWNMSG "DOWN"
#define LEFTMSG "LEFT"
#define RIGHTMSG "RIGHT"
#define RESETMSG "RESET"

int fd;        /* /dev/mem file descriptor  */

#define GPIOBASE	0x80840000
extern volatile unsigned int *gpio;

extern volatile unsigned int *mem_addr;
extern unsigned int mem_phys; /* base for opencore reg */
extern pthread_mutex_t keypad_mutex;

int key_val;

#define REPLAY_FILE "replay.txt"
FILE *path;

/* Function decelerations */
int initialization();
void Write_PWM(int* value);
void* Read_Keypad();
int emu_map(int x);
void emu_ikrun(float xyz_pos[3], int* servo_vals);
void emu_intialize(float* xyz_pos, int* servo_vals);
int Read_PWM(int servo);
void ik_update_xyz(float* xyz_pos,int which,int change);
void decrement_gripper(int* sVals);
void increment_gripper(int* sVals);
int move_closer(int current, int goal);
/* lcd */
int writechars(char *lcdMsg);
void lcdinit();
void lcd_message(char* msg);
/*replay */
int write_path(int* servo_vals);
int replay();
int new_file();

#endif
