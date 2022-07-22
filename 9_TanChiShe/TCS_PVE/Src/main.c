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

#define u8  unsigned char
#define u16 unsigned int

int last_num[32][16];

void delay(void);
void lcdwritecmd(unsigned char cmd);
void lcdwritedata(unsigned char dat);
void lcdinit(void);

void clear(void);
void drowpoint(char x,char y,char z);
void lcdsetcursor(unsigned char x, unsigned char y);
void lcdshowstr(unsigned char x, unsigned char y, unsigned char *str);


const int flg[4][2]={{1,0},{0,1},{0,-1},{-1,0}};//������������仯���� 
int n=127,m=63;  //n��m���ߵĻ��Χ28��76��+�߿�=���ӻ����ڴ�С
struct xcw{int x,y;}tang[5][15],foodd[2];//�ṹ��tan�洢ÿ���ߵ�����  �ṹ��foodd��¼����ʳ������ 
int tot[5];     //tot[0123]�������ߵĳ��� 
int f[5];       //�洢�ߵ�����״̬0123��������   ����flgʹ�� 
int score;      //�÷� 
int food;       //ʳ�����
int ms=0;       //��ʼ--ģʽѡ��
int ans_len=1e9; 
int vis[128][64];//��¼ʳ����ڵ�ľ���  ����(foddd[0].x,foddd[0].y)��ʳ����vis[foddd[0].x][foddd[0].y]=1 

//int  choose();     //ģʽѡ���жϴ���
int  mods();         //��ӦģʽС�߳�ʼ��
void draw();         //��ʾ�߿�
void rand_food();    //û��ʳ��ʱ���Ͷι
int  change(int t);  //û�а��������ߵ�����
int check(int x,int y);         //����ĳһ����
void machine(int t,int x,int y); //������Ѱ· 
void result(int t);              //���ֹ���-ʤ��
void fnd(int t,int x,int y);     //ײǽײ��--ʧ��
int  ads(int x);                 //ȡ����ֵ
int  other(int x);               //03������12����
int get_food(int x,int y,int xxx,int yyy); //������������Ѱʳ��

//void Ending();                   //����

/* Private includes ----------------------------------------------------------*/
ITStatus EXTI_GetITStatus(uint32_t EXTI_Line) //�ж��жϹܽ�
{
    ITStatus bitstatus = RESET; //��ʼλ״̬0
    uint32_t enablestatus = 0;  //��ʼʹ��״̬0
    /* Check the parameters */
    assert_param(IS_GET_EXTI_LINE(EXTI_Line));
    enablestatus =  EXTI->IMR1 & EXTI_Line;  
    if (((EXTI->FPR1 & EXTI_Line) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET)) 
		      bitstatus = SET;
    else
        bitstatus = RESET;
    return bitstatus;
}

void EXTI_ClearITPendingBit(uint32_t EXTI_Line) //�ͷŶ�Ӧ�ܽű�־λ
{
  /* Check the parameters */
  assert_param(IS_EXTI_LINE(EXTI_Line));
  EXTI->FPR1 = EXTI_Line;
}


void SystemClock_Config(void);
static void MX_GPIO_Init(void);


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  lcdinit();
	HAL_Delay(100);
	
  while (1)
  {
		while(ms==0)
		{
			lcdshowstr(1,1,"Select Mode");
			HAL_Delay(100);
		}
		
		while(ms==1)  //1����ģʽ
		{
			rand_food();
			if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4)==0&&f[0]^0)       //��������
				f[0]=3;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5)==0&&f[0]^3)
				f[0]=0;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_6)==0&&f[0]^1)
				f[0]=2;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_8)==0&&f[0]^2)
				f[0]=1;
			else ;
			change(0);
		}
		
		while(ms==2)	//2˫�˺���
		{
			rand_food();
			if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_0)==0&&f[0]^0) // ��
				f[0]=3;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_1)==0&&f[0]^3) //��
				f[0]=0;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)==0&&f[0]^1) //��
				f[0]=2;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)==0&&f[0]^2) //��
				f[0]=1;
			else ;
			if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4)==0&&f[1]^0) // ��
				f[1]=3;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5)==0&&f[1]^3) //��
				f[1]=0;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_6)==0&&f[1]^1) //��
				f[1]=2;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_8)==0&&f[1]^2) //��
				f[1]=1;
			else ;
			change(0),change(1);
		}
		
		while(ms==3)	//3˫�˶Կ�
		{
			rand_food();
			if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_0)==0&&f[0]^0) // ��
				f[0]=3;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_1)==0&&f[0]^3) //��
				f[0]=0;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)==0&&f[0]^1) //��
				f[0]=2;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)==0&&f[0]^2) //��
				f[0]=1;
			else ;
			if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4)==0&&f[1]^0) // ��
				f[1]=3;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5)==0&&f[1]^3) //��
				f[1]=0;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_6)==0&&f[1]^1) //��
				f[1]=2;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_8)==0&&f[1]^2) //��
				f[1]=1;
			else ;
			change(0),change(1);
		}
		
		while(ms==4)	//4�����Կ�����
		{
			rand_food();
			change(2),change(3);
		}
		
		while(ms==5)	//5�˻��Կ�����
		{
			rand_food();
			if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4)==0&&f[0]^0) // ��
				f[0]=3;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5)==0&&f[0]^3) //��
				f[0]=0;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_6)==0&&f[0]^1) //��
				f[0]=2;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_8)==0&&f[0]^2) //��
				f[0]=1;
			else ;
			change(0),change(2);
		}
		
		while(ms==6)	//6˫�˶Կ�����
		{
			rand_food();
			if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_0)==0&&f[0]^0) // ��
				f[0]=3;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_1)==0&&f[0]^3) //��
				f[0]=0;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)==0&&f[0]^1) //��
				f[0]=2;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)==0&&f[0]^2) //��
				f[0]=1;
			else ;
			if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4)==0&&f[1]^0) // ��
				f[1]=3;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5)==0&&f[1]^3) //��
				f[1]=0;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_6)==0&&f[1]^1) //��
				f[1]=2;
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_8)==0&&f[1]^2) //��
				f[1]=1;
			else ;
			change(0),change(1);
		}
  }
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
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC11 PC6 PC7 PC8
                           PC9 PC10 */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
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

  /*Configure GPIO pin : PC4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB3
                           PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PD8 PD0 PD1 PD2
                           PD3 PD4 PD5 PD6 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2
                          |GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
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
	delay();          //           ʹ�� д  ָ��
	GPIOB->ODR=0x18;  //RST=1\PSB=1\E=0\RW=0\RS=0
	GPIOA->ODR=cmd;
	GPIOB->ODR=0x1C;	//RST=1\PSB=1\E=1\RW=0\RS=0
	delay();
	GPIOB->ODR=0x18;	//RST=1\PSB=1\E=0\RW=0\RS=0
}

void lcdwritedata(unsigned char dat)
{
  delay();          //           ʹ��  д  ����
	GPIOB->ODR=0x19;	//RST=1\PSB=1\E=0\RW=0\RS=1
  GPIOA->ODR=dat;
	GPIOB->ODR=0x1D;	//RST=1\PSB=1\E=1\RW=0\RS=1
	delay();
	GPIOB->ODR=0x19;	//RST=1\PSB=1\E=0\RW=0\RS=1
}

void lcdinit()
{
	lcdwritecmd(0x34); //E ʹ��   RS1����0ָ��  RW1��0д
	lcdwritecmd(0x30); 
	lcdwritecmd(0x0C); 
	lcdwritecmd(0x01);
}

void clear(void)
{
	for(int i=0;i<=32;i++)  // 64��  ����һ��һ�����������
		for(int j=0;j<=18;j++) // 18����
	{
			lcdwritecmd(0x34);        //����չָ�
			lcdwritecmd(0x80 + i - 1);
			lcdwritecmd(0x80 + j);	
			lcdwritedata(0x00);
			lcdwritedata(0x00);
		  lcdwritecmd(0x36);        //�򿪻�ͼ��ʾ
			lcdwritecmd(0x30);        //�ص�����ָ�
	}
}

void drowpoint(char x,char y,char z)  //�������
{
  char i = 0;
	char j = 0;
	int num = 0x8000;         //16λ�����λ����
	lcdwritecmd(0x34);        //����չָ�
	if( y <= 32 )
	{
		i += y;     // i=y    -1      i�ķ�Χ32����
		j = (x - 1) / 16;  //����    j�ķ�Χ16����
		x = ( x%16>0? (x%16):16 ) - 1 ;
		num = num >> x;
	}
	else
	{
		i = y - 32;	 //i=y-32  -1
		j = ( (x - 1) / 16 ) + 8;
		x = ( x%16>0? (x%16):16 ) - 1 ;
		num = num >> x;
	}
	if(z>0)
 	  num = last_num[i][j] | num;
	else
	{
		num=~num;
		num=last_num[i][j] & num;
	}
	lcdwritecmd(0x80 + i - 1);
	lcdwritecmd(0x80 + j);	
	lcdwritedata(num / 256);
	lcdwritedata(num % 256);
  lcdwritecmd(0x36);        //�򿪻�ͼ��ʾ
	lcdwritecmd(0x30);        //�ص�����ָ�
	
	last_num[i][j] = num;
}

void lcdsetcursor(unsigned char x, unsigned char y) //�ַ���λ
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

void lcdshowstr(unsigned char x, unsigned char y, unsigned char *str) //�ַ����
{                       
	lcdsetcursor(x,y);
	while((*str)!='\0')  	 
	{
		 lcdwritedata(*str);
  		str++;            
	}
}

void EXTI4_15_IRQHandler(void)   // �жϿ���ģʽ
{
	if (EXTI_GetITStatus(0x2000) != 0x00)  // 0x2000==GPIO_PIN_13
  {
		EXTI_ClearITPendingBit(0x2000);  //Clear interrupt flag bit
		HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_4);
  }
	else
	{
		lcdshowstr(1,1,"           ");
		clear();
		score=0;
		food=0;
		draw();
	}
	
  if (EXTI_GetITStatus(0x0040) != 0x00)  // 0x0040==EXTI_PIN_6
  {
		EXTI_ClearITPendingBit(0x0040); 
		ms=1;
  }
	else if (EXTI_GetITStatus(0x0080) != 0x00)  // 0x0080==EXTI_PIN_7
  {
		EXTI_ClearITPendingBit(0x0080); 
		ms=2;
  }
	else if (EXTI_GetITStatus(0x0100) != 0x00)  // 0x0100==EXTI_PIN_8
  {
		EXTI_ClearITPendingBit(0x0100); 
		ms=3;
  }
	else if (EXTI_GetITStatus(0x0200) != 0x00)  // 0x0200==EXTI_PIN_9
  {
		EXTI_ClearITPendingBit(0x0200); 
		ms=4;
  }
	else if (EXTI_GetITStatus(0x0400) != 0x00)  // 0x0400==EXTI_PIN_10
  {
		EXTI_ClearITPendingBit(0x0400); 
		ms=5;
  }
	else if (EXTI_GetITStatus(0x0800) != 0x00)  // 0x0800==EXTI_PIN_11
  {
		EXTI_ClearITPendingBit(0x0800); 
		ms=6;
  }
	else ;
	
	if (EXTI_GetITStatus(0x2000) == 0x00)
		mods();    //��ӦģʽС�߳�ʼ��
}
/* USER CODE END 4 */



int mods()  //��ӦģʽС�߳�ʼ�� 
{
	if(ms!=4)											                                   
	{                                        		                            		                            		                     
		tang[0][1].x=2;							
		tang[0][1].y=5;
		tang[0][2].x=2;
		tang[0][2].y=4;
		tang[0][tot[0]=3].x=2;
		tang[0][tot[0]=3].y=3;
		f[0]=1;   //���1��ʼ����  ���� 
		for(int i=1;i<=tot[0];i++)
			drowpoint(tang[0][i].x,tang[0][i].y,1);
	}
	if(ms==2||ms==3||ms==6)
	{
		tang[1][1].x=n;
		tang[1][1].y=m-3;
		tang[1][2].x=n;
		tang[1][1].y=m-2;
		tang[1][tot[1]=3].x=n;
		tang[1][tot[1]=3].y=m-1;
		f[1]=2;    //����1��ʼ����  ����
		for(int i=1;i<=tot[1];i++)
			drowpoint(tang[1][i].x,tang[1][i].y,1);
	}
	if(ms==5||ms==4)
	{											//����1 
		tang[2][1].x=n;
		tang[2][1].y=m-3;
		tang[2][2].x=n;
		tang[2][1].y=m-2;
		tang[2][tot[2]=3].x=n;
		tang[2][tot[2]=3].y=m-1;
		f[2]=2;    //����1��ʼ����  ����
		for(int i=1;i<=tot[2];i++)
			drowpoint(tang[2][i].x,tang[2][i].y,1);
	}
	if(ms==4)
	{
		tang[3][1].x=2;
		tang[3][1].y=5;
		tang[3][2].x=2;
		tang[3][2].y=4;
		tang[3][tot[3]=3].x=2;
		tang[3][tot[3]=3].y=3;
		f[3]=1;   //����2��ʼ����  ���� 
		for(int i=1;i<=tot[3];i++)
			drowpoint(tang[3][i].x,tang[3][i].y,1);
	}
}


void draw()  //��ʾ�߿� 
{
	for(int i=1;i<=n+1;i++)
	{
		drowpoint(i,1,1); //****
		drowpoint(i,m+1,1);
	}
	for(int i=1;i<=m+1;i++) 
	{
		drowpoint(1,i,1);
		drowpoint(n+1,i,1);
	}
}

void rand_food()  //û��ʳ��ʱ���Ͷι
{
    if(!vis[foodd[0].x][foodd[0].y]) //���0��ʳ�ﲻ���� 
	{	
		int x=rand()%(n+1),y=(rand()%m/2)*2; //�������xy����yΪż�� 
		while(!check(x,y)) 
			x=rand()%(n+1),y=(rand()%m/2)*2; //�������Ͷι�ĵ㣬�Ƿ���Ҫ����Ͷι 
		drowpoint(x,y,1);
	    food++;
		vis[x][y]=1;
		foodd[0].x=x;
		foodd[0].y=y;
	}
	if(!vis[foodd[1].x][foodd[1].y])  //�ڶ���ʳ��--ͬ�� 
	{
		int x=rand()%n,y=rand()%m;
		while(!check(x,y)) 
			x=rand()%n,y=(rand()%m);
		drowpoint(x,y,1);
		food++;
		vis[x][y]=1;
		foodd[1].x=x;
		foodd[1].y=y;
	}
}

int change(int t)  //û�а��������ߵ�����--��t���� 
{
	int x=tang[t][1].x,y=tang[t][1].y;    //x��y����ͷλ�õ�xy 
	if(t==2||t==3)                      //2�ź�3�����ǵ���
		machine(t,x,y);                 //������Ѱ·
	x+=flg[f[t]][0],y+=flg[f[t]][1];    //��ͷ(x,y)�����ɱ仯 
	for(int i=tot[t];i;i--)             //�ߵ�����λ�����ݿ�ˢ�� 
	   {
	        tang[t][i+1]=tang[t][i];  
	        drowpoint(tang[t][i].x,tang[t][i].y,1); //������ʾˢ��
		} 
	drowpoint(tang[t][tot[t]+1].x,tang[t][tot[t]+1].y,0); //ԭ������β��ʧ 
	if(vis[x][y])  //����Ե�һ��ʳ��Ļ� 
	{
		vis[x][y]=0,score+=(t==0||t==1),food--;
		if(++tot[t]>=10)  //��һ���Թ��� 
			result(t);    
		drowpoint(tang[t][tot[t]].x,tang[t][tot[t]].y,1); //β�ʹ�+1 
	}
	fnd(t,x,y);   //һ��ײǽײ����ʧ��   
	tang[t][1].x=x;    //����ͷλ�÷�����ȫ�ֱ����� 
	tang[t][1].y=y;
	drowpoint(tang[t][1].x,tang[t][1].y,1);  //��ʾ��ͷ 
} 

int check(int x,int y)         //����ĳһ����
{
	if(x<1||x>n||y<2||y>m)  //�߽��ⷵ��0 
		return 0;  
	int t=1;
	if(ms!=4)
	    for(int i=1;i<=tot[0];i++)   //�������Ϸ���0 
	        if(x==tang[0][i].x&&y==tang[0][i].y)
			    {t=0;break;}
	if(ms==2||ms==3||ms==6)
	    for(int i=1;i<=tot[1];i++)
	        if(x==tang[1][i].x&&y==tang[1][i].y)
			    {t=0;break;}
	if(ms==5||ms==4)
		for(int i=1;i<=tot[2];i++)
			if(x==tang[2][i].x&&y==tang[2][i].y)
					{t=0;break;}
	if(ms==4)
		for(int i=1;i<=tot[3];i++)
			if(x==tang[3][i].x&&y==tang[3][i].y)
					{t=0;break;}
	return t;    //��������Щ�㷵��1 
}

void machine(int t,int x,int y)  //������Ѱ·
{ 
	int tt=0;
	int foodid,minn=1e9,newf=f[t];
	if((get_food(x,y,foodd[0].x,foodd[0].y)<=get_food(x,y,foodd[1].x,foodd[1].y)&&vis[foodd[0].x][foodd[0].y])||!vis[foodd[1].x][foodd[1].y]) 
		foodid=0;
	else 
		foodid=1;
	for(int i=0;i<4;i++)
		if(f[t]^other(i))
		{
			if(check(x+flg[i][0],y+flg[i][1]))
			{	
				int now=get_food(x+flg[i][0],y+flg[i][1],foodd[foodid].x,foodd[foodid].y);
				if(now<minn) 
					newf=i,tt=1,minn=now;
				if(!tt) 
					newf=i;
			}
		}
	f[t]=newf;
}

void result(int t)  //���ֹ���--ʤ�� 
{
	if(ms==5)           //�˻��Կ�-����
		if(t==0)
		    lcdshowstr(1,1,"Win!!!"); 
		else
		    lcdshowstr(1,1,"Lose!!!");
	else if(ms==4)      //�����Կ�-����
	  if(t==2)
		    lcdshowstr(1,1,"Robot1 Wine    (Enough)");
		else
		    lcdshowstr(1,1,"Robot2 Wine    (Enough)");
	else if(ms==6)      //�Կ�ģʽ-����
	    if(t==1) 
		    lcdshowstr(1,1,"Player1 Wine   (Enough)");
		  else 
		    lcdshowstr(1,1,"Player2 Wine   (Enough)");
	else ;
	while(1); //����
}

void fnd(int t,int x,int y)  //ײǽ
{
	if(check(x,y)) return;   //check(x,y)Ϊ0����ײ 
	if(ms==4)        //�����Կ�-���� 
	    if(t==3)
				lcdshowstr(1,1,"Robot2 Die");
	    else
	      lcdshowstr(1,1,"Robot1 Die");
	else if(ms==3||ms==6)   //�Կ�ģʽ  �Կ�ģʽ-���� 
	    if(t==1) 
		    lcdshowstr(1,1,"Player2 Die");
		else 
		    lcdshowstr(1,1,"Player1 Die");
	else
	{
		if(t==2) 
		    lcdshowstr(1,1,"Win!!!");
		else 
		    lcdshowstr(1,1,"Lose!!!");
		//printf("\n ��ҷ���:%d\n",score);
		score=0;
	}
	while(1);  //���� 
}

int ads(int x)  //ȡ����ֵ 
{
    return x<0?-x:x; //x�Ƿ�<0,���x<0�Ļ���ִ��:ǰ�����䣬���x��С��0�Ļ���ִ��:�������� 
}

int other(int x)  //03������12���� 
{
	if     (x==0) return 3;
	else if(x==1) return 2;
	else if(x==2) return 1;
	else          return 0;
}

int get_food(int x,int y,int xxx,int yyy)  //������������Ѱʳ�� 
{
    return ads(x-xxx)+ads(y-yyy);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
