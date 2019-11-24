#include"ledbar.h"
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
int my_ledbar_fd = 0;
int rd_buf;
int ledbar_init(void)
{
    my_ledbar_fd = open("/dev/my_ledbar_driver", 0);
    if(my_ledbar_fd<0)
    {
        printf("ledbar:[ERROR] Can't open device.");
        return 0;
    }	
    printf("ledbar: Open device. File description of my_ledbar is %d\n",my_ledbar_fd);

	return 1;
}

int ledbar_stop(void)
{
    my_ledbar_fd = close(my_ledbar_fd);
	return 1;
}


void C_HIGH(void)
{
	u32 Value=0x1;
	

	//temp = devmem('r',gpio_base_addr,0);
	rd_buf = ioctl(my_ledbar_fd, 0x80000000, 4);
	//rd32_buf = (int)temp[0] + ((int)temp[1]<<8) + ((int)temp[2]<<16) + ((int)temp[3]<<24);
	Value = rd_buf | Value;
	ioctl(my_ledbar_fd, 0, Value);
	//devmem('w',gpio_base_addr, Value);
}

void C_LOW(void)
{
	u32 Value=0xfffe;

	rd_buf = ioctl(my_ledbar_fd, 0x80000000, 4);
	//rd32_buf = (int)temp[0] + ((int)temp[1]<<8) + ((int)temp[2]<<16) + ((int)temp[3]<<24);
	Value = rd_buf & Value;
	ioctl(my_ledbar_fd, 0, Value);
}

void D_HIGH(void)
{
	u32 Value=0x2;

	rd_buf = ioctl(my_ledbar_fd, 0x80000000, 4);
	//rd32_buf = (int)temp[0] + ((int)temp[1]<<8) + ((int)temp[2]<<16) + ((int)temp[3]<<24);
	Value = rd_buf | Value;
	ioctl(my_ledbar_fd, 0, Value);
}

void D_LOW(void)
{
	u32 Value=0xfffd;

	rd_buf = ioctl(my_ledbar_fd, 0x80000000, 4);
	//rd32_buf = (int)temp[0] + ((int)temp[1]<<8) + ((int)temp[2]<<16) + ((int)temp[3]<<24);
	Value = rd_buf & Value;
	ioctl(my_ledbar_fd, 0, Value);
}

// Send the latch command
void latchData()
{
	D_LOW();
	usleep(10); 
	for (unsigned char i = 0; i < 4; i++)
	{
    	D_HIGH();
    	D_LOW();
	}
}



void sendData(unsigned int data)
{
	unsigned int state = 0;
	for (unsigned char i = 0; i < 16; i++)
	{  
		if ((data & 0x8000) != 0)
			D_HIGH();
		else
			D_LOW();
		
		state = 1-state;
		
		if (state != 0)
			C_HIGH();
		else
			C_LOW();
		data <<= 1;
	}
}

void setData(unsigned char __state[])
{bool __greenToRed=0;
  sendData(GLB_CMDMODE);
  for (unsigned char i = 0; i < 10; i++)
  {
    if (__greenToRed)
    {
	  // Go backward on __state
      sendData(__state[10-i-1]);
    }
    else
    {
	  // Go forward on __state
      sendData(__state[i]);
    }
  }
  // Two extra empty bits for padding the command to the correct length
  sendData(0x00);
  sendData(0x00);
  latchData();
}

void setBits(unsigned int bits)
{
unsigned char __state[10];
  for (unsigned char i = 0; i < 10; i++)
  {
    if ((bits % 2) == 1)
      __state[i] = 0xFF;
    else
      __state[i] = 0x00;
    bits /= 2;
  }
  setData(__state);
}