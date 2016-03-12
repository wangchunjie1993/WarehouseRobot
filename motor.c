#include"stm32f10x.h"

extern u32 total_distance;
extern u8 circle_direction;
extern u8 line_no;
s16 cmp_counter=0;
u8 count_flag=1;
u8 turn_flag=0;
u8 turn_count=0;

/****************************
* PB0,PWM���  
****************************/
void motor_init()
{
 GPIO_InitTypeDef GPIO_InitStructure;
 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
 TIM_OCInitTypeDef TimOCInitStructure;
 NVIC_InitTypeDef NVIC_InitStructure;
  
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 GPIO_Init(GPIOB, &GPIO_InitStructure);

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //���Ƶ������
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOB, &GPIO_InitStructure);

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //���Ƶ������
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOB, &GPIO_InitStructure);


 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
  TIM_DeInit(TIM3);
  TIM_InternalClockConfig(TIM3);
  //Ԥ��Ƶϵ��Ϊ72������������ʱ��Ϊ72MHz/72 = 1MHz
  TIM_TimeBaseStructure.TIM_Prescaler = 72;
  //����ʱ�ӷָ�
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  //���ü�����ģʽΪ���ϼ���ģʽ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  //���ü��������С��ÿ��1000�����Ͳ���һ�������¼�
  TIM_TimeBaseStructure.TIM_Period = 1000-1;
  //������Ӧ�õ�TIM1��
  TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
  //��ֹARRԤװ�ػ�����
  TIM_ARRPreloadConfig(TIM3, DISABLE);
 
  TIM_Cmd(TIM3, DISABLE);  //ʧ��TIMx����


  	   //����ȱʡֵ
       TIM_OCStructInit(&TimOCInitStructure);
       //PWMģʽ1���
       TimOCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
       //����ռ�ձȣ�ռ�ձ�=(CCRx/ARR)*100%��(TIM_Pulse/TIM_Period)*100%
       TimOCInitStructure.TIM_Pulse = 500-1;
       //TIM����Ƚϼ��Ը�
       TimOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
       //ʹ�����״̬
       TimOCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	   //TimOCInitStructure.TIM_Channel = TIM_Channel_4;
       //TIM1��CH4���
       TIM_OC3Init(TIM3, &TimOCInitStructure);
	   //TIM_OC4Init(TIM1, &TimOCInitStructure);
       //����TIM2��PWM���Ϊʹ��
       TIM_CtrlPWMOutputs(TIM3,ENABLE);


  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void motor_start()
{
 TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
}

void motor_stop()
{
 TIM_Cmd(TIM3, DISABLE);  //ʧ��TIMx����
 TIM_ITConfig(TIM3,TIM_IT_CC3,DISABLE);
}

void forward_count(s16 count)
{
 count_flag=1;
 cmp_counter = count;
 
 GPIO_WriteBit(GPIOB,GPIO_Pin_1,SET);
 GPIO_WriteBit(GPIOB,GPIO_Pin_5,RESET);
 TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE);
 TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
 while(cmp_counter>=0);
 TIM_Cmd(TIM3, DISABLE);  //ʧ��TIMx����
 TIM_ITConfig(TIM3,TIM_IT_CC3,DISABLE);
}

void forward()
{
 count_flag=1;
 GPIO_WriteBit(GPIOB,GPIO_Pin_1,SET);
 GPIO_WriteBit(GPIOB,GPIO_Pin_5,RESET);
 TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE);
 TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
}

void left_count(s16 count)
{
 count_flag=0;
 cmp_counter = count;
 
 GPIO_WriteBit(GPIOB,GPIO_Pin_1,RESET);
 GPIO_WriteBit(GPIOB,GPIO_Pin_5,RESET);
 TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE);
 TIM_Cmd(TIM3,ENABLE);
 while(cmp_counter>=0);
 TIM_Cmd(TIM3,DISABLE);
 TIM_ITConfig(TIM3,TIM_IT_CC3,DISABLE);
}

void left()
{
 count_flag=0;
 GPIO_WriteBit(GPIOB,GPIO_Pin_1,RESET);
 GPIO_WriteBit(GPIOB,GPIO_Pin_5,RESET);
 TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE);
 TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
}

void right_count(s16 count)
{
 count_flag=0;
 cmp_counter = count;
 
 GPIO_WriteBit(GPIOB,GPIO_Pin_1,SET);
 GPIO_WriteBit(GPIOB,GPIO_Pin_5,SET);
 TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE);
 TIM_Cmd(TIM3,ENABLE);
 while(cmp_counter>=0);
 TIM_Cmd(TIM3,DISABLE);
 TIM_ITConfig(TIM3,TIM_IT_CC3,DISABLE);
}

void right()
{
 count_flag=0;
 GPIO_WriteBit(GPIOB,GPIO_Pin_1,SET);
 GPIO_WriteBit(GPIOB,GPIO_Pin_5,SET);
 TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE);
 TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
}

void backward_count(s16 count)
{
 count_flag=1;
 cmp_counter = count;
 
 GPIO_WriteBit(GPIOB,GPIO_Pin_1,RESET);
 GPIO_WriteBit(GPIOB,GPIO_Pin_5,SET); 
 TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE);
 TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
 while(cmp_counter>=0);
 TIM_Cmd(TIM3, DISABLE);  //ʧ��TIMx����
 TIM_ITConfig(TIM3,TIM_IT_CC3,DISABLE);
}

void backward()
{
 count_flag=1;
 GPIO_WriteBit(GPIOB,GPIO_Pin_1,RESET);
 GPIO_WriteBit(GPIOB,GPIO_Pin_5,SET);
 TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE);
 TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
}

void TIM3_IRQHandler()
{
 if(TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
 {
  cmp_counter--;
  if(count_flag==1)
  {
   total_distance++;
  }
 }
 TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
}

void trace_gpio_init()			  //��翪��   GPIOD
{
 GPIO_InitTypeDef GPIO_InitStructure;

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void trace_single_line()
{
 if((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3) == Bit_SET)&&(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4) == Bit_RESET))
 {
  turn_count=0; 
  left_count(10);
 }
 else if((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3) == Bit_RESET)&&(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4) == Bit_SET))
 {
  turn_count=0; 
  right_count(10);
 }
 else if((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3) == Bit_SET)&&(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4) == Bit_SET)
 &&(!((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2) == Bit_SET)&&(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5) == Bit_SET))))
 {
  turn_count=0; 
  forward();
 }
 else if((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3) == Bit_RESET)&&(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4) == Bit_RESET))
 {
  if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2) == Bit_SET)
  {
   turn_count=0; 
   left_count(20);
  }
  else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5) == Bit_SET)
  {
   turn_count=0; 
   right_count(20);
  }
 }
 else if((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2) == Bit_RESET)&&(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3) == Bit_RESET)
 &&(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4) == Bit_RESET)&&(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5) == Bit_RESET))
 {
  forward();
 }
}

void trace_search()
{
 if((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2) == Bit_SET)&&(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3) == Bit_SET)
 &&(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4) == Bit_SET)&&(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5) == Bit_SET))
 {
   turn_count=0; 
   forward_count(400);
   right_count(850);
   line_no=(line_no+1)%4;
   while((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2) == Bit_SET)&&(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3) == Bit_SET)
   &&(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4) == Bit_SET)&&(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5) == Bit_SET))
   forward();
   motor_stop();
 }
 else 
 {
  trace_single_line();
 }
}


void search_gpio_init()
{
 GPIO_InitTypeDef GPIO_InitStructure;

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void store()
{
 exti_disable();
 left_count(425);
 backward_count(1000);		
 put_down();		
 up();
 backward_count(300);
 right_count(425);
 forward_count(900);
 right_count(850);
 exti_init();		
}

void goto_store()
{
 		forward();
		while((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2) == Bit_RESET)&&(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3) == Bit_RESET)
 		&&(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4) == Bit_RESET)&&(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5) == Bit_RESET));
		forward_count(400);
		motor_stop();

		right_count(850);				
		while((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2) == Bit_RESET)||(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3) == Bit_RESET)
 		||(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4) == Bit_RESET)||(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5) == Bit_RESET))
		{
		 trace_single_line();
		}
		forward_count(400);
		motor_stop();		
		store();		
}

void find_gpio_init()			 //Ѱ�ҷ���
{
 GPIO_InitTypeDef GPIO_InitStructure;

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 GPIO_Init(GPIOD, &GPIO_InitStructure);
}




