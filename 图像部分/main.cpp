/*
 * main.c: video, VDMA, ultra and moto application
 *
 *  Created on: 20170125
 *  Author: Wenchao Liu
 *
 */

#include "ees350_moto.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ov5640_config.h>
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
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/bufferpool.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>


#include <iostream>
#include <my_uart.h>
#include <line_finder.h>
#include <ledbar.h>
#include <LaneDetector.h>
#include <time.h>

using namespace std;
using namespace cv::ml;
using namespace cv;

#ifndef u32
#define u32 unsigned int
#endif
#ifndef u8
#define u8 unsigned char
#endif
#ifndef EES350_MOTO_H_
#define EES350_MOTO_H_
#endif
#define DDR_BASEADDR        0x30000000
#define H_STRIDE            1280
#define H_ACTIVE            1280
#define V_ACTIVE            720
#define pi					3.14159265358
#define MAP_SIZE 1280*720*3
#define MAP_MASK (MAP_SIZE - 1)

u32 ULTRASONIC_BASEADDR;

#define VIDEO_LENGTH  (H_STRIDE*V_ACTIVE)
#define VIDEO_BASEADDR0 DDR_BASEADDR + 0x02000000
#define VIDEO_BASEADDR1 DDR_BASEADDR + 0x03000000
#define VIDEO_BASEADDR2 DDR_BASEADDR + 0x04000000
#define VIDEO_BASEADDR3 DDR_BASEADDR + 0x05000000
//#define VIDEO_BASEADDR4 DDR_BASEADDR + 0x06000000
//#define VIDEO_BASEADDR5 DDR_BASEADDR + 0x07000000

void start_vdma(void);

void car_stop()
{
    motodir_set(0x43C00000,0);
    moto_pwm_set(0x43C00000,0);

    motodir_set(0x43C10000,0);
    moto_pwm_set(0x43C10000,0);
}
void car_forward()
{
    motodir_set(0x43C00000,0);
    moto_pwm_set(0x43C00000,3500);

    motodir_set(0x43C10000,0);
    moto_pwm_set(0x43C10000,3500);
}

void car_forward_low()
{
    motodir_set(0x43C00000,0);
    moto_pwm_set(0x43C00000,2500);

    motodir_set(0x43C10000,0);
    moto_pwm_set(0x43C10000,2500);
}
void car_back()
{
    motodir_set(0x43C00000,1);
    moto_pwm_set(0x43C00000,4000);

    motodir_set(0x43C10000,1);
    moto_pwm_set(0x43C10000,4000);
}
void car_right()
{
    motodir_set(0x43C00000,0);
    moto_pwm_set(0x43C00000,5000);

    motodir_set(0x43C10000,0);
    moto_pwm_set(0x43C10000,1000);
}
void car_left()
{
    motodir_set(0x43C00000,0);
    moto_pwm_set(0x43C00000,1000);

    motodir_set(0x43C10000,0);
    moto_pwm_set(0x43C10000,5000);
}
void car_right_adjust()
{
    motodir_set(0x43C00000,0);
    moto_pwm_set(0x43C00000,7000);

    motodir_set(0x43C10000,0);
    moto_pwm_set(0x43C10000,1000);
}
void car_left_adjust()
{
    motodir_set(0x43C00000,0);
    moto_pwm_set(0x43C00000,1000);

    motodir_set(0x43C10000,0);
    moto_pwm_set(0x43C10000,7000);
}
using namespace std;
using namespace cv;
using namespace cv::ml;


int main()
{
    Miz702_EMIO_init();
    ov5640_init_rgb();
    Miz702_EMIO_stop();
    start_vdma();

    /*********init*******/
    car_init();

    //motodir_set(0x43C00000,0);
    //moto_pwm_set(0x43C00000,0);

    //motodir_set(0x43C10000,0);
    //moto_pwm_set(0x43C10000,0);

    eescar_ultra_init();

    // Ultrasonic__value_all
    float ult_data[3];
    float ult_data_prev[3];
    int i;
    cv::Mat src, gray;
    gray=cv::imread("Lena.bmp", cv::IMREAD_GRAYSCALE);
    src = cv::imread("Lena.bmp"); 
    cv::imwrite("scr.bmp",src);
    cv::imwrite("gray.bmp", gray);
    printf("Save image successful");
    cv::Mat dst_rgb(720, 1280, CV_8UC3);//RGB888通道
    cv::Mat dst_bgr(720, 1280, CV_8UC3);//RGB888通道
    cv::Mat dst_gray;
    Mat imgCalibration;
    LaneDetector lanedetector;  // Create the class object
	//cv::Mat frame;
    cv::Mat img_resize;
	cv::Mat gamma;//add
	cv::Mat img_denoise;
	cv::Mat img_edges;
	cv::Mat img_mask;
    cv::Mat img_save;
	cv::Mat img_lines;
    cv::Mat img_proccessed;
    cv::Mat img_proccessed_rgb;
//	std::vector<cv::Vec4i> lines;
//	std::vector<std::vector<cv::Vec4i> > left_right_lines;
//	std::vector<cv::Point> lane;
//	std::string turn;
//  std::string turn_prev;
    Size size_resize(640,360);
    Size size_bksize(1280,720);

    int flag_plot = -1;
    cout <<"mat size is"<<endl;
    cout <<dst_rgb.size<<endl;
    int dev_mem_fd;
    void * map_video_base,* map_video_base1,* map_video_base2,* map_video_base3, * vdmaVirtualAddress,* vdmaVirtualAddress1,* vdmaVirtualAddress2,* vdmaVirtualAddress3;
    dev_mem_fd = open( "/dev/mem", O_RDWR | O_SYNC );
    map_video_base = mmap( 0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, dev_mem_fd, VIDEO_BASEADDR0 & ~MAP_MASK );
    map_video_base1 = mmap( 0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, dev_mem_fd, VIDEO_BASEADDR3 & ~MAP_MASK );
    vdmaVirtualAddress = map_video_base + ( VIDEO_BASEADDR0 & MAP_MASK );
    vdmaVirtualAddress1 = map_video_base1 + ( VIDEO_BASEADDR3 & MAP_MASK );
    //vdmaVirtualAddress2 = map_video_base2 + ( VIDEO_BASEADDR4 & MAP_MASK );
    //vdmaVirtualAddress3 = map_video_base3 + ( VIDEO_BASEADDR5 & MAP_MASK );
/*
    Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
    cameraMatrix.at<double>(0, 0) = 1160.271422;
    cameraMatrix.at<double>(0, 1) = 15.30428866;
    cameraMatrix.at<double>(0, 2) = 557.6471062;
    cameraMatrix.at<double>(1, 0) = 0;
    cameraMatrix.at<double>(1, 1) = 1143.145585;
    cameraMatrix.at<double>(1, 2) = 319.6988525;
    cameraMatrix.at<double>(2, 0) = 0;
    cameraMatrix.at<double>(2, 1) = 0;
    cameraMatrix.at<double>(2, 2) = 1;

    Mat distCoeffs = Mat::zeros(5, 1, CV_64F);
    distCoeffs.at<double>(0, 0) = -0.38025307;
    distCoeffs.at<double>(1, 0) = 0.310893578;
    distCoeffs.at<double>(2, 0) = 0.016857855;
    distCoeffs.at<double>(3, 0) = -0.007820673;
    distCoeffs.at<double>(4, 0) = 0;
*/
    //cout <<"mat channel is"<<endl;
    //cout <<dst_rgb.channels<<endl;
    //printf("size of dst is %d,channel of dst is %d",dst_rgb.size,dst_rgb.channels);
    //IplImage dst = dst_rgb;
    int dx=0; 
    char ad[128]={0};
    //car_forward();
    my_uart_inital();
    char b;
    int linefinder;
    clock_t start,stop;
    double duration;
	//Ptr<SVM> svm = SVM::create();
	//load train file
	//svm = cv::ml::SVM::load("SVM_HOG.xml");
    //	if (!svm)
	//{
	//	cout << "Load file failed..." << endl;
	//}
    my_line_finder_init();
    ledbar_init();
    setBits(0b000000000000001);
    usleep(100000);
    setBits(0b000000000000011);
    usleep(100000);
    setBits(0b000000000000111);
    usleep(100000);
    setBits(0b000000000001111);
    usleep(100000);
    setBits(0b000000000011111);
    usleep(100000);
    setBits(0b000000000111111);
    usleep(100000);
    setBits(0b000000001111111);
    usleep(100000);
    setBits(0b000000011111111);
    usleep(100000);
    setBits(0b000000111111111);
    while(1)
    {
        b = receive_uart();
        cout<<'a'<<endl;
        switch(b)
        {
            
            case 'a':
            {
                cout<<"寻线功能"<<endl;
                while(1)
                {
                    i++;
                    linefinder = get_value();
                    if(linefinder==1)
                    {
                        car_forward();
                        usleep(1000);
                    }
                    if(linefinder==0)
                    {
                        car_stop();
                        usleep(1000);
                    }
                    if(dete_emp()==0)
                    {break;}
                }
                break;
            }
            case 'b':
            {
                cout<<"小车前进"<<endl;
                setBits(0b000001111111100);
                car_forward();
                break;
            }
            case 'c':
            {
                cout<<"小车避障"<<endl;
                while(1)
                {
                ult_data_prev[0]=ult_data[0];
                ult_data_prev[1]=ult_data[1];
                ult_data_prev[2]=ult_data[2];
                eescar_ultra_get_all(ult_data);
                for(i = 1; i < 4; i++){
                    printf("u%d : %f mm",i , ult_data[i-1]);
                }
                printf("\n");
                if((ult_data[2] >2 &&ult_data[2] < 180&&ult_data_prev[2] >2 &&ult_data_prev[2] < 180) && (ult_data[1] >2 &&ult_data[1] < 180&&ult_data_prev[1] >2 &&ult_data_prev[1] < 180) )
                {
                    car_back();
                    usleep(500000);    //delay
                    car_right();
                    usleep(800000);    //delay
                }
                else if((ult_data[2] >2 &&ult_data[2] < 180&&ult_data_prev[2] >2 &&ult_data_prev[2] < 180) )
                {
                    car_back();
                    usleep(500000);    //delay
                    car_right();
                    usleep(800000);    //delay
                }
                else if(( ult_data[0] >2&&ult_data[0] < 180&& ult_data_prev[0] >2&&ult_data_prev[0] < 180)&& (ult_data[1] >2 &&ult_data[1] < 180&&ult_data_prev[1] >2 &&ult_data_prev[1] < 180) )
                {
                    car_back();
                    usleep(500000);    //delay
                    car_left();
                    usleep(800000);    //delay
                }
                else if(( ult_data[0] >2&&ult_data[0] < 180&& ult_data_prev[0] >2&&ult_data_prev[0] < 180) )
                {
                    car_back();
                    usleep(500000);    //delay
                    car_left();
                    usleep(800000);    //delay
                }
                else if((ult_data[1] >2 &&ult_data[1] < 180&&ult_data_prev[1] >2 &&ult_data_prev[1] < 180) )
                {
                    car_back();
                    usleep(500000);    //delay
                    car_left();
                    usleep(800000);    //delay
                }
                else {
                    car_forward();
                }
                    if(dete_emp()==0)
                    {break;}
                }
                break;
            }
            case 'd':
            {
                cout<<"左转弯"<<endl;
                car_left();
                while(1)
                {
                    setBits(0b00000111000000);
                    usleep(500000);
                    if(dete_emp()==0)
                    {break;}
                    setBits(0b00000000000000);
                    usleep(500000);
                    if(dete_emp()==0)
                    {break;}
                }
                break;
            }
            case 'e':
            {
                cout<<"停车"<<endl;
                car_stop();
                setBits(0b000000000000001);
                break;
            }
            case 'f':
            {
                cout<<"右转弯"<<endl;
                car_right();
                while(1)
                {
                    setBits(0b000000000011100);
                    usleep(500000);
                    if(dete_emp()==0)
                    {break;}
                    setBits(0b000000000000000);
                    usleep(500000);
                    if(dete_emp()==0)
                    {break;}
                }
                break;
            }
            case 'g':
            {
                cout<<"自动驾驶"<<endl;
                while(1)
                {
                std::vector<cv::Vec4i> lines;
                std::vector<std::vector<cv::Vec4i> > left_right_lines;
                std::vector<cv::Point> lane;
                std::string turn;
                memcpy((uchar *)dst_rgb.data,(uchar *)vdmaVirtualAddress,1280*720*3); 
                cv::resize(dst_rgb,img_resize,size_resize);
                cv::cvtColor(img_resize, dst_bgr,COLOR_RGB2BGR);//dst_bgr是图像
                gamma = lanedetector.gammatrans(dst_bgr);
                img_denoise = lanedetector.deNoise(gamma);
                img_edges = lanedetector.edgeDetector(img_denoise);
                img_mask = lanedetector.mask(img_edges);
                lines = lanedetector.houghLines(img_mask);
                if (!lines.empty())
                {
                    left_right_lines = lanedetector.lineSeparation(lines, img_edges);
                    lane = lanedetector.regression(left_right_lines, dst_bgr);
                    turn = lanedetector.predictTurn();
                    if(turn=="Left Turnbig")
                        {
                            car_left_adjust();
                            //usleep(100000);
                            setBits(0b00000111000000);
                        }
                    else if(turn=="Right Turnbig")
                        {
                            car_right_adjust();
                           // usleep(100000);
                            setBits(0b000000000011100);
                        }
                    else if(turn=="Left Turnsmall")
                        {
                            car_left();
                            setBits(0b00000100000000);
                        }
                    else if(turn=="Right Turnsmall")
                        {
                            car_right();
                            setBits(0b00000000000100);
                        }
                    else
                        {
                            car_forward();
                            setBits(0b000000111111111);
                        }
                    // Plot lane detection
                    img_proccessed = lanedetector.plotLane(gamma, lane, turn);
                    cv::cvtColor(img_proccessed, img_proccessed_rgb,COLOR_BGR2RGB);//dst_bgr是图像
                    resize(img_proccessed_rgb,img_save,size_bksize);
                    //resize(img_mask,img_save,size_bksize);
                    memcpy((uchar *)vdmaVirtualAddress1,(uchar *)img_save.data,1280*720*3);
                }
                else
                {
                flag_plot = -1;
                }
                //stop = clock();
                //duration = ((double)(start - stop));
	            //printf("control use %lf", duration);
                if(dete_emp()==0)
                {break;}
                }
                
            }
            case 'h':
            {
                cout<<"后退"<<endl;
                car_back();
                while(1)
                {
                    setBits(0b000000000000010);
                    usleep(500000);
                    if(dete_emp()==0)
                    {break;}
                    setBits(0b000000000000000);
                    usleep(500000);
                    if(dete_emp()==0)
                    {break;}
                }
                break;
            }
            case 'i':
            {
                cout<<"test"<<endl;
                
                while(1)
                {    
                    car_forward_low();
                    std::vector<cv::Vec3f> circles;

                     cv::Mat dst,imgHSV,imgThresholded;

                    memcpy((uchar *)dst_rgb.data,(uchar *)vdmaVirtualAddress,1280*720*3);
                    cv::resize(dst_rgb,img_resize,size_resize);
 
                      dst=img_resize;
   
                       cv::cvtColor(dst,imgHSV,COLOR_BGR2HSV);
                       vector<Mat> hsvSplit;
                       cv::inRange(imgHSV,Scalar(0,43,46),Scalar(10,255,255),imgThresholded);
                       //cv::inRange(imgHSV,Scalar(0,0,0),Scalar(180,255,255),imgThresholded);
                       GaussianBlur(imgThresholded,imgThresholded,Size(5,3),2,2);
                       HoughCircles(imgThresholded,circles,CV_HOUGH_GRADIENT,1,200,50,7,1,360);
                        for(size_t i=0;i<circles.size();i++)
                       {
                          cv::Vec3f circle = circles[i];
                          cv::circle(imgThresholded, cv::Point(circle[0],circle[1]),circle[2],cv::Scalar(255,255,0),-1);
                         cv::circle(imgThresholded, cv::Point(circle[0],circle[1]),2,cv::Scalar(0,255,255));
                         }
                        img_proccessed = imgThresholded;
                       cv::cvtColor(img_proccessed, img_proccessed_rgb,COLOR_HSV2RGB);//dst_bgr是图像
                        resize(img_proccessed,img_save,size_bksize);
                        //resize(img_mask,img_save,size_bksize);
                        memcpy((uchar *)vdmaVirtualAddress1,(uchar *)img_save.data,1280*720*3);
                        if(circles.size()>0)
                        {
                         car_stop();
                        }
                    if(dete_emp()==0)
                    {break;}
                }
                break;
            }
            default:
                cout<<"error"<<endl;
        }
    }
    return 0;
}

void start_vdma(void)
{
	int my_vdma_fd;
    my_vdma_fd = open("/dev/my_vdma_driver", 0);
    if(my_vdma_fd < 0)
    {
        printf("vdma:[ERROR] Can't open device.");
        return;
    }	
    printf("vdma: Open device. File description of my_vdma_fd is %d\n",my_vdma_fd);

    ioctl(my_vdma_fd, 0x030, 0x00000004);
    ioctl(my_vdma_fd, 0x030, 0x0000008b);
    ioctl(my_vdma_fd, 0x0AC, VIDEO_BASEADDR0);
    ioctl(my_vdma_fd, 0x0B0, VIDEO_BASEADDR1);
    ioctl(my_vdma_fd, 0x0B4, VIDEO_BASEADDR2);
    ioctl(my_vdma_fd, 0x0A8, (H_STRIDE*3));
    ioctl(my_vdma_fd, 0x0A4, (H_ACTIVE*3));
    ioctl(my_vdma_fd, 0x0A0, V_ACTIVE);

    ioctl(my_vdma_fd, 0x000, 0x0000008b );
    ioctl(my_vdma_fd, 0x05c, VIDEO_BASEADDR3);
    ioctl(my_vdma_fd, 0x060, VIDEO_BASEADDR3);
    ioctl(my_vdma_fd, 0x064, VIDEO_BASEADDR3);
    ioctl(my_vdma_fd, 0x058, (H_STRIDE*3) );
    ioctl(my_vdma_fd, 0x054, (H_ACTIVE*3) );
    ioctl(my_vdma_fd, 0x050, V_ACTIVE );
    my_vdma_fd = close(my_vdma_fd);
}
