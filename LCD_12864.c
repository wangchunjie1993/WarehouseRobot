//ICC-AVR application builder : 2013/7/4 18:16:05
// Target : M128
// Crystal: 8.0000Mhz

#include"stm32f10x.h"
#include"def.h"



 //���� : ����Һ���Ĵ���ͨ��Э�飬��������
void write_bit(u8 bits)
{
 u8 i;
 for(i=0;i<8;i++)
 {
  if((bits<<i) & 0x80)
  {
   SID_UP;
  }
  else
  {
   SID_DOWN;
  }
  SCLK_DOWN;
  SCLK_UP;
 }
}


//���� : д����ָ��
void write_command(u8 command)
{
 CS;
 write_bit(0xf8);//���ڸ�ʽ��д����ָ��
 write_bit(command & 0xf0);//д��4λ
 write_bit((command<<4) & 0xf0);//д��4λ
 delay_ms(2);
}

//���� : д��������
void write_data(u8 data)
{
 CS;
 write_bit(0xfa);//д����
 write_bit(data & 0xf0);//д��4λ
 write_bit((data<<4) & 0xf0);//д��4λ
 delay_ms(2);
}

 //�趨��ʾλ��
void write_address(u8 x,u8 y)
{
 u8 add;
 switch(x)
 {
  case 0:add=0x80+y;break;
  case 1:add=0x90+y;break;
  case 2:add=0x88+y;break;
  case 3:add=0x98+y;break;
 }
 write_command(add);
 delay_us(90);
}

void write_word(u8 *pos)
{
  while(*pos)
  {
   write_data(*pos);
   pos++;
   delay_us(90);
  }
}
void init_12864(void)
{

 GPIO_InitTypeDef GPIO_InitStructure;

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOE, &GPIO_InitStructure);
 
 RST_DOWN;
 delay_ms(10);
 RST_UP;
 delay_ms(50);
 write_command(0x30);//��������:����Ϊ����ָ�
 delay_us(90);
 write_command(0x0c);//������ʾ������ʾ����λ��
 delay_us(90);
 write_command(0x01);//����
 delay_ms(8);
 PSB;
}




