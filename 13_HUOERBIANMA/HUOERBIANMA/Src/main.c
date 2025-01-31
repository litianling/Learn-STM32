/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

unsigned char Step_table[]={0x08,0x04,0x02,0x01};
int speed=4;
int mode=0;
int number=0;
int number1=0;
unsigned char tstr[5];

void delay(void);
void lcdwritecmd(unsigned char cmd);
void lcdwritedata(unsigned char dat);
void lcdinit(void);
void lcdsetcursor(unsigned char x, unsigned char y);
void lcdshowstr(unsigned char x, unsigned char y, unsigned char *str);
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
ITStatus EXTI_GetITStatus(uint32_t EXTI_Line)
{
    ITStatus bitstatus = RESET; //chushi wei zhuangtai 0
    uint32_t enablestatus = 0;  //chushi shineng zhuangtai 0
    /* Check the parameters */
    assert_param(IS_GET_EXTI_LINE(EXTI_Line));
    enablestatus =  EXTI->IMR1 & EXTI_Line;  
    if (((EXTI->FPR1 & EXTI_Line) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET)) 
		      bitstatus = SET;
    else
        bitstatus = RESET;
    return bitstatus;
}

void EXTI_ClearITPendingBit(uint32_t EXTI_Line)
{
  /* Check the parameters */
  assert_param(IS_EXTI_LINE(EXTI_Line));
  EXTI->FPR1 = EXTI_Line;
}
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

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
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

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
  /* USER CODE BEGIN 2 */
	lcdinit();
  /* USER CODE END 2 */
	lcdshowstr(0,0,"Qian");
	lcdshowstr(5,0,"Hou");	
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		if(mode==0)
			for(int i=0;i<4;i++)
			{
				GPIOD->ODR=Step_table[i];
				HAL_Delay(speed);
			}
		else
			for(int i=0;i<4;i++)
			{
				GPIOD->ODR=Step_table[3-i];
				HAL_Delay(speed);
			}
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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PC12 PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB3
                           PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PD0 PD1 PD2 PD3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

}

/* USER CODE BEGIN 4 */
void delay ()
{
	for(int i=0;i<99;i++)
	  for(int j=0;j<99;j++)
	 {}
}
	  
void lcdwritecmd(unsigned char cmd)
{
	delay();          //           使能 写  指令
	GPIOB->ODR=0x18;  //RST=1\PSB=1\E=0\RW=0\RS=0
	GPIOA->ODR=cmd;
	GPIOB->ODR=0x1C;	//RST=1\PSB=1\E=1\RW=0\RS=0
	delay();
	GPIOB->ODR=0x18;	//RST=1\PSB=1\E=0\RW=0\RS=0
}

void lcdwritedata(unsigned char dat)
{
  delay();          //           使能  写  数据
	GPIOB->ODR=0x19;	//RST=1\PSB=1\E=0\RW=0\RS=1
  GPIOA->ODR=dat;
	GPIOB->ODR=0x1D;	//RST=1\PSB=1\E=1\RW=0\RS=1
	delay();
	GPIOB->ODR=0x19;	//RST=1\PSB=1\E=0\RW=0\RS=1
}

void lcdinit()
{
	lcdwritecmd(0x34); //E 使能   RS1数据0指令  RW1读0写
	lcdwritecmd(0x30); 
	lcdwritecmd(0x0C); 
	lcdwritecmd(0x01);
}

void lcdsetcursor(unsigned char x, unsigned char y) //字符定位
{
	unsigned char address;
	if(y==0)
		address=0x80+x;
	else if(y==1)
		address=0x90+x;
	else if(y==2)
		address=0x88+x;
	else
		address=0x98+x;
	lcdwritecmd(address|0x80);
}

void lcdshowstr(unsigned char x, unsigned char y, unsigned char *str) //字符输出
{                       
	lcdsetcursor(x,y);
	while((*str)!='\0')  	 
	{
		 lcdwritedata(*str);
  		str++;            
	}
}

void EXTI4_15_IRQHandler(void)
{
	if (EXTI_GetITStatus(0x2000) != 0x00)  // 0x2000==EXTI_PIN_13
  {
		EXTI_ClearITPendingBit(0x2000);  //Clear interrupt flag bit
		if(mode==0)
			mode++;
		else
			mode=0;
  } 
		
	if (EXTI_GetITStatus(0x1000) != 0x00)  // 0x1000==EXTI_PIN_12
  {
		EXTI_ClearITPendingBit(0x1000);
		if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_11)==1)
		{   number++;
				sprintf(tstr,"%d",number);
				lcdshowstr(0,1,tstr); }
		else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_11)==0)
		{		number1++;
				sprintf(tstr,"%d",number1);
				lcdshowstr(5,1,tstr);		}
		else ;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
