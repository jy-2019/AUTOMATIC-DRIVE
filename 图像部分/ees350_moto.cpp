/*
 *  ees350_moto.c
 *
 *  Created on: 20170125
 *  Author: Wenchao Liu
 */
#ifndef EES350_MOTO_H_
#define EES350_MOTO_H_
#endif

#include "ees350_moto.h"
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
int my_ultra_fd;
int my_car_r_fd;

// int zed_moto_init(PID *sptrL,PID *sptrR)
// {
// 	motodir_set( sptrL->bas_addr,0);
// 	moto_pwm_set( sptrL->bas_addr,0);

// 	motodir_set( sptrR->bas_addr,0);
// 	moto_pwm_set( sptrR->bas_addr,0);
// 	return XST_SUCCESS;
// }

/**************************
 * PID算法相关操作函数
 *************************/


/*
 * 转向设定
 * 0：正转
 * 1：反转
 * 这里从寄存器反馈的转速是一个霍尔传感器两个上升沿之间的累计单位脉冲个数，个数越少表示速度越快
 * 这里一个单位脉冲为10ns。既100MHZ，这里只能表示电机转子转一圈的时间。
 *
 * 测试经验值如下：

pwm = 0;	speed = 0;
pwm = 1000;	speed = 0;
pwm = 2000;	speed = 0;
pwm = 3000;	speed = 0;
pwm = 4000;	speed = 0;
pwm = 5000;	speed = 0;
pwm = 6000;	speed = 0;
pwm = 7000;	speed = 2731303;
pwm = 8000;	speed = 0;
pwm = 9000;	speed = 2485051;
pwm = 10000;	speed = 1207506;
pwm = 11000;	speed = 880402;
pwm = 12000;	speed = 674410;
pwm = 13000;	speed = 551379;
pwm = 14000;	speed = 464242;
pwm = 15000;	speed = 399156;
pwm = 16000;	speed = 350505;
pwm = 17000;	speed = 311761;
pwm = 18000;	speed = 282052;
pwm = 19000;	speed = 259159;
pwm = 20000;	speed = 239847;
pwm = 20000;	speed = 239973;
差不多pwm设置到10000的时候才能工作，速度最慢，当然精度可以再高，这里是个大概，12ms一圈。
pwm==20000时候，既全速运行时候速度最快约 2.39ms转一圈，这里是转子的转速不是轮子的转速
基本可确定转子的转速范围在0~1000/2.39之间。既0~415。00r/s之间。
后期设置PID参数时候，就以这个单位为准。PID的调节范围就在0~415r/s。不考虑轮子的转速
 * */
// int pid_speed_dir_get(PID *sptr,float *feedback_speed,  u32 *feedback_dir)
// {
// 	u32 speed_dir;
// 	u32 speed_num;//速度的计数个数
// 	float feedback_speed_t;

// 	speed_dir=Xil_In32(sptr->bas_addr+4);

// 	speed_num = speed_dir & 0x7fffffff;//轮子转一圈的单位时间计数个数。乘以单位时间后就是转一圈的时间
// 	*feedback_dir = (speed_dir & 0x80000000)>>31;//最高位31位为方向标志位
// 	if(speed_num !=0)
// 		feedback_speed_t = (float)1000000000/(speed_num*10);//1s的转圈数
// 	else feedback_speed_t = 0;

// 	if(feedback_speed_t>415) *feedback_speed = 415.00;
// 	else if(feedback_speed_t<0) *feedback_speed = 0;
// 	else *feedback_speed = feedback_speed_t;
// 	//	*feedback_speed = feedback_speed_t;
// 	return XST_SUCCESS;
// }


/*
 * 设置期望的转速值
 * speed范围：0~415
 * exp_speed :预期转速的存储变量地址
 * val ：预期转速值
 */
// int pid_expected_speed_set(PID *sptr,float val)
// {
// 	sptr->SetPoint = val;
// 	return XST_SUCCESS;
// }

/*
 * 设置PID的各个系数
 */
// int pid_set_Kp_Ki_Kd(PID *sptr,float Kp_val,float Ki_val,float Kd_val)
// {
// 	sptr->Proportion = Kp_val;
// 	sptr->Integral   = Ki_val;
// 	sptr->Derivative = Kd_val;
// 	return XST_SUCCESS;
// }

/*
 * 位置式PID计算
 * 输入：PID调节的当前采样值
 * 输出：位置式PID计算出的绝对位置值
 * 例如：goalvalue =pid_loc_calc(410),当前采样得到转速410rpm
 */
// u32 pid_loc_calc(PID *sptr,float speed_now)
// {
// 	u32 next_pwm_val;//pwm参数设定
// 	float ierror,derror;//当前误差

// 	ierror = sptr->SetPoint - speed_now;                     //偏差
// 	sptr->SumError += ierror;                                //积分
// 	derror = ierror - sptr->LastError;                       //微分

// 	//增量计算
// 	next_pwm_val =  (u32)(sptr->Proportion * ierror
// 				   +sptr->Integral * sptr->SumError
// 				   +sptr->Derivative * derror);
// 	printf("next_pwm_val = %d;\tierror = %.2f;\tSumError = %.2f;\tderror = %.2f;\n\r",next_pwm_val,ierror,sptr->SumError,derror);
// 	sptr->LastError = ierror;  //存储误差，用于下次计算
// 	//设置新的pwm参数，让其更逼近目标速度
// 	moto_pwm_set(sptr->bas_addr,next_pwm_val);
// 	return XST_SUCCESS;
// }

/*
 * 增量式PID参数初始化
 */
// void pid_inc_init(PID *sptr,u32 bas_addr)
// {
// 	sptr->SetPoint   =0;
// 	sptr->SumError   =0;

// 	sptr->Proportion =0;
// 	sptr->Integral   =0;
// 	sptr->Derivative =0;

// 	sptr->PrevError  =0;
// 	sptr->LastError  =0;

// 	sptr->bas_addr = bas_addr;
// }

////////////下面是最底层的硬件操作//////////////
/*
 * 转向设定
 * 0：正转
 * 1：反转
 * */
int motodir_set(u32 bas_addr,u32 dir)
{
	u32 val;
	val = (dir<<31);//防止修改低30位的速度值
	if (bas_addr == 0x43C00000)
	{
		ioctl(my_car_l_fd, 4, val);
	}
	else
	{
		ioctl(my_car_r_fd, 4, val);
	}
	// Xil_Out32(bas_addr+4, val);
	return 1;
}

/*
 * 底层的PL电路已经做过处理
 * 这里PWM的参数为0~20000
 * 对应周期为0~20ms
 * 这里是最SDK最底层设置转速的函数，仅仅是设置PWM的值，不是转速
 * */
int moto_pwm_set(u32 bas_addr,u32 pwm)
{
	//Xil_Out32(bas_addr+0, pwm);
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
	//Xil_Out32(ULTRASONIC_BASEADDR, 1);
	ioctl(my_ultra_fd, 0, 1);
	for(cnt = 1; cnt < 4; cnt ++){
		//while(!(Xil_In32(ULTRASONIC_BASEADDR) & (1 << cnt)));
		while(!(ioctl(my_ultra_fd, 0x80000000, 4) & (1 << cnt)));
		//*val = (float) Xil_In32(ULTRASONIC_BASEADDR + 4 * cnt) * ULTRASONIC_PARAM;
		temp = 4 * cnt;
		*val = ((float)ioctl(my_ultra_fd, 0x80000000 + temp, 4) * ULTRASONIC_PARAM);
		val ++;
	}

	return 1;
}

int eescar_ultra_get_sg(int num, float * val)
{
	unsigned int temp;

	//Xil_Out32(ULTRASONIC_BASEADDR, 1);
	ioctl(my_ultra_fd, 0, 1);
	//while(!(Xil_In32(ULTRASONIC_BASEADDR) & (1 << num)));
	while(!(ioctl(my_ultra_fd, 0x80000000, 4) & (1 << num)));
	temp = 4 * num;
	*val = (float) ioctl(my_ultra_fd, 0x80000000 + temp, 4) * ULTRASONIC_PARAM;
	return 1;
}

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
