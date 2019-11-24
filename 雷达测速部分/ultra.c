#include "ultra.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>

int my_ultra_fd;

int eescar_ultra_init()
{
    my_ultra_fd = open("/dev/my_ultra_driver", 0);
    if(my_ultra_fd<0)
    {
        printf("ultra:[ERROR] Can't open device.");
        return 0;
    }	
    printf("ultra: Open device. File description of car_l is %d\n",my_ultra_fd);

	return 1;
}

int eescar_ultra_get_all(float *val)
{
	char cnt;
	unsigned int temp;
	ioctl(my_ultra_fd, 0, 1);
	for(cnt = 1; cnt < 4; cnt ++){
		while(!(ioctl(my_ultra_fd, 0x80000000, 4) & (1 << cnt)));
		temp = 4 * cnt;
		*val = ((float)ioctl(my_ultra_fd, 0x80000000 + temp, 4) * ULTRASONIC_PARAM);
		val ++;
	}

	return 1;
}

int eescar_ultra_get_sg(int num, float * val)
{
	unsigned int temp;

	ioctl(my_ultra_fd, 0, 1);
	while(!(ioctl(my_ultra_fd, 0x80000000, 4) & (1 << num)));
	temp = 4 * num;
	*val = (float) ioctl(my_ultra_fd, 0x80000000 + temp, 4) * ULTRASONIC_PARAM;
	return 1;
}


