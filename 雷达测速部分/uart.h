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
int my_uart_inital();
void write_uart(char);
char receive_uart();
int dete_emp();
int trans(float a,char* tr);
    
