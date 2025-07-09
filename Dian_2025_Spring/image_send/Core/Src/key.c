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

KEY_Event_TypeDef key_event[ARR_LEN(KeyConfig)] = {KEY_Event_Null}; //�����¼�
//����״̬����
void KEY_ReadStateMachine(KEY_Configure_TypeDef *KeyCfg)
{
  static uint16_t tmpcnt[ARR_LEN(KeyConfig)] = {0};
  //����������ȡ
  if (KEY_ReadPin(KeyCfg->KEY_Label) == KEY_PRESSED_LEVEL)
    KeyCfg->KEY_Action = KEY_Action_Press;
  else
    KeyCfg->KEY_Action = KEY_Action_Release;

  //״̬��
  switch (KeyCfg->KEY_Status)
  {
  //״̬������
  case KEY_Status_Idle:
    if (KeyCfg->KEY_Action == KEY_Action_Press) //����������
    {
      KeyCfg->KEY_Status = KEY_Status_Debounce; //״̬->����
      KeyCfg->KEY_Event = KEY_Event_Null;       //�¼�->��
    }
    else //������Ĭ�϶������ͷ�
    {
      KeyCfg->KEY_Status = KEY_Status_Idle; //״̬->ά��
      KeyCfg->KEY_Event = KEY_Event_Null;   //�¼�->��
    }
    break;

  //״̬������
  case KEY_Status_Debounce:
    if ((KeyCfg->KEY_Action == KEY_Action_Press) && (KeyCfg->KEY_Count >= KEY_DEBOUNCE_TIME)) //���������ְ��£�����ʱ���ѵ�
    {
      KeyCfg->KEY_Count = 0;                        //��������
      KeyCfg->KEY_Status = KEY_Status_ConfirmPress; //״̬->ȷ�ϰ���
      KeyCfg->KEY_Event = KEY_Event_Null;           //�¼�->��
    }
    else if ((KeyCfg->KEY_Action == KEY_Action_Press) && (KeyCfg->KEY_Count < KEY_DEBOUNCE_TIME)) //���������ְ��£�����ʱ��δ��
    {
      KeyCfg->KEY_Count++;                      //��������
      KeyCfg->KEY_Status = KEY_Status_Debounce; //״̬->ά��
      KeyCfg->KEY_Event = KEY_Event_Null;       //�¼�->��
    }
    else //�������ͷţ�����ʱ��δ�����ж�Ϊ����
    {
      KeyCfg->KEY_Count = 0;                //��������
      KeyCfg->KEY_Status = KEY_Status_Idle; //״̬->����
      KeyCfg->KEY_Event = KEY_Event_Null;   //�¼�->��
    }
    break;

  //״̬��ȷ�ϰ���
  case KEY_Status_ConfirmPress:
    if ((KeyCfg->KEY_Action == KEY_Action_Press) && (KeyCfg->KEY_Count >= KEY_LONG_PRESS_TIME)) //���������ְ��£�����ʱ���ѵ�
    {
      KeyCfg->KEY_Count = KEY_QUICK_CLICK_TIME;         //�������������ɵ�һ�������¼�
      KeyCfg->KEY_Status = KEY_Status_ConfirmPressLong; //״̬->ȷ�ϳ���
      KeyCfg->KEY_Event = KEY_Event_Null;               //�¼�->��
    }
    else if ((KeyCfg->KEY_Action == KEY_Action_Press) && (KeyCfg->KEY_Count < KEY_LONG_PRESS_TIME)) //���������ְ��£�����ʱ��δ��
    {
      KeyCfg->KEY_Count++;                          //��������
      KeyCfg->KEY_Status = KEY_Status_ConfirmPress; //״̬->ά��
      KeyCfg->KEY_Event = KEY_Event_Null;           //�¼�->��
    }
    else //����������ʱ��δ�����ͷ�
    {
      if ((uint8_t)(KeyCfg->KEY_Mode) & 0x04) //˫��ģʽ
      {
        KeyCfg->KEY_Count = 0;                           //��������
        KeyCfg->KEY_Status = KEY_Status_WaitSecondPress; //״̬->�ȴ��ٰ�
        KeyCfg->KEY_Event = KEY_Event_Null;              //�¼�->��
      }
      else //��˫��ģʽ
      {
        KeyCfg->KEY_Count = 0;                     //��������
        KeyCfg->KEY_Status = KEY_Status_Idle;      //״̬->����
        KeyCfg->KEY_Event = KEY_Event_SingleClick; //�¼�->����****
      }
    }
    break;

  //״̬��ȷ�ϳ���
  case KEY_Status_ConfirmPressLong:
    if (KeyCfg->KEY_Action == KEY_Action_Press) //���������ְ���
    {
      if ((uint8_t)KeyCfg->KEY_Mode & 0x02) //����ģʽ
      {
        if (KeyCfg->KEY_Count >= KEY_QUICK_CLICK_TIME) //�������ʱ���ѵ�
        {
          KeyCfg->KEY_Count = 0;                            //��������
          KeyCfg->KEY_Status = KEY_Status_ConfirmPressLong; //״̬->ά��
          KeyCfg->KEY_Event = KEY_Event_QuickClick;         //�¼�->����****
        }
        else //�������ʱ��δ��
        {
          KeyCfg->KEY_Count++;                              //��������
          KeyCfg->KEY_Status = KEY_Status_ConfirmPressLong; //״̬->ά��
          KeyCfg->KEY_Event = KEY_Event_Null;               //�¼�->��
        }
      }
      else //������ģʽ
      {
        KeyCfg->KEY_Count = 0;                            //��������
        KeyCfg->KEY_Status = KEY_Status_ConfirmPressLong; //״̬->ά��
        KeyCfg->KEY_Event = KEY_Event_Null;               //�¼�->��
      }
    }
    else //�����������º��ͷ�
    {
      if ((uint8_t)KeyCfg->KEY_Mode & 0x01) //����ģʽ
      {
        KeyCfg->KEY_Count = 0;                   //��������
        KeyCfg->KEY_Status = KEY_Status_Idle;    //״̬->����
        KeyCfg->KEY_Event = KEY_Event_LongPress; //�¼�->����****
      }
      else //�ǳ���ģʽ
      {
        KeyCfg->KEY_Count = 0;                     //��������
        KeyCfg->KEY_Status = KEY_Status_Idle;      //״̬->����
        KeyCfg->KEY_Event = KEY_Event_SingleClick; //�¼�->����****
      }
    }
    break;

  //״̬���ȴ��Ƿ��ٴΰ���
  case KEY_Status_WaitSecondPress:
    if ((KeyCfg->KEY_Action != KEY_Action_Press) && (KeyCfg->KEY_Count >= KEY_DOUBLE_CLICK_TIME)) //�����������ͷţ�˫���ȴ�ʱ���ѵ�
    {
      KeyCfg->KEY_Count = 0;                     //��������
      KeyCfg->KEY_Status = KEY_Status_Idle;      //״̬->����
      KeyCfg->KEY_Event = KEY_Event_SingleClick; //�¼�->����****
    }
    else if ((KeyCfg->KEY_Action != KEY_Action_Press) && (KeyCfg->KEY_Count < KEY_DOUBLE_CLICK_TIME)) //�����������ͷţ�˫���ȴ�ʱ��δ��
    {
      KeyCfg->KEY_Count++;                             //˫���ȴ�����
      KeyCfg->KEY_Status = KEY_Status_WaitSecondPress; //״̬->ά��
      KeyCfg->KEY_Event = KEY_Event_Null;              //�¼�->��
    }
    else //������˫���ȴ�ʱ���ڣ��ٴΰ���
    {
      tmpcnt[KeyCfg->KEY_Label] = KeyCfg->KEY_Count;  //��������
      KeyCfg->KEY_Count = 0;                          //��������
      KeyCfg->KEY_Status = KEY_Status_SecondDebounce; //״̬->�ٴ�����
      KeyCfg->KEY_Event = KEY_Event_Null;             //�¼�->��
    }
    break;

  //״̬���ٴ�����
  case KEY_Status_SecondDebounce:
    if ((KeyCfg->KEY_Action == KEY_Action_Press) && (KeyCfg->KEY_Count >= KEY_DEBOUNCE_TIME)) //���������ְ��£�����ʱ���ѵ�
    {
      KeyCfg->KEY_Count = 0;                       //��������
      KeyCfg->KEY_Status = KEY_Status_SecondPress; //״̬->ȷ���ٴΰ���
      KeyCfg->KEY_Event = KEY_Event_Null;          //�¼�->��
    }
    else if ((KeyCfg->KEY_Action == KEY_Action_Press) && (KeyCfg->KEY_Count < KEY_DEBOUNCE_TIME)) //���������ְ��£�����ʱ��δ��
    {
      KeyCfg->KEY_Count++;                            //��������
      KeyCfg->KEY_Status = KEY_Status_SecondDebounce; //״̬->ά��
      KeyCfg->KEY_Event = KEY_Event_Null;             //�¼�->��
    }
    else //�������ͷţ�����ʱ��δ�����ж�Ϊ����
    {
      KeyCfg->KEY_Count = KeyCfg->KEY_Count + tmpcnt[KeyCfg->KEY_Label]; //��������
      KeyCfg->KEY_Status = KEY_Status_WaitSecondPress;                   //״̬->�ȴ��ٰ�
      KeyCfg->KEY_Event = KEY_Event_Null;                                //�¼�->��
    }
    break;

  //״̬���ٴΰ���
  case KEY_Status_SecondPress:
    if ((KeyCfg->KEY_Action == KEY_Action_Press) && (KeyCfg->KEY_Count >= KEY_LONG_PRESS_TIME)) //���������ְ��£�����ʱ���ѵ�
    {
      KeyCfg->KEY_Count = 0;                            //��������
      KeyCfg->KEY_Status = KEY_Status_ConfirmPressLong; //״̬->ȷ�ϳ���
      KeyCfg->KEY_Event = KEY_Event_SingleClick;        //�¼�->����Ӧ����
    }
    else if ((KeyCfg->KEY_Action == KEY_Action_Press) && (KeyCfg->KEY_Count < KEY_LONG_PRESS_TIME)) //���������ְ��£�����ʱ��δ��
    {
      KeyCfg->KEY_Count++;                         //����
      KeyCfg->KEY_Status = KEY_Status_SecondPress; //״̬->ά��
      KeyCfg->KEY_Event = KEY_Event_Null;          //�¼�->��
    }
    else //�������ͷţ�����ʱ��δ��
    {
      KeyCfg->KEY_Count = 0;                     //��������
      KeyCfg->KEY_Status = KEY_Status_Idle;      //״̬->����
      KeyCfg->KEY_Event = KEY_Event_DoubleClick; //�¼�->˫��
    }
    break;
  }

  if (KeyCfg->KEY_Event != KEY_Event_Null) //�¼���¼
    key_event[KeyCfg->KEY_Label] = KeyCfg->KEY_Event;
}

