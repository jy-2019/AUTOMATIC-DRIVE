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
 * PID�㷨��ز�������
 *************************/


/*
 * ת���趨
 * 0����ת
 * 1����ת
 * ����ӼĴ���������ת����һ����������������������֮����ۼƵ�λ�������������Խ�ٱ�ʾ�ٶ�Խ��
 * ����һ����λ����Ϊ10ns����100MHZ������ֻ�ܱ�ʾ���ת��תһȦ��ʱ�䡣
 *
 * ���Ծ���ֵ���£�

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
���pwm���õ�10000��ʱ����ܹ������ٶ���������Ȼ���ȿ����ٸߣ������Ǹ���ţ�12msһȦ��
pwm==20000ʱ�򣬼�ȫ������ʱ���ٶ����Լ 2.39msתһȦ��������ת�ӵ�ת�ٲ������ӵ�ת��
������ȷ��ת�ӵ�ת�ٷ�Χ��0~1000/2.39֮�䡣��0~415��00r/s֮�䡣
��������PID����ʱ�򣬾��������λΪ׼��PID�ĵ��ڷ�Χ����0~415r/s�����������ӵ�ת��
 * */
// int pid_speed_dir_get(PID *sptr,float *feedback_speed,  u32 *feedback_dir)
// {
// 	u32 speed_dir;
// 	u32 speed_num;//�ٶȵļ�������
// 	float feedback_speed_t;

// 	speed_dir=Xil_In32(sptr->bas_addr+4);

// 	speed_num = speed_dir & 0x7fffffff;//����תһȦ�ĵ�λʱ��������������Ե�λʱ������תһȦ��ʱ��
// 	*feedback_dir = (speed_dir & 0x80000000)>>31;//���λ31λΪ�����־λ
// 	if(speed_num !=0)
// 		feedback_speed_t = (float)1000000000/(speed_num*10);//1s��תȦ��
// 	else feedback_speed_t = 0;

// 	if(feedback_speed_t>415) *feedback_speed = 415.00;
// 	else if(feedback_speed_t<0) *feedback_speed = 0;
// 	else *feedback_speed = feedback_speed_t;
// 	//	*feedback_speed = feedback_speed_t;
// 	return XST_SUCCESS;
// }


/*
 * ����������ת��ֵ
 * speed��Χ��0~415
 * exp_speed :Ԥ��ת�ٵĴ洢������ַ
 * val ��Ԥ��ת��ֵ
 */
// int pid_expected_speed_set(PID *sptr,float val)
// {
// 	sptr->SetPoint = val;
// 	return XST_SUCCESS;
// }

/*
 * ����PID�ĸ���ϵ��
 */
// int pid_set_Kp_Ki_Kd(PID *sptr,float Kp_val,float Ki_val,float Kd_val)
// {
// 	sptr->Proportion = Kp_val;
// 	sptr->Integral   = Ki_val;
// 	sptr->Derivative = Kd_val;
// 	return XST_SUCCESS;
// }

/*
 * λ��ʽPID����
 * ���룺PID���ڵĵ�ǰ����ֵ
 * �����λ��ʽPID������ľ���λ��ֵ
 * ���磺goalvalue =pid_loc_calc(410),��ǰ�����õ�ת��410rpm
 */
// u32 pid_loc_calc(PID *sptr,float speed_now)
// {
// 	u32 next_pwm_val;//pwm�����趨
// 	float ierror,derror;//��ǰ���

// 	ierror = sptr->SetPoint - speed_now;                     //ƫ��
// 	sptr->SumError += ierror;                                //����
// 	derror = ierror - sptr->LastError;                       //΢��

// 	//��������
// 	next_pwm_val =  (u32)(sptr->Proportion * ierror
// 				   +sptr->Integral * sptr->SumError
// 				   +sptr->Derivative * derror);
// 	printf("next_pwm_val = %d;\tierror = %.2f;\tSumError = %.2f;\tderror = %.2f;\n\r",next_pwm_val,ierror,sptr->SumError,derror);
// 	sptr->LastError = ierror;  //�洢�������´μ���
// 	//�����µ�pwm������������ƽ�Ŀ���ٶ�
// 	moto_pwm_set(sptr->bas_addr,next_pwm_val);
// 	return XST_SUCCESS;
// }

/*
 * ����ʽPID������ʼ��
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

////////////��������ײ��Ӳ������//////////////
/*
 * ת���趨
 * 0����ת
 * 1����ת
 * */
int motodir_set(u32 bas_addr,u32 dir)
{
	u32 val;
	val = (dir<<31);//��ֹ�޸ĵ�30λ���ٶ�ֵ
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
 * �ײ��PL��·�Ѿ���������
 * ����PWM�Ĳ���Ϊ0~20000
 * ��Ӧ����Ϊ0~20ms
 * ��������SDK��ײ�����ת�ٵĺ���������������PWM��ֵ������ת��
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
