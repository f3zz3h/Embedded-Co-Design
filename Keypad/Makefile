# Makefile
#
# User-space programs makefile
#
# To build these programs for the ARM target, the
# environment variable CROSS_COMPILE must be defined.
# eg.
#
# export CROSS_COMPILE=arm-unknown-linux-gnu-
#
# and PATH needs to include the path to the compiler.
#

CC = $(CROSS_COMPILE)gcc
CFLAGS += -Wall -O2

# Test programs
PROGRAMS = \
	keypad

default: programs

programs: $(PROGRAMS)

clean:
	rm -rf $(PROGRAMS) *.o 
