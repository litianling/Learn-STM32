
#include "main.h"

#include "stdio.h"
#include "string.h"


#define unit8_t unsigned char
#define PUTCHAR_PROTOTYPE int fputc(int ch,FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#define BACKSPACE_PROTOTYPE int _backspace(FILE *f)

char a[50][50];
volatile int  PSWD;
volatile int  GK,p=1;    //GK�ؿ�ѡ����� p��ѡ�ؿ����� 
volatile int  Player_Coordinates_Y,Player_Coordinates_X; //��¼S����ʼλ��  ǰ�к���          
volatile int  Box1_Destination_Y,Box1_Destination_X;     //��¼����Ŀ�ĵ�1
volatile int  Box2_Destination_Y,Box2_Destination_X;     //��¼����Ŀ�ĵ�2
volatile int  Box3_Destination_Y,Box3_Destination_X;     //��¼����Ŀ�ĵ�3
volatile int  Box4_Destination_Y,Box4_Destination_X;     //��¼����Ŀ�ĵ�4

UART_HandleTypeDef hlpuart1;


void level_pushbox(int k);   //�ؿ���ͼ��ʼ�� 
void input_pushbox(int ch);  //���밴���ж��߼�
void supplement_pushbox();    //Ŀ��㲹�亯�� 
void success_pushbox();       //ͨ��֮������ 
void output_pushbox();        //��������ʾ������� 

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_LPUART1_UART_Init(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_LPUART1_UART_Init();

	
	printf("\r\n ����������鿴ͨ�ع���: \r\n");
	scanf("%d",&PSWD);
	HAL_Delay(100);
	printf("\r\n Receive PSWD=%d",PSWD);			
  if(PSWD==19991015)   //�ж������Ƿ���ȷ  
	{
		level_pushbox(0);
		output_pushbox();
	}
	else
		printf("\r\n ������� \r\n");
	
	
	printf("\r\n ����������������йؿ�: \r\n");
	scanf("%d",&PSWD);
	HAL_Delay(100);
	printf("\r\n Receive PSWD=%d",PSWD);		
  if(PSWD==19991015)   //�ж������Ƿ���ȷ  
	{
		p=5;
		printf("\r\n ������ȷ�����йؿ��ѽ��������� \r\n");
	}
	else
		printf("\r\n ������󣬽���ʧ�ܡ� \r\n");
	
	
	printf("\r\n ����������Ԥ��ͨ�ؽ��: \r\n");
	scanf("%d",&PSWD);
	HAL_Delay(100);
	printf("\r\n Receive PSWD=%d",PSWD);		
  if(PSWD==19991015)   //�ж������Ƿ���ȷ  
	  success_pushbox();
	else
		printf("\r\n ������� \r\n");
	
	do
	{
		printf("��ѡ��ؿ�(1-5): \r\n");
		scanf("%d",&GK);
		HAL_Delay(50);
		printf("\r\n");
		HAL_Delay(50);
		if (GK<1||GK>5) 
	    	printf("�ؿ�ѡ����� \r\n");
		else if(GK>p) 
	    	printf("��ֹ���� \r\n");
	}while(GK<1||GK>5||GK>p);
	
	
   while(GK>=1&&GK<=5) //��Ϸִ��
    {
	    level_pushbox(GK);  //�ؿ���ʼ������ 
      output_pushbox();  //��ʼ״̬��ʾ 
	    while(1)  //����ģ�� 
	    {
		    int ch;
				printf("��ִ�в�����");
				scanf("%d",&ch);
		    input_pushbox(ch);     //���밴���ж��߼� 
		    supplement_pushbox();  //Ŀ��㲹�亯�� 
		    output_pushbox(); //״̬��ʾˢ�� 
		    if(a[Box1_Destination_Y][Box1_Destination_X]=='@'&&a[Box2_Destination_Y][Box2_Destination_X]=='@'  //��ʤ����
		     &&a[Box3_Destination_Y][Box3_Destination_X]=='@'&&a[Box4_Destination_Y][Box4_Destination_X]=='@')
		    {
		    	GK++;
		    	if(GK>=p) p=GK; //ѡ������----������ 
          HAL_Delay(150);
          break; //ȷ�ϻ�ʤ������while(1)ѭ�� 
		    }
	    }
    }
		
    success_pushbox();//ͨ��֮������ 
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
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 115200;
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
}

static void MX_GPIO_Init(void)
{
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
}

/* USER CODE BEGIN 4 */


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

 
 
/* USER CODE END 4 */

void level_pushbox(int GK)  //�ؿ���ͼ��ʼ��
{
	switch(GK)
	{
		case 1:
			{
				strcpy(a[0],"   ###                                           ");
				strcpy(a[1],"   #*#                                           ");
				strcpy(a[2],"   # #                                           ");
				strcpy(a[3],"####O######                                      ");
				strcpy(a[4],"#*  OM O *#                                      ");
				strcpy(a[5],"#####O#####                                      ");
				strcpy(a[6],"    # #                                          ");
				strcpy(a[7],"    #*#                                          ");
				strcpy(a[8],"    ###                                          ");
				strcpy(a[9],"                                                 ");
				strcpy(a[10],"�ؿ� 1/5                                        ");
				strcpy(a[11],"                                                ");
				strcpy(a[12],"                                                ");
				strcpy(a[13],"                                                ");
				strcpy(a[14],"                                                ");
	            Player_Coordinates_Y=4,Player_Coordinates_X=5;         
	            Box1_Destination_Y=1,Box1_Destination_X=4;
	            Box2_Destination_Y=4,Box2_Destination_X=1;
	            Box3_Destination_Y=4,Box3_Destination_X=9;
	            Box4_Destination_Y=7,Box4_Destination_X=5;
			}
		break;
		case 2:
			{
				strcpy(a[0],"######                                           ");
				strcpy(a[1],"#*   #                                           ");
				strcpy(a[2],"###  #                                           ");
				strcpy(a[3],"#  O ######                                      ");
				strcpy(a[4],"#*  OM O *#                                      ");
				strcpy(a[5],"#####O#####                                      ");
				strcpy(a[6],"    # #                                          ");
				strcpy(a[7],"    #*#                                          ");
				strcpy(a[8],"    ###                                          ");
				strcpy(a[9],"                                                 ");
				strcpy(a[10],"�ؿ� 2/5                                        ");
				strcpy(a[11],"                                                ");
				strcpy(a[12],"                                                ");
				strcpy(a[13],"                                                ");
				strcpy(a[14],"                                                ");
	            Player_Coordinates_Y=4,Player_Coordinates_X=5;         
	            Box1_Destination_Y=1,Box1_Destination_X=1;
	            Box2_Destination_Y=4,Box2_Destination_X=1;
	            Box3_Destination_Y=4,Box3_Destination_X=9;
	            Box4_Destination_Y=7,Box4_Destination_X=5;
			}
		break;
		case 3:
			{
				strcpy(a[0],"  ####                                           ");
				strcpy(a[1],"  #  #                                           ");
				strcpy(a[2],"  #  #                                           ");
				strcpy(a[3],"  #M #                                           ");
				strcpy(a[4],"### ######                                       ");
				strcpy(a[5],"#   O  O*#                                       ");
				strcpy(a[6],"# O*   ###                                       ");
				strcpy(a[7],"#####* O*#                                       ");
				strcpy(a[8],"    ######                                       ");
				strcpy(a[9],"                                                 ");
				strcpy(a[10],"�ؿ� 3/5                                        ");
				strcpy(a[11],"                                                ");
				strcpy(a[12],"                                                ");
				strcpy(a[13],"                                                ");
				strcpy(a[14],"                                                ");
	            Player_Coordinates_Y=3,Player_Coordinates_X=3;         
	            Box1_Destination_Y=6,Box1_Destination_X=3;
	            Box2_Destination_Y=5,Box2_Destination_X=8;
	            Box3_Destination_Y=7,Box3_Destination_X=5;
	            Box4_Destination_Y=7,Box4_Destination_X=8;
			}
		break;
		case 4:
			{
				strcpy(a[0]," ########                                        ");
				strcpy(a[1]," #     ###                                       ");
				strcpy(a[2],"##O###   #                                       ");
				strcpy(a[3],"#M  O  O #                                       ");
				strcpy(a[4],"# **# O ##                                       ");
				strcpy(a[5],"##**#   #                                        ");
				strcpy(a[6]," ########                                        ");
				strcpy(a[7],"                                                 ");
				strcpy(a[8],"                                                 ");
				strcpy(a[9],"                                                 ");
				strcpy(a[10],"�ؿ� 4/5                                        ");
				strcpy(a[11],"                                                ");
				strcpy(a[12],"                                                ");
				strcpy(a[13],"                                                ");
				strcpy(a[14],"                                                ");
	            Player_Coordinates_Y=3,Player_Coordinates_X=1;         
	            Box1_Destination_Y=4,Box1_Destination_X=2;
	            Box2_Destination_Y=4,Box2_Destination_X=3;
	            Box3_Destination_Y=5,Box3_Destination_X=2;
	            Box4_Destination_Y=5,Box4_Destination_X=3;
			}
		break;
		case 5:
			{
				strcpy(a[0],"  ####                                           ");
				strcpy(a[1],"  #**#                                           ");
				strcpy(a[2]," ## *##                                          ");
				strcpy(a[3]," #  O*#                                          ");
				strcpy(a[4],"## O  ##                                         ");
				strcpy(a[5],"#  #OO #                                         ");
				strcpy(a[6],"#  M   #                                         ");
				strcpy(a[7],"########                                         ");
				strcpy(a[8],"                                                 ");
				strcpy(a[9],"                                                 ");
				strcpy(a[10],"�ؿ� 5/5                                        ");
				strcpy(a[11],"                                                ");
				strcpy(a[12],"                                                ");
				strcpy(a[13],"                                                ");
				strcpy(a[14],"                                                ");
	            Player_Coordinates_Y=6,Player_Coordinates_X=3;         
	            Box1_Destination_Y=1,Box1_Destination_X=3;
	            Box2_Destination_Y=1,Box2_Destination_X=4;
	            Box3_Destination_Y=2,Box3_Destination_X=4;
	            Box4_Destination_Y=3,Box4_Destination_X=5;
			}
		break;	
		case 0:
			{                                                                      
		    		strcpy(a[0],"             ͨ�ع���                           ");   
	    			strcpy(a[1],"�ؿ� 1/5 :                                       ");   
		    		strcpy(a[2],"  SS WW DDD AAAAAA DD WW                         "); 
    				strcpy(a[3],"�ؿ� 2/5 :                                       "); 
	    			strcpy(a[4],"  SS WW DDD AAAAAA DWW DWAA                      ");  
		    		strcpy(a[5],"�ؿ� 3/5 :                                       ");
			    	strcpy(a[6],"  SSSDDDWDA SSDAWWAASAW AASDDDWDS AAWWWD WWASSSS ");
    				strcpy(a[7],"�ؿ� 4/5 :                                       ");
	    			strcpy(a[8],"  DDDD SSDDWWA WWAAAA SSASDWDS WAWWDDDD SSAAADDD ");
		    		strcpy(a[9],"  WWAAAA SSASD WDDDDD SASAW DWAAA DDDWW AAAA SSS ");
			    	strcpy(a[10],"  WDDDDDD WAS AAAAA WWDDDD SDS AAAA DDDWW AAAASS");
    				strcpy(a[11],"�ؿ� 5/5 :                                      ");
	    			strcpy(a[12],"  AWW DDWW SSAASS DDWWW SSSDD WASAAA WWWDDD SAAA");
		    		strcpy(a[13],"  SSDD WW DWA SAWW SSASS DDDWW                  ");
			    	strcpy(a[14],"                                                ");
			}
		break;
	}
}


void input_pushbox(int ch)     //���밴���ж��߼�
{
	switch(ch)  //�����ж��߼� 
	{
	    case 8:
		    if(a[Player_Coordinates_Y-1][Player_Coordinates_X]!='#')                 //�ϱ߲���ǽ 
		    {
			    if(a[Player_Coordinates_Y-1][Player_Coordinates_X]!='O'&&a[Player_Coordinates_Y-1][Player_Coordinates_X]!='@')      //�ϱ߲������� 
			    {
				   a[Player_Coordinates_Y][Player_Coordinates_X]=' ';   //��ǰλ��������ʧ 
				   Player_Coordinates_Y--;                              //�������� 
				   a[Player_Coordinates_Y][Player_Coordinates_X]='M';   //������� 
			    }
			    else if(a[Player_Coordinates_Y-2][Player_Coordinates_X]!='#'&&a[Player_Coordinates_Y-2][Player_Coordinates_X]!='@'&&a[Player_Coordinates_Y-2][Player_Coordinates_X]!='O')
			    {                                                                    //�ϱ�������  ���ϲ���ǽҲ�������� 
				   a[Player_Coordinates_Y][Player_Coordinates_X]=' ';   //ͬ 
				   Player_Coordinates_Y--;                              // 
				   a[Player_Coordinates_Y][Player_Coordinates_X]='M';   //�� 
				   a[Player_Coordinates_Y-1][Player_Coordinates_X]='O'; //���ӳ��������Ϸ�  ��������ɹ� 
				   if(((Player_Coordinates_Y-1==Box1_Destination_Y)&&(Player_Coordinates_X==Box1_Destination_X))
				    ||((Player_Coordinates_Y-1==Box2_Destination_Y)&&(Player_Coordinates_X==Box2_Destination_X))
				    ||((Player_Coordinates_Y-1==Box3_Destination_Y)&&(Player_Coordinates_X==Box3_Destination_X))
					||((Player_Coordinates_Y-1==Box4_Destination_Y)&&(Player_Coordinates_X==Box4_Destination_X)))
				       a[Player_Coordinates_Y-1][Player_Coordinates_X]='@';//�ж������Ƿ���Ŀ��� 
			    }
		    }
		break;   
		case 4:
		    if(a[Player_Coordinates_Y][Player_Coordinates_X-1]!='#')
		    {
			    if(a[Player_Coordinates_Y][Player_Coordinates_X-1]!='O'&&a[Player_Coordinates_Y][Player_Coordinates_X-1]!='@')
			    {
				   a[Player_Coordinates_Y][Player_Coordinates_X]=' ';
				   Player_Coordinates_X--;
				   a[Player_Coordinates_Y][Player_Coordinates_X]='M';
			    }
			    else if(a[Player_Coordinates_Y][Player_Coordinates_X-2]!='#'&&a[Player_Coordinates_Y][Player_Coordinates_X-2]!='@'&&a[Player_Coordinates_Y][Player_Coordinates_X-2]!='O')
			    {
				   a[Player_Coordinates_Y][Player_Coordinates_X]=' ';
				   Player_Coordinates_X--;
				   a[Player_Coordinates_Y][Player_Coordinates_X]='M';
				   a[Player_Coordinates_Y][Player_Coordinates_X-1]='O';
			       if(((Player_Coordinates_Y==Box1_Destination_Y)&&(Player_Coordinates_X-1==Box1_Destination_X))
				    ||((Player_Coordinates_Y==Box2_Destination_Y)&&(Player_Coordinates_X-1==Box2_Destination_X))
				    ||((Player_Coordinates_Y==Box3_Destination_Y)&&(Player_Coordinates_X-1==Box3_Destination_X))
					||((Player_Coordinates_Y==Box4_Destination_Y)&&(Player_Coordinates_X-1==Box4_Destination_X)))
				       a[Player_Coordinates_Y][Player_Coordinates_X-1]='@';
			    }
		    }
		break;
		case 2:
		    if(a[Player_Coordinates_Y+1][Player_Coordinates_X]!='#')
		    {
			    if(a[Player_Coordinates_Y+1][Player_Coordinates_X]!='O'&&a[Player_Coordinates_Y+1][Player_Coordinates_X]!='@')
			    {
				   a[Player_Coordinates_Y][Player_Coordinates_X]=' ';
				   Player_Coordinates_Y++;
				   a[Player_Coordinates_Y][Player_Coordinates_X]='M';
			    }
			    else if(a[Player_Coordinates_Y+2][Player_Coordinates_X]!='#'&&a[Player_Coordinates_Y+2][Player_Coordinates_X]!='@'&&a[Player_Coordinates_Y+2][Player_Coordinates_X]!='O')
			    {
				   a[Player_Coordinates_Y][Player_Coordinates_X]=' ';
				   Player_Coordinates_Y++;
				   a[Player_Coordinates_Y][Player_Coordinates_X]='M';
				   a[Player_Coordinates_Y+1][Player_Coordinates_X]='O';
				   if(((Player_Coordinates_Y+1==Box1_Destination_Y)&&(Player_Coordinates_X==Box1_Destination_X))
				    ||((Player_Coordinates_Y+1==Box2_Destination_Y)&&(Player_Coordinates_X==Box2_Destination_X))
				    ||((Player_Coordinates_Y+1==Box3_Destination_Y)&&(Player_Coordinates_X==Box3_Destination_X))
					||((Player_Coordinates_Y+1==Box4_Destination_Y)&&(Player_Coordinates_X==Box4_Destination_X)))
				       a[Player_Coordinates_Y+1][Player_Coordinates_X]='@';
			    }
			}
		break;
    case 6:
		    if(a[Player_Coordinates_Y][Player_Coordinates_X+1]!='#')
		    {
			    if(a[Player_Coordinates_Y][Player_Coordinates_X+1]!='O'&&a[Player_Coordinates_Y][Player_Coordinates_X+1]!='@')
			    {
				   a[Player_Coordinates_Y][Player_Coordinates_X]=' ';
				   Player_Coordinates_X++;
				   a[Player_Coordinates_Y][Player_Coordinates_X]='M';
			    }
			    else if(a[Player_Coordinates_Y][Player_Coordinates_X+2]!='#'&&a[Player_Coordinates_Y][Player_Coordinates_X+2]!='@'&&a[Player_Coordinates_Y][Player_Coordinates_X+2]!='O')
			    {
				   a[Player_Coordinates_Y][Player_Coordinates_X]=' ';
				   Player_Coordinates_X++;
				   a[Player_Coordinates_Y][Player_Coordinates_X]='M';
				   a[Player_Coordinates_Y][Player_Coordinates_X+1]='O';
				   if(((Player_Coordinates_Y==Box1_Destination_Y)&&(Player_Coordinates_X+1==Box1_Destination_X))
				    ||((Player_Coordinates_Y==Box2_Destination_Y)&&(Player_Coordinates_X+1==Box2_Destination_X))
				    ||((Player_Coordinates_Y==Box3_Destination_Y)&&(Player_Coordinates_X+1==Box3_Destination_X))
					||((Player_Coordinates_Y==Box4_Destination_Y)&&(Player_Coordinates_X+1==Box4_Destination_X)))
				       a[Player_Coordinates_Y][Player_Coordinates_X+1]='@';					  
			    }
		    }
		break;
		default: 
			printf("\r\n  Υ����������� \r\n");;   //����������������Ӧ 
		break;
	}
} 


void supplement_pushbox()  //Ŀ��㲹�亯�� 
{
	if(a[Box1_Destination_Y][Box1_Destination_X]==' ')  
	    a[Box1_Destination_Y][Box1_Destination_X]='*'; 
	if(a[Box2_Destination_Y][Box2_Destination_X]==' ')  
	    a[Box2_Destination_Y][Box2_Destination_X]='*';
	if(a[Box3_Destination_Y][Box3_Destination_X]==' ')  
	    a[Box3_Destination_Y][Box3_Destination_X]='*';
	if(a[Box4_Destination_Y][Box4_Destination_X]==' ')  
	    a[Box4_Destination_Y][Box4_Destination_X]='*';
}


void success_pushbox()  //ͨ��֮������
{
    printf("\r\n��������ϲͨ�أ�����\r\n");
    printf("\r\n ������ 19991015 \r\n"); 
    for (float y = 1.3f; y > -1.0f; y -= 0.1f)  //������ͼ 
    {
        for (float x = -1.5f; x < 1.5f; x += 0.05f) 
		{
            float a = x * x + y * y - 1;
            putchar(a * a * a - x * x * y * y * y <= 0.0f ? '*' : ' ');
        }
        printf("\r\n");
    }
} 


void output_pushbox()  //��������ʾ�������
{
	int i;
	int j;
	printf("\r\n");
	for(i=0;i<15;i++)
  {
	  for(j=0;j<50;j++)
		{
			printf("%c",a[i][j]);  
		}
		HAL_Delay(20); //��������ʱ
		printf("\r\n");
  }
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
