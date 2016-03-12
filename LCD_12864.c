//ICC-AVR application builder : 2013/7/4 18:16:05
// Target : M128
// Crystal: 8.0000Mhz

#include"stm32f10x.h"
#include"def.h"



 //功能 : 按照液晶的串口通信协议，发送数据
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


//功能 : 写串口指令
void write_command(u8 command)
{
 CS;
 write_bit(0xf8);//串口格式，写控制指令
 write_bit(command & 0xf0);//写高4位
 write_bit((command<<4) & 0xf0);//写低4位
 delay_ms(2);
}

//功能 : 写串口数据
void write_data(u8 data)
{
 CS;
 write_bit(0xfa);//写数据
 write_bit(data & 0xf0);//写高4位
 write_bit((data<<4) & 0xf0);//写低4位
 delay_ms(2);
}

 //设定显示位置
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
 write_command(0x30);//功能设置:设置为基本指令集
 delay_us(90);
 write_command(0x0c);//整体显示，不显示光标和位置
 delay_us(90);
 write_command(0x01);//清屏
 delay_ms(8);
 PSB;
}




