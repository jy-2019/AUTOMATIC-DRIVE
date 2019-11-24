#include <speed.h>
#include<time.h>

float speed(int my_dma_fd)
{
    float v=0;
    int  dev_mem_fd;
    void * map_base, * virt_addr;
    int  n;
    int i,m;
    short *pdata;
	
	printf("OK4!\n");
	fflush(stdout);
    if( ( dev_mem_fd = open( "/dev/mem", O_RDWR | O_SYNC ) ) == -1 ) FATAL;
    map_base = mmap( 0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, dev_mem_fd, RX_BUFFER_BASE & ~MAP_MASK );
    virt_addr = map_base + ( RX_BUFFER_BASE & MAP_MASK ); 
	m=0;
	printf("int ok!");
	fflush(stdout);    
        XAxiDma_Up( my_dma_fd, 128*8);
													printf("int1 ok!");
        fflush( stdout );
        pdata=virt_addr;
        for(i=0;i<128+4;i++)
		{
			x[i+m].real=*pdata;
			pdata++;
			x[i+m].img=*pdata;
			pdata++;
		}
														printf("int2 ok!");
													fflush(stdout);     

	printf("finish ok!");
	fflush(stdout);
		if( munmap( map_base, MAP_SIZE ) == -1 ) FATAL;
		close(dev_mem_fd); 
    printf("Result:\n");
	printf("OK5!");
	fflush(stdout);
printf("%f,%f,%f,%f,%f,%f,%f,%f,%f\n",x[0].img,x[1].img,x[2].img,x[3].img,x[4].img,x[5].img,x[6].img,x[7].img,x[8].img);
printf("%f,%f,%f,%f,%f,%f,%f,%f,%f\n",x[0].real,x[1].real,x[2].real,x[3].real,x[4].real,x[5].real,x[6].real,x[7].real,x[8].real);
    v=output();//调用输出傅里叶变换结果函数  
return v;
}

int XAxiDma_Up_Init()
{
    int my_dma_fd;
	
    my_dma_fd = open("/dev/my_dma_driver", 0);
    if(my_dma_fd < 0)
    {
        printf("dma:[ERROR] Can't open device.");
        return  0;
    } 
    printf("dma: Open device. File description of my_dma_fd is %d\n", my_dma_fd);
    int j;
    j = ioctl( my_dma_fd, 0x80000030, 0 );
    ioctl( my_dma_fd, 0x00000030, j &  (~XAXIDMA_IRQ_ALL_MASK) );  
    if( !( ioctl( my_dma_fd, 0x80000034, 0 ) & 0x00000001 ) )
	      if( ( ioctl( my_dma_fd, 0x80000034, 0 ) & 0x00000002 ) == 0 )
	      {
	          printf("DMA is busy\r\n");
            return 0;
	      }
    ioctl( my_dma_fd, 0x00000048, RX_BUFFER_BASE ); 
    return my_dma_fd;
}



int XAxiDma_Up(int my_dma_fd, int MAX_PKT_LEN)
{
    int j;
	unsigned long status_30h,status_34h,status_58h;
    j = ioctl( my_dma_fd, 0x80000030, 0 );
    ioctl( my_dma_fd, 0x00000030, j | 0x00000001 );//control ,running
	ioctl( my_dma_fd, 0x00000048, RX_BUFFER_BASE );
    ioctl( my_dma_fd, 0x00000058, MAX_PKT_LEN );     //58,buffer,bytes,length
printf("OK9!\n");
					fflush(stdout);

while ( ( ioctl ( my_dma_fd, 0x80000034, 0 ) & 0x00000002 ) == 0 )  
   {
			;// Wait
   } 

status_34h=ioctl( my_dma_fd, 0x80000034, 0 );//control ,running

status_58h=ioctl( my_dma_fd, 0x80000058, 0 );//control ,running

printf("34h is %04X , 58h is %04X  \n",status_34h,status_58h);
    
	j = ioctl( my_dma_fd, 0x80000030, 0 );
    ioctl( my_dma_fd, 0x00000030, j & 0xFFFE );//control ,stop
return 0;
}





int XAxiDma_Up_first(int my_dma_fd, int MAX_PKT_LEN)
{
    int j;
	unsigned long status_30h,status_34h,status_58h;
    j = ioctl( my_dma_fd, 0x80000030, 0 );
    ioctl( my_dma_fd, 0x00000030, j | 0x00000001 );//control ,running
	ioctl( my_dma_fd, 0x00000048, RX_BUFFER_BASE );
    ioctl( my_dma_fd, 0x00000058, MAX_PKT_LEN );     //58,buffer,bytes,length
	printf("OK9!--first\n");
	fflush(stdout);
	status_34h=ioctl( my_dma_fd, 0x80000034, 0 );//control ,running
	status_58h=ioctl( my_dma_fd, 0x80000058, 0 );//control ,running
	printf("34h is %04X , 58h is %04X  \n",status_34h,status_58h);
    return 0;
}

float output()  
{  
        int q=0; 
        float fd=0,v=0;
		q=x[8].real;

if( q>8192 )
{
	printf("the max q is over ! that is %d\n",q);
	q = 16384-q; 
}


	if(q>300)
	printf("noise  %d\n",q);
	
	else
	{
		printf("%d\n",q);		
		  fd=2*(((float)(q-2))/((float)N))*100;
		  //v=fd*6.25;
          v=fd*1000*lamada/2;
		 printf("fd=%fkHz,v=%fm/s\n",fd,v);
	}
return v;
}












