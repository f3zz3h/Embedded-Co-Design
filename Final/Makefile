CC = gcc
CFLAGS = -Wall -O2
LIBS = -lm
INCLUDES= 
TARGET = ik
SRCS = main.c emu.c keypad.c pwm.c  
#lcd.c
OBJS = $(SRCS:.c=.o)

.PHONY: depend clean

all: $(TARGET)
	@echo Complete.

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^