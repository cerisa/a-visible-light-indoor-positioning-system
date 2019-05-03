#include "motor.h"
#include "delay.h"
#include <stdlib.h>



#define STEP_A_PIN GPIO_Pin_7	//A
#define STEP_B_PIN GPIO_Pin_6	//B
#define STEP_C_PIN GPIO_Pin_8	//C
#define STEP_D_PIN GPIO_Pin_9		//D

//IO组别定义
#define STEP_A_GPIO GPIOC
#define STEP_B_GPIO GPIOC
#define STEP_C_GPIO GPIOC
#define STEP_D_GPIO GPIOC

#define STEP_A PCout(7)
#define STEP_B PCout(6)
#define STEP_C PCout(8)
#define STEP_D PCout(9)

#define STEPER_ROUND        (64*2)                    //一圈走的周期
#define ANGLE_PER_ROUND  ((float)STEPER_ROUND/360)   //一个度要周的周期(0.3555)

static int gSpeed = 0;
static int gAngle = 0;
u16   steperPin   []            ={STEP_A_PIN,STEP_B_PIN ,STEP_C_PIN ,STEP_D_PIN};	
GPIO_TypeDef*  steperPorts[]={STEP_A_GPIO,STEP_B_GPIO,STEP_C_GPIO,STEP_D_GPIO};

/*
 * 初始化步进电机的IO
 */
void SteperIoInit(void){
  unsigned char i;
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//使能PGIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	                         //带上拉输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	                         //输出频率最大50M
  for(i=0;i<4;i++) 
  {
		GPIO_InitStructure.GPIO_Pin=steperPin[i];
		GPIO_Init(steperPorts[i], &GPIO_InitStructure);  
	}
	for(i=0;i<4;i++) 
	  GPIO_ResetBits(steperPorts[i], steperPin[i]);
}

/*
 * 设置步进电机的角度
 */
void SteperSetAngle(float ag){
	gAngle = ag;
}

/*
 * 获取步进电机的角度
 */
float  SteperGetAngle(void){
	return gAngle;
}



/*
 * 设置步进电机的转速
 */
void SteperSetSpeed(int sp){
	gSpeed = sp;
}


/*
 * 获取步进电机的转速
 */
int SteperGetSpeed(void){
	return gSpeed;
}

/*
 *  启动步进电机
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
	//清除转过的角度
	gAngle = 0;
	SteperWriteData(0x00);
}

/*
 *  往步进电机写数据
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
 *  步进电机往前转动
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
 *  步进电机往后转动
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
