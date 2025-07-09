#include "lcd_st7735.h"
#include "font.h"
#include <stdio.h>
#include "math.h"
#include "stdlib.h"
/*
要横屏显示：
1.长高对调、xyoffset对调
要竖屏显示：
1.长高对调、xyoffset对调
2.删去初始化最后的这两条指令
	TFT_WRITE_COMMAND(0x36);
  TFT_WRITE_DATA(0x60);
*/
extern SPI_HandleTypeDef hspi1;

_lcd_dev  lcddev;

void TFT_WRITE_DATA(uint8_t data)
{
	CS_LOW;
	DATA;
	HAL_SPI_Transmit_DMA(&hspi1,&data,1);
	CS_HIGH;
}

void TFT_WRITE_COMMAND(uint8_t command)
{
	CS_LOW;
	COMMAND;
	HAL_SPI_Transmit_DMA(&hspi1,&command,1);
	CS_HIGH;
}

void TFT_WRITE_REG(uint8_t reg,uint16_t reg_value)
{
	TFT_WRITE_COMMAND(reg);
	TFT_WRITE_DATA(reg_value);
}

void TFT_WRITE_u16_DATA(uint16_t temp)
{

		uint8_t tempBuf[2];
		tempBuf[0] = temp>>8;
		tempBuf[1] = temp;
		CS_LOW ;
		DATA ;
		HAL_SPI_Transmit_DMA(&hspi1,tempBuf, 2);
		CS_HIGH ;
}


//Set of coordinate for start point
void set_windows(uint16_t xStar,uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
{
	TFT_WRITE_COMMAND(lcddev.setxcmd);
	TFT_WRITE_DATA(0x00);
	TFT_WRITE_DATA(xStar+lcddev.xoffset);
	TFT_WRITE_DATA(0x00);
	TFT_WRITE_DATA(xEnd+lcddev.xoffset);

	TFT_WRITE_COMMAND(lcddev.setycmd);
	TFT_WRITE_DATA(0x00);
	TFT_WRITE_DATA(yStar+lcddev.yoffset);
	TFT_WRITE_DATA(0x00);
	TFT_WRITE_DATA(yEnd+lcddev.yoffset);

	TFT_WRITE_COMMAND(0X2C);
}

//Clear screen to single color
void lcd_clear(uint16_t color)
{
	uint16_t i,m;
	set_windows(0,0,lcddev.width-1,lcddev.height-1);
	DATA;
	CS_LOW;

	for(i=0;i<lcddev.height;i++)
		{
	    for(m=0;m<lcddev.width;m++)
	    {
	    	TFT_WRITE_u16_DATA(color);
			}
		}
	CS_HIGH;
}

//设置方向 默认为横屏幕
void  lcd_set_direction()
{
	lcddev.setxcmd=LCD_X;
	lcddev.setycmd=LCD_Y;
	lcddev.wramcmd=LCD_CMD;
	lcddev.width=LCD_Width;
	lcddev.height=LCD_Height;
	lcddev.xoffset=LCD_XOffset;
	lcddev.yoffset=LCD_YOffset;
//lcddev.width=LCD_Height;
//lcddev.height=LCD_Width;
//lcddev.xoffset=LCD_YOffset+1;
//lcddev.yoffset=LCD_XOffset;
	
	lcddev.dir=LCD_DIR;
}

void tft_init()
{

RSTL;
	HAL_Delay(100);
	RSTH;
	HAL_Delay(100);

	TFT_WRITE_COMMAND(0x11);
	HAL_Delay(120);

	TFT_WRITE_COMMAND(0xB1);
	TFT_WRITE_DATA(0x01);
	TFT_WRITE_DATA(0x2C);
	TFT_WRITE_DATA(0x2D);

	TFT_WRITE_COMMAND(0xB2);
	TFT_WRITE_DATA(0x01);
	TFT_WRITE_DATA(0x2C);
	TFT_WRITE_DATA(0x2D);


	TFT_WRITE_COMMAND(0xB3);
	TFT_WRITE_DATA(0x01);
	TFT_WRITE_DATA(0x2C);
	TFT_WRITE_DATA(0x2D);
	TFT_WRITE_DATA(0x01);
	TFT_WRITE_DATA(0x2C);
	TFT_WRITE_DATA(0x2D);

	TFT_WRITE_COMMAND(0xB4);
	TFT_WRITE_DATA(0x07);

	TFT_WRITE_COMMAND(0xC0);
	TFT_WRITE_DATA(0xA2);
	TFT_WRITE_DATA(0x02);
	TFT_WRITE_DATA(0x84);
	TFT_WRITE_COMMAND(0xC1);
	TFT_WRITE_DATA(0xC5);


	TFT_WRITE_COMMAND(0xC2);
	TFT_WRITE_DATA(0x0A);
	TFT_WRITE_DATA(0x00);

	TFT_WRITE_COMMAND(0xC3);
	TFT_WRITE_DATA(0x8A);
	TFT_WRITE_DATA(0x2A);
	TFT_WRITE_COMMAND(0xC4);
	TFT_WRITE_DATA(0x8A);
	TFT_WRITE_DATA(0xEE);

	TFT_WRITE_COMMAND(0xC5);
	TFT_WRITE_DATA(0x0E);

	TFT_WRITE_COMMAND(0x36);
	TFT_WRITE_DATA(0xC8);

	TFT_WRITE_COMMAND(0xe0);
	TFT_WRITE_DATA(0x0F);
	TFT_WRITE_DATA(0x1A);
	TFT_WRITE_DATA(0x0F);
	TFT_WRITE_DATA(0x18);
	TFT_WRITE_DATA(0x2F);
	TFT_WRITE_DATA(0x28);

	TFT_WRITE_DATA(0x20);
	TFT_WRITE_DATA(0x22);
	TFT_WRITE_DATA(0x1F);

	TFT_WRITE_DATA(0x1B);
	TFT_WRITE_DATA(0x23);
	TFT_WRITE_DATA(0x37);

	TFT_WRITE_DATA(0x00);

	TFT_WRITE_DATA(0x07);
	TFT_WRITE_DATA(0x02);
	TFT_WRITE_DATA(0x10);

	TFT_WRITE_COMMAND(0xe1);
	TFT_WRITE_DATA(0x0F);
	TFT_WRITE_DATA(0x1B);
	TFT_WRITE_DATA(0x0F);

	TFT_WRITE_DATA(0x17);

	TFT_WRITE_DATA(0x33);
	TFT_WRITE_DATA(0x2C);

	TFT_WRITE_DATA(0x29);

	TFT_WRITE_DATA(0x2E);
	TFT_WRITE_DATA(0x28);
	TFT_WRITE_DATA(0x30);

	TFT_WRITE_DATA(0x30);

	TFT_WRITE_DATA(0x39);
	TFT_WRITE_DATA(0x3F);

	TFT_WRITE_DATA(0x00);
	TFT_WRITE_DATA(0x07);
	TFT_WRITE_DATA(0x03);
	TFT_WRITE_DATA(0x10);

	TFT_WRITE_COMMAND(0x2A);
	TFT_WRITE_DATA(0x00);
	TFT_WRITE_DATA(0x00);
	TFT_WRITE_DATA(0x00);
	TFT_WRITE_DATA(0x7F);

	TFT_WRITE_COMMAND(0x2B);
	TFT_WRITE_DATA(0x00);
	TFT_WRITE_DATA(0x00);
	TFT_WRITE_DATA(0x00);
	TFT_WRITE_DATA(0x9F);

	TFT_WRITE_COMMAND(0xF0);
	TFT_WRITE_DATA(0x01);
	TFT_WRITE_COMMAND(0xF6);
	TFT_WRITE_DATA(0x00);

	TFT_WRITE_COMMAND(0x3A);
	TFT_WRITE_DATA(0x05);
	TFT_WRITE_COMMAND(0x29);
	lcd_set_direction();

	TFT_WRITE_REG(0x36,(1<<3)|(1<<6)|(1<<7));
	TFT_WRITE_COMMAND(0xc0);
	TFT_WRITE_DATA(0x0A);
//	TFT_WRITE_COMMAND(0x36);
//  TFT_WRITE_DATA(0x60);
	lcd_clear(0xffe0);
	LEDH;
}

//Display point in (x, y) with color
void draw_point(uint16_t x,uint16_t y,uint16_t color)
{
	set_windows(x,y,x,y);
	TFT_WRITE_u16_DATA(color);
}

//Display ch (char type) in (x, y) with back_color, front_color and front_size. font.h is essential.
void TFT_ShowChar(uint16_t x, uint16_t y, char ch, uint16_t back_color, uint16_t font_color, uint8_t font_size)
{
	int i = 0, j = 0;
	uint8_t temp = 0;
	uint8_t size = 0;
	uint8_t t = 0;

	 if((x > (LCD_Width - font_size / 2)) || (y > (LCD_Height - font_size)))
		 return;

	 set_windows(x, y, x + font_size/2 - 1, y + font_size - 1);

	 ch = ch - ' ';


	 if((font_size == 16) || (font_size == 32) )
	 {
 			size = (font_size / 8 + ((font_size % 8) ? 1 : 0)) * (font_size / 2);

			for(i = 0; i < size; i++)
			{
					if(font_size == 16)
						temp = asc2_1608[ch][i];
					else if(font_size == 32)
							temp = asc2_3216[ch][i];
					else
							return;

					for(j = 0; j < 8; j++)
					{
							if(temp & 0x80)
								{

									TFT_WRITE_u16_DATA(font_color);
								}
							else
							{

								TFT_WRITE_u16_DATA(back_color);
							}
							temp <<= 1;
					}
			}
	 }
	 else if(font_size == 12)
	 {
 			size = (font_size / 8 + ((font_size % 8) ? 1 : 0)) * (font_size / 2);

			for(i = 0; i < size; i++)
			{
				  temp = asc2_1206[ch][i];

					for(j = 0; j < 6; j++)
					{
							if(temp & 0x80)
							{
								TFT_WRITE_u16_DATA(font_color);
							}
							else
							{
								TFT_WRITE_u16_DATA(back_color);
							}

							temp <<= 1;
					}
			}
	 }
	 else if(font_size == 24)
	 {
 			size = (font_size * 16) / 8;

			for(i = 0; i < size; i++)
			{
				  temp = asc2_2412[ch][i];
					if(i % 2 == 0)
							t = 8;
					else
							t = 4;
					for(j = 0; j < t; j++)
					{
							if(temp & 0x80)
							{

								TFT_WRITE_u16_DATA(font_color);
							}
							else
							{

								TFT_WRITE_u16_DATA(back_color);
							}

							temp <<= 1;
					}
			}
	 }
	 else
		 return;
}

//Display str (string type) in (x, y) with back_color, front_color and front_size under max_width. font.h is essential.
void LCD_ShowCharStr(uint16_t x, uint16_t y, uint32_t max_width, char* str, uint16_t back_color, uint16_t font_color, uint8_t font_size)
{

	max_width += x;

	while((*str <= '~') && (*str >= ' '))
	{
			if(x >= max_width)
			{
					break;
			}

			TFT_ShowChar(x,y,*str,back_color, font_color,font_size);
			x += font_size / 2;
			str++;
	}
}


//Display number (byte type) in (x, y) with back_color, front_color and front_size under max_width. font.h is essential.
void LCD_ShowCharNumber(uint16_t x, uint16_t y, uint32_t max_width, uint8_t  number, uint16_t back_color, uint16_t font_color, uint8_t font_size)
{
	char number_ascii[10];

	sprintf(number_ascii,"%d",number);

	LCD_ShowCharStr(x,y,max_width,number_ascii,back_color,font_color,font_size);
}
void showimage(const unsigned char *p,uint16_t width,uint16_t height) //显示40*40 QQ图片
{
  int i; 
	unsigned char picH,picL;
	uint16_t wid=width;
	uint16_t hei=height;
	lcd_clear(WHITE); //清屏
	set_windows(0,0,width-1,height-1);		//坐标设置
	for(i=0;i<wid*height;i++)
 {	
	picL=*(p+i*2);	//数据低位在前
	picH=*(p+i*2+1);				
	TFT_WRITE_u16_DATA(~(picH<<8|picL));
 }	
}

uint8_t rgb565_to_grayscale(uint16_t rgb565) {
    // 提取RGB分量
    uint8_t r = (rgb565 >> 11) & 0x1F; // 红色分量
    uint8_t g = (rgb565 >> 5) & 0x3F;  // 绿色分量
    uint8_t b = rgb565 & 0x1F;         // 蓝色分量

    // 扩展到8位
    r = (r << 3) | (r >> 2); // 5位 -> 8位
    g = (g << 2) | (g >> 4); // 6位 -> 8位
    b = (b << 3) | (b >> 2); // 5位 -> 8位

    // 计算灰度值
    uint8_t gray = (uint8_t)(0.299 * r + 0.587 * g + 0.114 * b + 0.5);

    return gray;
}
uint16_t grayscale_to_rgb565(uint8_t gray) {
    // 将灰度值映射到RGB565的每个通道
    uint8_t r = (gray * 31) / 255; // 红色分量（5位）
    uint8_t g = (gray * 63) / 255; // 绿色分量（6位）
    uint8_t b = (gray * 31) / 255; // 蓝色分量（5位）

    // 将R、G、B分量组合成16位真彩色值
    uint16_t rgb565 = (r << 11) | (g << 5) | b;

    return rgb565;
}
void showgray(const unsigned char *p,uint16_t width,uint16_t height)
{
	int i; 
	unsigned char picH,picL;
	uint16_t wid=width;
	uint16_t hei=height;
	picL=*(p+i*2);	//数据低位在前
	picH=*(p+i*2+1);
	uint16_t rgb565;
	uint16_t gray_rgb;
	lcd_clear(WHITE); //清屏
	set_windows(0,0,width-1,height-1);		//坐标设置
		for(i=0;i<wid*height;i++)
 {	
	picL=*(p+i*2);	//数据低位在前
	picH=*(p+i*2+1);				
	rgb565=(picH<<8|picL);
	gray_rgb=grayscale_to_rgb565(rgb565_to_grayscale(rgb565));
	TFT_WRITE_u16_DATA(~(gray_rgb));	 
 }	
}
void sobel_edge_detection(uint8_t input[40 * 40], uint8_t output[40 * 40]) {
    int gx, gy;
    int i, j;

    // Sobel算子卷积核
    int sobel_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int sobel_y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // 计算梯度幅值的最大值
    int g_max = 0;

    for (i = 1; i < 40 - 1; i++) {
        for (j = 1; j < 40 - 1; j++) {
            gx = 0;
            gy = 0;

            // 计算梯度
            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    int index = (i + x) * 40 + (j + y); // 计算一维索引
                    gx += input[index] * sobel_x[x + 1][y + 1];
                    gy += input[index] * sobel_y[x + 1][y + 1];
                }
            }

            // 计算梯度幅值
            int g = (int)sqrt(gx * gx + gy * gy);
            if (g > g_max) g_max = g; // 更新最大值

            // 存储原始梯度幅值
            output[i * 40 + j] = g;
        }
    }

    // 将梯度幅值缩放到 [0, 255]
    for (i = 1; i < 40 - 1; i++) {
        for (j = 1; j < 40 - 1; j++) {
            int index = i * 40 + j;
            output[index] = (uint8_t)((output[index] / (float)g_max) * 255);
        }
    }
}
void graypic(const unsigned char *p, uint8_t output[40 * 40]) 
{
	unsigned char picH,picL;
	uint16_t rgb565;
	uint16_t gray_rgb;
	for(int i=0;i<40*40;i++)
 {	
	picL=*(p+i*2);	//数据低位在前
	picH=*(p+i*2+1);				
	rgb565=(picH<<8|picL);
	output[i]= rgb565_to_grayscale(rgb565);
 }
}
void direct_show_gray(uint8_t gray[1600])
{
	lcd_clear(WHITE); //清屏
	set_windows(0,0,39,39);		//坐标设置
for(int i=0;i<1600;i++)
{
	TFT_WRITE_u16_DATA(grayscale_to_rgb565(gray[i]));	
}
}
void color_detection(const unsigned char *p)
{
	uint16_t rgb565;
	unsigned char picH,picL;
	uint16_t Rnum=0,Gnum=0,Bnum=0,othernum=0;
		for(int i=0;i<40*40;i++)
 {	
	picL=*(p+i*2);	//数据低位在前
	picH=*(p+i*2+1);				
	rgb565=(picH<<8|picL);
	 if(rgb565==~RED)Rnum++;
		else if(rgb565==~GREEN)Gnum++;
		else if(rgb565==~BLUE)Bnum++;
			else othernum++;
 }
// char *Rstr,*Gstr,*Bstr,*othstr;
// sprintf(Rstr,"%d",Rnum);
// sprintf(Gstr,"%d",Gnum);
// sprintf(Bstr,"%d",Bnum);
// sprintf(othstr,"%d",othernum);
// LCD_ShowCharStr(0,40,10,"R:",WHITE,BLACK,16);
// LCD_ShowCharStr(0,56,10,"G:",WHITE,BLACK,16);
// LCD_ShowCharStr(0,72,10,"B:",WHITE,BLACK,16);
// LCD_ShowCharStr(0,88,30,"Other:",WHITE,BLACK,16);
// LCD_ShowCharStr(16,40,30,Rstr,WHITE,BLACK,16);
// LCD_ShowCharStr(16,56,30,Gstr,WHITE,BLACK,16);
// LCD_ShowCharStr(16,72,30,Bstr,WHITE,BLACK,16);
// LCD_ShowCharStr(16,88,30,othstr,WHITE,BLACK,16);
 printf("\xff\xff\xfft1.txt=\"R:%d G:%d B:%d Other:%d\"\xff\xff\xff",Rnum,Gnum,Bnum,othernum);
}