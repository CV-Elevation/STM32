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
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
ALIGN_32BYTES (uint16_t adc1_data[FFT_LENGTH]) 	__attribute__((section(".ARM.__at_0x30000000")));
__IO uint8_t Adc1_ConvEnd = 0;//表示adc1采集完成
__IO uint8_t Adc2_ConvEnd = 0;//表示adc2采集完成
__IO uint8_t Adc3_ConvEnd = 0;//表示adc3采集完成

unsigned char buffer[50];
u8 RX_dat;
u8 flag=0;
uint8_t KEY_state=0;


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
void PeriphCommonClock_Config(void);
static void MPU_Initialize(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

void adc1_init(void);
void adc2_init(void);
void adc3_init(void);
void FFT_and_DIS(void);

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

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM7_Init();
  MX_TIM15_Init();
  MX_USART1_UART_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	printf("start\n");
	HAL_UART_Receive_IT(&huart1,&RX_dat,1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if(KEY_state==1)
		{
			KEY_state=0;
			adc1_init();
			
			float vi_max,vi_min = 1.65;
			float voltage = 0;
			//求峰峰值
			for (u16 temp = 10 ;temp < FFT_LENGTH-10 ;temp++)
			{
				voltage = adc1_data[temp]*3.3/65535;
				if(voltage>vi_max)
				{
					vi_max = voltage;
				}
				if(voltage<vi_min)
				{
					vi_min = voltage;
				}
			}
				float	vi_pp = vi_max - vi_min;
//				printf("vi_max = %f\n",vi_max);
//				printf("vi_min = %f\n",vi_min);
//				printf("vi_pp = %f\n",vi_pp);
			
			//分档控制	
//			if(vi_pp > 1.6)
//			{
//				printf("adc2\n");
//				adc2_init();
//				flag=1;
//			}
//			else if(vi_pp < 0.3)
//			{
//				printf("adc3\n");
//				adc3_init();
//				flag=2;
//			}
//			else
//			{
//				printf("adc1\n");
//				flag=0;
//			}
			//FFT及显示
			//			float voltage = 0;
//			for (u16 temp = 0 ;temp < FFT_LENGTH ;temp++)
//			{
//				voltage = adc1_data[temp]*3.3/65535;
//				printf("%f\r\n",voltage);
//		//		printf("%d\r\n",adc1_data[temp]);
//			}
//		FFT_Init();
//		FFT_DIS();
			FFT_and_DIS();
			switch(flag){
		case 0:printf("adc1\n");break;
		case 1:printf("adc2\n");break;
		case 2:printf("adc3\n");break;	
	}
		}
		
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

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInitStruct.PLL2.PLL2M = 25;
  PeriphClkInitStruct.PLL2.PLL2N = 360;
  PeriphClkInitStruct.PLL2.PLL2P = 5;
  PeriphClkInitStruct.PLL2.PLL2Q = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_0;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
	//ADC
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
   if(hadc->Instance == ADC1) 
	{
      //SCB_InvalidateDCache_by_Addr((uint32_t *) &adc1_data[0], ADC1_BUFFER_SIZE);
		Adc1_ConvEnd = 1;
  }
	
   if(hadc->Instance == ADC2) 
	{
      //SCB_InvalidateDCache_by_Addr((uint32_t *) &adc1_data[0], ADC1_BUFFER_SIZE);
		Adc2_ConvEnd = 1;
  }
	
   if(hadc->Instance == ADC3) 
	{
      //SCB_InvalidateDCache_by_Addr((uint32_t *) &adc1_data[0], ADC1_BUFFER_SIZE);
		Adc3_ConvEnd = 1;
  }
	
}

	//IO
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == KEY1_Pin)
	{
		KEY_state=1;
	}
}

	//adc校准及启动采样
void adc1_init(void)
{
	printf("start_adc1\n");
	MX_ADC1_Init();	//初始化调用放这里, 确保在MX_DMA_Init()初始化后靿  	
//	TIM15->PSC =5-1;
	HAL_Delay(100);	//有地方说这里可以等等电压稳定后再校准
	if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
	{
			printf("hadc1 error with HAL_ADCEx_Calibration_Start\r\n");
			Error_Handler();
	}

	if (HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc1_data, FFT_LENGTH) != HAL_OK)
	{
			printf("hadc1 error with HAL_ADC_Start_DMA\r\n");
			Error_Handler();
	}

	HAL_TIM_Base_Start(&htim15);
	while (!Adc1_ConvEnd);
	Adc1_ConvEnd = 0;
	HAL_ADC_DeInit(&hadc1);			//逆初始化
	HAL_TIM_Base_Stop(&htim15);
}

void adc2_init(void)
{
	printf("start_adc2\n");
	MX_ADC2_Init();	//初始化调用放这里, 确保在MX_DMA_Init()初始化后靿  	
//	TIM4->PSC =5-1;
	HAL_Delay(100);	//有地方说这里可以等等电压稳定后再校准
	if (HAL_ADCEx_Calibration_Start(&hadc2, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
	{
			printf("hadc2 error with HAL_ADCEx_Calibration_Start\r\n");
			Error_Handler();
	}

	if (HAL_ADC_Start_DMA(&hadc2, (uint32_t *)adc1_data, FFT_LENGTH) != HAL_OK)
	{
			printf("hadc2 error with HAL_ADC_Start_DMA\r\n");
			Error_Handler();
	}

	HAL_TIM_Base_Start(&htim4);
	while (!Adc2_ConvEnd);
	Adc2_ConvEnd = 0;
	HAL_ADC_DeInit(&hadc2);			//逆初始化
	HAL_TIM_Base_Stop(&htim4);
}

void adc3_init(void)
{
	printf("start_adc3\n");
	MX_ADC3_Init();	//初始化调用放这里, 确保在MX_DMA_Init()初始化后靿  	
//	TIM6->PSC =5-1;
	HAL_Delay(100);	//有地方说这里可以等等电压稳定后再校准
	if (HAL_ADCEx_Calibration_Start(&hadc3, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
	{
			printf("hadc3 error with HAL_ADCEx_Calibration_Start\r\n");
			Error_Handler();
	}

	if (HAL_ADC_Start_DMA(&hadc3, (uint32_t *)adc1_data, FFT_LENGTH) != HAL_OK)
	{
			printf("hadc3 error with HAL_ADC_Start_DMA\r\n");
			Error_Handler();
	}

	HAL_TIM_Base_Start(&htim1);
	while (!Adc3_ConvEnd);
	Adc3_ConvEnd = 0;
	HAL_ADC_DeInit(&hadc3);			//逆初始化
	HAL_TIM_Base_Stop(&htim1);
}

	//串口中断接收
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	 if(huart->Instance == USART1)
	 {
			if(RX_dat == '1')
			{
				KEY_state = 1;
				HAL_UART_Receive_IT(&huart1,&RX_dat,1);
			}
			else HAL_UART_Receive_IT(&huart1,&RX_dat,1);
	}
}

	//adc按档完成采样，对最终adc1_data进行FFT
	//显示结果
void	FFT_and_DIS(void)
{
		FFT_Init();
		FFT_DIS();
		uint16_t	Point_period  =(uint16_t)(Fs/f[0]);
		u32 voltage_ = 0;
		u32 voltage_1 = 0;
		if(Point_period>200)
  	{
			TIM15->PSC =(int)(15000/f[0])-1;
			adc1_init();
			printf("cle s0.id,0\xff\xff\xff");
		for (uint16_t i = 0 ;i <= 200 ;i++)
		{
			voltage_ = adc1_data[i]*200/65536;
			printf("add s0.id,0,%d\xff\xff\xff",voltage_);
			startValuePack(buffer);
			putFloat(temp);//THD
			putFloat(adc1_data[i]);//wave
			putFloat(U[0]);//V1
			putFloat(U[1]);
			putFloat(U[2]);
			putFloat(U[3]);
			putFloat(U[4]);//V5
			sendBuffer(buffer,endValuePack());
		}
		TIM15->PSC=0;
		}
		else if(Point_period>=100&&Point_period<=200){
		printf("cle s0.id,0\xff\xff\xff");
		for (uint16_t i = 0 ;i <= Point_period ;i++)
		{
			voltage_ = adc1_data[i]*200/65536;
			printf("add s0.id,0,%d\xff\xff\xff",voltage_);
			startValuePack(buffer);
			putFloat(temp);//THD
			putFloat(adc1_data[i]);//wave
			putFloat(U[0]);//V1
			putFloat(U[1]);
			putFloat(U[2]);
			putFloat(U[3]);
			putFloat(U[4]);//V5
			sendBuffer(buffer,endValuePack());
		}
	}
		else if(Point_period>=50&&Point_period<=100)
		{
//			u16 point=200/Point_period;;
			printf("cle s0.id,0\xff\xff\xff");
		for (uint16_t i = 0 ;i <= Point_period ;i++)
		{
			voltage_ = adc1_data[i]*200/65536;
			voltage_1 = adc1_data[i+1]*200/65536;
			printf("add s0.id,0,%d\xff\xff\xff",voltage_);
			printf("add s0.id,0,%d\xff\xff\xff",(voltage_+voltage_1)/2);			
			startValuePack(buffer);
			putFloat(temp);//THD
			putFloat(adc1_data[i]);//wave
			putFloat(U[0]);//V1
			putFloat(U[1]);
			putFloat(U[2]);
			putFloat(U[3]);
			putFloat(U[4]);//V5
			sendBuffer(buffer,endValuePack());
		}
		}
		else
		{
			printf("cle s0.id,0\xff\xff\xff");
		for (uint16_t i = 1 ;i <= Point_period ;i++)
		{
			voltage_ = adc1_data[i]*200/65536;
			voltage_1 = adc1_data[i+1]*200/65536;
			printf("add s0.id,0,%d\xff\xff\xff",voltage_);
			printf("add s0.id,0,%d\xff\xff\xff",(voltage_+voltage_1)/2);
			printf("add s0.id,0,%d\xff\xff\xff",(voltage_+3*voltage_1)/4);
			startValuePack(buffer);
			putFloat(temp);//THD
			putFloat(adc1_data[i]);//wave
			putFloat(U[0]);//V1
			putFloat(U[1]);
			putFloat(U[2]);
			putFloat(U[3]);
			putFloat(U[4]);//V5
			sendBuffer(buffer,endValuePack());
		}
	}
//			u16 delti;
//			u16 point;
//			if(Point_period>200)
//			{
//				delti=Point_period/200;
//				point=0;
//			}
//			else {
//				point=200/Point_period;
//				delti=1;
//			}
//			for (uint16_t i = 0 ;i <= Point_period ;i=i+delti)
//			{
//				voltage_ = adc1_data[i]*200/65536;
//				for(u16 j=0;j<=point;j++)
//				{
//					printf("add s0.id,0,%d\xff\xff\xff",(voltage_+adc1_data[i-1]*200/65536)/2);
//				}
//				printf("add s0.id,0,%d\xff\xff\xff",voltage_);
	//			printf("%d\r\n",adc1_data[temp]);
//			}
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
