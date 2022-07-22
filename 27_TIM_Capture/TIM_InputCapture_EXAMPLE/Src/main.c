#include "main.h"
TIM_HandleTypeDef htim1;
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);


uint32_t uwIC2Value1 = 0;			//��������ĵ�һ������ֵ
uint32_t uwIC2Value2 = 0;			//��������ĵڶ�������ֵ
uint32_t uwDiffCapture = 0;		//���������ʱ���
uint16_t uhCaptureIndex = 0;	//����ָ�꣺������һ�β����ǵڶ��β���
uint32_t uwFrequency = 0;			//Ƶ��ֵ




int main(void)
{
  HAL_Init();
  SystemClock_Config();
  BSP_LED_Init(LED4);
  MX_GPIO_Init();
  MX_TIM1_Init();

  /* ��ʱ��1���ã��жϲ���ģʽ ---------------------
     �ⲿ�ź����ӵ���ʱ��1ͨ��������(PA9)  
     �����س����ж�,TIM1 CCR2���ڼ���Ƶ��ֵ 
  ----------------------------------------------- */

  /* ���ж�ģʽ�£��򿪶�ʱ������ */
  if(HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_2) != HAL_OK)
    Error_Handler();  //��ʧ��
  while (1)
  {
  }
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 70;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV10;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV5;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV5;
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
}


static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0xFFFF;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.BreakAFMode = TIM_BREAK_AFMODE_INPUT;
  sBreakDeadTimeConfig.Break2AFMode = TIM_BREAK_AFMODE_INPUT;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
  {
    if(uhCaptureIndex == 0)		/* ��һ�β��� */
    {
      uwIC2Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);  /* ��ȡ����ֵ */
      uhCaptureIndex = 1;
    }
    else if(uhCaptureIndex == 1)      /* �ڶ��β��� */
    {
      uwIC2Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);		/* ��ȡ����ֵ */
      if (uwIC2Value2 > uwIC2Value1)		/* �������β��������ֵ֮�� */
      {
        uwDiffCapture = (uwIC2Value2 - uwIC2Value1); 
      }
      else if (uwIC2Value2 < uwIC2Value1)
      {
        /*0xFFFF ������TIM1_CCRx�Ĵ���ֵ��ת��һȦ������Value2+max+1 - Value1 */
        uwDiffCapture = ((0xFFFF - uwIC2Value1) + uwIC2Value2) + 1;
      }
      else        /*�������ֵ���,�ʹ���������ߵ�Ƶ�ʲ���ֵ����������û��Ӧ�����Ѿ����������ˣ�*/
        Error_Handler();
      /* Ƶ�ʼ���: ����������У���ʱ��1ʱ��Դ��APB1Clk */      
      uwFrequency = HAL_RCC_GetPCLK1Freq() / uwDiffCapture;
      uhCaptureIndex = 0;
    }
  }
}


void Error_Handler(void)
{
  BSP_LED_On(LED4);
  while(1) 
  {
  }
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
  /* Infinite loop */
  while (1)
  {
  }

  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
