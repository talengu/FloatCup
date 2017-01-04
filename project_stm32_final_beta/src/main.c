/* Includes ------------------------------------------------------------------*/
#include "data.h"

vu16 CCR3_Val = 7500;				   		/* ?????????3?????? */
vu16 CCR4_Val = 7500;	
ErrorStatus HSEStartUpStatus;

void init(void);

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void TIM_Configuration(void);
void ADC_Configuration(void);
void USART_Configuration(void);
void EXIT_Configuration(void);

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void delay(int t)
{
	int tt=7200;			///ʱ���̣�Ϊ100us
	for(;t>0;t--)
	{
		for(tt=60;tt>0;tt--){}
	}
}


int main(void)
{
	axis xynow,xyold;
	init();
	__adc(&xynow);
  while (1)
  {
  	delay(delaytime);
  	xyold.x=xynow.x;
	xyold.y=xynow.y;
  	__adc(&xynow);
	__pid(&xynow,&xyold,&p);

	__pwm(&p);


   // printf("%f,%f\n",xynow.x,xynow.y);

//	printf("%f,%d\n",xynow.x,p.px);

//	printf("%f,%d\n",xynow.x,p.px);

//	printf("%f,%f,%d,%d\n",xynow.x,xynow.y,p.px,p.py);
  }
  return 0;
}

void init(void)
{

	p.px=0;
	p.py=0;
	RCC_Configuration();
	GPIO_Configuration();
	NVIC_Configuration();
	EXIT_Configuration();
	TIM_Configuration();
	ADC_Configuration();
	USART_Configuration();
}
 void RCC_Configuration(void)
{
	/* ???????? HSEStartUpStatus */
	ErrorStatus HSEStartUpStatus;

  	/* ????????*/
  	RCC_DeInit();

  	/* ??HSE*/
  	RCC_HSEConfig(RCC_HSE_ON);

  	/* ??HSE?????*/
  	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	/* ??HSE??????,????if()?? */
  	if(HSEStartUpStatus == SUCCESS)
  	{
    	/* ??HCLK(AHB)????SYSCLK 1?? */
    	RCC_HCLKConfig(RCC_SYSCLK_Div1);

    	/* ??PCLK2???? HCLK(AHB) 1?? */
    	RCC_PCLK2Config(RCC_HCLK_Div1);

    	/* ??PCLK1???? HCLK(AHB) 2?? */
    	RCC_PCLK1Config(RCC_HCLK_Div2);

    	/* ??FLASH??????2 */
    	FLASH_SetLatency(FLASH_Latency_2);
    	/* ??FLASH???? */
    	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    	/* ?????(PLL)????HSE 1??,????9,?PLL????? 8MHz * 9 = 72MHz */
    	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    	/* ??PLL */
    	RCC_PLLCmd(ENABLE);

    	/* ??PLL???? */
    	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    	/* ??SYSCLK????PLL */
    	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    	/* ??PLL??SYSCLK??? */
    	while(RCC_GetSYSCLKSource() != 0x08);
  	}

	/*??APB1????TIM2??*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
  	/* ??APB2????GPIOA??*/
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB |
								 RCC_APB2Periph_AFIO| RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | RCC_APB2Periph_ADC1 , ENABLE);		
}



/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configure the TIM3 Ouput Channels.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
	/*GPIO_InitStructure */
  	GPIO_InitTypeDef GPIO_InitStructure;

  	/* PC.2,PC.3,PC.4,PC.5 �����ж�(EXTI Line0)*/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOC , &GPIO_InitStructure);
	
		/* ���� USART1 ��Tx�ţ�PA.9��Ϊ�ڶ���������������� */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA , &GPIO_InitStructure);

	 
  	/* ���� USART1 ��Rx�ţ�PA.10��Ϊ��������� */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOA , &GPIO_InitStructure);

	  	/* �� PC.0 ����Ϊģ������� */
	//x��
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  	GPIO_Init(GPIOC , &GPIO_InitStructure);

	/* �� PC.1 ����Ϊģ������� */
	//y��
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  	GPIO_Init(GPIOC , &GPIO_InitStructure);

	/* PC.2(EXIT2) */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC , GPIO_PinSource2);
	/* PC.3(EXIT3) */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC , GPIO_PinSource3);
	/* PC.4(EXIT4) */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC , GPIO_PinSource4);
	/* PC.4(EXIT4) */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC , GPIO_PinSource5);

//	/* PWM */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOA,GPIO_Pin_6);	//
//    GPIO_SetBits(GPIOA,GPIO_Pin_7);	//
//	/* PWMʹ��*/
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);	//
	GPIO_SetBits(GPIOB,GPIO_Pin_1);	//
//
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_6;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_ResetBits(GPIOA,GPIO_Pin_6);	//
//	GPIO_SetBits(GPIOA,GPIO_Pin_7);	//

}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*�����ж�����
*******************************************************************************/

void NVIC_Configuration(void)
{
	/* ����NVIC��ʼ���ṹ�� NVIC_InitStructure */
	NVIC_InitTypeDef NVIC_InitStructure;

	/* #ifdef...#else...#endif�ṹ�������Ǹ���Ԥ�������������ж���������ʼ��ַ*/
#ifdef  VECT_TAB_RAM
  	/* �ж���������ʼ��ַ�� 0x20000000 ��ʼ */
 	NVIC_SetVectorTable(NVIC_VectTab_RAM , 0x0);
#else 	/* VECT_TAB_FLASH */
  	/* �ж���������ʼ��ַ�� 0x80000000 ��ʼ */
  	NVIC_SetVectorTable(NVIC_VectTab_FLASH , 0x0);
#endif

  	/* ѡ��NVIC���ȼ�����2 */
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  	/* ʹ��EXIT 2ͨ�� ��2����ռ���ȼ� ��0����ռ���ȼ� */
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQChannel;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

	/* ʹ��EXIT 3ͨ�� ��1����ռ���ȼ� ��0����ռ���ȼ� */
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQChannel;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

	/* ʹ��EXIT 4ͨ�� ��0����ռ���ȼ� ��0����ռ���ȼ� */
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQChannel;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

	/* ʹ��EXIT 5ͨ�� ��3����ռ���ȼ� ��0����ռ���ȼ� */
  	NVIC_InitStructure.NVIC_IRQChannel =EXTI9_5_IRQChannel;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
}

//�����ж�����
void EXIT_Configuration(void)
{
	/* ����EXIT��ʼ���ṹ�� EXTI_InitStructure */
	EXTI_InitTypeDef EXTI_InitStructure;

	/* �����ⲿ�ж�0ͨ����EXIT Line0�����½���ʱ�����ж� */
  	EXTI_InitStructure.EXTI_Line = EXTI_Line2 | EXTI_Line3 | EXTI_Line4 | EXTI_Line5;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//˫���ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  	EXTI_Init(&EXTI_InitStructure);
}


void TIM_Configuration(void)
{
  /* -----------------------------------------------------------------------
    TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
    TIM3CLK = 36 MHz, Prescaler = 0x0, TIM3 counter clock = 36 MHz
    TIM3 ARR Register = 999 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
    TIM3 Frequency = 36 KHz.
    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
    TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
    TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%
  ----------------------------------------------------------------------- */
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = TIM_PERIOD;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;


  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;

  TIM_OC3Init(TIM3, &TIM_OCInitStructure);

  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;

  TIM_OC4Init(TIM3, &TIM_OCInitStructure);

  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM3, ENABLE);

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);
}


/*******************************************************************************
* ������  		: ADC_Configuration
* ��������    	: ��ʼ��������ADCת��
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/

void ADC_Configuration(void)
{
	/* ���� ADC ��ʼ���ṹ�� ADC_InitStructure */
	ADC_InitTypeDef ADC_InitStructure;
	
	/* ����ADCʱ�ӷ�Ƶ */
	RCC_ADCCLKConfig(RCC_PCLK2_Div4);

	/*	
	*	��������ģʽ;
	*	ģ��ת�������ڵ�ͨ��ģʽ;
	*	ģ��ת�������ڵ���ת��ģʽ;
	*	ת��������ʽ��ת���������������;
	*	ADC �����Ҷ��� ;
	*	���й���ת���� ADC ͨ������ĿΪ1; 
	*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	

	/* ʹ�� ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	/* ��λ ADC1 ��У׼�Ĵ��� */   
	ADC_ResetCalibration(ADC1);
	/* �ȴ� ADC1 У׼�Ĵ�����λ��� */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* ��ʼ ADC1 У׼ */
	ADC_StartCalibration(ADC1);
	/* �ȴ� ADC1 У׼��� */
	while(ADC_GetCalibrationStatus(ADC1));
	/* ���� ADC1 ת�� */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	
}


/*******************************************************************************
* ������  		: USART_Configuration
* ��������    	: ����USART1
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/

void USART_Configuration(void)
{
	/* ���� USART ��ʼ���ṹ�� USART_InitStructure */
  	USART_InitTypeDef USART_InitStructure;

	/*	������Ϊ9600bps;
	*	8λ���ݳ���;
	*	1��ֹͣλ����У��;
	*	����Ӳ��������;
	*	��ֹUSARTʱ��;
	*	ʱ�Ӽ��Ե�;
	*	�ڵ�2�����ز�������
	*	���һλ���ݵ�ʱ�����岻�� SCLK ����� 
	*/

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl =  USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1 , &USART_InitStructure);
    
  	/* ʹ�� USART1 */
  	USART_Cmd(USART1 , ENABLE);
}


 
/*******************************************************************************
* ������  		: fputc
* ��������    	: ��printf�����ض�λ��USATR1
* �������    	: ��
* ������    	: ��
* ����ֵ		: ��
*******************************************************************************/

int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (u8) ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	return ch;
}

