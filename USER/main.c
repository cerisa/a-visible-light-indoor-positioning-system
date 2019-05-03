#include "timer.h"
#include "usart.h"
#include "delay.h"
#include "trig.h"
#include "sys.h"
#include "OLED_ii.h"
#include "usart2.h"
#include "HC_05.h"
#include "timer2.h"
#include "key.h"
#include "math.h"
#include "inv_mpu.h"
#include "mpu6050.h"
#include "motor.h"
#include "kaerman.h"


extern u8  TIM5CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u16  TIM5CH1_CAPTURE_VAL;	//���벶��ֵ	
extern u8  TIM5CH2_CAPTURE_STA;		//���벶��״̬		    				
extern u16  TIM5CH2_CAPTURE_VAL;	//���벶��ֵ	
float pitch,roll,yaw,r,d,ASPECT,pwmvalue; 		//ŷ���� ����MPU6050�õ���PWMֵ





int main(void)
{
	u32 temp=0,temq=0;
	u8 t=0;
	u16 ttt=0;
  u8 key=0;	
 	

	float dis,dic;        //������������
	char string1 [10];	
	char string2 [10];
  extern float SteperGetAngle();
	short x1,y1,i;

  uart_init(115200);	 //���ڲ�����
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	trig_Init(); //������������ʼ��
	TIM_Cap_Init(0XFFFF,72-1); //��ʼ����������ʱ��TIM5
	KEY_Init();//������ʼ��

	I2C_Configuration();  //OLED IIC��ʼ��
	MPU_Init();  //MPU6050��ʼ��

	OLED_Init();   //OLED��ʼ��
	OLED_CLS(); //OLED����
	printf("ok");
	while(mpu_dmp_init())
	{
		printf("error");
		OLED_ShowStr(5,3,(unsigned char*)"NOREADY",2);
	}
	OLED_ShowStr(5,3,(unsigned char*)"READY",2);
	
	TIM1_PWM_Init(199,7199);//��ʱ����ʼ��20ms 50hz
	SteperIoInit();	//��ʼ���������IO��
	 
	SteperSetSpeed(20);//�趨����������ٶ�

		
while(1) 
	{
			
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		  GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		  delay_us(2);
		  GPIO_SetBits(GPIOB,GPIO_Pin_5);
		  GPIO_SetBits(GPIOA,GPIO_Pin_5);
		  delay_us(10);
		  GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		  GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	
		
		
 		if(TIM5CH1_CAPTURE_STA&0X80&&TIM5CH2_CAPTURE_STA&0X80)//�ɹ�������һ��������  �����ǳ�����ģ��1�Ĳ�๦��
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F;
			temp*=65536;//���ʱ���ܺ�
			temp+=TIM5CH1_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
			temq=TIM5CH2_CAPTURE_STA&0X3F;
			temq*=65536;//���ʱ���ܺ�
			temq+=TIM5CH2_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
			
			dis=temp/58.82;
			dic=temq/58.82;
			if(dic<80&&dis<80)
			{
				x1=dis-40;
				y1=dic-40;
			  sprintf(string1,"%d  ",x1);
			  sprintf(string2,"%d  ",y1);
				
//				key=shaomiao(dis,dic);
//				switch(key)
//				{
//					case 1:
//						printf("A");
//					  
//					  break;
//					case 2:
//						printf("B");
//					  break;
//					case 3:
//						printf("C");
//					  break;
//					case 4:
//						printf("D");
//				    break;
//					case 5:
//						printf("E");
//					  break;
//					default:break;
//				}
//				printf("X distance is:%0.2lf cm\r\n",dis-40);//��ӡ�ܵĸߵ�ƽʱ��
//			  printf("Y distance is:%0.2lf cm\r\n",dic-40);//��ӡ�ܵĸߵ�ƽʱ��
				
			}

      OLED_ShowStr(5,3,(unsigned char*)string1,2); //OLED��ʾX Yλ��
			OLED_ShowStr(60,3,(unsigned char*)string2,2); //OLED��ʾX Yλ��
			TIM5CH1_CAPTURE_STA=0;//������һ�β���
			TIM5CH2_CAPTURE_STA=0;//������һ�β���
		}
			
				if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
				{
					
//					printf("yaw:%lf\r\n",yaw);
					t=KEY_Scan(1);
//					printf("t:%d\r\n",t);
					if(t==1)
					{
						delay_ms(1000);
						SteperSetAngle(yaw);
			      SteperRun();			 //�����������
					}
						
				}
				if(ttt>100)
				{
//					printf("yaw:%lf\r\n",yaw);
					printf("( %d, %d )area: %c \r\n\r\n",(int)(37-dis), (int)(31-dic), shaomiao(dis,dic));
					ttt = 0;
				}
				ttt++;
				
			
	 
}
	
}


