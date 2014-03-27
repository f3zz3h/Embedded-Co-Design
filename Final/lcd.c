#include "global.h"
#include "lcd.h"

void lcdinit() {
	gpio = (unsigned int *) mmap(0, getpagesize(), PROT_READ | PROT_WRITE,
			MAP_SHARED, fd, GPIOBASE);

	phdr = &gpio[PHDR];
	padr = &gpio[PADR];
	paddr = &gpio[PADDR];

	pcdr = &gpio[PCDR];
	pcddr = &gpio[PCDDR];

	phddr = &gpio[PHDDR];

	*paddr & ~PAMASK; // port A to inputs
	*pcddr & ~PCMASK; // port C to inputs

	*phddr |= 0x38; // bits 3:5 of port H to outputs
	*phdr &= ~0x18; // de-assert EN, de-assert RS
	usleep(15000);
	command(0x38); // two rows, 5x7, 8 bit
	usleep(4100);
	command(0x38); // two rows, 5x7, 8 bit
	usleep(100);
	command(0x38); // two rows, 5x7, 8 bit
	command(0x6); // cursor increment mode
	usleep(2000);
	command(0x1); // clear display
	usleep(2000);
	command(0xC); // display on, blink off, cursor off
	usleep(2000);
	command(0x34); // Number of lines = 1 + font size = 5x11
	usleep(2000);
	command(0x2); // return home
}

void command(unsigned int cmd) {
	int i;
	unsigned int ctrl = *phdr;

	*paddr = *paddr | PAMASK; //set port A to outputs
	*pcddr = *pcddr | PCMASK; //set port C to outputs

	// step 1, apply RS & WR, send data
	*padr = *padr & ~PAMASK;
	*pcdr = *pcdr & ~PCMASK;
	*padr = (*padr & ~PAMASK) | (cmd & PAMASK);
	//if bit 7 of cmd is set the set bit 0 of pcdr
	//if bit 7 of cmd is clear then clear bit 0 of pcdr
	*pcdr = *pcdr | (cmd >> 7);

	ctrl &= ~0x30; // de-assert RS, assert WR
	*phdr = ctrl;

	// step 2, wait
	i = SETUP;
	COUNTDOWN(i);

	/* step 3, assert EN */
	ctrl |= 0x8;
	*phdr = ctrl;

	/*  step 4, wait */
	i = PULSE;
	COUNTDOWN(i);

	/* step 5, de-assert EN */
	ctrl &= ~0x8; /*  de-assert EN */
	*phdr = ctrl;

	/* step 6, wait */
	i = HOLD;
	COUNTDOWN(i);
}
void* writechars() {
	int i;
	unsigned int ctrl = *phdr;

	command(0x1);

	do {
		*paddr = *paddr | PAMASK; //set port A to outputs
		*pcddr = *pcddr | PCMASK; //set port C to outputs

		/* step 1, apply RS & WR, send data */

		*padr = *padr & ~PAMASK;
		*pcdr = *pcdr & ~PCMASK;
		*padr = *padr | (*lcdMsg & PAMASK);
		*pcdr = *pcdr | ((*lcdMsg >> 7) & PCMASK);

		/* Incrementing the character */
		*lcdMsg++;

		ctrl |= 0x10; /* assert RS */
		ctrl &= ~0x20; /* assert WR */
		*phdr = ctrl;

		// step 2
		i = SETUP;
		COUNTDOWN(i);

		/* step 3, assert EN */
		ctrl |= 0x8;
		*phdr = ctrl;

		// step 4, wait 800 nS
		i = PULSE;
		COUNTDOWN(i);

		/* step 5, de-assert EN */
		ctrl &= ~0x8; // de-assert EN
		*phdr = ctrl;

		/* step 6, wait */
		i = HOLD;
		COUNTDOWN(i);
	} while (*lcdMsg);

	pthread_exit(NULL);
}

void lcd_message(char* msg) {
	lcdMsg = malloc(strlen(msg) + 1);
	strcpy(lcdMsg, msg);
}
/* USE THIS FOR IN COMPILING ON x86
 *
 */
#ifdef DEBUG
void COUNTDOWN(int limit)
{
	int i;
	for (i = 0; i < limit; i++)
	{
		usleep(1);
	}
}
#endif
