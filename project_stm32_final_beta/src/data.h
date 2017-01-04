#include "stm32f10x_lib.h"
#include "stdio.h"
typedef struct
 {
 	float x;
	float y;
 }axis;
 
 ///px,pyä¸ºç™¾åˆ†æ¯”ä¸‹çš„æ•°å€¼ï¼Œå¦‚50ï¼Œè¡¨ç¤º50%
 typedef struct
 {
 	 int px;
	 int py;
 }pwn;

void __adc(axis *xy);

void __pid(axis *xynow,axis *xyold,pwn *p);

void __pwm(pwn *p);

#define TIM_PERIOD 50000
static pwn p;	//å…¨å±€å˜é‡ï¼Œä¿å­˜å ç©ºæ¯”
static int locked=0; //é”ï¼Œå½“å¤§äº1æ—¶å ç©ºæ¯”pä¸èƒ½æ”¹å˜ã€

static axis x_border={2672,57};				//x×ø±ê±ß½çÖµ£¬{L1£¬L3},´ıµ÷

static axis y_border={2673,55};				//Í¬ÉÏ

static axis targetxy={1170,1280};			//ÖĞĞÄµã×ø±êÖµ£¬´ıµ÷
#define Kpx 10
#define Kdx 130	//0.1				  	//×èÄá
#define Kpy 15						//15						
#define Kdy 195	 //0.1

#define Kpx0 6	//Ó¦½Ï´ó£¬µ«²»Ó¦³¬	   /6
#define Kpy0 6		//Ó¦½ÏĞ¡			 /6
#define Kdx0 200		//40,50,60,70,,80....					 //0.01
#define Kdy0 200		//	40,50,60 ,70,,80...
#define Diffx 1000 			  //
#define Diffy 1000
//#define dx 10
//#define dy 10
#define delaytime 1
