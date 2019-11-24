#include "my_uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>

int my_uart_fd;

int my_uart_inital(){
    my_uart_fd = open("/dev/my_uart_driver", 0);
    if(my_uart_fd<0)
    {
        printf("uart:[ERROR] Can't open device.");
        return 0;
    }	
    printf("uart: Open device. File description of uart is %d\n",my_uart_fd);
    
	ioctl(my_uart_fd, 0x000c, 0x00000003);	
	ioctl(my_uart_fd, 0x000c, 0x00000000);
	printf("done");
	return 1;	
}

char receive_uart(){
    int data;
	char b;
	while((0x00000001&ioctl(my_uart_fd, 0x80000000+0x08, 4))==0){
	;
	}
	data=ioctl(my_uart_fd, 0x80000000, 4);
	b=data&0x000000ff;
        return b;
}

void write_uart(char data){
	int b;
	b=data;
	while((0x00000008&ioctl(my_uart_fd, 0x80000000+0x08, 4))!=0){
	;
	}
	data=ioctl(my_uart_fd, 0x04, b);
}

int dete_emp(){
	if((0x00000001&ioctl(my_uart_fd, 0x80000000+0x08, 4))==0){
        return 1;
	}
	else 
		return 0;
}

/*char receive_uart0(){

    int data;

	char b;

	while((0x00000001&ioctl(my_uart_fd, 0x80000000+0x08, 4))==0){

	return 0;

	}

	data=ioctl(my_uart_fd, 0x80000000, 4);


	b=data&0x000000ff;


        return b;


}*/
