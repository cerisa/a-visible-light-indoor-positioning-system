#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"


void TIM5_Cap_Init(u16 arr,u16 psc);
void TIM5_IRQHandler(void);


#endif


