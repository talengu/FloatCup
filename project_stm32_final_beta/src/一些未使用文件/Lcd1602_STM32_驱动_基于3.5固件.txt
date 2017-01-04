
/****************************************************************
                  LCD1602模块 STM32 通用驱动
            不同引脚定义，只需要修改”各端口定义“部分
			        集成1602 显示变量函数
					By bushiniu 2012.12
				  QQ:120954178  长沙&啊牛					
****************************************************************/

/**********************使用方法**********************************
主程序中调用：
#include "Lcd1602.h" //包含LCD驱动头文件
Lcd_Init();	//LCD初始化
函数说明在头文件中
*****************************************************************/


#include "Lcd1602.h"
#include "SysTick.h"

/********************各端口定义*********************************/
#define GPIO_EN       GPIOC                 //  使能端口组
#define GPIO_EN_PIN   GPIO_Pin_7            //  使能端口号
#define RCC_GPIO_EN   RCC_APB2Periph_GPIOC  //  使能时钟组

#define GPIO_RW       GPIOC                 //  读/写选择端口组
#define GPIO_RW_PIN   GPIO_Pin_6            //  读/写选择端口号
#define RCC_GPIO_RW   RCC_APB2Periph_GPIOC  //  读/写时钟组

#define GPIO_RS       GPIOB                 //  数据/命令端口组
#define GPIO_RS_PIN   GPIO_Pin_5            //  数据/命令端口号
#define RCC_GPIO_RS   RCC_APB2Periph_GPIOB  //  数据/命令时钟组

#define GPIO_DATA_0       GPIOB         //  数据线0_端口组
#define GPIO_DATA_0_PIN   GPIO_Pin_8    //  数据线0_端口号
#define GPIO_DATA_1       GPIOB         //  数据线1_端口组
#define GPIO_DATA_1_PIN   GPIO_Pin_9    //  数据线1_端口号
#define GPIO_DATA_2       GPIOB         //  数据线2_端口组
#define GPIO_DATA_2_PIN   GPIO_Pin_10   //  数据线2_端口号
#define GPIO_DATA_3       GPIOB         //  数据线3_端口组
#define GPIO_DATA_3_PIN   GPIO_Pin_11   //  数据线3_端口号
#define GPIO_DATA_4       GPIOB         //  数据线4_端口组
#define GPIO_DATA_4_PIN   GPIO_Pin_12   //  数据线4_端口号
#define GPIO_DATA_5       GPIOB         //  数据线5_端口组
#define GPIO_DATA_5_PIN   GPIO_Pin_13   //  数据线5_端口号
#define GPIO_DATA_6       GPIOB         //  数据线6_端口组
#define GPIO_DATA_6_PIN   GPIO_Pin_14   //  数据线6_端口号
#define GPIO_DATA_7       GPIOB         //  数据线7_端口组
#define GPIO_DATA_7_PIN   GPIO_Pin_15   //  数据线7_端口号
#define RCC_GPIO_DATA   RCC_APB2Periph_GPIOB  //  数据线时钟组


/******************************************************************/


/***********************基本指令***********************************/
#define SET_EN  GPIO_SetBits(GPIO_EN, GPIO_EN_PIN)		//EN 使能  输出1
#define CLE_EN  GPIO_ResetBits(GPIO_EN, GPIO_EN_PIN)	//      输出0 
#define SET_RW  GPIO_SetBits(GPIO_RW, GPIO_RW_PIN)      //RW 读写  输出1
#define CLE_RW  GPIO_ResetBits(GPIO_RW, GPIO_RW_PIN)	//      输出0
#define SET_RS  GPIO_SetBits(GPIO_RS, GPIO_RS_PIN)		//RS 指令  输出1
#define CLE_RS  GPIO_ResetBits(GPIO_RS, GPIO_RS_PIN)	//      输出0
/******************************************************************/


/************************端口初始化*******************************/
void Lcd_GPIO_init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;   //声明结构体

    /********Data端口设置*************/
	 RCC_APB2PeriphClockCmd(RCC_GPIO_DATA, ENABLE);  //打开端口B时钟 
     GPIO_InitStructure.GPIO_Pin  = GPIO_DATA_0_PIN|GPIO_DATA_1_PIN|GPIO_DATA_2_PIN|GPIO_DATA_3_PIN|GPIO_DATA_4_PIN|GPIO_DATA_5_PIN|GPIO_DATA_6_PIN|GPIO_DATA_7_PIN; //  DB8~DB15
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //标准输出模式
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50M时钟速度
     GPIO_Init(GPIO_DATA_0, &GPIO_InitStructure);      //初始化端口

	 /********使能端口设置**********/
	 RCC_APB2PeriphClockCmd(RCC_GPIO_EN, ENABLE);    //打开端口时钟 
     GPIO_InitStructure.GPIO_Pin =  GPIO_EN_PIN;     // 使能端口
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //标准输出模式
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50M时钟速度
     GPIO_Init(GPIO_EN, &GPIO_InitStructure);

	 /********读/写端口设置**********/
	 RCC_APB2PeriphClockCmd(RCC_GPIO_RW, ENABLE);    //打开端口时钟 
     GPIO_InitStructure.GPIO_Pin =  GPIO_RW_PIN;     // 使能端口
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //标准输出模式
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50M时钟速度
     GPIO_Init(GPIO_RW, &GPIO_InitStructure);

	 /********指令/数据端口设置**********/
	 RCC_APB2PeriphClockCmd(RCC_GPIO_RS, ENABLE);    //打开端口时钟 
     GPIO_InitStructure.GPIO_Pin =  GPIO_RS_PIN;     // 使能端口
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽复用输出
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50M时钟速度
     GPIO_Init(GPIO_RS, &GPIO_InitStructure);


}
/******************************************************************/



void Lcd_Init( void )  //初始化
{  
 	Lcd_GPIO_init();
	Delay_us(1500);                     //延时15ms
	Lcd_Write_Command( 0x38,0);       // 写指令38H 不检测忙信号
	Delay_us(500);				      //延时5ms
    Lcd_Write_Command( 0x38,0);       // 写指令38H 不检测忙信号
	Delay_us(500);					  //延时5ms
    Lcd_Write_Command( 0x38,0);       // 写指令38H 不检测忙信号
   									  //以后每次写指令、读/写数据操作之前需检测忙信号
	Lcd_Write_Command( 0x38,1);       //显示模式设置 
    Lcd_Write_Command( 0x08,1);		  //显示关闭
	Lcd_Write_Command( 0x01,1);       //显示清屏
    Lcd_Write_Command( 0x06,1);       //显示光标移动设置 
    Lcd_Write_Command( 0x0C,1); 	  //显示开、光标不显示
} 
/******************************************************/

void Lcd_En_Toggle(void) //发使能脉冲
{
	SET_EN;        //使能1
	Delay_us(5);   //延时160us
	CLE_EN;
}


void Lcd_Busy(void)//判断忙
{
     GPIO_InitTypeDef GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_GPIO_DATA, ENABLE);    //打开DATA端口时钟 

     GPIO_InitStructure.GPIO_Pin  = GPIO_DATA_0_PIN|GPIO_DATA_1_PIN|GPIO_DATA_2_PIN|GPIO_DATA_3_PIN|GPIO_DATA_4_PIN|GPIO_DATA_5_PIN|GPIO_DATA_6_PIN|GPIO_DATA_7_PIN; //  DB8~DB15
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //输入模式 = 上拉输入
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50M时钟速度
     GPIO_Init(GPIO_DATA_7, &GPIO_InitStructure);      //打开忙检测端口

 	 CLE_RS; //RS = 0
	 //Delay_us(1);   //延时10us
	 SET_RW; //RW = 1
	 //Delay_us(1);   //延时10us
	 SET_EN; //EN = 1
	 //Delay_us(2);   //延时20us
     while (GPIO_ReadInputDataBit(GPIO_DATA_7,GPIO_DATA_7_PIN)); //循环等待忙检测端口 = 0
	 CLE_EN; //EN = 0

     //恢复端口为输出状态 
   	 RCC_APB2PeriphClockCmd(RCC_GPIO_DATA, ENABLE);    //打开DATA端口时钟 
	 GPIO_InitStructure.GPIO_Pin  = GPIO_DATA_0_PIN|GPIO_DATA_1_PIN|GPIO_DATA_2_PIN|GPIO_DATA_3_PIN|GPIO_DATA_4_PIN|GPIO_DATA_5_PIN|GPIO_DATA_6_PIN|GPIO_DATA_7_PIN; //  DB8~DB15
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50M时钟速度
     GPIO_Init(GPIO_DATA_7, &GPIO_InitStructure);

} 

void Gpio_data(unsigned char x)  //端口置入数据
{  
GPIO_ResetBits(GPIO_DATA_0, GPIO_DATA_0_PIN);  //DB0
GPIO_ResetBits(GPIO_DATA_0, GPIO_DATA_1_PIN);  //DB1
GPIO_ResetBits(GPIO_DATA_0, GPIO_DATA_2_PIN);  //DB2
GPIO_ResetBits(GPIO_DATA_0, GPIO_DATA_3_PIN);  //DB3
GPIO_ResetBits(GPIO_DATA_0, GPIO_DATA_4_PIN);  //DB4
GPIO_ResetBits(GPIO_DATA_0, GPIO_DATA_5_PIN);  //DB5
GPIO_ResetBits(GPIO_DATA_0, GPIO_DATA_6_PIN);  //DB6
GPIO_ResetBits(GPIO_DATA_0, GPIO_DATA_7_PIN);  //DB7
if(x&0X01)GPIO_SetBits(GPIO_DATA_0, GPIO_DATA_0_PIN);//DB0
if(x&0X02)GPIO_SetBits(GPIO_DATA_0, GPIO_DATA_1_PIN);//DB1
if(x&0X04)GPIO_SetBits(GPIO_DATA_0, GPIO_DATA_2_PIN);//DB2
if(x&0X08)GPIO_SetBits(GPIO_DATA_0, GPIO_DATA_3_PIN);//DB3
if(x&0X10)GPIO_SetBits(GPIO_DATA_0, GPIO_DATA_4_PIN);//DB4
if(x&0X20)GPIO_SetBits(GPIO_DATA_0, GPIO_DATA_5_PIN);//DB5
if(x&0X40)GPIO_SetBits(GPIO_DATA_0, GPIO_DATA_6_PIN);//DB6
if(x&0X80)GPIO_SetBits(GPIO_DATA_0, GPIO_DATA_7_PIN);//DB7
}


//向液晶里面写入指令  时序：RS=L,RW=L,Data0-Data7=指令码，E=高脉冲
void Lcd_Write_Command(unsigned char x,char Busy) 
{ 
    if(Busy) Lcd_Busy();
    //Delay_us(1);   //延时10us
	CLE_RS;  //RS = 0 
    //Delay_us(1);   //延时10us
	CLE_RW;  //RW = 0 
    //Delay_us(4);   //延时40us
    Gpio_data(x);  //端口置入数据
    //Delay_us(4);   //延时40us
	Lcd_En_Toggle();  //发使能脉冲
    //Delay_us(1);   //延时100us
	Lcd_Busy(); //测忙

}
//向液晶里面写入数据  时序：RS=H,RW=L,Data0-Data7=指令码，E=高脉冲
void Lcd_Write_Data( unsigned char x) //向液晶里面写入数据 
{ 
	Lcd_Busy(); //测忙
    //Delay_us(1);   //延时10us
	SET_RS;   //RS = 1 
    //Delay_us(1);   //延时10us
    CLE_RW;   //RW = 0
    //Delay_us(4);   //延时40us
    Gpio_data(x);
    //Delay_us(4);   //延时40us
	Lcd_En_Toggle();  //发使能脉冲
    //Delay_us(1);   //延时100us
	Lcd_Busy(); //测忙

} 


void Lcd_SetXY(unsigned char x,unsigned char y)   //字符初始位置设定，x表示列，y表示行 
{ 
     unsigned char addr; 
     if(y==0) 
          addr=0x80+x; 
     else if(y==1)
          addr=0xC0+x; 
     Lcd_Write_Command(addr,1) ; 
} 
/******************************************************/

void Lcd_Puts(unsigned char x,unsigned char y, unsigned char *string) //向1602写一个字符串 
{ 
   //unsigned char i=0;
   Lcd_SetXY(x,y); 
	while(*string) 
	  { 
	   Lcd_Write_Data(*string); 
       string++; 
      } 
}



unsigned char strTemp2[2];
unsigned char strTemp3[3];
unsigned char strTemp4[4];
unsigned char strTemp5[5];
unsigned char strTemp6[6];
unsigned char strTemp7[7];

unsigned char *tr_2(unsigned int outData_2)	
{
	strTemp2[0]=(unsigned char)(((outData_2/10) % 10) + 0x30);
	strTemp2[1]=(unsigned char)((outData_2%10) + 0x30);
	return strTemp2;
}

unsigned char *tr_3(unsigned int outData3)
{
	strTemp3[0]=(unsigned char)(((outData3/100) % 10) + 0x30);
	strTemp3[1]=(unsigned char)(((outData3/10) % 10) + 0x30);
	strTemp3[2]=(unsigned char)((outData3%10) + 0x30);
	return strTemp3;
}



unsigned char *tr_4(unsigned int outData4)
{
	strTemp4[0]=(unsigned char)(((outData4/1000)% 10) + 0x30);
	strTemp4[1]=(unsigned char)(((outData4/100)% 10) + 0x30);
	strTemp4[2]=(unsigned char)(((outData4/10)% 10) + 0x30);
	strTemp4[3]=(unsigned char)((outData4%10) + 0x30);
	return strTemp4;
}


unsigned char *tr_5(unsigned int outData5)
{
	strTemp5[0]=(unsigned char)(((outData5/10000)% 10) + 0x30);
	strTemp5[1]=(unsigned char)(((outData5/1000)% 10) + 0x30);
	strTemp5[2]=(unsigned char)(((outData5/100)% 10) + 0x30);
	strTemp5[3]=(unsigned char)(((outData5/10)% 10) + 0x30);
	strTemp5[4]=(unsigned char)((outData5%10) + 0x30);
	return strTemp5;
}

unsigned char *tr_6(unsigned int outData6)
{
	strTemp6[0]=(unsigned char)(((outData6/100000)% 10) + 0x30);
	strTemp6[1]=(unsigned char)(((outData6/10000)% 10) + 0x30);
	strTemp6[2]=(unsigned char)(((outData6/1000)% 10) + 0x30);
	strTemp6[3]=(unsigned char)(((outData6/100)% 10) + 0x30);
	strTemp6[4]=(unsigned char)(((outData6/10)% 10) + 0x30);
	strTemp6[5]=(unsigned char)((outData6%10) + 0x30);
	return strTemp6;
}



unsigned char *tr_7(unsigned int outData7)
{
	strTemp7[0]=(unsigned char)(((outData7/1000000)% 10) + 0x30);
	strTemp7[1]=(unsigned char)(((outData7/100000)% 10) + 0x30);
	strTemp7[2]=(unsigned char)(((outData7/10000)% 10) + 0x30);
	strTemp7[3]=(unsigned char)(((outData7/1000)% 10) + 0x30);
	strTemp7[4]=(unsigned char)(((outData7/100)% 10) + 0x30);
	strTemp7[5]=(unsigned char)(((outData7/10)% 10) + 0x30);
	strTemp7[6]=(unsigned char)((outData7%10) + 0x30);
	return strTemp7;
}


/******************************************************/
