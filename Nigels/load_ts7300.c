#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

/* For memmem() */
#define __USE_GNU 
#include <string.h>

#include <fcntl.h>
#include <assert.h>
#include "peekpoke.h"

static int verbose = 0;

void usage(char **argv) {
	fprintf(stderr, "Usage: %s [OPTION] [BITSTREAM] ...\n"
	  "  -v                      Be verbose\n"
	  "  -h                      This help\n"
	  "\n" , argv[0]
	);
}

void reset_ts7300(unsigned long io_loc)
{
	POKE8(io_loc + 1, 0x0);
	usleep(30);
	POKE8(io_loc + 1, 0x2);
	usleep(80);
}

int load_ts7300(unsigned long io_loc, FILE *bitstream)
{
	int dat, len;
	int i = 0;

	while ((dat = fgetc(bitstream)) != EOF) {
		while(PEEK8(io_loc + 1) & 0x1);	
		POKE8(io_loc, dat);
	}
	usleep(1000);
	POKE8(io_loc + 1, (PEEK8(io_loc + 1) | 0x8));
	usleep(1000);
	POKE8(io_loc + 1, (PEEK8(io_loc + 1) & ~0x8));
	
	if (verbose) {
		printf("ts7300_loaded=%d\n", 
		  ((PEEK8(io_loc + 1) & 0x4) == 0x4));
	}
	
	return 0;
}

int main(int argc, char *argv[])
{
	unsigned long io_loc;
	FILE *bitstream;
	int i;
	int devmem = open("/dev/mem", O_RDWR|O_SYNC);
	char *optstring = "vh";

	while ((i = getopt(argc, argv, optstring)) != -1) {
		switch (i) {
		case 'v': 
			verbose = 1;
			break;
		default:
			usage(argv);
			return 4;
			break;
		}
	}

	if (optind >= argc) {
		usage(argv);
		return 5;
	} else {
		assert(devmem != -1);
		io_loc = (unsigned long)mmap(0, getpagesize(), 
			PROT_READ|PROT_WRITE, MAP_SHARED, devmem, 0x13c00000);
		assert((void *)io_loc != MAP_FAILED);
		if (strcmp(argv[optind], "-") == 0)
			bitstream = stdin;
		else
			bitstream = fopen(argv[optind], "r");
		if(bitstream != NULL) {
			reset_ts7300(io_loc);
			load_ts7300(io_loc, bitstream);
		} else {
			fprintf(stderr, "%s: can't open %s\n", argv[0], 
			  argv[1]);
			return 2;
		}
	}

	i = PEEK8(io_loc + 0x1);
	if (!(i & 0x4)) {
		fprintf(stderr, 
		  "%s: FPGA load failure: %x\n", argv[0], i);
		return 3;
	}
	return 0;
}
