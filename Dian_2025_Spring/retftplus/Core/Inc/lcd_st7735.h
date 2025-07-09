/*
 *
 * For 0.96 inch TFT LCD driven by ST7735S
 *
 */
#ifndef _lcd_st7735_h
#define _lcd_st7735_h

#include "main.h"
/* TFT LCD Module Connection:
 MODULE          STM32G030F6P6
 CS                 PA4
 DC(DATA/COMMAND)   PA6
 BACK_LED           PA11
 RESET              PA12
 SCK                PA5 (SPI1 CLK)
 SDA                PA7 (SPI1 MOSI)
 VCC                3.3V/5V
 GND                0V
 */

#define CS_GPIO GPIO_PIN_4
#define CS_GPIO_PORT GPIOA

#define CMD_DATA_GPIO GPIO_PIN_6
#define CMD_DATA_GPIO_PORT GPIOA

#define BACK_LED_GPIO GPIO_PIN_11
#define BACK_LED_GPIO_PORT GPIOA

#define RESET_GPIO GPIO_PIN_12
#define RESET_GPIO_PORT GPIOA


#define CS_HIGH   HAL_GPIO_WritePin(CS_GPIO_PORT, CS_GPIO, GPIO_PIN_SET)
#define CS_LOW    HAL_GPIO_WritePin(CS_GPIO_PORT, CS_GPIO, GPIO_PIN_RESET)

#define DATA      HAL_GPIO_WritePin(CMD_DATA_GPIO_PORT, CMD_DATA_GPIO, GPIO_PIN_SET)
#define COMMAND   HAL_GPIO_WritePin(CMD_DATA_GPIO_PORT, CMD_DATA_GPIO, GPIO_PIN_RESET)

#define RSTH      HAL_GPIO_WritePin(RESET_GPIO_PORT, RESET_GPIO, GPIO_PIN_SET)
#define RSTL      HAL_GPIO_WritePin(RESET_GPIO_PORT, RESET_GPIO, GPIO_PIN_RESET)

#define LEDH      HAL_GPIO_WritePin(RESET_GPIO_PORT, BACK_LED_GPIO, GPIO_PIN_SET)
#define LEDL      HAL_GPIO_WritePin(RESET_GPIO_PORT, BACK_LED_GPIO, GPIO_PIN_RESET)

//LCD colors used as inversion of RGB565
#define WHITE     ~0xFFFF
#define BLACK     ~0x0000
#define RED       ~0xF800
#define GREEN     ~0x07E0
#define BLUE      ~0x001F
#define YELLOW    ~0xFFE0
#define PURPLE    ~0xF81F
#define MAGENTA   ~0xF81F
#define NAVY      ~0x000F
#define DGREEN    ~0x03E0
#define DCYAN     ~0x03EF
#define MAROON    ~0x7800
#define OLIVE     ~0x7BE0
#define LGRAY     ~0xC618
#define DGRAY     ~0x7BEF
#define CYAN      ~0x07FF        

 //LCD parameters
#define LCD_Width 80
#define LCD_Height 160
#define LCD_X 0x2a
#define LCD_Y 0x2b
#define LCD_XOffset 0x1a  //Adjust to correct display location of left-top point
#define LCD_YOffset 0x00  //Adjust to correct display location of left-top point
#define LCD_CMD 0x2c
#define LCD_DIR 0

 typedef struct
 {
 	uint16_t  width;			//LCD width
 	uint16_t  height;	        //LCD height
 	uint16_t  id;		        //LCD ID
 	uint8_t   dir;			    //LCD direction  0: vertical; 1: horizontal;
 	uint16_t  wramcmd;		    //cmd to write gram
 	uint16_t  setxcmd;		    //x coordinate
 	uint16_t  setycmd;		    //y coordinate
 	uint8_t   xoffset;          //x offset
 	uint8_t	  yoffset;          //y offset
 } _lcd_dev;
void TFT_WRITE_DATA(uint8_t data);
 void TFT_WRITE_COMMAND(uint8_t command);
 void TFT_WRITE_REG(uint8_t reg,uint16_t reg_value);
 void TFT_WRITE_u16_DATA(uint16_t temp);
 void lcd_clear(uint16_t color);
 void tft_init();
void set_windows(uint16_t xStar,uint16_t yStar,uint16_t xEnd,uint16_t yEnd);
 void draw_point(uint16_t x,uint16_t y,uint16_t color);
 void TFT_ShowChar(uint16_t x, uint16_t y, char ch, uint16_t back_color, uint16_t font_color, uint8_t font_size);
 void LCD_ShowCharNumber(uint16_t x, uint16_t y, uint32_t max_width, uint8_t  number, uint16_t back_color, uint16_t font_color, uint8_t font_size);
 void LCD_ShowCharStr(uint16_t x, uint16_t y, uint32_t max_width, char* str, uint16_t back_color, uint16_t font_color, uint8_t font_size);
void showimage(const unsigned char *p,uint16_t width,uint16_t height);
 void showgray(const unsigned char *p,uint16_t width,uint16_t height);
 void sobel_edge_detection(uint8_t input[40*40], uint8_t output[40*40]) ;
 void graypic(const unsigned char *p, uint8_t output[40 * 40]) ;
void direct_show_gray(uint8_t gray[1600]);
 void color_detection(const unsigned char *p);
#endif



