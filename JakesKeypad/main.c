#include "keypad.h"

int KP_DEBUG_MODE = 0;
kp_handler *handlers = NULL;
pthread_mutex_t kp_handlers_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Helper function to print debug messages */
void printd(unsigned int level, const char* format, ...) {
	va_list arg;

	if(!KP_DEBUG_MODE)
		return;

	switch(level) {
		case KP_DEBUG:
			printf("[*] ");
			break;
		case KP_INFO:
			printf("[-] ");
			break;
		case KP_ERROR:
			printf("[!] ");
			break;
		default:	
			printf("[-] ");
	}

	va_start(arg, format);
	vprintf(format, arg);
	va_end(arg);
	printf("\n");
}

int kp_init() {
	int fd;	
	int page_size = getpagesize();
	unsigned int *mem_addr = NULL;
	unsigned int mem_phys = 0x72A00000;
	volatile unsigned int *enable_vga;
	volatile unsigned int *vga_reg;
	
	fd = open("/dev/mem", O_RDWR | O_SYNC);

	if(fd < 0) {
		printd(KP_ERROR, "Open /dev/mem failed - %s %d",
			strerror(errno), errno);
		return -1;
	}

	mem_addr = (int *)mmap(
		0,
		page_size,
		PROT_READ|PROT_WRITE,
		MAP_SHARED,
		fd,
		mem_phys);

	if(mem_addr == (unsigned int *)MAP_FAILED) {
		printd(KP_ERROR, "mmap failed");
		close(fd);	
		return -1;
	}

	printd(KP_INFO, "%x: %x", (int)mem_addr, *(mem_addr));

	vga_reg = mem_addr + 0x00;
	enable_vga = mem_addr + 0x01;

	*enable_vga = 0x000001E0;

	kp_loop(vga_reg);

	return 0;
}

void kp_loop(volatile unsigned int *port) {
// check keypad for input
}

/* 	This function uses ifs instead of switch case so the same
	handler can be registered for multiple events. */
void kp_register_handler(unsigned int event, void(*handler)(int)) {
	pthread_mutex_lock(&kp_handlers_mutex);
	
	if(handlers == NULL)
		handlers = (kp_handler *)malloc(sizeof(kp_handler));
	
	if(KP_BIT_SET(event, KP_KEY_PRESS_EVENT)) {
		printd(KP_DEBUG, "Registering key press event: %x", 
			handler);
		handlers->key_pressed = handler;
	}

	if(KP_BIT_SET(event, KP_KEY_DOWN_EVENT)) {
		printd(KP_DEBUG, "Registering key down event: %x", handler);
		handlers->key_down = handler;
	}

	if(KP_BIT_SET(event, KP_KEY_UP_EVENT)) {
		printd(KP_DEBUG, "Registering key up event: %x", handler);
		handlers->key_up = handler;
	}
	
	pthread_mutex_unlock(&kp_handlers_mutex);
}

void test_handler(int key) {
	printd(KP_INFO, "key pressed: %d", key);
}

int main(int argc, char **argv) {
	int c;

	while((c = getopt(argc, argv, "dh")) != EOF) {
		switch(c) {
			case 'd':
				KP_DEBUG_MODE = 1;
				break;
			default:
				break;
		}
	}

	printd(0, "Debug mode: %d", KP_DEBUG_MODE);

	/*
	if(kp_init() < 0) {
		printd(KP_ERROR, "Unable to initialize keypad.");
		return -1;
	}
	*/
	kp_register_handler(KP_KEY_PRESS_EVENT | KP_KEY_DOWN_EVENT, 
				test_handler);
	
	handlers->key_pressed(5);	
	
	return 0;
}
