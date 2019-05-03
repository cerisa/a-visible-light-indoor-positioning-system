#ifndef __HC05_H
#define __HC05_H

#include "sys.h"

//连接模块GPIO相关参数的一层封装
//**********************************************************************************
#define RCC_STATE  RCC_APB2Periph_GPIOA
#define RCC_EN		RCC_APB2Periph_GPIOC
#define STATE_Pin GPIO_Pin_4 
#define EN_Pin GPIO_Pin_4
//**********************************************************************************

#define HC05_EN  	    PCout(4) 	//蓝牙控制EN信号
#define HC05_STATE  	PAin(4)		//蓝牙连接状态信号


u8 HC05_Init(void);
void HC05_CFG_CMD(u8 *str);
u8 HC05_Get_Role(void);
u8 HC05_Set_Cmd(u8* atstr);	
#endif

