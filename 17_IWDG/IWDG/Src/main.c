#include "main.h"

static volatile uint8_t ubKeyPressed = 0;  //volatile�ؼ��֣����ɱ�δ֪���ظ���   �ⲿ��λ������־λ��ʼΪ0û��λ����

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_IWDG_Init(void);

void     Check_IWDG_Reset(void);  //���������Ź���λ
void     LED_On(void);						//LD4��״̬

int main(void)
{

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);		//APB���ߡ�������ϵͳʱ��ʹ��
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);			//APB���ߡ�������PWRʱ��ʹ��

  LL_SYSCFG_DisableDBATT(LL_SYSCFG_UCPD1_STROBE | LL_SYSCFG_UCPD2_STROBE);

  SystemClock_Config();	//ϵͳʱ�ӳ�ʼ��
  MX_GPIO_Init();		//GPIO��ʼ��
  MX_IWDG_Init();		//���Ź���ʼ��
	Check_IWDG_Reset();		//���������Ź���λ
  while (1)
  {
		if (1 != ubKeyPressed)  //û���ⲿ��λ�������
    {
      LL_IWDG_ReloadCounter(IWDG);		//���¼��ؿ��Ź����¼����������£���������ι��
      
      LL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin); //LD4״̬��ת������˸
      LL_mDelay(200);	//��ʱ
    }
  }
}


void SystemClock_Config(void)
{
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  }

  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_Init1msTick(16000000);
  LL_SetSystemCoreClock(16000000);
}

static void MX_IWDG_Init(void)
{

  LL_IWDG_Enable(IWDG);
  LL_IWDG_EnableWriteAccess(IWDG);
  LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_4);
  LL_IWDG_SetReloadCounter(IWDG, 4078);
  while (LL_IWDG_IsReady(IWDG) != 1)
  {
  }

  LL_IWDG_ReloadCounter(IWDG);
}

static void MX_GPIO_Init(void)
{
  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

  LL_GPIO_ResetOutputPin(LED4_GPIO_Port, LED4_Pin);

  LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTC, LL_EXTI_CONFIG_LINE13);

  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_13;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
  LL_EXTI_Init(&EXTI_InitStruct);

  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin, LL_GPIO_PULL_UP);

  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin, LL_GPIO_MODE_INPUT);

  GPIO_InitStruct.Pin = LED4_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED4_GPIO_Port, &GPIO_InitStruct);

  NVIC_SetPriority(EXTI4_15_IRQn, 0);
  NVIC_EnableIRQ(EXTI4_15_IRQn);

}

void Check_IWDG_Reset(void) 
{
  if (LL_RCC_IsActiveFlag_IWDGRST()) //���RCCʱ�Ӽ�������Ԥ��ĸ�λֵ
  {
    LL_RCC_ClearResetFlags(); //���RCC��λ��־

    LED_On(); //��LD4�Ƶ���
    
    while(ubKeyPressed != 1)  //�ⲿ��λ������־λ��0����û�и�λ������LD4����
    {
    }

    ubKeyPressed = 0;		//����ⲿ��λ������־λ
  }
}

void LED_On(void)
{
  LL_GPIO_SetOutputPin(LED4_GPIO_Port, LED4_Pin);	//����LD4
}

void UserButton_Callback(void)  //�жϻص������������ⲿ�жϸ�λ����
{
  ubKeyPressed = 1;
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif 
