

/* Includes ------------------------------------------------------------------*/		
 #include"data.h"
u16 Get_Adc(u8 ch,u8 sx);
void __adc(axis *xy)
{
  xy->x=Get_Adc(10,1);
  xy->y=Get_Adc(11,1);
}

 /*******************************************************************************
* 函数名  		: Get_Adc
* 函数描述    	: PC0--ADC_IN10  PC1--ADC_IN11,对ad采样
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/

//获得ADC值
//ch:通道值 0~3
//PA0--ADC_IN0  PA1--ADC_IN1  PA2--ADC_IN2  PA3--ADC_IN3  PA4--ADC_IN4  PA5--ADC_IN5  PA6--ADC_IN6  PA7--ADC_IN7  
	//PB0--ADC_IN8  PB1--ADC_IN9  PC0--ADC_IN10  PC1--ADC_IN11  PC2--ADC_IN12  PC3--ADC_IN13  PC4--ADC_IN14  PC5--ADC_IN15                   
	
	
	//PC0--ADC_IN10  PC1--ADC_IN11
u16 Get_Adc(u8 ch,u8 sx)   
{	unsigned short result=0;
	unsigned char i;

  	//设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
	ADC_RegularChannelConfig(ADC1, ch, sx, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道ch,规则采样顺序值序列为1,采样时间为239.5周期	  			    
  	for(i=0;i<8;i++)
	{
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	 
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
		result+=ADC_GetConversionValue(ADC1);
	}
	result=result/8;	 //取8次采样的平均值
	return result;	//返回平均值
}
