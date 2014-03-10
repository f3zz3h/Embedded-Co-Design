#ifndef GLOBAL_H
#define GLOBAL_H

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
#include "emu.h"

/* Function decelerations */
void Write_PWM(int output_servo, int value);
int Read_Keypad();
int emu_map(int x, int in_min, int in_max, int out_min, int out_max);
void emu_ikrun(double xE, double yE, double zE, int angle4in);
void emu_intialize();

#endif
