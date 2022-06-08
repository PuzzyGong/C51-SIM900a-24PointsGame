//***************************************************//
//main.h的流程文件，和main.h的空间相同
//***************************************************//
#ifndef _DEAL_H_
#define _DEAL_H_

#include "main.h"


void infrared_deal(unsigned char ir_getchar);
void uart_deal(unsigned char* str);
void uart_init(void);

#endif