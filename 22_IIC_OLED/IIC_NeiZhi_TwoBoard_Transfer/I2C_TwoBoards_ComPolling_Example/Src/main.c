/*
	SCL Pin: PB6 (CN10, pin24)
	SDA Pin: PB7 (CN7, pin21)
 */

#include "main.h"
I2C_HandleTypeDef hi2c1;
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);

//#define MASTER_BOARD
uint8_t aTxBuffer[] = " ****I2C_TwoBoards communication based on Polling****  ****I2C_TwoBoards communication based on Polling****  ****I2C_TwoBoards communication based on Polling**** ";
uint8_t aRxBuffer[RXBUFFERSIZE];
static uint16_t Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();
  BSP_LED_Init(LED4);


	
#ifdef MASTER_BOARD

  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);								/* �û�������ʼ�� */
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_RESET);		/* ��ʼ���ݴ���֮ǰ�ȴ��û��������� */
  HAL_Delay(50);																						/* �ӳ��Ա�����ܵ��źŷ�������Ϊ��ť�ͷ� */
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_SET);			/* ��ʼ���ݴ���֮ǰ�ȴ��û������ͷ� */
	
/*  �˶�Ϊ���Դ��룬��ʽ���п�ɾ��
	while(1)
	{
		HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)I2C_ADDRESS, (uint8_t *)aTxBuffer, 5, 10000);
		HAL_Delay(10);
	}
*/

  /* ����ͨ�����仺�����������ݣ���ʱ10S */
  while (HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)I2C_ADDRESS, (uint8_t *)aTxBuffer, TXBUFFERSIZE, 10000) != HAL_OK)
  {
    /* ��Timeout������ʱ������Error_Handler()������
		��ȷ��ʧ�ܷ���(������������ȷ�����ַ)ʱ������������������ͨ��		*/
    if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
      Error_Handler();
  }

  BSP_LED_On(LED4);	/* ���ݷ�����ȷ��LED4�� */

	
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_RESET);		/* ��ʼ���ݴ���֮ǰ�ȴ��û��������� */
  HAL_Delay(50);																						/* �ӳ��Ա�����ܵ��źŷ�������Ϊ��ť�ͷ� */
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_SET);			/* ��ʼ���ݴ���֮ǰ�ȴ��û������ͷ� */

  /* �������ģʽ�ȴ����ݴ��룬����������ʱ10S */
  while (HAL_I2C_Master_Receive(&hi2c1, (uint16_t)I2C_ADDRESS, (uint8_t *)aRxBuffer, RXBUFFERSIZE, 10000) != HAL_OK)
  {
    /* ��Timeout������ʱ������Error_Handler()������
		��ȷ��ʧ�ܷ���(������������ȷ�����ַ)ʱ������������������ͨ��		*/
    if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
      Error_Handler();
  }

  /* ���ݽ�����ȷ�ر�LED�� */
  BSP_LED_Off(LED4);

#else

  /* �Ӱ�������ݲ����䷢�� */

  /*�Ӱ����������ʱ10S  */
/*	�˶�Ϊ���Դ��룬��ʽ���п�ɾ��
	while(1)
	{
		HAL_I2C_Slave_Receive(&hi2c1, (uint8_t *)aRxBuffer, 5, 10000);
		HAL_Delay(10);
	}
*/
	
  if (HAL_I2C_Slave_Receive(&hi2c1, (uint8_t *)aRxBuffer, RXBUFFERSIZE, 10000) != HAL_OK)
    Error_Handler();

  /* �������ݳɹ���LED4 */
  BSP_LED_On(LED4);

  /* �Ӱ����÷��ͻ������������ݣ���ʱ10S */
  if (HAL_I2C_Slave_Transmit(&hi2c1, (uint8_t *)aTxBuffer, TXBUFFERSIZE, 10000) != HAL_OK)
    Error_Handler();

  /* �ر�LED4���ͳɹ� */
  BSP_LED_Off(LED4);

#endif

  /* �ȽϷ��ͻ���������ջ���������ͬ�򾯱� */
  if (Buffercmp((uint8_t *)aTxBuffer, (uint8_t *)aRxBuffer, RXBUFFERSIZE))
    Error_Handler();
	
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

static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00303D5B;
  hi2c1.Init.OwnAddress1 = I2C_ADDRESS;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_10BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /** I2C Fast mode Plus enable
  */
  HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C1);
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOB_CLK_ENABLE();
}



static uint16_t Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
      return BufferLength;
    pBuffer1++;
    pBuffer2++;
  }
  return 0;
}


void Error_Handler(void)
{
  while (1)
  {
    BSP_LED_Toggle(LED4);
    HAL_Delay(1000);
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
  Error_Handler();
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
