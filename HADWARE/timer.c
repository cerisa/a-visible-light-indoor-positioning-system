#include "timer.h"

//��ʱ��5ͨ��1��2�����벶�������Զ�ת��ֵ��Ԥ��Ƶϵ��

void TIM_Cap_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM5_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//ʹ��TIM5ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIOAʱ��
	
	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  //PB9 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//PB9 ����  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);						 //PB9����
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;  //PB8���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PB9����  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);						 //PB8����
	
	//��ʼ��TIMI5��ʱ��
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ�� 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM5ͨ��1���벶�����
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC4ӳ�䵽TI4��
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
  TIM_ICInit(TIM5, &TIM5_ICInitStructure);
		
		//��ʼ��TIMI5ͨ��2���벶�����
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	ѡ������� IC3ӳ�䵽TI4��
  TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI��
  TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
  TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC4IE�����ж�
  TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC2,ENABLE);//��������ж� ,����CC3IE�����ж�	
	
   	TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��4
   
	
	
}


u8  TIM5CH1_CAPTURE_STA=0;	//��ʱ��5ͨ��1���벶��״̬		
u8  TIM5CH2_CAPTURE_STA=0;  //��ʱ��5ͨ��2���벶��״̬
u16	TIM5CH1_CAPTURE_VAL;	//TIMI5CH1���벶��ֵ
u16	TIM5CH2_CAPTURE_VAL;	//TIMI5CH2���벶��ֵ
 
//��ʱ��5���жϷ�����
void TIM5_IRQHandler(void)
{ 

 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{	  
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)            //�����ж��¼�
		 
		{	    
			if(TIM5CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM5CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM5CH1_CAPTURE_VAL=0XFFFF;
				}else TIM5CH1_CAPTURE_STA++;
			}	 
		}
	if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//����1���������¼������������ؾ����½��ء�
		{	
			if(TIM5CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM5CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�������أ���������һ�ι���
				TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);
		   		TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}else  								//��δ��ʼ,��һ�β���������
			{
				TIM5CH1_CAPTURE_STA=0;			//���
				TIM5CH1_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM5,0);                    //����TIMx->CNT����ֵ
				TIM5CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
		   		TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			}		    
		}			     	    					   
 	}
	if((TIM5CH2_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{	  
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)            //�����ж��¼�
		 
		{	    
			if(TIM5CH2_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM5CH2_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM5CH2_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM5CH2_CAPTURE_VAL=0XFFFF;//����һ�����ֵ
				}else TIM5CH2_CAPTURE_STA++;//�������ʱ���1
			}	 
		}
	if (TIM_GetITStatus(TIM5, TIM_IT_CC2) != RESET)//����1���������¼������������ؾ����½��ء�
		{	
			if(TIM5CH2_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM5CH2_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�������أ���������һ�ι���
				TIM5CH2_CAPTURE_VAL=TIM_GetCapture2(TIM5);
		   		TIM_OC2PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}else  								//��δ��ʼ,��һ�β���������
			{
				TIM5CH2_CAPTURE_STA=0;			//���
				TIM5CH2_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM5,0);                    //����TIMx->CNT����ֵ
				TIM5CH2_CAPTURE_STA|=0X40;		//��ǲ�����������
		   		TIM_OC2PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			}		    
		}			     	    					   
 	}
 
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_Update); //����жϱ�־λ
}
 



	
	
	
	
	



 




