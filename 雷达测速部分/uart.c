#include<uart.h>
#include<time.h>

int my_uart_inital()
{
    my_uart_fd = open("/dev/my_uart_driver", 0);

    if(my_uart_fd<0)
    {
        printf("uart:[ERROR] Can't open device.");
        return 0;
    }	
    printf("blueteeth: Open device. File description of blueteeth is %d",my_uart_fd);
   
	printf("uart ioctl frist\n");
	ioctl(my_uart_fd, 0x0c, 0x00000003);	
	printf("uart ioctl second\n");
	ioctl(my_uart_fd, 0x0c, 0x00000000);
	return 1;	
}

char receive_uart()
{
	char b='0';
    int data=1;//,j=0;
	data=ioctl(my_uart_fd, 0x80000000+0x08, 4);
	while((0x00000001&ioctl(my_uart_fd, 0x80000000+0x08, 4))==0)
	{;
	}
	data=ioctl(my_uart_fd, 0x80000000, 4);
	b=data&0x000000ff;
	

return b;
}

void write_uart(char data)
	{
	while((0x00000008&ioctl(my_uart_fd, 0x80000000+0x08, 4))!=0)
	{
		;
	} 
	ioctl(my_uart_fd,0x04,data);
}

int dete_emp(){
	if((0x00000001&ioctl(my_uart_fd, 0x80000000+0x08, 4))==0){
        return 1;
	}
	else 
		return 0;
}



int trans(float data,char* tr)
    
 {       
       int x,y,n,i;
        n=0;
        x=y=0;
        i=0;
	x=(int)(data*1000000+0.5);
	//printf("%d\n",x);
	y=x;
	while(x)
	{
		x=x/10;
		n++;
	}
	//printf("%d\n",n);
	for(i=0;i<6;i++)
	{
		
			tr[i]=y%10+0x30;
			y=y/10;
	}
    tr[6]=0x2e;
for(i=7;i<n+1;i++)
{

			tr[i]=y%10+0x30;
			y=y/10;
}
return n;
}




