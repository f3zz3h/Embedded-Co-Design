CC = gcc
CFLAGS = -Wall -O2
LIBS = -lm -pthread
TARGET = release-0.1
SRCS = main.c emu.c keypad.c pwm.c lcd.c replay.c
OBJS = $(SRCS:.c=.o)

.PHONY: depend clean

all: $(TARGET)
	@echo Complete.

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o *~ $(TARGET)

depend: $(SRCS)
	makedepend $(INCLUDES) $^
	
debug: CXX += -DDEBUG -g
debug: CC += -DDEBUG -g
debug: all

ikrun: CXX += -DIKRUN -g
ikrun: CC += -DIKRUN -g
ikrun: all
	
