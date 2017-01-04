

/* Includes ------------------------------------------------------------------*/		
 #include"data.h"
u16 Get_Adc(u8 ch,u8 sx);
void __adc(axis *xy)
{
  xy->x=Get_Adc(10,1);
  xy->y=Get_Adc(11,1);
}

 /*******************************************************************************
* ������  		: Get_Adc
* ��������    	: PC0--ADC_IN10  PC1--ADC_IN11,��ad����
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/

//���ADCֵ
//ch:ͨ��ֵ 0~3
//PA0--ADC_IN0  PA1--ADC_IN1  PA2--ADC_IN2  PA3--ADC_IN3  PA4--ADC_IN4  PA5--ADC_IN5  PA6--ADC_IN6  PA7--ADC_IN7  
	//PB0--ADC_IN8  PB1--ADC_IN9  PC0--ADC_IN10  PC1--ADC_IN11  PC2--ADC_IN12  PC3--ADC_IN13  PC4--ADC_IN14  PC5--ADC_IN15                   
	
	
	//PC0--ADC_IN10  PC1--ADC_IN11
u16 Get_Adc(u8 ch,u8 sx)   
{	unsigned short result=0;
	unsigned char i;

  	//����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC1, ch, sx, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��ch,�������˳��ֵ����Ϊ1,����ʱ��Ϊ239.5����	  			    
  	for(i=0;i<8;i++)
	{
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	 
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
		result+=ADC_GetConversionValue(ADC1);
	}
	result=result/8;	 //ȡ8�β�����ƽ��ֵ
	return result;	//����ƽ��ֵ
}
