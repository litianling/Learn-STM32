
#include "main.h"

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

//**************************************��������������
/* ����ģ��ֵ�Ķ��� */
/* ģ��ο���ѹֵ(Vref+)������ģ���ѹ   */
/* ����� (��λ: mV).      */
#define VDDA_APPLI (3300UL)
	
#define ADC_CONVERTED_DATA_BUFFER_SIZE   (3)
__IO   uint16_t   aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]; /* ADCת������(��������)  ��СΪ3 */

/* ��ADCת�����ݼ���õ�����ֵ */
__IO uint16_t uhADCxConvertedData_VoltageGPIO_mVolt = 0U;        /* GPIO�ܽ�(��ӦADCͨ��)�ϵĵ�ѹֵ(��λ:mV) */
__IO uint16_t uhADCxConvertedData_VrefInt_mVolt = 0U;            /* �ڲ���ѹ�ο�VrefIntֵ(��λ:mV) */
__IO  int16_t hADCxConvertedData_Temperature_DegreeCelsius = 0U; /* �¶�ֵ(��λ:���϶�) */
__IO uint16_t uhADCxConvertedData_VrefAnalog_mVolt = 0U;         /* ģ��ο���ѹֵ(Vref+)������ģ���ѹ��ԴVdda����ADCת�����ݼ���(��λ:mV) */


/* ADCת�����ݵ�DMA����״̬ */
/*  0: DMA ����û�����	*/
/*  1: DMA �������	*/
/*  2: DMA ��δ��ʼ����(��ʼ״̬)	*/
__IO   uint8_t ubDmaTransferStatus = 2; /*��������DMA�жϻص� */

__IO   uint8_t ubUserButtonClickEvent = RESET;  /* �¼����:�û������жϺ���һ���ɿ������� */



ITStatus EXTI_GetITStatus(uint32_t EXTI_Line)
{
    ITStatus bitstatus = RESET; 
    uint32_t enablestatus = 0;  
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
//**************************************

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);


int main(void)
{
	
  uint32_t tmp_index_adc_converted_data = 0; //���鸳ֵ��������
	
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
	
  for (tmp_index_adc_converted_data = 0; tmp_index_adc_converted_data < ADC_CONVERTED_DATA_BUFFER_SIZE; tmp_index_adc_converted_data++)
    aADCxConvertedData[tmp_index_adc_converted_data] = 0x1000;  //ȫ������Ϊ��ʼֵ����1000
  
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,RESET);		//LED��ʼ״̬��
	
  /* ����ADCУ׼ */
  if (HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK)
    Error_Handler();	  /* У׼���� */
  

  /*## ��ʼADCת�� ###############################################*/
  /* ʹ��DMA����ADC�鳣��ת�� */
  if (HAL_ADC_Start_DMA(&hadc1,(uint32_t *)aADCxConvertedData,ADC_CONVERTED_DATA_BUFFER_SIZE) != HAL_OK)
    Error_Handler();		/* ADCת���������� */
	
	
  while (1)
  {
    /* �ȴ��û����°���ִ�����²��� */
    while (ubUserButtonClickEvent == RESET)
    {
    }
    /* ������±�־λ */
    HAL_Delay(200);
    ubUserButtonClickEvent = RESET;
    
    /* ��ʼADCת�� */
    /* �������������ڲ�����ģʽ�����õģ��⽫ִ��ת������һ����������������    */
    /* �ʼ�: �����������, �������������ת������ˣ�ÿ��ת�����������"HAL_ADC_Start()".     */
    /*       ��ΪDMA�����Ѿ��ɺ���"HAL_ADC_Start_DMA()"��֮ǰ����, �˺�������DMA�����������  */
    if (HAL_ADC_Start(&hadc1) != HAL_OK)
    {
      Error_Handler(); 
    }

    /* ��ʱ�Եȴ�ADCת����DMA�������(���±���ubdmateransferstatus) */
    HAL_Delay(1000);

		
    /* ���ADC�Ƿ�ת�������е����м��� */
    if (ubDmaTransferStatus == 1)  //���DMA�������
    {
      /* ADC�ļ��㽫ԭʼ����ת��Ϊ����ֵ  */
      /* ʹ��LL ADC������������ꡣ  */
      /* Note: ADC�����ת�Ƶ����顰aADCxConvertedData��  */
      /*       �����ǵ�����˳����ADC���С�  */					        	/*   A*B/C   */
			uhADCxConvertedData_VoltageGPIO_mVolt        = __LL_ADC_CALC_DATA_TO_VOLTAGE(VDDA_APPLI, aADCxConvertedData[0], LL_ADC_RESOLUTION_12B); //�������ѹ*��������/�ֱ���----������
			uhADCxConvertedData_VrefInt_mVolt            = __LL_ADC_CALC_DATA_TO_VOLTAGE(VDDA_APPLI, aADCxConvertedData[2], LL_ADC_RESOLUTION_12B);
			hADCxConvertedData_Temperature_DegreeCelsius = __LL_ADC_CALC_TEMPERATURE(VDDA_APPLI, aADCxConvertedData[1], LL_ADC_RESOLUTION_12B);
      
      /* ��ѡ�أ�Ϊ��������ӵ�Ŀ�ģ����ڲ���ѹ�ο�VrefInt��ADCת������ģ��ο���ѹ(Vref+)��*/
      /* �����ѹӦ�ö�Ӧ�����֡�VDDA_APPLI����ֵ��     ע:Ӧ���е�ѹVref+ֵδ֪ʱ�ɽ��д˼��㡣  */
			uhADCxConvertedData_VrefAnalog_mVolt = __LL_ADC_CALC_VREFANALOG_VOLTAGE(aADCxConvertedData[2], LL_ADC_RESOLUTION_12B);
      
      /* ������� */
      for (tmp_index_adc_converted_data = 0; tmp_index_adc_converted_data < ADC_CONVERTED_DATA_BUFFER_SIZE; tmp_index_adc_converted_data++)
        aADCxConvertedData[tmp_index_adc_converted_data] = 0x00;
			
      ubDmaTransferStatus = 0;		// ����DMA����״̬���� 
    }
    /* ע:LED״̬ȡ����DMA����״̬����ΪDMA IRQ���������ο�����"HAL_ADC_ConvCpltCallback()"��"HAL_ADC_ConvHalfCpltCallback()"�� */
    /* ע: ADCת�����ݴ洢�����顰aADCxConvertedData����(������ԣ�����ļ��Ӵ����еı�������)��  */
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

static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.LowPowerAutoPowerOff = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 3;
  hadc1.Init.DiscontinuousConvMode = ENABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_1CYCLE_5;
  hadc1.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_160CYCLES_5;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_VREFINT;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

}


void EXTI4_15_IRQHandler(void)
{
	if (EXTI_GetITStatus(0x2000) != 0x00)  // 0x2000==EXTI_PIN_13
  {
		EXTI_ClearITPendingBit(0x2000);  //Clear interrupt flag bit
		   /* ���ñ����������򱨸水ť�¼� */
   ubUserButtonClickEvent = SET;
  }
}



void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  /* ����DMA����״̬���� 1���*/
  ubDmaTransferStatus = 1;  
  /* ������������DMA������� */
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,SET);
}


void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	/* ������DMA����״̬����δ���*/
  /* �رա�������DMA����δ��� */
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,RESET);
}


void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
	//��ADC����ʱ��ִ����Ҫ�Ĵ���������ʾ�û�
  Error_Handler();
}



void Error_Handler(void)
{
  while(1) 
  {
    /* LED4��˸ */
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
    HAL_Delay(50);
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
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/