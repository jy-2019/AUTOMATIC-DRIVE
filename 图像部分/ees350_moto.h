/*
 *  ees350_moto.c
 *
 *  Created on: 20170125
 *  Author: Wenchao Liu
 */

#ifndef EES350_MOTO_H_
#define EES350_MOTO_H_
#endif

#include <unistd.h>

#define MOTO_L_BASEADDR 0x43C00000
#define MOTO_R_BASEADDR 0x43C10000

#ifndef u32
#define u32 unsigned int
#endif
// #define MOTO_L_VAL_OFFSET (0+MOTO_L_BASEADDR) //�������������ƼĴ�����bit[0]=1��ת 0��ת
// #define MOTO_L_SPEED_OFFSET (4+MOTO_L_BASEADDR)//���ת��pwm����ֵ��0~20000��ģ���ڲ��з�Ƶ����ʾ0~20ms

// #define MOTO_R_VAL_OFFSET (0+MOTO_R_BASEADDR) //�������������ƼĴ�����bit[0]=1��ת 0��ת
// #define MOTO_R_SPEED_OFFSET (4+MOTO_R_BASEADDR)//���ת��pwm����ֵ��0~20000��ģ���ڲ��з�Ƶ����ʾ0~20ms

extern u32 ULTRASONIC_BASEADDR;

#define ULTRASONIC_PARAM 0.0017


//PID ���ݽṹ
typedef struct PID
{
	float  SetPoint;     //�趨Ŀ�� Desired Value
	float SumError;               //����ۼ�

	float  Proportion;         //�������� Proportional Const
	float  Integral;           //���ֳ��� Integral Const
	float  Derivative;         //΢�ֳ���Derivative Const

	float LastError;               //Error[-1]
	float PrevError;               //Error[-2]

	u32 bas_addr;
} PID;

/*
 * PID���ܺ���
 */
int pid_speed_dir_get(PID *sptr,float *feedback_speed,  u32 *feedback_dir);
int pid_expected_speed_set(PID *sptr,float val);
int pid_set_Kp_Ki_Kd(PID *sptr,float Kp_val,float Ki_val,float Kd_val);
u32 pid_loc_calc(PID *sptr,float speed_now);
void pid_inc_init(PID *sptr,u32 bas_addr);

/*
 * ��������ײ㺯��
 */
int zed_moto_init(PID *sptrL,PID *sptrR);
int motodir_set(u32 bas_addr,u32 dir);
int moto_pwm_set(u32 bas_addr,u32 pwm);

/****************************************************************************/
/**
* Init ultrasonic'base address.
*
** @return	return 0 if succeed
*
*****************************************************************************/
int eescar_ultra_init();

/****************************************************************************/
/**
* Get value of  ultrasonic module(all)
*
* @param	pointor array of integer,store 3 channel ultrasonic values
*
* @return	return 0 if succeed
*
*****************************************************************************/
int eescar_ultra_get_all(float *val);

/****************************************************************************/
/**
* Get value of  ultrasonic module(single)
*
* @param	num   ultrasonic's index
* @param    pointor  value store here
*
* @return	return 0 if succeed
*
*****************************************************************************/
int eescar_ultra_get_sg(int num, float * val);
int car_init(void);
//#endif /* ZED_MOTO_H_ */
