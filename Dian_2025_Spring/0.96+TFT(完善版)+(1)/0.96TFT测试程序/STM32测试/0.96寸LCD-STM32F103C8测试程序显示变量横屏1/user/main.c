
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��������   : 2018-1111
//  ����޸�   : 
//  ��������   : 0.96��ISP LCD 4�ӿ���ʾ����(STM32ϵ��)
/******************************************************************************
//������������STM32F103C8
//              GND   ��Դ��
//              VCC   ��5V��3.3v��Դ
//              SCL   ��PA5��SCL��
//              SDA   ��PA7��SDA��
//              RES   ��PB0
//              DC    ��PB1
//              CS    ��PA4//����ֱ�ӽӵ�
//              BLK    ��P10 BLK�������ղ��ӣ�����Ҫ�رձ����ʱ��BLK�õ͵�ƽ
*******************************************************************************/
// �޸���ʷ   :
// ��    ��   : 
// �޸�����   : 
//******************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "QDTFT_demo.h"
#include "Lcd_Driver.h"
#include "GUI.h"


int main(void)
{
u8 t;
			t=0;
  SystemInit();	//System init.
  delay_init(72);//Delay init.
				Lcd_Init();

	Lcd_Clear(WHITE);
	//	Gui_DrawFont_num(16,70, "ȫ�̼���֧��");
	//Gui_DrawFont_char(20,100,'a');
 // while(t<255)
 // {  

	//	t++;
	//	LCD_ShowNum(50,100,t,3);
//		if(t==255) t=0;
 // }
  while(1)
  {  
	QDTFT_Test_Demo();	//See the test details in QDTFT_Demo.c		
  }
}





/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
