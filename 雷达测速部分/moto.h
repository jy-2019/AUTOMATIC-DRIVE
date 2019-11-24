#include "ultra.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <emio_initial.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#ifndef u32
#define u32 unsigned int
#endif
#ifndef u8
#define u8 unsigned char
#endif

#define DDR_BASEADDR        0x30000000
#define H_STRIDE            1280
#define H_ACTIVE            1280
#define V_ACTIVE            720
#define pi					3.14159265358


int motodir_set(u32 bas_addr,u32 dir);
int moto_pwm_set(u32 bas_addr,u32 pwm);

void car_stop();
void car_forward();
void car_back();
void car_left();
void car_right();
void avoid_left();
void avoid_right();
void avoid_forward();
void avoid_back();
void speed_up();
void speed_down();
void speed1();
void speed2();
void speed3();
void speed4();
