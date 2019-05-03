#include "motor.h"
#include "delay.h"
#include <stdlib.h>



#define STEP_A_PIN GPIO_Pin_7	//A
#define STEP_B_PIN GPIO_Pin_6	//B
#define STEP_C_PIN GPIO_Pin_8	//C
#define STEP_D_PIN GPIO_Pin_9		//D

//IO�����
#define STEP_A_GPIO GPIOC
#define STEP_B_GPIO GPIOC
#define STEP_C_GPIO GPIOC
#define STEP_D_GPIO GPIOC

#define STEP_A PCout(7)
#define STEP_B PCout(6)
#define STEP_C PCout(8)
#define STEP_D PCout(9)

#define STEPER_ROUND        (64*2)                    //һȦ�ߵ�����
#define ANGLE_PER_ROUND  ((float)STEPER_ROUND/360)   //һ����Ҫ�ܵ�����(0.3555)

static int gSpeed = 0;
static int gAngle = 0;
u16   steperPin   []            ={STEP_A_PIN,STEP_B_PIN ,STEP_C_PIN ,STEP_D_PIN};	
GPIO_TypeDef*  steperPorts[]={STEP_A_GPIO,STEP_B_GPIO,STEP_C_GPIO,STEP_D_GPIO};

/*
 * ��ʼ�����������IO
 */
void SteperIoInit(void){
  unsigned char i;
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//ʹ��PGIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	                         //���������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	                         //���Ƶ�����50M
  for(i=0;i<4;i++) 
  {
		GPIO_InitStructure.GPIO_Pin=steperPin[i];
		GPIO_Init(steperPorts[i], &GPIO_InitStructure);  
	}
	for(i=0;i<4;i++) 
	  GPIO_ResetBits(steperPorts[i], steperPin[i]);
}

/*
 * ���ò�������ĽǶ�
 */
void SteperSetAngle(float ag){
	gAngle = ag;
}

/*
 * ��ȡ��������ĽǶ�
 */
float  SteperGetAngle(void){
	return gAngle;
}



/*
 * ���ò��������ת��
 */
void SteperSetSpeed(int sp){
	gSpeed = sp;
}


/*
 * ��ȡ���������ת��
 */
int SteperGetSpeed(void){
	return gSpeed;
}

/*
 *  �����������
 */
void SteperRun(void){
	int i;
	if(gAngle == 0)
		return;
	
	if(gAngle>0){
		for(i = 0; i < abs(ANGLE_PER_ROUND*gAngle); i++){
			SteperFrontTurn();
		}					
	}else{	
		for(i = 0; i < abs(ANGLE_PER_ROUND*gAngle); i++){
			SteperBackTurn();
		}
	}
	//���ת���ĽǶ�
	gAngle = 0;
	SteperWriteData(0x00);
}

/*
 *  ���������д����
 */
void SteperWriteData(int data){
	if(data&1){
		STEP_A = 1;
	}else{
		STEP_A = 0;
	}
	
	if(data&2){
		STEP_B = 1;
	}else{
		STEP_B = 0;
	}
	
	if(data&4){
		STEP_C = 1;
	}else{
		STEP_C = 0;
	}
	
	if(data&8){
		STEP_D = 1;
	}else{
		STEP_D = 0;
	}
}

/*
 *  ���������ǰת��
 */
void SteperFrontTurn(void){
	SteperWriteData(0x03);
	delay_ms(gSpeed);
	SteperWriteData(0x06);
	delay_ms(gSpeed);
	SteperWriteData(0x0C);
	delay_ms(gSpeed);
	SteperWriteData(0x09);
	delay_ms(gSpeed);
}


/*
 *  �����������ת��
 */
void SteperBackTurn(void){
	SteperWriteData(0x03);
	delay_ms(gSpeed);
	SteperWriteData(0x09);
	delay_ms(gSpeed);
	SteperWriteData(0x0C);
	delay_ms(gSpeed);
	SteperWriteData(0x06);
	delay_ms(gSpeed);
}
