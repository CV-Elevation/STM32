#ifndef ___MY_KEY_H__
#define ___MY_KEY_H__
#include "main.h"
#define ARR_LEN(arr) ((sizeof(arr)) / (sizeof(arr[0]))) //数组大小宏函数

#define KEY_DEBOUNCE_TIME 10      //消抖时间
#define KEY_LONG_PRESS_TIME 2000   //长按判定时间
#define KEY_QUICK_CLICK_TIME 10  //连按时间间隔
#define KEY_DOUBLE_CLICK_TIME 500 //双击判定时间
#define KEY_PRESSED_LEVEL 0 //按键被按下时的电平

//按键动作
typedef enum
{
  KEY_Action_Press,   //按住
  KEY_Action_Release, //松开
} KEY_Action_TypeDef;

//按键状态
typedef enum
{
  KEY_Status_Idle,             //空闲
  KEY_Status_Debounce,         //消抖
  KEY_Status_ConfirmPress,     //确认按下
  KEY_Status_ConfirmPressLong, //确认长按
  KEY_Status_WaitSecondPress,  //等待再次按下
  KEY_Status_SecondDebounce,   //再次消抖
  KEY_Status_SecondPress,      //再次按下
} KEY_Status_TypeDef;

//按键事件
typedef enum
{
  KEY_Event_Null,        //空事件
  KEY_Event_SingleClick, //单击
  KEY_Event_LongPress,   //长按
  KEY_Event_QuickClick,  //连击
  KEY_Event_DoubleClick, //双击
} KEY_Event_TypeDef;

//按键模式使能选择
typedef enum
{
  KEY_Mode_OnlySinge = 0x00,         //只有单击
  KEY_Mode_Long = 0x01,              //单击长按
  KEY_Mode_Quick = 0x02,             //单击连按
  KEY_Mode_Long_Quick = 0x03,        //单击长按连按
  KEY_Mode_Double = 0x04,            //单击双击
  KEY_Mode_Long_Double = 0x05,       //单击长按双击
  KEY_Mode_Quick_Double = 0x06,      //单击连按双击
  KEY_Mode_Long_Quick_Double = 0x07, //单击长按连按双击
} KEY_Mode_TypeDef;

//按键配置
typedef struct
{
  uint8_t KEY_Label;             //按键标号
  KEY_Mode_TypeDef KEY_Mode;              //按键模式
  uint16_t KEY_Count;            //按键按下计时
  KEY_Action_TypeDef KEY_Action; //按键动作，按下或释放
  KEY_Status_TypeDef KEY_Status; //按键状态
  KEY_Event_TypeDef KEY_Event;   //按键事件
} KEY_Configure_TypeDef;

extern KEY_Configure_TypeDef KeyConfig[];
extern KEY_Event_TypeDef key_event[];

void KEY_ReadStateMachine(KEY_Configure_TypeDef *KeyCfg);

#endif

