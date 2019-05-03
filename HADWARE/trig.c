#include "trig.h"
#include "delay.h"
#include "key.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK 精英STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化

void trig_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟
	

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_14|GPIO_Pin_15);						 //PB.5 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PA.5 端口配置, 推挽输出
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;      //推挽输出
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;   //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOA,GPIO_Pin_5); 						 //PA.5 输出高 
}
 
//初始化按键
void KEY_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA,PORTC时钟

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;//PA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOA15
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;//PC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC5
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0

}

//按键处理函数
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))//WK_UP是1
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0)return 1;
		else if(KEY1==0)return 2;
		else if(WK_UP==0)return 3; 
	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1; 	     
	return 0;// 无按键按下
}



//char shaomiao(float dis,float dic)
//{
//	float x=0,y=0;
//	x=dis+3;
//	y=dic+7;
//	if(x>20&&x<60&&y>20&&y<60)
//	return 'A';
//	if(x<20)
//	{
//		if(y<x)
//			return 'B';
//		if(y+x>80)
//			return 'D';
//		return 'C';
//	}
//	if(x>20&&x<60)
//	{
//		if(y<20)
//			return 'B';
//		if(y>60)
//			return 'D';
//	}
//	if(x>60)
//	{
//		if(y>x)
//			return 'D';
//		if(y+x<80)
//			return 'B';
//		return 'E';
//	}
//}
//	


//}
//	
int fabs1(int a)
{
	if(a>0)
		return a;
	else
		return -a;
}
char shaomiao(float dis,float dic)
{
	float x=0,y=0;
	x=37-dis;
	y=31-dic;
		if ( fabs1(x)<20 && fabs1(y)<20 )
			return 'A';

		if ( y>20 && fabs1(x)<y )
			return 'B';

		if ( x>20 && fabs1(y)<x )
			return 'C';

		if ( y<-20 && y<x && y<-x )
			return 'D';

		if ( x<-20 && y>x && y<-x )
			return 'E';
	return 'F';
}
