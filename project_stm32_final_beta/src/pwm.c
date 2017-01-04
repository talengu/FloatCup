#include "stm32f10x_lib.h"
#include "data.h"

void Tim3_Pwm_Px(int ccr_val,int cmd);
void Tim3_Pwm_Py(int ccr_val,int cmd);
/**************************************************
*根据传递过来的千分比数值来设置定时器TIM3的通道，
*从而改变pwm波输出的占空比
*px为ccr3_val接pb0
***************************************************/
////////////////////////////////////////////////	///
///interface.......
///////////////////////////////////////////////////
int ABS(int x)
{
	return (x>0)?(x):(-x);
}

/////////
///坐标
void __pwm(pwn *p)
{
	/* 定义 TIM_OCInit 初始化结构体 TIM_OCInitStructure */
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	int ccr3_val,ccr4_val;

	//如果占空比上锁，则直接返回
	if(locked)
	{
		return;
	}
//	ccr3_val=0;
//	ccr4_val=0;
    ccr3_val=TIM_PERIOD-TIM_PERIOD/5000*ABS(p->px);//实际应该在坐标上，加一个基础值	  
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
    //命令1表示pwm1
    if(cmd>0)
    {
		//当px为正时，PA6使能，B0输出pwm波
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
    	 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
      	 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
    	 TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC3
    	 TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR3上的预装载寄存器	  //

		 GPIO_ResetBits(GPIOB,GPIO_Pin_0);	//

    //     TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1/
    }
    else
    {	  //当px为负时，B0使能，A6输出pwm波
	     GPIO_SetBits(GPIOB,GPIO_Pin_0);	//

		 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    	 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
		 TIM_OCInitStructure.TIM_Pulse = ccr_val;
    	 TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC3
    	 TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR3上的预装载寄存器	  //
    }

}

void Tim3_Pwm_Py(int ccr_val,int cmd)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
    //命令1表示pwm1
    if(cmd>0)
    {
		 //当px为正时，PA7使能，B1输出pwm波
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
    	 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
      	 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
    	 TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC3
    	 TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR3上的预装载寄存器	  //

		 GPIO_ResetBits(GPIOB,GPIO_Pin_1);	//

    }
    else
    {

		 GPIO_SetBits(GPIOB,GPIO_Pin_1);	//

		 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    	 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
      	 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高		      
		 TIM_OCInitStructure.TIM_Pulse = ccr_val;
    	 TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC3
    	 TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR3上的预装载寄存器	  //
    }
}
