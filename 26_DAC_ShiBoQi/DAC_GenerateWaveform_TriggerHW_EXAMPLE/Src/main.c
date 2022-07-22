#include "main.h"


/* ���廷��ģ���ѹ��ģ���ѹֵ��λmV */
#define VDDA_APPLI  ((uint32_t)3300)
  
/* 12λ�ֱ��ʵ�����������ֵ����ѹ��Χ��ģ���ѹ�ο�Vref+��Vref-ȷ����                                       */
#define DIGITAL_SCALE_12BITS  (__LL_DAC_DIGITAL_SCALE(LL_DAC_RESOLUTION_12B))


/* ��������:���β��� */
#define WAVEFORM_AMPLITUDE     (VDDA_APPLI)			  /* �������(��λmV) */
#define WAVEFORM_FREQUENCY     ((uint32_t)1000)	  /* ����Ƶ��(��λ:Hz) */
#define WAVEFORM_SAMPLES_SIZE  (sizeof(WaveformSine_12bits_32samples)/ sizeof(uint16_t)) /* ����DAC�������������д�С */
  
/* ���β���:��ʱ������(����DAC������) */
/*��ʱ��Ƶ��(��λ:Hz)�����ʱ��16λ��ʱ��Ƶ����С1Hz����Χ����С=1Hz�����=32kHz��*/
#define WAVEFORM_TIMER_FREQUENCY                (WAVEFORM_FREQUENCY * WAVEFORM_SAMPLES_SIZE)
#define WAVEFORM_TIMER_FREQUENCY_RANGE_MIN      ((uint32_t)    1)			   /* ��ʱ����СƵ��(��λ:Hz)�����ڼ���Ƶ�ʷ�Χ�� ʹ��16λ�Ķ�ʱ�������Ƶ�ʽ������ֵ��32000����  */
#define WAVEFORM_TIMER_PRESCALER_MAX_VALUE      ((uint32_t)0xFFFF-1)		  /* ��ʱ��Ԥ��Ƶ�����ֵ(0xFFFFΪһ����ʱ��16λ) */



//�����̶ֿ��ϵ����ֵ12λ(��Ӧ�ڵ�ѹVdda)���¿̶��ϵ�ֵ(��Ӧ����WAVEFORM_AMPLITUDE����ĵ�ѹ)�ļ��㡣
#define __WAVEFORM_AMPLITUDE_SCALING(__DATA_12BITS__)                                     \
  (__DATA_12BITS__                                                                        \
   * __LL_DAC_CALC_VOLTAGE_TO_DATA(VDDA_APPLI, WAVEFORM_AMPLITUDE, LL_DAC_RESOLUTION_12B) \
   / __LL_DAC_DIGITAL_SCALE(LL_DAC_RESOLUTION_12B)                                        \
  )

//__IO uint8_t ubButtonPress = 0;  //�������µı�־λ


//�洢�����ź�������
const uint16_t WaveformSine_12bits_32samples[] =
{
__WAVEFORM_AMPLITUDE_SCALING(2048),
__WAVEFORM_AMPLITUDE_SCALING(2447), 
__WAVEFORM_AMPLITUDE_SCALING(2831), 
__WAVEFORM_AMPLITUDE_SCALING(3185), 
__WAVEFORM_AMPLITUDE_SCALING(3495), 
__WAVEFORM_AMPLITUDE_SCALING(3750), 
__WAVEFORM_AMPLITUDE_SCALING(3939), 
__WAVEFORM_AMPLITUDE_SCALING(4056), 
__WAVEFORM_AMPLITUDE_SCALING(4095), 
__WAVEFORM_AMPLITUDE_SCALING(4056),
__WAVEFORM_AMPLITUDE_SCALING(3939),
__WAVEFORM_AMPLITUDE_SCALING(3750),
__WAVEFORM_AMPLITUDE_SCALING(3495),
__WAVEFORM_AMPLITUDE_SCALING(3185),
__WAVEFORM_AMPLITUDE_SCALING(2831),
__WAVEFORM_AMPLITUDE_SCALING(2447),
__WAVEFORM_AMPLITUDE_SCALING(2048),
__WAVEFORM_AMPLITUDE_SCALING(1649),
__WAVEFORM_AMPLITUDE_SCALING(1265),
__WAVEFORM_AMPLITUDE_SCALING(911),
__WAVEFORM_AMPLITUDE_SCALING(601),
__WAVEFORM_AMPLITUDE_SCALING(346),
__WAVEFORM_AMPLITUDE_SCALING(157),
__WAVEFORM_AMPLITUDE_SCALING(40),
__WAVEFORM_AMPLITUDE_SCALING(0),
__WAVEFORM_AMPLITUDE_SCALING(40),
__WAVEFORM_AMPLITUDE_SCALING(157),
__WAVEFORM_AMPLITUDE_SCALING(346),
__WAVEFORM_AMPLITUDE_SCALING(601),
__WAVEFORM_AMPLITUDE_SCALING(911),
__WAVEFORM_AMPLITUDE_SCALING(1265),
__WAVEFORM_AMPLITUDE_SCALING(1649)
};

void     SystemClock_Config(void);
//void     LED_Init(void);
//void     UserButton_Init(void);
//void     WaitForUserButtonPress(void);
//void     LED_Off(void);
void     Configure_DMA(void);
void     Configure_TIM_TimeBase_DAC_trigger(void);
void     Configure_DAC(void);
void     Activate_DAC(void);
//void     LED_On(void);
//void     LED_Blinking(uint32_t Period);

int main(void)
{
  SystemClock_Config();   
  //LED_Init();  
  //UserButton_Init();   				/* �û������������ж�ģʽ */
  //WaitForUserButtonPress(); 	/* �ȴ��û��������� */
	
  //LED_Off();									/* �ر�LED4�� */
	
  Configure_DMA();						/* Ϊ��DAC������������DMA */
  Configure_TIM_TimeBase_DAC_trigger();  /* ���ö�ʱ����Ϊһ��ʱ���׼�����ڴ���DACת�������� */
  Configure_DAC();						/* ����DACͨ��1 */
  Activate_DAC();  						/* ʹ��DACͨ��1 */
	
  //LED_On();  									/* �� LED4 */
	
  while (1)
  {
  }
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 56000000
  *            HCLK(Hz)                       = 56000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            Flash Latency(WS)              = 2
  *            PLLM                           = 4
  *            PLLN                           = 70
  *            PLLP                           = 10
  *            PLLQ                           = 5
  *            PLLR                           = 5
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  /* HSI configuration and activation */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1) 
  {
  }
  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_4, 70, LL_RCC_PLLR_DIV_5);
  LL_RCC_PLL_Enable();
  LL_RCC_PLL_EnableDomain_SYS();
  while(LL_RCC_PLL_IsReady() != 1) 
  {
  }
  /* Sysclk activation on the main PLL */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) 
  {
  }
  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  /* Set systick to 1ms in using frequency set to 56MHz */
  /* This frequency can be calculated through LL RCC macro */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ(__LL_RCC_CALC_HSI_FREQ(), 
                                  LL_RCC_PLLM_DIV_4, 70, LL_RCC_PLLR_DIV_5)*/
  LL_Init1msTick(56000000);
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(56000000);
}

/*
void LED_Init(void)
{
  // ʹ�� LED4 ʱ��
  LED4_GPIO_CLK_ENABLE();
  // ��IO�˿������������������ģʽ�����ⲿLED4 
  LL_GPIO_SetPinMode(LED4_GPIO_PORT, LED4_PIN, LL_GPIO_MODE_OUTPUT);
  // Reset value is LL_GPIO_OUTPUT_PUSHPULL 
  //LL_GPIO_SetPinOutputType(LED4_GPIO_PORT, LED4_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  // Reset value is LL_GPIO_SPEED_FREQ_LOW 
  //LL_GPIO_SetPinSpeed(LED4_GPIO_PORT, LED4_PIN, LL_GPIO_SPEED_FREQ_LOW);
  // Reset value is LL_GPIO_PULL_NO 
  //LL_GPIO_SetPinPull(LED4_GPIO_PORT, LED4_PIN, LL_GPIO_PULL_NO);
}


void UserButton_Init(void)
{
  // ʹ�ܰ���ʱ�� 
  USER_BUTTON_GPIO_CLK_ENABLE();
  // ���ð�����GPIO�˿� 
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_NO);
  // ���ⲿ�����ӵ�GIO�˿� 
  USER_BUTTON_SYSCFG_SET_EXTI();
  // ʹ�������ش�����13���ⲿ�ж��� 
  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();
  // ��USER_BUTTON_EXTI_IRQn ����NVIC  
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn); 
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn,0x03);  
}


void WaitForUserButtonPress(void)
{
  while (ubButtonPress == 0) //����û�а���ʱ��LED4��ʾ����˸��ͣ
  {
    LL_GPIO_TogglePin(LED4_GPIO_PORT, LED4_PIN);
    LL_mDelay(LED_BLINK_FAST);
  }
  ubButtonPress = 0; //���а�������ʱ���ж����ｫ����һ�����ϱߵ�while����֮������������±�־λ
}


void LED_Off(void)
{
  LL_GPIO_ResetOutputPin(LED4_GPIO_PORT, LED4_PIN);
} */


void Configure_DMA(void)
{
  /* ����NVIC ��ʹ��DMA�ж� */
  NVIC_SetPriority(DMA1_Channel2_3_IRQn, 1); /* DMA IRQ���ȼ�����DAC IRQ */
  NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
  /* ʹ���͸�DMA���ⲿʱ�� */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
  /* ����DMA���� */
  /*  - DMA������ѭ��ģʽ�������޵�DAC�źŲ�����  */
  /*  - ֱ�Ӵ洢����ȡת�Ƶ�DACû�е�ַ������  	*/
  /*  - ֱ�Ӵ洢����ȡת�����ַ������          	*/
  /*  - DMA���䵽DAC�԰���ƥ��DAC�ֱ���12λ				*/
  /*  - DMA�Ӵ洢��ͨ������ת�Ƶ���DAC���ݻ�����ƥ��ı�������:���֡�  */
  LL_DMA_ConfigTransfer(DMA1,LL_DMA_CHANNEL_3,LL_DMA_DIRECTION_MEMORY_TO_PERIPH | LL_DMA_MODE_CIRCULAR |
                        LL_DMA_PERIPH_NOINCREMENT | LL_DMA_MEMORY_INCREMENT | LL_DMA_PDATAALIGN_HALFWORD |
                        LL_DMA_MDATAALIGN_HALFWORD | LL_DMA_PRIORITY_HIGH );
  /* ѡ��DAC��ΪDMA�������� */
  LL_DMA_SetPeriphRequest(DMA1,LL_DMA_CHANNEL_3,LL_DMAMUX_REQ_DAC1_CH1);
  /* ����Դ��Ŀ�ĵ�DMA�����ַ */
  LL_DMA_ConfigAddresses(DMA1,LL_DMA_CHANNEL_3,(uint32_t)&WaveformSine_12bits_32samples,LL_DAC_DMA_GetRegAddr(DAC1, LL_DAC_CHANNEL_1,
												LL_DAC_DMA_REG_DATA_12BITS_RIGHT_ALIGNED),LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
  /* ����DMA�����С */
  LL_DMA_SetDataLength(DMA1,LL_DMA_CHANNEL_3,WAVEFORM_SAMPLES_SIZE);
  /* ʹ��DMA�����ж�: ������� */
  LL_DMA_EnableIT_TE(DMA1,LL_DMA_CHANNEL_3);
  /* ע: �ڱ����У�Ψһ�����DMA�ж��Ǵ�����������Ҫ��DMA�жϴ����һ��ʹ�����ɿ��Ա�����ο�DMAʾ����     */
  /* ����DMA���� */
	LL_DMA_EnableChannel(DMA1,LL_DMA_CHANNEL_3);
}


void Configure_TIM_TimeBase_DAC_trigger(void)
{
  uint32_t timer_clock_frequency = 0;             /* ��ʱ����ʱ��Ƶ�� */
  uint32_t timer_prescaler = 0;                   /* ʱ��Ԥ��Ƶ����ʹʱ�������ܵ���СƵ�ʶ��� */
  uint32_t timer_reload = 0;                      /* ��ʱ��Ԥ��Ƶ�������еĶ�ʱ�����¼���ֵ����ʵ��ʱ���׼���� */
  /* ����NVIC */ 
  /* Note: In this example, timer interruptions are not activated.            */
  /*       If needed, timer interruption at each time base period is          */
  /*       possible.                                                          */
  /*       Refer to timer examples.                                           */
  /* ���ö�ʱ�� */
  /* Configuration of timer as time base:                                     */ 
  /* Caution: Computation of frequency is done for a timer instance on APB1   */
  /*          (clocked by PCLK1)                                              */
  /* Timer frequency is configured from the following constants:              */
  /* - WAVEFORM_TIMER_FREQUENCY: timer frequency (unit: Hz).                  */
  /* - WAVEFORM_TIMER_FREQUENCY_RANGE_MIN: timer minimum frequency possible   */
  /*   (unit: Hz).                                                            */
  /* Note: Refer to comments at these literals definition for more details.   */
  /* Retrieve timer clock source frequency */
  /* If APB1 prescaler is different of 1, timers have a factor x2 on their    */
  /* clock source.                                                            */
  if (LL_RCC_GetAPB1Prescaler() == LL_RCC_APB1_DIV_1)
  {
    timer_clock_frequency = __LL_RCC_CALC_PCLK1_FREQ(SystemCoreClock, LL_RCC_GetAPB1Prescaler());
  }
  else
  {
    timer_clock_frequency = (__LL_RCC_CALC_PCLK1_FREQ(SystemCoreClock, LL_RCC_GetAPB1Prescaler()) * 2);
  }
  /* ��ʱ��Ԥ��Ƶ������ */
  /* (computation for timer 16 bits, additional + 1 to round the prescaler up) */
  timer_prescaler = ((timer_clock_frequency / (WAVEFORM_TIMER_PRESCALER_MAX_VALUE * WAVEFORM_TIMER_FREQUENCY_RANGE_MIN)) +1);
  /* ��ʱ�����ؼ��� */
  timer_reload = (timer_clock_frequency / (timer_prescaler * WAVEFORM_TIMER_FREQUENCY));
  /* ʹ�ܶ�ʱ����Χʱ�� */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);
  /* ���ö�ʱ��Ԥ��Ƶ��ֵ */
  LL_TIM_SetPrescaler(TIM6, (timer_prescaler - 1));
  /* ���ö�ʱ���Զ�����ֵ */
  LL_TIM_SetAutoReload(TIM6, (timer_reload - 1));
  /* ���õ�ǰģʽ */
  LL_TIM_SetCounterMode(TIM6, LL_TIM_COUNTERMODE_UP); 
  /* ע: �ڱ����У���ʱ���ж�û�б���������Ҫ����ÿ��ʱ���׼�����϶�ʱ���ж��ǿ��ܵġ� */
  /* ���ö�ʱ��������� (TRGO) */
  LL_TIM_SetTriggerOutput(TIM6, LL_TIM_TRGO_UPDATE);
  /* ʹ�ܼ����� */
  LL_TIM_EnableCounter(TIM6);
}


void Configure_DAC(void)
{
  /* ʹ��DAC��ص�GPIOʱ��*/
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA); 
  /* ��GPIO������ģ��ģʽ��ΪDAC������� */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_ANALOG);
  /* ����NVICʹ��DAC1�ж� */
  NVIC_SetPriority(TIM6_DAC_LPTIM1_IRQn, 0);
  NVIC_EnableIRQ(TIM6_DAC_LPTIM1_IRQn);
  /* ʹ��DACʱ�� */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_DAC1);
  /* ����ģʽ��DACͨ��1 */
  // LL_DAC_SetMode(DAC1, LL_DAC_CHANNEL_1, LL_DAC_MODE_NORMAL_OPERATION);
  /* ѡ�񴥷�Դ */
  LL_DAC_SetTriggerSource(DAC1, LL_DAC_CHANNEL_1, LL_DAC_TRIG_EXT_TIM6_TRGO);
  /* ����DACͨ��1����� */
  LL_DAC_ConfigOutput(DAC1, LL_DAC_CHANNEL_1, LL_DAC_OUTPUT_MODE_NORMAL, LL_DAC_OUTPUT_BUFFER_ENABLE, LL_DAC_OUTPUT_CONNECT_GPIO);
  /* Note: DAC channel output configuration can also be done using            */
  /*       LL unitary functions:                                              */
  // LL_DAC_SetOutputMode(DAC1, LL_DAC_CHANNEL_1, LL_DAC_OUTPUT_MODE_NORMAL);
  // LL_DAC_SetOutputBuffer(DAC1, LL_DAC_CHANNEL_1, LL_DAC_OUTPUT_BUFFER_ENABLE);
  // LL_DAC_SetOutputConnection(DAC1, LL_DAC_CHANNEL_1, LL_DAC_OUTPUT_CONNECT_GPIO);
  /* ����DACģʽ����-����ʱ�� */
  // LL_DAC_SetSampleAndHoldSampleTime (DAC1, LL_DAC_CHANNEL_1, 0x3FF);
  // LL_DAC_SetSampleAndHoldHoldTime   (DAC1, LL_DAC_CHANNEL_1, 0x3FF);
  // LL_DAC_SetSampleAndHoldRefreshTime(DAC1, LL_DAC_CHANNEL_1, 0xFF);
  /* ����DACͨ��DMA���� */
  LL_DAC_EnableDMAReq(DAC1, LL_DAC_CHANNEL_1);
  /* ʹ��DACͨ��һ�ж� */
  LL_DAC_EnableIT_DMAUDR1(DAC1);
}


void Activate_DAC(void)
{
  __IO uint32_t wait_loop_index = 0;
  /* ʹ��DACͨ��1 */
  LL_DAC_Enable(DAC1, LL_DAC_CHANNEL_1);
  /* Delay for DAC channel voltage settling time from DAC channel startup.    */
  /* Compute number of CPU cycles to wait for, from delay in us.              */
  /* Note: Variable divided by 2 to compensate partially                      */
  /*       CPU processing cycles (depends on compilation optimization).       */
  /* Note: If system core clock frequency is below 200kHz, wait time          */
  /*       is only a few CPU processing cycles.                               */
  wait_loop_index = ((LL_DAC_DELAY_STARTUP_VOLTAGE_SETTLING_US * (SystemCoreClock / (100000 * 2))) / 10);
  while(wait_loop_index != 0)
  {
    wait_loop_index--;
  }
  /* Enable DAC channel trigger */
  /* Note: DAC channel conversion can start from trigger enable:              */
  /*       - if DAC channel trigger source is set to SW:                      */
  /*         DAC channel conversion will start after trig order               */
  /*         using function "LL_DAC_TrigSWConversion()".                      */
  /*       - if DAC channel trigger source is set to external trigger         */
  /*         (timer, ...):                                                    */
  /*         DAC channel conversion can start immediately                     */
  /*         (after next trig order from external trigger)                    */
  LL_DAC_EnableTrigger(DAC1, LL_DAC_CHANNEL_1);
}

/*
void LED_On(void)
{
  LL_GPIO_SetOutputPin(LED4_GPIO_PORT, LED4_PIN);
}


void LED_Blinking(uint32_t Period)
{
  LL_GPIO_SetOutputPin(LED4_GPIO_PORT, LED4_PIN);
  while (1)
  {
    LL_GPIO_TogglePin(LED4_GPIO_PORT, LED4_PIN);  
    LL_mDelay(Period);
  }
}*/ 

//******************************************************************�жϻص�����

void UserButton_Callback(void)
{
  //if(ubButtonPress == 0)	//�������´������ж�
  //  ubButtonPress = 1;		//����������־�����ߴ������Ѿ�����
}

void DacDmaTransferError_Callback()
{
  //LED_Blinking(LED_BLINK_ERROR);
}

void DacUnderrunError_Callback(void)
{
  // ע��:�ڽ������������ѭ��֮ǰ�����õ��´˴����DAC�жϡ� 
  // �ر��ж�DAC channel1 underrun 
  //LL_DAC_DisableIT_DMAUDR1(DAC1);
  //LED_Blinking(LED_BLINK_ERROR);
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


