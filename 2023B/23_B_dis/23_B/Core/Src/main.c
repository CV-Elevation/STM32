/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t RX_dat;
uint8_t flag=0;
uint8_t num=0;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
int getfrequence(void);
uint32_t getvalue(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int count,time,f;
uint32_t value;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim1);
	HAL_UART_Receive_IT(&huart1,&RX_dat,1);
//	USART1PutString("start");
	printf("\xff\xff\xfft4.txt=\"正在检测\"\xff\xff\xff");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		if(flag==1&&num==0)
		{
			printf("length mode\r\n");
			printf("\xff\xff\xfft4.txt=\"正在检测\"\xff\xff\xff");
			HAL_Delay(1000);
			float length=100.78;//长度值
			printf("\xff\xff\xffn0.val=%d\xff\xff\xff",(int)length);
		  printf("\xff\xff\xffn1.val=%d\xff\xff\xff",(int)((length-(int)length)*10));
			printf("\xff\xff\xfft4.txt=\"结果保持\"\xff\xff\xff");
			num=1;
		}
		else if(flag==2&&num==0)
		{
			printf("load mode\r\n");
			printf("\xff\xff\xfft4.txt=\"正在检测\"\xff\xff\xff");
			HAL_Delay(1000);			
			float Rvalue=22.3;//电阻
			printf("\xff\xff\xfft6.txt=\"电阻\"\xff\xff\xff");
			printf("\xff\xff\xffn2.val=%d\xff\xff\xff",(int)Rvalue);
		  printf("\xff\xff\xffn3.val=%d\xff\xff\xff",(int)((Rvalue-(int)Rvalue)*10));
			printf("\xff\xff\xfft9.txt=\"欧\"\xff\xff\xff");
			printf("\xff\xff\xfft4.txt=\"结果保持\"\xff\xff\xff");
			HAL_Delay(1000);
			float Cvalue=167;//电容
			printf("\xff\xff\xfft6.txt=\"电容\"\xff\xff\xff");
			printf("\xff\xff\xffn2.val=%d\xff\xff\xff",(int)Cvalue);
			printf("\xff\xff\xffn3.val=%d\xff\xff\xff",(int)((Cvalue-(int)Cvalue)*10));		
			printf("\xff\xff\xfft9.txt=\"pF\"\xff\xff\xff");
			printf("\xff\xff\xfft4.txt=\"结果保持\"\xff\xff\xff");
			num=1;
		}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	//f=getfrequence();
	//value=getvalue();
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 10;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 3;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/* USER CODE BEGIN 4 */
int getfrequence(void)
{
	time=0;
	HAL_TIM_Base_Start(&htim1);
	HAL_TIM_Base_Start(&htim2);
	while(1)
	{
	 if(time==2)break;
	}
	int fre=count*2;
	return fre;

}
uint32_t getvalue(void)
{
	uint32_t a;
	for(int i=0;i<10;i++)
	{
		HAL_ADC_Start(&hadc1);
		value=HAL_ADC_GetValue(&hadc1);
		a=a+value;
	}
	a=a/10;
	return a;
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	time++;
	if(time==1)TIM2->CNT=0;
	if(time==2)
	{
		count=TIM2->CNT;
		HAL_TIM_Base_Stop(&htim1);
		HAL_TIM_Base_Stop(&htim2);
	}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	 if(huart->Instance == USART1)
	 {
			if(RX_dat == '1')
			{
				flag = 1;
				num=0;
				HAL_UART_Receive_IT(&huart1,&RX_dat,1);
				
			}
			else if(RX_dat == '2')
			{
				flag = 2;
				num =0;
				HAL_UART_Receive_IT(&huart1,&RX_dat,1);
				
			}
			else HAL_UART_Receive_IT(&huart1,&RX_dat,1);
	}
}
/* USER CODE END 4 */

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
