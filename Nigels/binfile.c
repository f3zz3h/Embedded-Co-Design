/* binfile.c
 * 
 * This program is used to create a binary file for
 * testing od, dd, etc. with a known binary file.
 * These tools can then be used on /dev/mem mapped
 * devices to extract binary data, or code images.
 *
 * Create the file using; ./binfile
 *
 * Then display data from the file using, eg.
 *
 * od -Ax -tx4 -N0x40 binfile.dat
 *
 * the start-point can be changed using -j, i.e.,
 *
 * od -Ax -tx4 -N0x40 -j0x20 binfile.dat
 *
 * Data can be copied using dd, eg.
 *
 * dd if=bindata.dat of=test.dat bs=16 skip=2 count=4
 *
 * copies data in blocks of 16-bytes, skips the first
 * two blocks, and then copies four blocks.
 * 
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define LENGTH 0x1000

int main(int argc, char *argv[])
{
	int i, fd, len, status;
	int buffer[LENGTH];
	for (i = 0; i < LENGTH; i++) {
		buffer[i] = 4*i; /* data = byte address */
	}
	fd = open("binfile.dat", O_RDWR|O_CREAT, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		printf("File open failed - %s\n", strerror(errno));
		return -1;
	}
	len = LENGTH*sizeof(int); 
	status = write(fd, buffer, len);
	if (status < 0) {
		printf("File write failed - %s\n", strerror(errno));
		return -1;
	}
	if (status != len) {
		printf("File write was short\n");
		return -1;
	}
	return 0;
}

