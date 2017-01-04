 #include "adc.h"
/*----------------------------------------------------------------
 * �ļ����ƣ� ��·ADC������������  
 * ����оƬ�� STM32F103RBT6        
 * �ļ�˵���� PA0��ӦADC_IN0���޸����ŵ�ʱ����ע�� 
 *			  1.GPIO��ʼ�����޸�	
 *			  2.Get_Adc(ADC_Channel_x)������x���޸�
 *			  3.ADCģʽ���ò����޸�
 * 			  4.����ʱ�����ã�����Get_Adc()�е�ADC_SampleTime		  		
 * ������ʶ��416ʵ����  ���㳬
 * ����ʱ�䣺2013.5.9   
**----------------------------------------------------------------*/	 
		   
//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ͨ��0~3																	   
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
 						   //RCC_APB2Periph_GPIOx,x=GPIOx
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //AD����ʱ��72M/6=12M,���ܳ���14M�����򾫶��½�
	//PA0--ADC_IN0  PA1--ADC_IN1  PA2--ADC_IN2  PA3--ADC_IN3  PA4--ADC_IN4  PA5--ADC_IN5  PA6--ADC_IN6  PA7--ADC_IN7  
	//PB0--ADC_IN8  PB1--ADC_IN9  PC0--ADC_IN10  PC1--ADC_IN11  PC2--ADC_IN12  PC3--ADC_IN13  PC4--ADC_IN14  PC5--ADC_IN15                   
	//PA0~3  4·��Ϊģ��ͨ����������                         
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//����Ϊģ������
	GPIO_Init(GPIOA, &GPIO_InitStructure);			  //x=GPIOx

//------------------------------------ADC����--------------------------------------------------------

	ADC_DeInit(ADC1);  //������ ADC1��ȫ���Ĵ�������Ϊȱʡֵ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ���˴�Ϊ1��ͨ��
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	ADC_ResetCalibration(ADC1);	//����ָ����ADC1��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));	//��ȡADC1����У׼�Ĵ�����״̬,ֱ��У׼�Ĵ����������
	ADC_StartCalibration(ADC1);		//��ʼָ��ADC1��У׼״̬
	while(ADC_GetCalibrationStatus(ADC1));		//��ȡָ��ADC1��У׼����,ֱ��У׼���
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������

}				  
//���ADCֵ
//ch:ͨ��ֵ 0~3
u16 Get_Adc(u8 ch)   
{	unsigned int result=0;
	unsigned char i;

  	//����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��ch,�������˳��ֵ����Ϊ1,����ʱ��Ϊ239.5����	  			    
  	for(i=0;i<8;i++)
	{
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	 
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
		result+=ADC_GetConversionValue(ADC1);
	}
	result=result/8;	 //ȡ8�β�����ƽ��ֵ
	return result;	//����ƽ��ֵ
}




