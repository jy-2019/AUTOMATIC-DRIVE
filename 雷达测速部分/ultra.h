#ifndef EES350_MOTO_H_
#define EES350_MOTO_H_

#include <unistd.h>

#define MOTO_L_BASEADDR 0x43C00000
#define MOTO_R_BASEADDR 0x43C10000

#ifndef u32
#define u32 unsigned int
#endif

u32 ULTRASONIC_BASEADDR;

#define ULTRASONIC_PARAM 0.0017


int eescar_ultra_init();

int eescar_ultra_get_all(float *val);


int eescar_ultra_get_sg(int num, float * val);
int car_init(void);

#endif /* ZED_MOTO_H_ */
