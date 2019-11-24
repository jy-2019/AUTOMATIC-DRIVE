/*
 * i2c_ctr.h
 *
 *  Created on: 20170125
 *  Author: Wenchao Liu
 */

#ifndef I2C_CTR_H_
#define I2C_CTR_H_

void sccb_start(void);
void sccb_end(void);

void sccb_sendbyte( unsigned char value );

void sccb_senddata(unsigned char subaddr1,unsigned char subaddr2,unsigned char value);
#endif /* I2C_CTR_H_ */
