#include"stm32f10x.h"

void sevor_init()
{
 GPIO_InitTypeDef GPIO_InitStructure;
 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
 TIM_OCInitTypeDef TimOCInitStructure;


 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 GPIO_Init(GPIOA, &GPIO_InitStructure);


 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
  TIM_DeInit(TIM1);
  TIM_InternalClockConfig(TIM1);
  //Ԥ��Ƶϵ��Ϊ72������������ʱ��Ϊ72MHz/72 = 1MHz
  TIM_TimeBaseStructure.TIM_Prescaler = 72;
  //����ʱ�ӷָ�
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  //���ü�����ģʽΪ���ϼ���ģʽ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  //���ü��������С��ÿ��1000�����Ͳ���һ�������¼�
  TIM_TimeBaseStructure.TIM_Period = 20000-1;
  //������Ӧ�õ�TIM1��
  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
  //��ֹARRԤװ�ػ�����
  TIM_ARRPreloadConfig(TIM1, DISABLE);
 
  TIM_Cmd(TIM1, ENABLE);  //ʹ��TIMx����



  //����ȱʡֵ
       TIM_OCStructInit(&TimOCInitStructure);
       //PWMģʽ1���
       TimOCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
       //����ռ�ձȣ�ռ�ձ�=(CCRx/ARR)*100%��(TIM_Pulse/TIM_Period)*100%
       TimOCInitStructure.TIM_Pulse = 1500-1;
       //TIM����Ƚϼ��Ը�
       TimOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
       //ʹ�����״̬
       TimOCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	   //TimOCInitStructure.TIM_Channel = TIM_Channel_4;
       //TIM1��CH4���
       TIM_OC1Init(TIM1, &TimOCInitStructure);
	   TIM_OC4Init(TIM1, &TimOCInitStructure);
       //����TIM2��PWM���Ϊʹ��
       TIM_CtrlPWMOutputs(TIM1,ENABLE);
}


void sevor1_modify(u16 duty)
{
 TIM_SetCompare1(TIM1,duty);
}

void sevor2_modify(u16 duty)
{
 TIM_SetCompare4(TIM1,duty);
}

void pick_up()
{
 sevor2_modify(1315); 
 sevor1_modify(1000);
 delay_ms(1000);
 sevor2_modify(1700);
 delay_ms(500);
}

void put_down()
{
 sevor2_modify(1380);
 delay_ms(200); 
 sevor1_modify(1930);
 delay_ms(200);

}

void up()
{
 sevor2_modify(1900);
}

void recover()
{
 sevor2_modify(1380);
}

void adjust()
{
 sevor2_modify(1870);
}