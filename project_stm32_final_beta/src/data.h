#include "stm32f10x_lib.h"
#include "stdio.h"
typedef struct
 {
 	float x;
	float y;
 }axis;
 
 ///px,py为百分比下的数值，如50，表示50%
 typedef struct
 {
 	 int px;
	 int py;
 }pwn;

void __adc(axis *xy);

void __pid(axis *xynow,axis *xyold,pwn *p);

void __pwm(pwn *p);

#define TIM_PERIOD 50000
static pwn p;	//全局变量，保存占空比
static int locked=0; //锁，当大于1时占空比p不能改变�

static axis x_border={2672,57};				//x����߽�ֵ��{L1��L3},����

static axis y_border={2673,55};				//ͬ��

static axis targetxy={1170,1280};			//���ĵ�����ֵ������
#define Kpx 10
#define Kdx 130	//0.1				  	//����
#define Kpy 15						//15						
#define Kdy 195	 //0.1

#define Kpx0 6	//Ӧ�ϴ󣬵���Ӧ��	   /6
#define Kpy0 6		//Ӧ��С			 /6
#define Kdx0 200		//40,50,60,70,,80....					 //0.01
#define Kdy0 200		//	40,50,60 ,70,,80...
#define Diffx 1000 			  //
#define Diffy 1000
//#define dx 10
//#define dy 10
#define delaytime 1
