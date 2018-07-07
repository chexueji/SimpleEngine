#include "stdafx.h"
#include "2D_DDALine.h"


void DDA_DrawLine(const Vec2f &StartPnt,const Vec2f &EndPnt,COLORREF Color1,COLORREF Color2,VTYPE *pVertex,long *pVertexNum)  
{  
	long YDis = (EndPnt[1] - StartPnt[1])+0.5;  
	long XDis = (EndPnt[0]-StartPnt[0])+0.5;  
	long MaxStep = max(abs(XDis),abs(YDis)); 

	float r1 = GetR(Color1);
	float g1 = GetG(Color1);
	float b1 = GetB(Color1);

	float r2 = GetR(Color2);
	float g2 = GetG(Color2);
	float b2 = GetB(Color2);

	if(MaxStep == 0)
	{
		//SetPixelV(hDC,int(StartPnt[0]+0.5),int(StartPnt[1]+0.5),Color1); 
		//glColor3f(r1/255.0,g1/255.0,b1/255.0);
		//glVertex2d(StartPnt[0]+0.5,StartPnt[1]+0.5);
		return;
	}
	double fXStep = 1.0f;  
	double fYStep = 1.0f;   
	fXStep = double(XDis)/double(MaxStep);  
	fYStep = double(YDis)/double(MaxStep);  

	float fRStep = 0.0f;
	float fGStep = 0.0f;
	float fBStep = 0.0f;
	fRStep = float(r2-r1)/float(MaxStep);
	fGStep =float(g2-g1)/float(MaxStep);
	fBStep = float(b2-b1)/float(MaxStep);
 
	//SetPixelV(hDC,int(StartPnt[0]+0.5),int(StartPnt[1]+0.5),Color1); 
	//glColor3f(r1/255.0,g1/255.0,b1/255.0);
	//glVertex2d(StartPnt[0]+0.5,StartPnt[1]+0.5);

	double x = double(StartPnt[0]);  
	double y = double(StartPnt[1]);  
	float r = r1;
	float g = g1;
	float b = b1;

	for (long i = 1;i<=MaxStep;i++)  
	{  
		x = x + fXStep;  
		y = y + fYStep;  
		r = r + fRStep;
		g = g + fGStep;
		b = b + fBStep;
		//glColor3f(r/255.0,g/255.0,b/255.0);
		//glVertex2d(x+0.5,y+0.5);
		//pVertex[(*pVertexNum)++]=VTYPE(r/255.0,g/255.0,b/255.0,x+0.5,y+0.5,0.0);
		pVertex[(*pVertexNum)++]=VTYPE(GLubyte(r),GLubyte(g),GLubyte(b),GLubyte(0.0),x,y);
		//SetPixelV(hDC,int(x+0.5),int(y+0.5),RGB(r,g,b)); 
	}  

}

//Liang&Barsky line clipping method
bool ClipTest(double p,double q,double *u1,double *u2)
{
	double r;
	bool flag=true;
	if(p<0.0f)
	{
		r=q/p;
		if(r>*u2)
		{
			flag=false;
		}else if(r>*u1)
		{
			*u1=r;
		}
	}else if(p>0.0f)
	{
		r=q/p;
		if(r<*u1)
		{
			flag=false;
		}else if(r<*u2)
		{
			*u2=r;
		}
	}else
	{
		if(q<0.0f)
			flag=false;
	}
	return flag;
}
//Liang&Barsky line clipping method
void LineClip_LiangBarsky(Vec2f winMin,Vec2f winMax,const Vec2f pnt_1,const Vec2f pnt_2,\
										 const COLORREF Color1,const COLORREF Color2,VTYPE *pVertex,long *pVertexNum)
{	
	Vec2f pnt1=pnt_1;
	Vec2f pnt2=pnt_2;
	COLORREF color1=Color1;
	COLORREF color2=Color2;

	float r1=GetR(color1);
	float g1=GetG(color1);
	float b1=GetB(color1);

	float r2=GetR(color2);
	float g2=GetG(color2);
	float b2=GetB(color2);

	double u1=0.0f,u2=1.0f,deltaX=pnt2[0]-pnt1[0],deltaY;
	if(ClipTest(-deltaX,pnt1[0]-winMin[0],&u1,&u2))
	{
		if(ClipTest(deltaX,winMax[0]-pnt1[0],&u1,&u2))
		{
			deltaY=pnt2[1]-pnt1[1];
			if(ClipTest(-deltaY,pnt1[1]-winMin[1],&u1,&u2))
			{
				if(ClipTest(deltaY,winMax[1]-pnt1[1],&u1,&u2))
				{
					if(u2<1.0f)
					{
						pnt2[0]=pnt1[0]+u2*deltaX;
						pnt2[1]=pnt1[1]+u2*deltaY;
						r2=r1+u2*(r2-r1);
						g2=g1+u2*(g2-g1);
						b2=b1+u2*(b2-b1);
						color2=RGB(r2,g2,b2);
					}
					if(u1>0.0f)
					{
						pnt1[0]=pnt1[0]+u1*deltaX;
						pnt1[1]=pnt1[1]+u1*deltaY;	
						r1=r1+u1*(r2-r1);
						g1=g1+u1*(g2-g1);
						b1=b1+u1*(b2-b1);
						color1=RGB(r1,g1,b1);
					}
					DDA_DrawLine(pnt1,pnt2,color1,color2,pVertex,pVertexNum); 
				}
			}
		}	
	}
	

}