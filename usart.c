#include"stm32f10x.h"

s8 receive_count=0;
u8 RxData=0;
u8 dis_H=0,dis_L=0;

void usart_init()
{
 GPIO_InitTypeDef GPIO_InitStructure;
 USART_InitTypeDef USART_InitStructure;
 NVIC_InitTypeDef NVIC_InitStructure;

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		   		//usart1  Tx
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 GPIO_Init(GPIOA,&GPIO_InitStructure);

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		   		//usart1  Rx
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 GPIO_Init(GPIOA,&GPIO_InitStructure);

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		   		//usart3  Tx
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 GPIO_Init(GPIOB,&GPIO_InitStructure);

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;		   		//usart3  Rx
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 GPIO_Init(GPIOB,&GPIO_InitStructure);

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

 USART_InitStructure.USART_BaudRate = 115200;
 USART_InitStructure.USART_WordLength = USART_WordLength_8b;
 USART_InitStructure.USART_StopBits = USART_StopBits_1;
 USART_InitStructure.USART_Parity = USART_Parity_No;
 USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
 USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
 USART_Init(USART1,&USART_InitStructure);
 USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
 
 USART_InitStructure.USART_BaudRate = 115200;
 USART_InitStructure.USART_WordLength = USART_WordLength_8b;
 USART_InitStructure.USART_StopBits = USART_StopBits_1;
 USART_InitStructure.USART_Parity = USART_Parity_No;
 USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
 USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
 USART_Init(USART3,&USART_InitStructure);
 USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  
 
 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

 NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 NVIC_Init(&NVIC_InitStructure);

 NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 NVIC_Init(&NVIC_InitStructure);

 USART_Cmd(USART1, ENABLE);
 USART_Cmd(USART3, ENABLE);
}

void usart1_transmit(u8 data)
{
 USART_SendData(USART1, data);
 while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void usart3_transmit(u8 data)
{
 USART_SendData(USART3, data);
 while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}



void USART1_IRQHandler()
{
 if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
 {
  RxData = USART_ReceiveData(USART1);
  if(receive_count==0)
  {
   dis_H=RxData;
  }
  else if(receive_count==1)
  {
   dis_L=RxData;
  }
 }
 USART_ClearITPendingBit(USART1,USART_IT_RXNE);
 receive_count++; 
}

void USART3_IRQHandler()
{
 if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET)
 {
  RxData = USART_ReceiveData(USART3);
  if(receive_count==0)
  {
   dis_H=RxData;
  }
  else if(receive_count==1)
  {
   dis_L=RxData;
  }
 }
 USART_ClearITPendingBit(USART3,USART_IT_RXNE);
 receive_count++; 
} 
