/*
 * mem_copy.c
 *
 * 5/10/07 D. W. Hawkins (dwh@ovro.caltech.edu)
 *
 * Copy a /dev/mem mapped area to a file.
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

/* Mapped address and size */
static char        *mem_addr  = NULL;
static unsigned int mem_phys  = 0;
static unsigned int mem_pages = 0; 
static unsigned int mem_size = 0; 

static void show_usage()
{
	printf("\nUsage: mem_copy -a <address> -n <bytes>\n"\
		 "  -h            Help (this message)\n"\
		 "  -a <address>  Hex address of start of memory\n"\
		 "  -n <bytes>    Number of bytes to copy\n"\
		 "  -f <name>     Filename\n\n");
}

int main(int argc, char *argv[])
{
	int opt;       /* getopt processing         */
	int fd[2];     /* /dev/mem and file descriptors  */
	int page_size = getpagesize();
	int status;
	int byte_count = 0;
	char *filename = NULL;
	char *file_addr;
	
	while ((opt = getopt(argc, argv, "a:f:hn:")) != -1) {
		switch (opt) {
			case 'a':
				status = sscanf(optarg, "%X", &mem_phys);
				if (status != 1) {
					printf("Parse error for -a option\n");
					show_usage();
					return -1;
				}
				break;
			case 'f':
				filename = optarg;
				break;
			case 'h':
				show_usage();
				return -1;
			case 'n':
				byte_count = atoi(optarg);
				mem_pages = byte_count/page_size + 1;
				break;
			default:
				show_usage();
				return -1;
		}
	}
	if ((filename == NULL) || (byte_count == 0)) {
		show_usage();
		return -1;
	}
	mem_size = mem_pages*page_size;

	/* Open /dev/mem */
	printf(" * open /dev/mem\n");
	fd[0] = open("/dev/mem", O_RDWR | O_SYNC);
	if (fd[0] < 0) {
		printf("Open /dev/mem failed - %s\n", 
			strerror(errno));
		return -1;
	}

	/* Open the output file */
	printf(" * open %s\n", filename);
	fd[1] = open(
			filename, 
			O_RDWR  | O_CREAT, 
			S_IRUSR | S_IWUSR |
			S_IRGRP | S_IWGRP |
			S_IROTH | S_IWOTH);
	if (fd[1] < 0) {
		printf("File open failed - %s\n", strerror(errno));
		close(fd[0]);
		return -1;
	}
	/* Force the file size to increase */
	ftruncate(fd[1], byte_count);
	
	/* Map to process address space */
	printf(" * map /dev/mem\n");
	mem_addr = (char *)mmap(
		0,
		mem_size,
		PROT_READ|PROT_WRITE,
		MAP_SHARED,
		fd[0],
		mem_phys);
	if (mem_addr == (char *)MAP_FAILED) {
		printf("Error: /dev/mem mmap failed\n");
		close(fd[0]);
		close(fd[1]);
		return -1;
	}
	printf(" * map %s\n", filename);
	file_addr = (char *)mmap(
		0,
		mem_size,
		PROT_READ|PROT_WRITE,
		MAP_SHARED,
		fd[1],
		0);
	if (file_addr == (char *)MAP_FAILED) {
		printf("Error: file mmap failed\n");
		munmap((void *)mem_addr, mem_pages*page_size);
		close(fd[1]);
		close(fd[0]);
		return -1;
	}
	
	/* Copy the bytes */
	printf(" * copy %d-bytes\n", byte_count);
	memcpy(file_addr, mem_addr, byte_count);

	/* Cleanup */
	munmap((void *)mem_addr, mem_pages*page_size);
	munmap((void *)file_addr, mem_pages*page_size);
	close(fd[0]);
	close(fd[1]);
	return 0;
}

