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


extern u8  TIM5CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u16  TIM5CH1_CAPTURE_VAL;	//输入捕获值	
extern u8  TIM5CH2_CAPTURE_STA;		//输入捕获状态		    				
extern u16  TIM5CH2_CAPTURE_VAL;	//输入捕获值	
float pitch,roll,yaw,r,d,ASPECT,pwmvalue; 		//欧拉角 根据MPU6050得到的PWM值





int main(void)
{
	u32 temp=0,temq=0;
	u8 t=0;
	u16 ttt=0;
  u8 key=0;	
 	

	float dis,dic;        //超声波检测距离
	char string1 [10];	
	char string2 [10];
  extern float SteperGetAngle();
	short x1,y1,i;

  uart_init(115200);	 //串口波特率
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	trig_Init(); //超声波触发初始化
	TIM_Cap_Init(0XFFFF,72-1); //初始化超声波定时器TIM5
	KEY_Init();//按键初始化

	I2C_Configuration();  //OLED IIC初始化
	MPU_Init();  //MPU6050初始化

	OLED_Init();   //OLED初始化
	OLED_CLS(); //OLED清屏
	printf("ok");
	while(mpu_dmp_init())
	{
		printf("error");
		OLED_ShowStr(5,3,(unsigned char*)"NOREADY",2);
	}
	OLED_ShowStr(5,3,(unsigned char*)"READY",2);
	
	TIM1_PWM_Init(199,7199);//定时器初始化20ms 50hz
	SteperIoInit();	//初始化步进电机IO口
	 
	SteperSetSpeed(20);//设定步进电机的速度

		
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
	
		
		
 		if(TIM5CH1_CAPTURE_STA&0X80&&TIM5CH2_CAPTURE_STA&0X80)//成功捕获到了一次上升沿  这里是超声波模块1的测距功能
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F;
			temp*=65536;//溢出时间总和
			temp+=TIM5CH1_CAPTURE_VAL;//得到总的高电平时间
			temq=TIM5CH2_CAPTURE_STA&0X3F;
			temq*=65536;//溢出时间总和
			temq+=TIM5CH2_CAPTURE_VAL;//得到总的高电平时间
			
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
//				printf("X distance is:%0.2lf cm\r\n",dis-40);//打印总的高点平时间
//			  printf("Y distance is:%0.2lf cm\r\n",dic-40);//打印总的高点平时间
				
			}

      OLED_ShowStr(5,3,(unsigned char*)string1,2); //OLED显示X Y位置
			OLED_ShowStr(60,3,(unsigned char*)string2,2); //OLED显示X Y位置
			TIM5CH1_CAPTURE_STA=0;//开启下一次捕获
			TIM5CH2_CAPTURE_STA=0;//开启下一次捕获、
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
			      SteperRun();			 //启动步进电机
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


