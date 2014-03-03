/* Based on Nigel's toggle_leds.c

Takes an integer argument and writes as a hex value to PWM registers implemented on the Cyclone II FPGA for the TS7300

BUGS: Currently only supports 1 out of the 4 PWM registers
Argument has to be integer format

Team members
Nathan Toft
Peter Smith
Jake Baker

 */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>   /* toupper */
#include <signal.h>


int main(int argc, char *argv[])
{
    int fd;        /* /dev/mem file descriptor  */
    int page_size = getpagesize();
    unsigned int *mem_addr  = NULL;
    unsigned int mem_phys  = 0x72A00000; /*base for opencore reg */

    volatile unsigned int *pwm_A;
    /*volatile unsigned int *pwm_B;
    volatile unsigned int *pwm_C;
    volatile unsigned int *pwm_D;*/
    volatile unsigned int *pwm_reset_n;
    volatile unsigned int *pwm_A_enable;

    if (argc < 2)
    {
        printf("Enter a PWM value you derp\n"
            "USAGE: pwm [n]\n"
            "example: ./pwm 127\n");
        return 0;
    }

    /* Open a page at the FPGA base address */
    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        printf("Open /dev/mem failed - %s %d\n",
            strerror(errno),errno);
        return -1;
    }
    mem_addr = (int *)mmap(
        0,
        page_size,
        PROT_READ|PROT_WRITE,
        MAP_SHARED,
        fd,
        mem_phys);
    if (mem_addr == (unsigned int *)MAP_FAILED) {
        printf("Error: mmap failed\n");
        close(fd);
        return -1;
    }
    printf("%x: %x \n",(int)mem_addr,*(mem_addr));

    /**********************************************
     * set up the pointers to the registers that
     * have been created in the fpga. Then make
     * all configurable pins outputs.
     */

    /* IO registers */
    pwm_A = mem_addr + 0x06;
    /*pwm_B = mem_addr + 0x07;
    pwm_C = mem_addr + 0x08;
    pwm_D = mem_addr + 0x09;*/
    pwm_reset_n = mem_addr + 0x0A;
    pwm_A_enable = mem_addr + 0x0B;

    /*Enable PWM registers if previously disabled*/
    *pwm_A_enable = 0x1;

    printf("Writing PWM Value 0x%x to header pin 1\n" ,atoi(argv[1]));
    *pwm_A = atoi(argv[1]);
    usleep(500000);
    /**pwm_A = atoi(argv[2]);
       usleep(500000);*/
    *pwm_A_enable = 0x0;
    //*pwm_A = 0x0000003F;
    //*pwm_A = 0xDD;
    /*while(1)
    {
        //*pwm_A = atoi(argv[1]);
        usleep(1000);
        *pwm_A = 0xFF;
        usleep(1000);
        *pwm_A = 0x0;
        usleep(1000);
        *pwm_A = 0x2D;
        }    */
    return 0;
}