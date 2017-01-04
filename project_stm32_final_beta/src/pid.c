/* Includes ------------------------------------------------------------------*/		
 #include"data.h"

	   	void __pid(axis *xynow,axis *xyold,pwn *p)
    {
		  if(((targetxy.x-Diffx<=xynow->x)&&xynow->x<=targetxy.x+Diffx)&&(targetxy.y-Diffy<=xynow->y&&xynow->y<=targetxy.y+Diffy))
		  {
		  		p->px=Kpx0*(targetxy.x-xynow->x)-Kdx0*(xynow->x-xyold->x);
				p->py=Kpy0*(targetxy.y-xynow->y)-Kdy0*(xynow->y-xyold->y);
				return;
		  }
			  if((targetxy.x-Diffx<=xynow->x)&&xynow->x<=targetxy.x+Diffx)
		  {
		  		p->px=Kpx0*(targetxy.x-xynow->x)-Kdx0*(xynow->x-xyold->x);
		  }
		  else
		  {
		  		p->px=Kpx*(targetxy.x-xynow->x)-Kdx*(xynow->x-xyold->x)/delaytime;
		  }
		  if(targetxy.y-Diffy<=xynow->y&&xynow->y<=targetxy.y+Diffy)
		  {
			  	p->py=Kpy0*(targetxy.y-xynow->y)-Kdy0*(xynow->y-xyold->y);
		  }
		  else
		  {
				p->py=Kpy*(targetxy.y-xynow->y)-Kdy*(xynow->y-xyold->y)/delaytime;			       
	      }
	}

	//备用
//	void __pid(axis *xynow,axis *xyold,pwn *p)
//    {
//			  if(((targetxy.x-Diff<=xynow->x)&&xynow->x<=targetxy.x+Diff)&&(targetxy.y-Diff<=xynow->y&&xynow->y<=targetxy.y+Diff))
//		  {
//		  		p->px=Kpx0*(targetxy.x-xynow->x)-Kdx0*(xynow->x-xyold->x);
//				p->py=Kpy0*(targetxy.y-xynow->y)-Kdy0*(xynow->y-xyold->y);
//				return;
//		  }
//		p->px=Kpx*(targetxy.x-xynow->x)-Kdx*(xynow->x-xyold->x)/delaytime;
//		p->py=Kpy*(targetxy.y-xynow->y)-Kdy*(xynow->y-xyold->y)/delaytime;
//	}


//	//备用
//   	void __pid(axis *xynow,axis *xyold,pwn *p)
//	{
//			//如果磁铁位于允许域范围内，则输出较小的pwm波
//		  if(((targetxy.x-Diff<=xynow->x)&&xynow->x<=targetxy.x+Diff)&&(targetxy.y-Diff<=xynow->y&&xynow->y<=targetxy.y+Diff))
//		  {
//		  		p->px=(xynow->x-xyold->x)/dx;		//dx待考察
//				p->py=(xynow->y-xyold->x)/dy;		//同上
//				return;
//		  }
//		  //如果处于x轴左侧，则线圈L1产生向上力，px为负。否则，相反。
//		  if(xynow->x>targetxy.x)
//		  {
//		  		p->px=Kpx*(targetxy.x-xynow->x)-Kdx*(xynow->x-xyold->x);		
//		  }
//		  else
//		  {
//		  		p->px=Kpx*(-targetxy.x-xynow->x+x_border.x+x_border.y)-Kdx*(xynow->x-xyold->x); //参数待调
//		  }
//		  if(xynow->y>targetxy.y)
//		  {
//		  		p->py=Kpy*(targetxy.y-xynow->y)-Kdy*(xynow->y-xyold->y);
//		  }
//		  else
//		  {
//		  		p->py=Kpy*(-targetxy.y-xynow->y+y_border.x+y_border.y)-Kdy*(xynow->y-xyold->y);	//参数待调
//		  }
//	}    							   		   