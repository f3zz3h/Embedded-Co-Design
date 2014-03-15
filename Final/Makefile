CC = gcc
CFLAGS = -Wall -O2
LIBS = -lm -pthread
TARGET = release-0.1
SRCS = main.c emu.c keypad.c pwm.c  
# // ToDo: lcd.c needs adding to the mix
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
