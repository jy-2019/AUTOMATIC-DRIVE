#include"line_finder.h"
//#include<bluzzer.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
//#include <uart.h>
#include "ees350_moto.h"

#ifndef u32
#define u32 unsigned int
#endif
int my_line_finder_fd=0;


int my_line_finder_init()
{
    my_line_finder_fd = open("/dev/my_line_finder_driver", 0);
    if(my_line_finder_fd<0)
    {
        printf("line_finder:[ERROR] Can't open device.");
        return 0;
    }	
    printf("line_finder: Open device. File description of line_finder is %d\n",my_line_finder_fd);
	return 1;
}

int get_value()
{
	int sig_line=1;
	
	sig_line = ioctl(my_line_finder_fd, 0x80000000, 4);
	if(sig_line==0)
	return 0;
	else 
	return 1;
}
/*
void line_finder()
{
	int sig_value=1;
	//car_forward();
	usleep(200);
	while(1)
	{
		sig_value=get_value();
		if(sig_value ==0 )
		{  	
			write_uart('w'); 
			write_uart('h');
			write_uart('i');
			write_uart('t');
			write_uart('e');
			write_uart('\n');
			//car_stop();
            //bluzzer_on();	
			usleep(100000);    //delay
			//bluzzer_off();
        }
		else 
		{
			write_uart('b'); 
			write_uart('l');
			write_uart('a');
			write_uart('c');
			write_uart('k');
			write_uart('\n');
            //car_forward();
			usleep(200000);    //delay
        }
		if(dete_emp()==0)
		{
			break;
		}
	}
	//car_stop();
	
}*/
