#ifndef ___MY_KEY_H__
#define ___MY_KEY_H__
#include "main.h"
#define ARR_LEN(arr) ((sizeof(arr)) / (sizeof(arr[0]))) //�����С�꺯��

#define KEY_DEBOUNCE_TIME 10      //����ʱ��
#define KEY_LONG_PRESS_TIME 2000   //�����ж�ʱ��
#define KEY_QUICK_CLICK_TIME 10  //����ʱ����
#define KEY_DOUBLE_CLICK_TIME 500 //˫���ж�ʱ��
#define KEY_PRESSED_LEVEL 0 //����������ʱ�ĵ�ƽ

//��������
typedef enum
{
  KEY_Action_Press,   //��ס
  KEY_Action_Release, //�ɿ�
} KEY_Action_TypeDef;

//����״̬
typedef enum
{
  KEY_Status_Idle,             //����
  KEY_Status_Debounce,         //����
  KEY_Status_ConfirmPress,     //ȷ�ϰ���
  KEY_Status_ConfirmPressLong, //ȷ�ϳ���
  KEY_Status_WaitSecondPress,  //�ȴ��ٴΰ���
  KEY_Status_SecondDebounce,   //�ٴ�����
  KEY_Status_SecondPress,      //�ٴΰ���
} KEY_Status_TypeDef;

//�����¼�
typedef enum
{
  KEY_Event_Null,        //���¼�
  KEY_Event_SingleClick, //����
  KEY_Event_LongPress,   //����
  KEY_Event_QuickClick,  //����
  KEY_Event_DoubleClick, //˫��
} KEY_Event_TypeDef;

//����ģʽʹ��ѡ��
typedef enum
{
  KEY_Mode_OnlySinge = 0x00,         //ֻ�е���
  KEY_Mode_Long = 0x01,              //��������
  KEY_Mode_Quick = 0x02,             //��������
  KEY_Mode_Long_Quick = 0x03,        //������������
  KEY_Mode_Double = 0x04,            //����˫��
  KEY_Mode_Long_Double = 0x05,       //��������˫��
  KEY_Mode_Quick_Double = 0x06,      //��������˫��
  KEY_Mode_Long_Quick_Double = 0x07, //������������˫��
} KEY_Mode_TypeDef;

//��������
typedef struct
{
  uint8_t KEY_Label;             //�������
  KEY_Mode_TypeDef KEY_Mode;              //����ģʽ
  uint16_t KEY_Count;            //�������¼�ʱ
  KEY_Action_TypeDef KEY_Action; //�������������»��ͷ�
  KEY_Status_TypeDef KEY_Status; //����״̬
  KEY_Event_TypeDef KEY_Event;   //�����¼�
} KEY_Configure_TypeDef;

extern KEY_Configure_TypeDef KeyConfig[];
extern KEY_Event_TypeDef key_event[];

void KEY_ReadStateMachine(KEY_Configure_TypeDef *KeyCfg);

#endif

