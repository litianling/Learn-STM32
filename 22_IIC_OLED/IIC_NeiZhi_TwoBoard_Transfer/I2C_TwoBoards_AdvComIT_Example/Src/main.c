#include "main.h"

#define MASTER_BOARD	//��ע�ʹ����ǴӰ�  ע�͵�����������
#define MASTER_REQ_READ    0x12
#define MASTER_REQ_WRITE   0x34

I2C_HandleTypeDef hi2c1;

uint8_t bTransferRequest = 0;  //�����������������Ƿ��ͻ��ǽ���

/* Buffer used for transmission ���ͻ�����*/
uint8_t aTxBuffer[] = " ****I2C_TwoBoards communication based on IT****  ****I2C_TwoBoards communication based on IT****  ****I2C_TwoBoards communication based on IT**** ";
__IO uint16_t hTxNumData = 0;

/* Buffer used for reception ���ջ�����*/
uint8_t aRxBuffer[RXBUFFERSIZE];  //�ⶨ���С����ֵ
__IO uint16_t hRxNumData = 0;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);

static uint16_t Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);
static void Flush_Buffer(uint8_t *pBuffer, uint16_t BufferLength);



int main(void)
{
  /* USER CODE BEGIN 1 */
  /*STM32G0xx HAL library initialization:
		- Configure the Flash prefetch
		- Systick timer is configured by default as source of time base, but user
		can eventually implement his proper time base source (a general purpose
		timer for example or other time source), keeping in mind that Time base
		duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
		handled in milliseconds basis.
		- Low Level Initialization 
	
	STM32G0xx HAL���ʼ��:
		������FlashԤȡ
		��ϵͳstick��ʱ��Ĭ������Ϊʱ��Դ���������û�
		�����ܷ�ʵ���Լ����ʵ�ʱ��Դ(ͨ��Ŀ��
		���綨ʱ��������ʱ��Դ)����סʱ�����
		durationӦ�ñ���1ms����Ϊ������PPP_TIMEOUT_VALUEs
		�Ժ���Ϊ��������
		-�ͼ����ʼ��
     */

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();
  BSP_LED_Init(LED4);


#ifdef MASTER_BOARD
  /* Configure User push-button �����û���ť*/
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  /* �ڿ�ʼͨ��֮ǰ���ȴ��û����°�ť*/
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_RESET) ; //����Ϊ0  ��Ϊ0��û����
		
  /*�ڿ�ʼͨ��֮ǰ���ȴ��û���ť�ͷ�*/
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_SET) ;
	
#endif /* MASTER_BOARD */

  while (1)
  {
/*
		while(1)
		{
			if (HAL_I2C_Master_Transmit_IT(&hi2c1, (uint16_t)I2C_ADDRESS, (uint8_t *)&hTxNumData, 2) != HAL_OK)
        Error_Handler();
			HAL_Delay(1);
		}
*/
		
#ifdef MASTER_BOARD
    /* Initialize number of data variables ��ʼ�����ݱ���������*/
    hTxNumData = TXBUFFERSIZE;	//*hTxNumData ָ�����һ��Ҫ���͵��ַ�
    hRxNumData = RXBUFFERSIZE;	//��СͬTXBUFFERSIZE
		

    /* Update bTransferRequest to send buffer write request for Slave ����bTransferRequestΪ�ӷ��������ͻ�����д����*/
    bTransferRequest = MASTER_REQ_WRITE; //������Ϊ0x34 ��ʼд
		
		
    /*##-2- Master sends write request for slave ����������ӷ���������д����*/
    do
    {
      if (HAL_I2C_Master_Transmit_IT(&hi2c1, (uint16_t)I2C_ADDRESS, (uint8_t *)&bTransferRequest, 1) != HAL_OK)
        Error_Handler();
      /*  �ڿ�ʼ�µ�ͨ�Ŵ���֮ǰ������Ҫ�������ĵ�ǰ״̬;���æ������Ҫ�ȴ���ǰ����������ٿ�ʼ�µĴ��䡣
			Ϊ�˼��������ʾ��ֻ�ǵȴ��������������Ӧ�ó�������ڴ����������ʱִ����������*/
			while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
      /* ��ȷ��ʧ�ܷ���ʱ(�ӷ�������ȷ�����ĵ�ַ)����������������ͨ��*/
    }
    while (HAL_I2C_GetError(&hi2c1) == HAL_I2C_ERROR_AF);


		
    /*##-3- Master sends number of data to be written ������������Ҫд���������*/
    do
    {
      if (HAL_I2C_Master_Transmit_IT(&hi2c1, (uint16_t)I2C_ADDRESS, (uint8_t *)&hTxNumData, 2) != HAL_OK)
        Error_Handler();
      while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY) ;  /* ������贫��״̬*/
    }
    while (HAL_I2C_GetError(&hi2c1) == HAL_I2C_ERROR_AF);/* ��ȷ��ʧ�ܷ���ʱ(�ӷ�������ȷ�����ĵ�ַ)����������������ͨ�� */
		

		
    /*##-4- Master sends aTxBuffer to slave   ����������aTxBuffer���͸��ӷ�����*/
    do
    {
      if (HAL_I2C_Master_Transmit_IT(&hi2c1, (uint16_t)I2C_ADDRESS, (uint8_t *)aTxBuffer, TXBUFFERSIZE) != HAL_OK)
        Error_Handler();
      while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY) ;   /*  ������贫��״̬ */
    }
    while (HAL_I2C_GetError(&hi2c1) == HAL_I2C_ERROR_AF);		/* ��ȷ��ʧ�ܷ���ʱ(�ӷ�������ȷ�����ĵ�ַ)����������������ͨ�� */
		

		
    /* Update bTransferRequest to send buffer read request for Slave ����bTransferRequest��������Ϊ�ӷ��������ͻ�����������*/
    bTransferRequest = MASTER_REQ_READ;  //0x12��ʼ��
		
		
		
    /*##-5- Master sends read request for slave ����������ӷ��������Ͷ�����*/
    do
    {
      if (HAL_I2C_Master_Transmit_IT(&hi2c1, (uint16_t)I2C_ADDRESS, (uint8_t *)&bTransferRequest, 1) != HAL_OK)
        Error_Handler();
      while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY) ;   /* ������贫��״̬ */
    }
    while (HAL_I2C_GetError(&hi2c1) == HAL_I2C_ERROR_AF);/* ��ȷ��ʧ�ܷ���ʱ(�ӷ�������ȷ�����ĵ�ַ)����������������ͨ�� */

		
		
    /*##-6- Master sends number of data to be read ������������Ҫ��ȡ��������*/
    do
    {
      if (HAL_I2C_Master_Transmit_IT(&hi2c1, (uint16_t)I2C_ADDRESS, (uint8_t *)&hRxNumData, 2) != HAL_OK)
        Error_Handler();
      while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY) ;   /*  ������贫��״̬ */
    }
    while (HAL_I2C_GetError(&hi2c1) == HAL_I2C_ERROR_AF);/* ��ȷ��ʧ�ܷ���ʱ(�ӷ�������ȷ�����ĵ�ַ)����������������ͨ�� */

	

    /*##-7- Master receives aRxBuffer from slave ���������Ӵӷ���������aRxBuffer*/
    do
    {
      if (HAL_I2C_Master_Receive_IT(&hi2c1, (uint16_t)I2C_ADDRESS, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
        Error_Handler();
      while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY) ;      /* ������贫��״̬ */
    }
    while (HAL_I2C_GetError(&hi2c1) == HAL_I2C_ERROR_AF);/* ��ȷ��ʧ�ܷ���ʱ(�ӷ�������ȷ�����ĵ�ַ)����������������ͨ�� */

		
		
		
    /* Check correctness of received buffer �����ջ���������ȷ��*/
    if (Buffercmp((uint8_t *)aTxBuffer, (uint8_t *)aRxBuffer, hRxNumData))
      Error_Handler();
		
    /* Flush Rx buffers ˢ�½��ջ�����*/
    Flush_Buffer((uint8_t *)aRxBuffer, RXBUFFERSIZE);

    /* ��LED4��ת*/
    BSP_LED_Toggle(LED4);

    /*����ӳ�������LED4���л�*/
    HAL_Delay(25);
		
#else /* MASTER_BOARD */

    /* Initialize number of data variables ��ʼ�����ݱ���������*/
    hTxNumData = 0;
    hRxNumData = 0;

    /*##-2- Slave receive request from master �Ӱ�������������*/
    while (HAL_I2C_Slave_Receive_IT(&hi2c1, (uint8_t *)&bTransferRequest, 1) != HAL_OK) ;
		
    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY) ; /*  ������贫��״̬ */

    /* If master request write operation �����������д����*/
    if (bTransferRequest == MASTER_REQ_WRITE)
    {
      /*##-3- Slave receive number of data to be read ����׼����������*/
      while (HAL_I2C_Slave_Receive_IT(&hi2c1, (uint8_t *)&hRxNumData, 2) != HAL_OK);

      while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY) ;  /*  ������贫��״̬ */

      /*##-4- Slave receives aRxBuffer from master ����������aRxBuffer*/
      while (HAL_I2C_Slave_Receive_IT(&hi2c1, (uint8_t *)aRxBuffer, hRxNumData) != HAL_OK);

      while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)  ;  /*  ������贫��״̬ */

      /* Check correctness of received buffer �����ջ���������ȷ��*/
      if (Buffercmp((uint8_t *)aTxBuffer, (uint8_t *)aRxBuffer, hRxNumData))
        Error_Handler();

      /* Flush Rx buffers ˢ�½��ջ�����*/
      Flush_Buffer((uint8_t *)aRxBuffer, RXBUFFERSIZE);

      /* Toggle LED4 ��תLED4��*/
      BSP_LED_Toggle(LED4);
    }
    /* If master request write operation  ����������������*/
    else
    {
      /*##-3- Slave receive number of data to be written �Ӱ����Ҫд���������*/
      while (HAL_I2C_Slave_Receive_IT(&hi2c1, (uint8_t *)&hTxNumData, 2) != HAL_OK);

      while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY) ;   /*  ������贫��״̬ */
			
      /*##-4- Slave transmit aTxBuffer to master �Ӱ彫aTxBuffer���������*/
      while (HAL_I2C_Slave_Transmit_IT(&hi2c1, (uint8_t *)aTxBuffer, RXBUFFERSIZE) != HAL_OK);
      
			/* ������贫��״̬ */
      while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY) ;
    }
#endif /* MASTER_BOARD */  
	}
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.  */
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
  /** Initializes the CPU, AHB and APB buses clocks  ��ʼ��CPU��AHB��APB����*/
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
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00303D5B;
  hi2c1.Init.OwnAddress1 = I2C_ADDRESS;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter  ����ģ���˲���*/
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter  ���������˲���*/
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /** I2C Fast mode Plus enable     IIC����ģʽʹ��*/
  HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C1);
}


static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

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


static void Flush_Buffer(uint8_t *pBuffer, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    *pBuffer = 0;
    pBuffer++;
  }
}


void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
  /** 1- When Slave doesn't acknowledge its address, Master restarts communication.
    * 2- When Master doesn't acknowledge the last data transferred, Slave doesn't care in this example. */
  if (HAL_I2C_GetError(I2cHandle) != HAL_I2C_ERROR_AF)
    Error_Handler();
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
