#include"emio_initial.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>

#ifndef u32
#define u32 unsigned int
#endif
char temp[4];
int my_gpio_fd = 0;
int ret, rd32_buf;
int Miz702_EMIO_init(void)
{
    my_gpio_fd = open("/dev/my_gpio_driver", 0);
    if(my_gpio_fd<0)
    {
        printf("video:[ERROR] Can't open device.");
        return 0;
    }	
    printf("video: Open device. File description of my_gpio_video is %d\n",my_gpio_fd);

	return 1;
}

int Miz702_EMIO_stop(void)
{
    my_gpio_fd = close(my_gpio_fd);
	return 1;
}

void CLOCK_HIGH(void)
{
	u32 Value=0x1;
	rd32_buf = ioctl(my_gpio_fd, 0x80000000, 4);
	Value = rd32_buf | Value;
	ioctl(my_gpio_fd, 0, Value);
}

void CLOCK_LOW(void)
{
	u32 Value=0xfffe;
	rd32_buf = ioctl(my_gpio_fd, 0x80000000, 4);
	Value = rd32_buf & Value;
	ioctl(my_gpio_fd, 0, Value);
}

void DATA_HIGH(void)
{
	u32 Value=0x2;
	rd32_buf = ioctl(my_gpio_fd, 0x80000000, 4);
	Value = rd32_buf | Value;
	ioctl(my_gpio_fd, 0, Value);
}

void DATA_LOW(void)
{
	u32 Value=0xfffd;
	rd32_buf = ioctl(my_gpio_fd, 0x80000000, 4);
	Value = rd32_buf & Value;
	ioctl(my_gpio_fd, 0, Value);
}

