#include<moto.h>
#include<speed.h>
#include<uart.h>
#include<stdio.h>
#include<time.h>
#include <stdlib.h>
#include <string.h>
#include <ultra.h>
#include <emio_initial.h>

#ifndef u8
#define u8 unsigned char
#endif

#define MEASURE_V 'v' //静态测速
#define MEASURE_S 'b' //动态测速
#define MOTO_AWAY 'm' //自动避障
#define follow 'f' //跟随
#define first '1'
#define second '2'
#define third '3'
#define fouth '4'
#define LEFT 'a'
#define FRONT 'w'
#define BACK 's'
#define RIGHT 'd'
#define STOP 'q'

double rece_dis(){
	double dis=0;
	//double dis2;
	char ch[10];
	int i=0;
	ch[0]=receive_uart();
	while(((ch[i]>='0')&&(ch[i]<='9'))||(ch[i]=='.')){
		i++;
		ch[i]=receive_uart();	
	}
	ch[i]='\0';
	dis=atof(ch);
	return dis;
}

char buf[15] = "HelloWorld";
char buf1[15] = "wasd ctrl car";
char buf2[15] = "q stop car";
char buf3[15] = "m aviod";
char buf4[15] = "v meas V";
char buf5[15] = "b meas B";
char buf6[15] = "warning!";
int main()
{
	int  my_dma_fd=0;
	char ch2[50]="please input the distance:";
	double distance=0;
	//double time=0;
	double v=0;
	//double s=0;;
	char tr[10];
	int i=0,n=0,j=0,k=0;
	double speed_s[100]={0};
	int speed_num[100]={0};
	int max_n=0;
	float ult_data[3];
	double dis1=0;
	clock_t start2=0,end2=0;
	long int NUM=0;


	v=0;j=0;k=0;
	u8 b=MEASURE_V;
	eescar_ultra_get_all(ult_data);
	
	my_uart_inital();
	my_dma_fd = XAxiDma_Up_Init();
	Miz702_EMIO_init();
	
	car_init();
	eescar_ultra_init();


	printf("init complete\n");
	while(1)
	{
		b=receive_uart();
		//fflush(stdout);
		switch(b)
		{
			case '1':
			{
				speed1();
				usleep(800000);
			}
			break;
			
			case '2':
			{
				speed2();
				usleep(800000);
			}
			break;
			
			case '3':
			{
				speed3();
				usleep(800000);
			}
			break;
			
			case '4':
			{
				speed4();
				usleep(800000);
			}
			break;
			case 'f':
			{
				while(1)
				{
					v=speed(my_dma_fd);
					if(v>=0.25)
					{
						speed1();
						usleep(500000);
						v=speed(my_dma_fd);
						if(v>=0.25)
						{
							speed2();
							usleep(500000);
							v=speed(my_dma_fd);
							if(v>=0.25)
							{
								speed3();
								usleep(500000);
								v=speed(my_dma_fd);
								if(v>=0.25)
								{
									speed4();
									usleep(500000);	
								}
							}
							else
							{
								speed2();
								usleep(500000);
							}
						}
						else
						{
							speed1();
							usleep(500000);
						}
					}
					else
					{
						usleep(500000);
					}
					if(dete_emp()==0)
					{
						break;
					}		
				}
				
			}
			break;
			
			case 't':
			{
				
				printf("bluetooth is ok!\n");
				write_uart('R'); 
				write_uart('E'); 
				write_uart('A'); 
				write_uart('D'); 
				write_uart('Y'); 
			}
			break;
			
			case MEASURE_V:
			{
				//clearALL();
				while(1)
				{
					v=0;
					printf("OK3!\n");
					fflush(stdout);
					v=speed(my_dma_fd);
					printf("v=%lf  \n",v); 
					write_uart('v');
					write_uart('=');

					//putString("v=",2,1);
					//if(v<0.47)
					//v=0;
					n=trans(v,tr);
					
					if(v==0)
					{
						write_uart('0');
					}

					else if(v>0 && v<1)
					{
						write_uart('0'); 
						//putChar('0',2,0);
						for(i=6;i>=0;i--)
						{
						write_uart(tr[i]);
						//putChar(tr[i],2,0);
						}
					}
					else
					{
						for(i=n;i>=0;i--)
						{
							write_uart(tr[i]);
							//putChar(tr[i],2,0);
						}

					}
					write_uart(' '); 
					write_uart(' ');
					write_uart(' ');
					write_uart('\n'); 
					if(dete_emp()==0)
					{
						break;
					}
					usleep(1000000); //##
				}
			}
			break;
		
		
			case MEASURE_S:
			{
				dis1=0;
				start2=0,end2=0;
				v=0;j=0;k=0;
				for(i=1;i<=100;i++)
				{
					speed_s[i]=0;
					speed_num[i]=0;
				}
				 max_n=0;
				car_forward();
				start2=clock();
				while(1)
				{
					v=speed(my_dma_fd);
					//
					if(v>=0.25)
					{
						speed_up();
					    usleep(1000000);
					}
					else
					{
						speed_down();
					    usleep(1000000);
					}
					//
					speed_s[++j]=v;
					write_uart('v');
					write_uart('=');
					n=trans(v,tr);
					if(v>0 && v<1)
						write_uart('0'); 
					for(i=n;i>=0;i--)
					{
						write_uart(tr[i]);
					}
					write_uart(' '); 
					write_uart(' ');
					write_uart(' ');
					write_uart('\n'); 
					ult_data[0]=0;
					ult_data[1]=0;
					ult_data[2]=0;
					eescar_ultra_get_all(ult_data);
					
					if(ult_data[1] >0 &&ult_data[1] < 200)
					{  
						car_stop();	
						break;							
					}	
					else if( ult_data[0] >0&&ult_data[0] < 110 )
					{  
						car_stop();	
						break;
					}
					else if( ult_data[2] >0&&ult_data[2] < 110 )
					{  
						car_stop();	
						break;	
					}
					else 
					{
						car_forward();
						usleep(200000);        //delay
					}
						if(dete_emp()==0)
					{
						break;
					}
                                        usleep(1000000);
					
				}
				end2=clock();
				for(i=1;i<=j;i++)
				{
					for(k=1;k<=j;k++)
					{
						if(speed_s[i]==speed_s[k])
							speed_num[i]++;
					}
				}
				for(i=1;i<=j;i++)
				{
					if((speed_num[i]>max_n) &&(speed_s[i]>0) &&(speed_s[i]<1) )
						v=speed_s[i];
				}
				i=0;
		
				
				dis1=(end2-start2)*v/280000;
				
		                printf("time=%d   \n",(end2-start2));
                                printf("s=%d   \n",dis1);
                                
				n=trans(dis1,tr);
				write_uart('s');
				write_uart('='); 
				if(dis1>0 && dis1<1)
					write_uart('0');
				for(i=n;i>=0;i--)
				{
					write_uart(tr[i]);
				}
					write_uart('\n'); 
					write_uart('v');
					write_uart('=');
					
					n=trans(v,tr);
					if(v>0 && v<1)
						write_uart('0'); 
					for(i=n;i>=0;i--)
					{
						write_uart(tr[i]);
					}
					write_uart(' '); 
					write_uart(' ');
					write_uart(' ');
					write_uart('\n'); 					
			}	
			break;
	
	
			case MOTO_AWAY:	
			{
				moto();
			}
			break;


			case LEFT: 
			{
				car_left();
				usleep(800000);
			}
			break;
			
			
			case RIGHT: 
			{
				car_right(); 
				usleep(800000);
			}
			break;
			
			
			case FRONT:
			{ 
				car_forward(); 
				usleep(800000);
                                if(ult_data[1] >0 &&ult_data[1] < 500)
                                {
                                   car_stop();
                                   usleep(800000);
                                 }
			}
			break;
			
			
			case BACK:
			{
				car_back(); 	
				usleep(800000);
			}
			break;
			
			
			case STOP: 
			{
				car_stop();	
				//usleep(800000);
			}
			break;	
			
			default:
			;
		}
	}
return 0;
}
int moto()
{
    motodir_set(0x43C00000,0);
    moto_pwm_set(0x43C00000,0);

    motodir_set(0x43C10000,0);
    moto_pwm_set(0x43C10000,0);

    float ult_data[3];
    //float ult_data_prev[3];

    avoid_forward();
    int i;
    ult_data[0]=0;//right
    ult_data[1]=0;//mid
    ult_data[2]=0;//left 对的
    int number1 = 0;int number2 = 0;
	while(1)
	{

        eescar_ultra_get_all(ult_data);
        for(i = 1; i < 4; i++)
		{
         printf("distance= u%d : %f mm",i , ult_data[i-1]);
		}
        printf("\n");



         if(ult_data[1] >0 &&ult_data[1] < 200)
               {  
		      avoid_back();
                      usleep(800000);
                      avoid_right();
                      usleep(800000);
					
		}	
               
        else if(ult_data[2] >10 &&ult_data[2] < 110 )
        {
			number1 = number1+1;
		    if(number1>5)
			{
				number1 = 0;
				avoid_back();
                usleep(800000);    //delay
                avoid_right();
                usleep(1000000);
			}    //delay
        }
        else if( ult_data[0] >10&&ult_data[0] < 110 )
		{
			number2 = number2+1;
	    	if(number2>5)
            {
				number2 = 0;
				avoid_back();
                usleep(800000);    //delay
                avoid_left();
                usleep(1000000);
			}   //delay
                
		}
            
        else 
		{
			avoid_forward();
        }
        if(dete_emp()==0)
		{break;}
    }
    return 0;
}









