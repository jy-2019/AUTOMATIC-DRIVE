#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#define TX_BUFFER_BASE       0x30200000
#define RX_BUFFER_BASE       0x30400000
#define XAXIDMA_IRQ_ALL_MASK 0x00007000

// float temp[1024];
//#define MAX_PKT_LEN 10*4

#define FATAL do { fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
  __LINE__, __FILE__, errno, strerror(errno)); exit(1); } while(0)
 
#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

int XAxiDma_Up_Init()
{
    // Disable interrupts, we use polling mode
    int my_dma_fd;
    my_dma_fd = open("/dev/my_dma_driver", 0);
    if(my_dma_fd < 0)
    {
        printf("dma:[ERROR] Can't open device.");
        return;
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
   // Disable interrupts, we use polling mode 
    int j;

    j = ioctl( my_dma_fd, 0x80000030, 0 );
    ioctl( my_dma_fd, 0x00000030, j | 0x00000001 );
    ioctl( my_dma_fd, 0x00000058, MAX_PKT_LEN );

    while ( ( ioctl ( my_dma_fd, 0x80000034, 0 ) & 0x00000002 ) == 0 ) 
    {
				// Wait
    }

    return 0;
}

int main()
{
    int  my_dma_fd;
    int  dev_mem_fd;
    void * map_base, * virt_addr;
    FILE * file_fp;
    int  n;
    
    my_dma_fd = XAxiDma_Up_Init();

    if( ( dev_mem_fd = open( "/dev/mem", O_RDWR | O_SYNC ) ) == -1 ) FATAL;
    map_base = mmap( 0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, dev_mem_fd, RX_BUFFER_BASE & ~MAP_MASK );
    virt_addr = map_base + ( RX_BUFFER_BASE & MAP_MASK );

    if ( ( file_fp = fopen( "/mnt/result_data.raw", "w" ) ) == NULL ) 
    {
        printf( "open data.txt failed\n" );
        return 0;
    }   

    fseek( file_fp, 0L, 1 );
    for( n = 0; n < 32; n++ ) 
    {
        XAxiDma_Up( my_dma_fd, 1024 * 1 * 4 );
        fflush( stdout );
        fwrite( virt_addr, sizeof( float ), 1024, file_fp );  
        fflush( stdout );
    }
    fclose(file_fp);

    if( munmap( map_base, MAP_SIZE ) == -1 ) FATAL;
    close(dev_mem_fd); 
    printf("128KB WRITE OVER\n");
   
 //   FILE *fp, *fp1, *fp2 ,*fp3, *fpcs1,*fpcs2,*fpcs3;
 //   if ( (fp = fopen("/mnt/combine_cross_original_data1.raw", "r")) == NULL ) {
 //       printf("open data.txt failed\n");
 //       return 0;
 //   }

   
 //   int flag=0;
 //   struct timeval start,end;
 //   long timeuse=0;
   
 //   XAxiDma_Down_Init(1024*1*4);
 //   for( m = 0 ; m < 1 ; m++ ) 
 //   {
 //        fseek(fp,0L,1);
 //        if((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) FATAL;
 //        map_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, TX_BUFFER_BASE & ~MAP_MASK);
 //        virt_addr = map_base + ( TX_BUFFER_BASE & MAP_MASK);
	// for( n = 0 ; n < 262144 ; n++ ) 
	// {
 //                fflush(stdout);
	//         fread( virt_addr , sizeof( float ) , 1024 , fp );  
 //                fflush(stdout);
	// 	//devmem_f('w', TX_BUFFER_BASE, temp, 1024);
		
	//         //gettimeofday(&start, NULL );
	// 	//printf("s\n");

	// 	XAxiDma_Down(1024*1*4);
		
	//         //printf("%d!\n",++flag);
	// }
	// if(munmap(map_base, MAP_SIZE) == -1) FATAL;
	// close(fd);
 //   }
 //   fclose(fp);
 //   printf("1GB READ OVER\n",++flag);
   
 //   if ( (fp1 = fopen("/mnt/combine_cross_original_data2.raw", "r")) == NULL ) {
 //       printf("open data.txt failed\n");
 //       return 0;
 //   }
 //  for( m = 0 ; m < 1 ; m++ ) 
 //   {
 //        fseek(fp1,0L,1);
 //        if((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) FATAL;
 //        map_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, TX_BUFFER_BASE & ~MAP_MASK);
 //        virt_addr = map_base + ( TX_BUFFER_BASE & MAP_MASK);
	// for( n = 0 ; n < 262144 ; n++ ) 
	// {
 //                fflush(stdout);
	//         fread( virt_addr , sizeof( float ) , 1024 , fp1 );  
 //                fflush(stdout);
	// 	//devmem_f('w', TX_BUFFER_BASE, temp, 1024);
		
	//         //gettimeofday(&start, NULL );
	// 	//printf("s\n");

	// 	XAxiDma_Down(1024*1*4);
		
	//         //printf("%d!\n",++flag);
	// }
	// if(munmap(map_base, MAP_SIZE) == -1) FATAL;
	// close(fd);
 //   }
 //   fclose(fp1);
 //   printf("2GB READ OVER\n",++flag);
}

