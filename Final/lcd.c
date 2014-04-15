/* ********************************************************
 * Project Title: EMU Pick & Place
 * File: lcd.c
 * Description: This file contains the fundamental functions
 * for writing to the LCD screen attached to the pick and 
 * place system.
 * Authors: Luke Hart, Joe Ellis, Kerrim Morris & Lukasz Matczak
 * Last edited: 14/04/2014
 * ******************************************************* */
#include "global.h"
#include "lcd.h"
/* ********************************************************
 * void lcdinit();
 * Initialises the LCD screen for use with the TS-7300.
 * Last edited: 14/04/2014
 * ******************************************************* */
void lcdinit() {

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

/* ********************************************************
 * void command();
 * Used for port set up on the TS-7300 and sending instructions
 * to the LCD screen.
 * Last edited: 14/04/2014
 * ******************************************************* */
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

	//De-assert RS, assert WR
	ctrl &= ~0x30;
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

/* ********************************************************
 * void* writechars();
 * Writes the message stored in the *lcdMsg global to the
 * LCD screen.
 * Last edited: 14/04/2014
 * ******************************************************* */
int writechars(char *lcdMsg)
{
	int i;
	unsigned int ctrl = *phdr;

	command(0x1);

	while (*lcdMsg); {
		*paddr = *paddr | PAMASK; //set port A to outputs
		*pcddr = *pcddr | PCMASK; //set port C to outputs

		/* step 1, apply RS & WR, send data */
		*padr = *padr & ~PAMASK;
		*pcdr = *pcdr & ~PCMASK;
		*padr = *padr | (*lcdMsg & PAMASK);
		*pcdr = *pcdr | ((*lcdMsg >> 7) & PCMASK);

		/* Incrementing the character pointer */
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
	}
}
/* ********************************************************
 * void COUNTDOWN(int limit);
 * Parameters: limit - The time limit to countdown by.
 * Imitates the COUNTDOWN macro used when using the LCD
 * screen on ARM architectures.
 * USE THIS FOR IN COMPILING ON x86.
 * Last edited: 14/04/2014
 * ********************************************************/
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
