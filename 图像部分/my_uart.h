#ifndef uart_H_

#define uart_H_



#define UART_BASEADDR 0x42C00000

#define UART_RX 0

#define UART_TX 1

#define UART_STAT 2

#define UART_CONT 3


#ifndef u32

#define u32 unsigned int

#endif /* I2C_CTR_H_ */

int my_uart_inital();


char receive_uart();

//char judge_uart();

void write_uart(char data);

char receive_uart0();

int dete_emp();
#endif 
