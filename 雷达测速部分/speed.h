#include "ultra.h"
#include <sys/mman.h>
#include <emio_initial.h>
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
#include <math.h>
#define PI  3.14159265358
#define N 4096*2*2
#define size_x (N)
#define lamada 0.012478
#define accnum 5
#define K      2.8
#define pro    1
#ifndef u32
#define u32 unsigned int
#endif
#ifndef u8
#define u8 unsigned char
#endif


typedef struct{  
double real;  
double img;  
}complex; /*定义复数类型*/    
complex W[size_x];
complex x[size_x];

#define TX_BUFFER_BASE       0x30200000
#define RX_BUFFER_BASE       0x30400000
#define XAXIDMA_IRQ_ALL_MASK 0x00007000

#define FATAL do { fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
  __LINE__, __FILE__, errno, strerror(errno)); exit(1); } while(0)
 
#define MAP_SIZE 1280UL
#define MAP_MASK (MAP_SIZE- 1)
int XAxiDma_Up_Init();
int XAxiDma_Up(int my_dma_fd, int MAX_PKT_LEN);
void fft();     /*快速傅里叶变换*/  
void initW();   /*初始化变换核*/  
void change(); /*变址*/  
void add(complex ,complex ,complex *); 
void mul(complex ,complex ,complex *);  
void sub(complex ,complex ,complex *);
float output();
//void filter();
void CFAR(float data[], float cfardata[]);
float speed(int my_dma_fd);
