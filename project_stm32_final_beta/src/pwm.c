#include "stm32f10x_lib.h"
#include "data.h"

void Tim3_Pwm_Px(int ccr_val,int cmd);
void Tim3_Pwm_Py(int ccr_val,int cmd);
/**************************************************
*���ݴ��ݹ�����ǧ�ֱ���ֵ�����ö�ʱ��TIM3��ͨ����
*�Ӷ��ı�pwm�������ռ�ձ�
*pxΪccr3_val��pb0
***************************************************/
////////////////////////////////////////////////	///
///interface.......
///////////////////////////////////////////////////
int ABS(int x)
{
	return (x>0)?(x):(-x);
}

/////////
///����
void __pwm(pwn *p)
{
	/* ���� TIM_OCInit ��ʼ���ṹ�� TIM_OCInitStructure */
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	int ccr3_val,ccr4_val;

	//���ռ�ձ���������ֱ�ӷ���
	if(locked)
	{
		return;
	}
//	ccr3_val=0;
//	ccr4_val=0;
    ccr3_val=TIM_PERIOD-TIM_PERIOD/5000*ABS(p->px);//ʵ��Ӧ���������ϣ���һ������ֵ	  
	ccr4_val=TIM_PERIOD-TIM_PERIOD/5000*ABS(p->py);

	if(ccr3_val<=0)
	{
		ccr3_val=0;
	}
	if(ccr4_val<=0)
	{
		ccr4_val=0;			    
	}

	Tim3_Pwm_Px(ccr3_val,p->px);
	Tim3_Pwm_Py(ccr4_val,p->py);
	TIM_ARRPreloadConfig(TIM3, ENABLE);		 //
}
///////////////////////////////////////////////////
///////////////////////////////////////////////////

void Tim3_Pwm_Px(int ccr_val,int cmd)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
    //����1��ʾpwm1
    if(cmd>0)
    {
		//��pxΪ��ʱ��PA6ʹ�ܣ�B0���pwm��
//
//		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	// 	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		 GPIO_Init(GPIOA, &GPIO_InitStructure);
//		 GPIO_SetBits(GPIOA,GPIO_Pin_6);	//
//
//		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	 	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		 GPIO_Init(GPIOB, &GPIO_InitStructure);
//
		 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		 TIM_OCInitStructure.TIM_Pulse = TIM_PERIOD-ccr_val;
    	 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
      	 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
    	 TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC3
    	 TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR3�ϵ�Ԥװ�ؼĴ���	  //

		 GPIO_ResetBits(GPIOB,GPIO_Pin_0);	//

    //     TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1/
    }
    else
    {	  //��pxΪ��ʱ��B0ʹ�ܣ�A6���pwm��
	     GPIO_SetBits(GPIOB,GPIO_Pin_0);	//

		 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    	 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
		 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
		 TIM_OCInitStructure.TIM_Pulse = ccr_val;
    	 TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC3
    	 TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR3�ϵ�Ԥװ�ؼĴ���	  //
    }

}

void Tim3_Pwm_Py(int ccr_val,int cmd)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
    //����1��ʾpwm1
    if(cmd>0)
    {
		 //��pxΪ��ʱ��PA7ʹ�ܣ�B1���pwm��
//
//		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	 	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		 GPIO_Init(GPIOA, &GPIO_InitStructure);
//		 GPIO_SetBits(GPIOA,GPIO_Pin_7);	//
//
//		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
//		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	 	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		 GPIO_Init(GPIOB, &GPIO_InitStructure);

		 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		 TIM_OCInitStructure.TIM_Pulse = TIM_PERIOD-ccr_val;
    	 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
      	 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
    	 TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC3
    	 TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR3�ϵ�Ԥװ�ؼĴ���	  //

		 GPIO_ResetBits(GPIOB,GPIO_Pin_1);	//

    }
    else
    {

		 GPIO_SetBits(GPIOB,GPIO_Pin_1);	//

		 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    	 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
      	 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�		      
		 TIM_OCInitStructure.TIM_Pulse = ccr_val;
    	 TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC3
    	 TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR3�ϵ�Ԥװ�ؼĴ���	  //
    }
}
