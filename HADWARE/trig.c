#include "trig.h"
#include "delay.h"
#include "key.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK ��ӢSTM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��

void trig_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_14|GPIO_Pin_15);						 //PB.5 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PA.5 �˿�����, �������
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;      //�������
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;   //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOA,GPIO_Pin_5); 						 //PA.5 ����� 
}
 
//��ʼ������
void KEY_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTA,PORTCʱ��

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;//PA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOA15
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;//PC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC5
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0

}

//����������
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))//WK_UP��1
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==0)return 1;
		else if(KEY1==0)return 2;
		else if(WK_UP==0)return 3; 
	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1; 	     
	return 0;// �ް�������
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
