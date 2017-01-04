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
	int tt=7200;			///时间变短，为100us
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

  	/* PC.2,PC.3,PC.4,PC.5 按键中断(EXTI Line0)*/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOC , &GPIO_InitStructure);
	
		/* 设置 USART1 的Tx脚（PA.9）为第二功能推挽输出功能 */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA , &GPIO_InitStructure);

	 
  	/* 设置 USART1 的Rx脚（PA.10）为浮空输入脚 */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOA , &GPIO_InitStructure);

	  	/* 将 PC.0 设置为模拟输入脚 */
	//x轴
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  	GPIO_Init(GPIOC , &GPIO_InitStructure);

	/* 将 PC.1 设置为模拟输入脚 */
	//y轴
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
//	/* PWM使能*/
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
*按键中断配置
*******************************************************************************/

void NVIC_Configuration(void)
{
	/* 定义NVIC初始化结构体 NVIC_InitStructure */
	NVIC_InitTypeDef NVIC_InitStructure;

	/* #ifdef...#else...#endif结构的作用是根据预编译条件决定中断向量表起始地址*/
#ifdef  VECT_TAB_RAM
  	/* 中断向量表起始地址从 0x20000000 开始 */
 	NVIC_SetVectorTable(NVIC_VectTab_RAM , 0x0);
#else 	/* VECT_TAB_FLASH */
  	/* 中断向量表起始地址从 0x80000000 开始 */
  	NVIC_SetVectorTable(NVIC_VectTab_FLASH , 0x0);
#endif

  	/* 选择NVIC优先级分组2 */
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  	/* 使能EXIT 2通道 ，2级先占优先级 ，0级次占优先级 */
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQChannel;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

	/* 使能EXIT 3通道 ，1级先占优先级 ，0级次占优先级 */
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQChannel;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

	/* 使能EXIT 4通道 ，0级先占优先级 ，0级次占优先级 */
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQChannel;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

	/* 使能EXIT 5通道 ，3级先占优先级 ，0级次占优先级 */
  	NVIC_InitStructure.NVIC_IRQChannel =EXTI9_5_IRQChannel;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
}

//按键中断配置
void EXIT_Configuration(void)
{
	/* 定义EXIT初始化结构体 EXTI_InitStructure */
	EXTI_InitTypeDef EXTI_InitStructure;

	/* 设置外部中断0通道（EXIT Line0）在下降沿时触发中断 */
  	EXTI_InitStructure.EXTI_Line = EXTI_Line2 | EXTI_Line3 | EXTI_Line4 | EXTI_Line5;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//双边沿触发
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
* 函数名  		: ADC_Configuration
* 函数描述    	: 初始化并启动ADC转换
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/

void ADC_Configuration(void)
{
	/* 定义 ADC 初始化结构体 ADC_InitStructure */
	ADC_InitTypeDef ADC_InitStructure;
	
	/* 配置ADC时钟分频 */
	RCC_ADCCLKConfig(RCC_PCLK2_Div4);

	/*	
	*	独立工作模式;
	*	模数转换工作在单通道模式;
	*	模数转换工作在单次转换模式;
	*	转换触发方式：转换由软件触发启动;
	*	ADC 数据右对齐 ;
	*	进行规则转换的 ADC 通道的数目为1; 
	*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	

	/* 使能 ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	/* 复位 ADC1 的校准寄存器 */   
	ADC_ResetCalibration(ADC1);
	/* 等待 ADC1 校准寄存器复位完成 */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* 开始 ADC1 校准 */
	ADC_StartCalibration(ADC1);
	/* 等待 ADC1 校准完成 */
	while(ADC_GetCalibrationStatus(ADC1));
	/* 启动 ADC1 转换 */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	
}


/*******************************************************************************
* 函数名  		: USART_Configuration
* 函数描述    	: 设置USART1
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/

void USART_Configuration(void)
{
	/* 定义 USART 初始化结构体 USART_InitStructure */
  	USART_InitTypeDef USART_InitStructure;

	/*	波特率为9600bps;
	*	8位数据长度;
	*	1个停止位，无校验;
	*	禁用硬件流控制;
	*	禁止USART时钟;
	*	时钟极性低;
	*	在第2个边沿捕获数据
	*	最后一位数据的时钟脉冲不从 SCLK 输出； 
	*/

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl =  USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1 , &USART_InitStructure);
    
  	/* 使能 USART1 */
  	USART_Cmd(USART1 , ENABLE);
}


 
/*******************************************************************************
* 函数名  		: fputc
* 函数描述    	: 将printf函数重定位到USATR1
* 输入参数    	: 无
* 输出结果    	: 无
* 返回值		: 无
*******************************************************************************/

int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (u8) ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	return ch;
}

