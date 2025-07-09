#include "stm32h7xx_hal.h"
#include "tim.h"
#include "Fre_measure.h"
extern unsigned int Forward_Posedge_Flag,Negedge_Flag;


extern double fre;
extern uint16_t GATE_OPEN,GATE_READY2CLOSE;
extern uint32_t Nx,Nb;
extern uint8_t Fre_State;
void Parameter_Init(void)
{

	GATE_OPEN=0;
	GATE_READY2CLOSE=0;
	Nx=0;
	Nb=0;
	fre=0.0;
}

void Turn2HIGH_FREQUENCY_MODE(void)
{
		  Fre_State=HIGH_FREQUENCY;
			MX_TIM3_Init_ETR();
			
			Parameter_Init();
	  	__HAL_TIM_SetCounter(&htim2,0);  //注意！！！！！！！计数值不会因为定时器暂停而清零
			__HAL_TIM_SetCounter(&htim3,0);  //注意！！！！！！！计数值不会因为定时器暂停而清零

			HAL_TIM_Base_Start_IT(&htim5);
			HAL_TIM_Base_Start_IT(&htim3);
			HAL_TIM_Base_Start(&htim2);
}


void Turn2LOW_FREQUENCY_MODE(void)
{
		Fre_State=LOW_FREQUENCY;
		Parameter_Init();
	  __HAL_TIM_SetCounter(&htim2,0);  //注意！！！！！！！计数值不会因为定时器暂停而清零
		MX_TIM3_Init();
		HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);    //启动输入捕获
}

