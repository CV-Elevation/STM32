/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "comp.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint16_t GATE_OPEN,GATE_READY2CLOSE;
uint32_t Nx,Nb;
uint8_t Fre_State=LOW_FREQUENCY;	//默认低频
double fre;
uint8_t KEY_State=1;//1测频2测时
uint8_t Flag=0;
uint32_t	CH1=0;
uint32_t	CH2=0;
uint32_t Delta=0;

double DeltaT=0.0;
double DeltaT_temp=0.0;
double DeltaT_tran=0.0;
double WC_Time=0.0;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Initialize(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */
void Parameter_Init(void);
void Turn2HIGH_FREQUENCY_MODE(void);
void Turn2LOW_FREQUENCY_MODE(void);
void MX_TIM3_Init_ETR(void);
void MX_COMP1_Init_HighFre(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM7_Init();
  MX_USART1_UART_Init();
  MX_COMP1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM6_Init();
  MX_COMP2_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */
	Parameter_Init();
	printf("\nstart\n");

	HAL_COMP_Start(&hcomp1);
	HAL_COMP_Start(&hcomp2);
	
	HAL_TIM_IC_Start_IT(&htim5,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim5,TIM_CHANNEL_2);

	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);    //启动T3输入捕获

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */




//初始化
void Parameter_Init(void)
{

	GATE_OPEN=0;
	GATE_READY2CLOSE=0;
	Nx=0;
	Nb=0;
	fre=0.0;
}

//输入捕获的中断回调函数
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(TIM3 == htim->Instance)
	{
		
		if(!GATE_OPEN)
		{
			//启动闸门 TIM6 开始计数标准信号
			__HAL_TIM_CLEAR_IT(&htim6, TIM_IT_UPDATE);
			HAL_TIM_Base_Start_IT(&htim6);
			HAL_TIM_Base_Start(&htim2);
			GATE_OPEN=1;
		}
		else
		{
			//测频中，捕捉到一次信号 输入计数值加1
			Nx++;
			
			//避免低频模式，输入频率过高，程序卡死
			if(Nx>20000)
			{
				HAL_TIM_Base_Stop(&htim2);
				HAL_TIM_IC_Stop_IT(&htim3, TIM_CHANNEL_1);
				HAL_TIM_Base_Stop_IT(&htim6);
				Turn2HIGH_FREQUENCY_MODE();
			}
		}
		
		if(GATE_OPEN&&GATE_READY2CLOSE)
		{
			//关闭闸门 停止捕捉 
			HAL_TIM_Base_Stop(&htim2);
			HAL_TIM_IC_Stop_IT(&htim3, TIM_CHANNEL_1);
			HAL_TIM_Base_Stop_IT(&htim6);
			//锁存Nx Nb数据 计算频率值
			Nb=__HAL_TIM_GetCounter(&htim2); 
			fre=((double)Nx)/((double)Nb)*240000000.0;
			//判断
			if(fre<10000.0)
			{
	//		  	// LCD显示
	//				LCD_Display();
//					printf("freL = %f\n",fre);
				
	//				HAL_Delay(500);
				
					//准备下一次测量
					Parameter_Init();
					__HAL_TIM_SetCounter(&htim2,0);  //注意！！！！！！！计数值不会因为定时器暂停而清零
					HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);    //启动输入捕获
			}
			else
			{
				Turn2HIGH_FREQUENCY_MODE();
			}
		}
	}

	if(htim->Instance==TIM5 )//TIM_5_用于计算时间间隔
		{
			if( htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1 )
			{
				if(Flag==0)
				{
					Flag=1;
					CH1=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
				}
			 }  
			
			if( htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2 )
			{
				if(Flag==1)
				{
					Flag=0;
					CH2=HAL_TIM_ReadCapturedValue(&htim5,TIM_CHANNEL_2);
					HAL_TIM_IC_Stop_IT(&htim5,TIM_CHANNEL_1);
					HAL_TIM_IC_Stop_IT(&htim5,TIM_CHANNEL_2);
					
					DeltaT_temp=DeltaT;
					Delta=CH2-CH1;
					DeltaT=Delta/240.0;
					WC_Time=(DeltaT-DeltaT_temp)/DeltaT;
					if((WC_Time<0.001)&&(WC_Time>-0.001))
					{
//						printf("WC_Time = %f	\n",WC_Time);
//						printf("DeltaT_temp = %f us\n",DeltaT_temp);
//						printf("DeltaT = %f us\n",DeltaT+0.006);
						DeltaT_tran=DeltaT;
					}
//					else
//					{
//					printf("wait\n");
//					}
					__HAL_TIM_SET_COUNTER(&htim5,0);
					HAL_TIM_IC_Start_IT(&htim5,TIM_CHANNEL_1);
					HAL_TIM_IC_Start_IT(&htim5,TIM_CHANNEL_2);
				}
			}
		}	
}   

//定时器触发的中断回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//闸门溢出 规定测量时间到
		if(TIM6 == htim->Instance&&Fre_State==LOW_FREQUENCY)
    {
			GATE_READY2CLOSE=1;
    }
		else if(TIM6 == htim->Instance&&Fre_State==HIGH_FREQUENCY)
    {
			HAL_TIM_Base_Stop_IT(&htim6);
			HAL_TIM_Base_Stop_IT(&htim3);
			HAL_TIM_Base_Stop(&htim2);
			Nx+=TIM3->CNT;
			Nb=TIM2->CNT;
	  	fre=((double)Nx)/((double)Nb)*240000000.0;
			if(Nx>=10000.0)
			{
				//显示
//			LCD_Display();
			
//			printf("freH = %f\n",fre);
//			HAL_Delay(500);
				
	    Parameter_Init();
	  	__HAL_TIM_SetCounter(&htim2,0);  //注意！！！！！！！计数值不会因为定时器暂停而清零
			__HAL_TIM_SetCounter(&htim3,0);  //注意！！！！！！！计数值不会因为定时器暂停而清零

			
			HAL_TIM_Base_Start_IT(&htim6);
			HAL_TIM_Base_Start_IT(&htim3);
			HAL_TIM_Base_Start(&htim2);
			}
			else
			{
				Turn2LOW_FREQUENCY_MODE();
			}
    }
		
		//溢出
		else if(TIM3 == htim->Instance&&Fre_State==HIGH_FREQUENCY)
    {
			Nx+=0xFFFF;
    }

}

//测高频
void Turn2HIGH_FREQUENCY_MODE(void)
{
		  Fre_State=HIGH_FREQUENCY;
			MX_TIM3_Init_ETR();
			MX_COMP1_Init_HighFre();
			Parameter_Init();
	  	__HAL_TIM_SetCounter(&htim2,0);  //注意！！！！！！！计数值不会因为定时器暂停而清零
			__HAL_TIM_SetCounter(&htim3,0);  //注意！！！！！！！计数值不会因为定时器暂停而清零

			HAL_TIM_Base_Start_IT(&htim6);
			HAL_TIM_Base_Start_IT(&htim3);
			HAL_TIM_Base_Start(&htim2);
}

//测低频
void Turn2LOW_FREQUENCY_MODE(void)
{
		Fre_State=LOW_FREQUENCY;
		MX_TIM3_Init();
		MX_COMP1_Init();
		Parameter_Init();
	  __HAL_TIM_SetCounter(&htim2,0);  //注意！！！！！！！计数值不会因为定时器暂停而清零
		HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);    //启动输入捕获
}

//按键中断
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == KEY1_Pin)
	{
		if(KEY_State==1)				//开3关5
		{
//			HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);    //启动T3输入捕获
//			HAL_TIM_IC_Stop_IT(&htim5,TIM_CHANNEL_1);
//			HAL_TIM_IC_Stop_IT(&htim5,TIM_CHANNEL_2);
//			HAL_TIM_Base_Stop_IT(&htim5);

			KEY_State=2;
//			printf("\nFre\n");
			int a0,a1;
			int b0,b1;
			double period;
			period=1.0/fre;
			//频率单位	
			if(fre<=1000)
			{	
				printf("t4.txt=\"Hz\"\xFF\xFF\xFF");
			}
			else if(fre<=1000000&&fre>1000)
			{
				printf("t4.txt=\"kHz\"\xFF\xFF\xFF");
				fre=fre/1000;
			}
			else if(fre>1000000)
			{
				printf("t4.txt=\"MHz\"\xFF\xFF\xFF");
				fre=fre/1000000;
			}
			//周期单位
			if(period>=1)
			{	
				printf("t5.txt=\"s\"\xFF\xFF\xFF");
			}
			else if(period<1&&period>=0.001)
			{
				printf("t5.txt=\"ms\"\xFF\xFF\xFF");
				period=period*1000.0;
			}
			else if(period<0.001&&period>=0.000001)
			{
				printf("t5.txt=\"us\"\xFF\xFF\xFF");
				period=period*1000000.0;
			}
			a0=fre;
			a1=(fre-a0)*1000000;
			b0=period;
			b1=(period-b0)*1000000;
			printf("n0.val=%d\xFF\xFF\xFF",a0);
			printf("n1.val=%d\xFF\xFF\xFF",a1);
			printf("n2.val=%d\xFF\xFF\xFF",b0);
			printf("n5.val=%d\xFF\xFF\xFF",b1);
		}
		else if(KEY_State==2)		//开5关3
		{
//			HAL_TIM_IC_Start_IT(&htim5,TIM_CHANNEL_1);
//			HAL_TIM_IC_Start_IT(&htim5,TIM_CHANNEL_2);
//			HAL_TIM_IC_Stop_IT(&htim3, TIM_CHANNEL_1);    //关闭T3输入捕获
//			HAL_TIM_Base_Stop_IT(&htim6);
//			HAL_TIM_Base_Stop_IT(&htim3);
//			HAL_TIM_Base_Stop(&htim2);
//			MX_COMP1_Init();
//			MX_COMP2_Init();
			KEY_State=1;
//			printf("\nDeltaT\n");
			int c0,c1;
			if(DeltaT_tran>=1)
			{	
				printf("t6.txt=\"s\"\xFF\xFF\xFF");
			}
			else if(DeltaT_tran<1&&DeltaT_tran>=0.001)
			{
				printf("t6.txt=\"ms\"\xFF\xFF\xFF");
				DeltaT_tran=DeltaT_tran*1000.0;
			}
			else if(DeltaT_tran<0.001&&DeltaT_tran>=0.000001)
			{
				printf("t6.txt=\"us\"\xFF\xFF\xFF");
				DeltaT_tran=DeltaT_tran*1000000.0;
			}
			c0=DeltaT_tran;
			c1=(DeltaT_tran-c0)*1000000;
			printf("n6.val=%d\xFF\xFF\xFF",c0);
			printf("n7.val=%d\xFF\xFF\xFF",c1);

		}
	}
}

/* USER CODE END 4 */

/* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x24000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress = 0x30000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_128KB;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER2;
  MPU_InitStruct.BaseAddress = 0x30020000;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER3;
  MPU_InitStruct.BaseAddress = 0x30040000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_32KB;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER4;
  MPU_InitStruct.BaseAddress = 0x38000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_64KB;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
