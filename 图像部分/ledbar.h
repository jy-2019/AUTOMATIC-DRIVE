/*
 * emio_initial.h
 *
 *  Created on: 20170125
 *  Author: Wenchao Liu
 */

#ifndef LEDBAR_H_
#define LEDBAR_H_
#define GLB_CMDMODE 0x00  // Work on 8-bit mode
#define GLB_ON      0xff  // 8-bit 1 data
#define GLB_OFF     0x00  // 8-bit 0 data
int ledbar_stop(void);

int ledbar_init(void);

void C_HIGH(void);

void C_LOW(void);

void D_HIGH(void);

void D_LOW(void);

void sendData(unsigned int data);  // Send a word to led bar

void latchData(void);              // Load data into the latch register

void setData(unsigned char bits[]);//Set data array

void setBits(unsigned int bits);                 // Toggle leds to match given bits
#endif
