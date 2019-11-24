#include<moto.h>
#include<uart.h>
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


int my_car_l_fd;

int my_car_r_fd;


int car_init(void)
{
    my_car_l_fd = open("/dev/my_car_l_driver", 0);
    if(my_car_l_fd<0)
    {
        printf("car:[ERROR] Can't open device.");
        return 0;
    }	
    printf("car: Open device. File description of car_l is %d\n",my_car_l_fd);

    my_car_r_fd = open("/dev/my_car_r_driver", 0);
    if(my_car_r_fd<0)
    {
        printf("car:[ERROR] Can't open device.");
        return 0;
    }	
    printf("car: Open device. File description of car_r is %d\n",my_car_r_fd);

	return 1;
}

int motodir_set(u32 bas_addr,u32 dir)
{
	u32 val;
	val = (dir<<31); //防止修改低30位的速度值
	if (bas_addr == 0x43C00000)
	{
		ioctl(my_car_l_fd, 4, val);
	}
	else
	{
		ioctl(my_car_r_fd, 4, val);
	}
	return 1;
}


int moto_pwm_set(u32 bas_addr,u32 pwm)
{
	if (bas_addr == 0x43C00000)
	{
		ioctl(my_car_l_fd, 0, pwm);
	}
	else
	{
		ioctl(my_car_r_fd, 0, pwm);
	}
	return 1;
}
void car_stop()
{
    motodir_set(0x43C00000,0);
    moto_pwm_set(0x43C00000,0);

    motodir_set(0x43C10000,0);
    moto_pwm_set(0x43C10000,0);
}
void car_forward()
{
    motodir_set(0x43C00000,1);
    moto_pwm_set(0x43C00000,7150);

    motodir_set(0x43C10000,0);
    moto_pwm_set(0x43C10000,7000);
}
void car_back()
{
    motodir_set(0x43C00000,0);
    moto_pwm_set(0x43C00000,10000);

    motodir_set(0x43C10000,1);
    moto_pwm_set(0x43C10000,10000);
}
void car_right()
{
    motodir_set(0x43C00000,1);
    moto_pwm_set(0x43C00000,10000);

    motodir_set(0x43C10000,0);
    moto_pwm_set(0x43C10000,2000);
}
void car_left()
{
    motodir_set(0x43C00000,1);
    moto_pwm_set(0x43C00000,2000);

    motodir_set(0x43C10000,0);
    moto_pwm_set(0x43C10000,10000);//right
}
void avoid_right()
{
    motodir_set(0x43C00000,1);
    moto_pwm_set(0x43C00000,10000);

    motodir_set(0x43C10000,0);
    moto_pwm_set(0x43C10000,1000);
}
void avoid_left()
{
    motodir_set(0x43C00000,1);
    moto_pwm_set(0x43C00000,1000);

    motodir_set(0x43C10000,0);
    moto_pwm_set(0x43C10000,10000);
}
void avoid_forward()
{
    motodir_set(0x43C00000,1);
    moto_pwm_set(0x43C00000,3500);

    motodir_set(0x43C10000,0);
    moto_pwm_set(0x43C10000,3500);
}
void avoid_back()
{
    motodir_set(0x43C00000,0);
    moto_pwm_set(0x43C00000,8000);

    motodir_set(0x43C10000,1);
    moto_pwm_set(0x43C10000,8000);
}
void speed_up(int vu)
{
    motodir_set(0x43C00000,1);
    moto_pwm_set(0x43C00000,10000);

    motodir_set(0x43C10000,0);
    moto_pwm_set(0x43C10000,10000);
}
void speed_down(int vd)
{
    motodir_set(0x43C00000,1);
    moto_pwm_set(0x43C00000,3000);

    motodir_set(0x43C10000,0);
    moto_pwm_set(0x43C10000,3000);
}
void speed1()
{
    motodir_set(0x43C00000,1);//
    moto_pwm_set(0x43C00000,3150);

    motodir_set(0x43C10000,0);//
    moto_pwm_set(0x43C10000,3000);
}
void speed2()
{
    motodir_set(0x43C00000,1);//
    moto_pwm_set(0x43C00000,5150);

    motodir_set(0x43C10000,0);//
    moto_pwm_set(0x43C10000,5000);
}
void speed3()
{
    motodir_set(0x43C00000,1);//
    moto_pwm_set(0x43C00000,7150);

    motodir_set(0x43C10000,0);//
    moto_pwm_set(0x43C10000,7000);
}
void speed4()
{
    motodir_set(0x43C00000,1);//
    moto_pwm_set(0x43C00000,10150);

    motodir_set(0x43C10000,0);//
    moto_pwm_set(0x43C10000,10000);
}

