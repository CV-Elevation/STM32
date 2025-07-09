#include "key.h"

static uint8_t KEY_ReadPin(uint8_t key_label)
{
  switch (key_label)
  {
  case 0:
    return (uint8_t)HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
  case 1:
    return (uint8_t)HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
  case 2:
    return (uint8_t)HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
  case 3:
    return (uint8_t)HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
  case 4:
    return (uint8_t)HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
  // case X:
  //   return (uint8_t)HAL_GPIO_ReadPin(KX_GPIO_Port, KX_Pin);
  }
  return 0;
}

KEY_Configure_TypeDef KeyConfig[] = {
    {0, KEY_Mode_Long_Quick_Double, 0, KEY_Action_Release, KEY_Status_Idle, KEY_Event_Null},
    {1, KEY_Mode_Long_Quick_Double, 0, KEY_Action_Release, KEY_Status_Idle, KEY_Event_Null},
    {2, KEY_Mode_Long_Quick_Double, 0, KEY_Action_Release, KEY_Status_Idle, KEY_Event_Null},
    {3, KEY_Mode_Long_Quick_Double, 0, KEY_Action_Release, KEY_Status_Idle, KEY_Event_Null},
    {4, KEY_Mode_Long_Quick_Double, 0, KEY_Action_Release, KEY_Status_Idle, KEY_Event_Null},
    // {X, KEY_Mode_Long_Quick_Double, 0, KEY_Action_Release, KEY_Status_Idle, KEY_Event_Null},
};

KEY_Event_TypeDef key_event[ARR_LEN(KeyConfig)] = {KEY_Event_Null}; //按键事件
//按键状态处理
void KEY_ReadStateMachine(KEY_Configure_TypeDef *KeyCfg)
{
  static uint16_t tmpcnt[ARR_LEN(KeyConfig)] = {0};
  //按键动作读取
  if (KEY_ReadPin(KeyCfg->KEY_Label) == KEY_PRESSED_LEVEL)
    KeyCfg->KEY_Action = KEY_Action_Press;
  else
    KeyCfg->KEY_Action = KEY_Action_Release;

  //状态机
  switch (KeyCfg->KEY_Status)
  {
  //状态：空闲
  case KEY_Status_Idle:
    if (KeyCfg->KEY_Action == KEY_Action_Press) //动作：按下
    {
      KeyCfg->KEY_Status = KEY_Status_Debounce; //状态->消抖
      KeyCfg->KEY_Event = KEY_Event_Null;       //事件->无
    }
    else //动作：默认动作，释放
    {
      KeyCfg->KEY_Status = KEY_Status_Idle; //状态->维持
      KeyCfg->KEY_Event = KEY_Event_Null;   //事件->无
    }
    break;

  //状态：消抖
  case KEY_Status_Debounce:
    if ((KeyCfg->KEY_Action == KEY_Action_Press) && (KeyCfg->KEY_Count >= KEY_DEBOUNCE_TIME)) //动作：保持按下，消抖时间已到
    {
      KeyCfg->KEY_Count = 0;                        //计数清零
      KeyCfg->KEY_Status = KEY_Status_ConfirmPress; //状态->确认按下
      KeyCfg->KEY_Event = KEY_Event_Null;           //事件->无
    }
    else if ((KeyCfg->KEY_Action == KEY_Action_Press) && (KeyCfg->KEY_Count < KEY_DEBOUNCE_TIME)) //动作：保持按下，消抖时间未到
    {
      KeyCfg->KEY_Count++;                      //消抖计数
      KeyCfg->KEY_Status = KEY_Status_Debounce; //状态->维持
      KeyCfg->KEY_Event = KEY_Event_Null;       //事件->无
    }
    else //动作：释放，消抖时间未到，判定为抖动
    {
      KeyCfg->KEY_Count = 0;                //计数清零
      KeyCfg->KEY_Status = KEY_Status_Idle; //状态->空闲
      KeyCfg->KEY_Event = KEY_Event_Null;   //事件->无
    }
    break;

  //状态：确认按下
  case KEY_Status_ConfirmPress:
    if ((KeyCfg->KEY_Action == KEY_Action_Press) && (KeyCfg->KEY_Count >= KEY_LONG_PRESS_TIME)) //动作：保持按下，长按时间已到
    {
      KeyCfg->KEY_Count = KEY_QUICK_CLICK_TIME;         //计数置数，生成第一次连按事件
      KeyCfg->KEY_Status = KEY_Status_ConfirmPressLong; //状态->确认长按
      KeyCfg->KEY_Event = KEY_Event_Null;               //事件->无
    }
    else if ((KeyCfg->KEY_Action == KEY_Action_Press) && (KeyCfg->KEY_Count < KEY_LONG_PRESS_TIME)) //动作：保持按下，长按时间未到
    {
      KeyCfg->KEY_Count++;                          //长按计数
      KeyCfg->KEY_Status = KEY_Status_ConfirmPress; //状态->维持
      KeyCfg->KEY_Event = KEY_Event_Null;           //事件->无
    }
    else //动作：长按时间未到，释放
    {
      if ((uint8_t)(KeyCfg->KEY_Mode) & 0x04) //双击模式
      {
        KeyCfg->KEY_Count = 0;                           //计数清零
        KeyCfg->KEY_Status = KEY_Status_WaitSecondPress; //状态->等待再按
        KeyCfg->KEY_Event = KEY_Event_Null;              //事件->无
      }
      else //非双击模式
      {
        KeyCfg->KEY_Count = 0;                     //计数清零
        KeyCfg->KEY_Status = KEY_Status_Idle;      //状态->空闲
        KeyCfg->KEY_Event = KEY_Event_SingleClick; //事件->单击****
      }
    }
    break;

  //状态：确认长按
  case KEY_Status_ConfirmPressLong:
    if (KeyCfg->KEY_Action == KEY_Action_Press) //动作：保持按下
    {
      if ((uint8_t)KeyCfg->KEY_Mode & 0x02) //连按模式
      {
        if (KeyCfg->KEY_Count >= KEY_QUICK_CLICK_TIME) //连按间隔时间已到
        {
          KeyCfg->KEY_Count = 0;                            //计数清零
          KeyCfg->KEY_Status = KEY_Status_ConfirmPressLong; //状态->维持
          KeyCfg->KEY_Event = KEY_Event_QuickClick;         //事件->连按****
        }
        else //连按间隔时间未到
        {
          KeyCfg->KEY_Count++;                              //连按计数
          KeyCfg->KEY_Status = KEY_Status_ConfirmPressLong; //状态->维持
          KeyCfg->KEY_Event = KEY_Event_Null;               //事件->无
        }
      }
      else //非连按模式
      {
        KeyCfg->KEY_Count = 0;                            //计数清零
        KeyCfg->KEY_Status = KEY_Status_ConfirmPressLong; //状态->维持
        KeyCfg->KEY_Event = KEY_Event_Null;               //事件->无
      }
    }
    else //动作：长按下后释放
    {
      if ((uint8_t)KeyCfg->KEY_Mode & 0x01) //长按模式
      {
        KeyCfg->KEY_Count = 0;                   //计数清零
        KeyCfg->KEY_Status = KEY_Status_Idle;    //状态->空闲
        KeyCfg->KEY_Event = KEY_Event_LongPress; //事件->长按****
      }
      else //非长按模式
      {
        KeyCfg->KEY_Count = 0;                     //计数清零
        KeyCfg->KEY_Status = KEY_Status_Idle;      //状态->空闲
        KeyCfg->KEY_Event = KEY_Event_SingleClick; //事件->单击****
      }
    }
    break;

  //状态：等待是否再次按下
  case KEY_Status_WaitSecondPress:
    if ((KeyCfg->KEY_Action != KEY_Action_Press) && (KeyCfg->KEY_Count >= KEY_DOUBLE_CLICK_TIME)) //动作：保持释放，双击等待时间已到
    {
      KeyCfg->KEY_Count = 0;                     //计数清零
      KeyCfg->KEY_Status = KEY_Status_Idle;      //状态->空闲
      KeyCfg->KEY_Event = KEY_Event_SingleClick; //事件->单击****
    }
    else if ((KeyCfg->KEY_Action != KEY_Action_Press) && (KeyCfg->KEY_Count < KEY_DOUBLE_CLICK_TIME)) //动作：保持释放，双击等待时间未到
    {
      KeyCfg->KEY_Count++;                             //双击等待计数
      KeyCfg->KEY_Status = KEY_Status_WaitSecondPress; //状态->维持
      KeyCfg->KEY_Event = KEY_Event_Null;              //事件->无
    }
    else //动作：双击等待时间内，再次按下
    {
      tmpcnt[KeyCfg->KEY_Label] = KeyCfg->KEY_Count;  //计数保存
      KeyCfg->KEY_Count = 0;                          //计数清零
      KeyCfg->KEY_Status = KEY_Status_SecondDebounce; //状态->再次消抖
      KeyCfg->KEY_Event = KEY_Event_Null;             //事件->无
    }
    break;

  //状态：再次消抖
  case KEY_Status_SecondDebounce:
    if ((KeyCfg->KEY_Action == KEY_Action_Press) && (KeyCfg->KEY_Count >= KEY_DEBOUNCE_TIME)) //动作：保持按下，消抖时间已到
    {
      KeyCfg->KEY_Count = 0;                       //计数清零
      KeyCfg->KEY_Status = KEY_Status_SecondPress; //状态->确认再次按下
      KeyCfg->KEY_Event = KEY_Event_Null;          //事件->无
    }
    else if ((KeyCfg->KEY_Action == KEY_Action_Press) && (KeyCfg->KEY_Count < KEY_DEBOUNCE_TIME)) //动作：保持按下，消抖时间未到
    {
      KeyCfg->KEY_Count++;                            //消抖计数
      KeyCfg->KEY_Status = KEY_Status_SecondDebounce; //状态->维持
      KeyCfg->KEY_Event = KEY_Event_Null;             //事件->无
    }
    else //动作：释放，消抖时间未到，判定为抖动
    {
      KeyCfg->KEY_Count = KeyCfg->KEY_Count + tmpcnt[KeyCfg->KEY_Label]; //计数置数
      KeyCfg->KEY_Status = KEY_Status_WaitSecondPress;                   //状态->等待再按
      KeyCfg->KEY_Event = KEY_Event_Null;                                //事件->无
    }
    break;

  //状态：再次按下
  case KEY_Status_SecondPress:
    if ((KeyCfg->KEY_Action == KEY_Action_Press) && (KeyCfg->KEY_Count >= KEY_LONG_PRESS_TIME)) //动作：保持按下，长按时间已到
    {
      KeyCfg->KEY_Count = 0;                            //计数清零
      KeyCfg->KEY_Status = KEY_Status_ConfirmPressLong; //状态->确认长按
      KeyCfg->KEY_Event = KEY_Event_SingleClick;        //事件->先响应单击
    }
    else if ((KeyCfg->KEY_Action == KEY_Action_Press) && (KeyCfg->KEY_Count < KEY_LONG_PRESS_TIME)) //动作：保持按下，长按时间未到
    {
      KeyCfg->KEY_Count++;                         //计数
      KeyCfg->KEY_Status = KEY_Status_SecondPress; //状态->维持
      KeyCfg->KEY_Event = KEY_Event_Null;          //事件->无
    }
    else //动作：释放，长按时间未到
    {
      KeyCfg->KEY_Count = 0;                     //计数清零
      KeyCfg->KEY_Status = KEY_Status_Idle;      //状态->空闲
      KeyCfg->KEY_Event = KEY_Event_DoubleClick; //事件->双击
    }
    break;
  }

  if (KeyCfg->KEY_Event != KEY_Event_Null) //事件记录
    key_event[KeyCfg->KEY_Label] = KeyCfg->KEY_Event;
}

