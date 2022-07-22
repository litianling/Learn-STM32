
#include "main.h"
#include "stdio.h"
#include "string.h"

#define unit8_t unsigned char
#define PUTCHAR_PROTOTYPE int fputc(int ch,FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#define BACKSPACE_PROTOTYPE int _backspace(FILE *f)

//����100�ַ����ȵĽ������,��0λ���ţ���һλ�ո�
/*
unsigned char Char_Out[100]=
{
	' ', ' ', '!', '"', '#', '$', '%', '&', ' ', '(',
	')', '*', '+', ',', '-', '.', '/', '0', '1', '2',
	'3', '4', '5', '6', '7', '8', '9', ':', ';', '<',
	'=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F',
	'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',

	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
  '[', '\ ', ']', '^', ' ', '��', 'a', 'b', 'c', 'd',	//92��б����϶     96�ٺ�   
 	'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
	'y', 'z', '{', '|', '}', '~', ' ', ' ', ' ', ' ' 	
};
*/

unsigned char Char_Out[10][10]=
{
	' ', ' ', '!', '"', '#', '$', '%', '&', ' ', '(',
	')', '*', '+', ',', '-', '.', '/', '0', '1', '2',
	'3', '4', '5', '6', '7', '8', '9', ':', ';', '<',
	'=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F',
	'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',

	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
  '[', '\ ', ']', '^', ' ', '��', 'a', 'b', 'c', 'd',	//92��б����϶     96�ٺ�   
 	'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
	'y', 'z', '{', '|', '}', '~', ' ', ' ', ' ', ' ' 	
};
volatile int  High_number=0;		//��������
volatile int Transfer_Data=0;		//��������
volatile int Output_Y=0;				//�����������
volatile int Output_X=0;				//
//volatile int ASC_Output=0;			//�任���ASC����Ͻ���������
int  Flag_TIM2=0; 							//��ʱ��2ͨ��1��־λ  1��0�ر�  ���Ӽ�����׼ȷ��
int	 Flag_Transfer=1;						//�����־���жϴ���ʮλ��1�������λ��0��

UART_HandleTypeDef hlpuart1;
TIM_HandleTypeDef htim2;


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_TIM2_Init(void);


int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_LPUART1_UART_Init();
  MX_TIM2_Init();
	printf("��ʼ���� \r\n");
  while (1)
  {
		HAL_Delay(10);
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPUART1;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 112500;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  hlpuart1.FifoMode = UART_FIFOMODE_DISABLE;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&hlpuart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&hlpuart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

}

static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 1023;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 3750;//62500;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 1875;//31250;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pins : PC4 PC5 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

}



PUTCHAR_PROTOTYPE	//�ض��� fputc()����
{
	HAL_UART_Transmit(&hlpuart1,(unit8_t*) &ch,1,0xFFFF);  //���ô��ڷ��ͺ���
	return ch;		//���ط��͵��ַ�
}


GETCHAR_PROTOTYPE //�ض��� fgetc()���� 
{ 
	uint8_t value; //�����޷����ַ��ͱ��� value 
	while((LPUART1->ISR & 0x00000020)==0){} //�жϴ����Ƿ���յ��ַ� 
	value=(uint8_t)LPUART1->RDR; //��ȡ���ڽ��յ����ַ� 
	HAL_UART_Transmit(&hlpuart1,(uint8_t *)&value,1,0x1000); //���Խ��յ����ַ� 
	return value; //���ؽ��յ���ֵ value 
}

BACKSPACE_PROTOTYPE //�ض���__backspace ���� 
{ 
	return 0; 
}



//printf(" �յ����� %d \r\n",Transfer_Data);
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
/*
	if(GPIO_Pin==0x0010) //PC4 ��ʱ��-----���+����
	{
		//��ʱ�����رն�ʱ��2ͨ��1
		HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_1);
		Flag_TIM2=0;
		Transfer_Data=(High_number+4)/8;
		if(Transfer_Data!=0)	//���������ǲ����ܵ���0�ģ���ֹ�󴥷���
		{
			if(Flag_Transfer==1)	//�������ʮλ
			{
				ASC_Output=(Transfer_Data-1)*10;
				Flag_Transfer=0;		//ת�䴫���־λ׼�����ո�λ
			}
			else									//������Ǹ�λ
			{
				ASC_Output=ASC_Output+(Transfer_Data-1);	//��λҲ�������
				printf("%c",Char_Out[ASC_Output]);				//�����Ӧ�ַ�
				ASC_Output=0;															//���ASC���Ѱַ
				Flag_Transfer=1;		//ת�䴫���־λ׼��������һ��ʮλ
			}
			Transfer_Data=0;			//���������˼�ʱ���㡪����ֹ�󴥷�
		}
		High_number=0;					//����������
	}
*/

	if(GPIO_Pin==0x0010) //PC4 ��ʱ��-----���+����
	{
		//��ʱ�����رն�ʱ��2ͨ��1
		HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_1);
		Flag_TIM2=0;
		Transfer_Data=(High_number-8+4)/8;
		if(Transfer_Data!=0)	//���������ǲ����ܵ���0�ģ���ֹ�󴥷���
		{
			if(Flag_Transfer==1)	//�������ʮλ
			{
				Output_Y=Transfer_Data-1;
				Flag_Transfer=0;		//ת�䴫���־λ׼�����ո�λ
			}
			else									//������Ǹ�λ
			{
				Output_X=Transfer_Data-1;									//��λҲ�������
				printf("%c",Char_Out[Output_Y][Output_X]);//�����Ӧ�ַ�
				Output_Y=0;Output_X=0;										//������Ѱַ
				Flag_Transfer=1;		//ת�䴫���־λ׼��������һ��ʮλ
			}
			Transfer_Data=0;			//���������˼�ʱ���㡪����ֹ�󴥷�
		}
		High_number=0;					//����������
	}
	
	else if(GPIO_Pin==0x0020) //PC5 �ж�����  �����ʱ���ر������  ������
	{
		if(Flag_TIM2==0)
		{
			HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
			Flag_TIM2=1;
		}
		High_number++;
	}
	
	else ;
}


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
