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

	//����
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


//	//����
//   	void __pid(axis *xynow,axis *xyold,pwn *p)
//	{
//			//�������λ��������Χ�ڣ��������С��pwm��
//		  if(((targetxy.x-Diff<=xynow->x)&&xynow->x<=targetxy.x+Diff)&&(targetxy.y-Diff<=xynow->y&&xynow->y<=targetxy.y+Diff))
//		  {
//		  		p->px=(xynow->x-xyold->x)/dx;		//dx������
//				p->py=(xynow->y-xyold->x)/dy;		//ͬ��
//				return;
//		  }
//		  //�������x����࣬����ȦL1������������pxΪ���������෴��
//		  if(xynow->x>targetxy.x)
//		  {
//		  		p->px=Kpx*(targetxy.x-xynow->x)-Kdx*(xynow->x-xyold->x);		
//		  }
//		  else
//		  {
//		  		p->px=Kpx*(-targetxy.x-xynow->x+x_border.x+x_border.y)-Kdx*(xynow->x-xyold->x); //��������
//		  }
//		  if(xynow->y>targetxy.y)
//		  {
//		  		p->py=Kpy*(targetxy.y-xynow->y)-Kdy*(xynow->y-xyold->y);
//		  }
//		  else
//		  {
//		  		p->py=Kpy*(-targetxy.y-xynow->y+y_border.x+y_border.y)-Kdy*(xynow->y-xyold->y);	//��������
//		  }
//	}    							   		   