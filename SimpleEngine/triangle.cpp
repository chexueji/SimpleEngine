#include "stdafx.h"
#include "triangle.h"
#include "cmath"
#include "light.h"
#include "camera.h"

Triangle::Triangle()
{
	pnt_local[0].v=Vec4(0,0,0,1.0);
	pnt_local[1].v=Vec4(0,0,0,1.0);
	pnt_local[2].v=Vec4(0,0,0,1.0);
	pnt_local[0].n=Vec4(0,0,0,1.0);
	pnt_local[1].n=Vec4(0,0,0,1.0);
	pnt_local[2].n=Vec4(0,0,0,1.0);
	memcpy(pnt_trans,pnt_local,3*sizeof(Vertex3D));
	VertexIdx[0]=0;
	VertexIdx[1]=1;
	VertexIdx[2]=2;
	bHiddenFace=false;
	tri_normal=Vec4(0.0,0.0,0.0,1.0);
	tri_color=RGB(0,255,0);
	tri_flat_color=RGB(0,0,0);
	for(int  i=0;i<3;i++)
	{
		vertex_color[i]=RGB(0,255,0);
	}	
}
Triangle::Triangle(Vec3 v1,Vec3 v2,Vec3 v3)
{
	pnt_local[0].v=Vec4(v1[0],v1[1],v1[2],1.0);
	pnt_local[1].v=Vec4(v2[0],v2[1],v2[2],1.0);
	pnt_local[2].v=Vec4(v3[0],v3[1],v3[2],1.0);
	pnt_local[0].n=Vec4(0,0,0,1.0);
	pnt_local[1].n=Vec4(0,0,0,1.0);
	pnt_local[2].n=Vec4(0,0,0,1.0);
	memcpy(pnt_trans,pnt_local,3*sizeof(Vertex3D));
	VertexIdx[0]=0;
	VertexIdx[1]=1;
	VertexIdx[2]=2;
	bHiddenFace = false;
	tri_normal=Vec4(0.0,0.0,0.0,1.0);
	tri_color=RGB(0,255,0);
	tri_flat_color=RGB(0,0,0);
	for(int  i=0;i<3;i++)
	{
		vertex_color[i]=RGB(0,255,0);
	}	

}
Triangle::~Triangle()
{

}

Triangle & Triangle::operator =(const Triangle &tri)
{
	pnt_local[0]=tri.pnt_local[0];pnt_local[1]=tri.pnt_local[1];pnt_local[2]=tri.pnt_local[2];
	pnt_trans[0]=tri.pnt_trans[0];pnt_trans[1]=tri.pnt_trans[1];pnt_trans[2]=tri.pnt_trans[2];
	VertexIdx[0]=tri.VertexIdx[0];VertexIdx[1]=tri.VertexIdx[1];VertexIdx[2]=tri.VertexIdx[2];
	bHiddenFace=tri.bHiddenFace;
	tri_color=tri.tri_color;
	for(int  i=0;i<3;i++)
	{
		vertex_color[i]=tri.vertex_color[i];
	}	
	return *this;
}
void Triangle::swap_pnt(Vec2i &pnt1,Vec2i &pnt2)
{
	Vec2i tmp;
	tmp = pnt1;
	pnt1 = pnt2;
	pnt2 = tmp;
}
void Triangle::swap_color(COLORREF &color1,COLORREF &color2)
{
	COLORREF tmp;
	tmp = color1;
	color1 = color2;
	color2 = tmp;
}
void Triangle::swap_uv(Vec2f &uv1,Vec2f &uv2)
{
	Vec2f tmp;
	tmp = uv1;
	uv1 = uv2;
	uv2 = tmp;
}

void Triangle::swap_normal(Vec3 &nml1,Vec3 &nml2)
{
	Vec3 tmp;
	tmp=nml1;
	nml1=nml2;
	nml2=tmp;	
}
void Triangle::swap_z(float &z1,float &z2)
{
	float tmp;
	tmp=z1;
	z1=z2;
	z2=tmp;
}
COLORREF Triangle::get_texture_color(int x,int y)
{   
	if (texture.pImageData==NULL)
	{
		return RGB(0,0,0);
	}
	BYTE* pPixel = (BYTE *)texture.pImageData;
	BYTE *pOffset = pPixel;	
	pOffset =  (pOffset+y*(texture.bih.biWidth)*(PIXELBYTES/8))+x *(PIXELBYTES/8); 
	COLORREF rgb = 0;  
	memcpy(&rgb,pOffset,sizeof(BYTE)*(PIXELBYTES/8));

	return rgb;
}
#if 0
bool Triangle::draw_flat_top_triangle(Vec2i p0,Vec2i p1,Vec2i p2,\
										 COLORREF v0_color,COLORREF v1_color,COLORREF v2_color,Vec2f winMin,Vec2f winMax)
{
	int minY=p0[1];
	int maxY=p1[1];
	//pnt0,pnt1,pnt2 are ordered by clock wise
	Vec2i pnt0=p0;
	Vec2i pnt1=p1;
	Vec2i pnt2=p2;
	COLORREF color0=v0_color;
	COLORREF color1=v1_color;
	COLORREF color2=v2_color;

	//clip the triangle
	if(pnt0[1]>winMax[1] || pnt2[1]<winMin[1] ||\
		(pnt0[0]<winMin[0] && pnt1[0]<winMin[0] && pnt2[0]<winMin[0])||\
		(pnt0[0]>winMax[0] && pnt1[0]>winMax[0] && pnt2[0]>winMax[0]))
	{
		return 0; //the triangle is invisible
	}

	double r0=GetR(color0);
	double g0=GetG(color0);
	double b0=GetB(color0);
	double r1=GetR(color1);
	double g1=GetG(color1);
	double b1=GetB(color1);
	double r2=GetR(color2);
	double g2=GetG(color2);
	double b2=GetB(color2);

	Vec2f xy_left = Vec2f(pnt0[0],pnt0[1]);
	Vec2f xy_right = Vec2f(pnt0[0],pnt0[1]);

	double r_left = r0; double g_left = g0; double b_left = b0;
	double r_right = r0; double g_right = g0; double b_right = b0;

	double dx_left,dx_right;
	double deltaR_left,deltaG_left,deltaB_left;
	double deltaR_right,deltaG_right,deltaB_right;
	if(pnt2[0]<pnt1[0])
	{
		dx_left = double(pnt2[0] - pnt0[0])/double(pnt2[1]-pnt0[1]);
		dx_right = double(pnt1[0] - pnt0[0])/double(pnt1[1]-pnt0[1]);

		deltaR_left = double(r2-r0)/double(pnt2[1]-pnt0[1]);
		deltaG_left = double(g2-g0)/double(pnt2[1]-pnt0[1]);
		deltaB_left = double(b2-b0)/double(pnt2[1]-pnt0[1]);

		deltaR_right = double(r1-r0)/double(pnt2[1]-pnt0[1]);
		deltaG_right= double(g1-g0)/double(pnt2[1]-pnt0[1]);
		deltaB_right = double(b1-b0)/double(pnt2[1]-pnt0[1]);
	
	}else 
	{
	
		dx_right = double(pnt2[0] - pnt0[0])/double(pnt2[1]-pnt0[1]);
		dx_left = double(pnt1[0] - pnt0[0])/double(pnt1[1]-pnt0[1]);

		deltaR_right = double(r2-r0)/double(pnt2[1]-pnt0[1]);
		deltaG_right = double(g2-g0)/double(pnt2[1]-pnt0[1]);
		deltaB_right = double(b2-b0)/double(pnt2[1]-pnt0[1]);

		deltaR_left = double(r1-r0)/double(pnt2[1]-pnt0[1]);
		deltaG_left= double(g1-g0)/double(pnt2[1]-pnt0[1]);
		deltaB_left = double(b1-b0)/double(pnt2[1]-pnt0[1]);	
	}

	for(int y=minY;y<=maxY;)
	{
		LineClip_LiangBarsky(winMin,winMax,xy_left,xy_right,RGB(r_left,g_left,b_left),RGB(r_right,g_right,b_right));
		//DDA_DrawLine(hDC,xy_left,xy_right,RGB(r_left,g_left,b_left),RGB(r_right,g_right,b_right));
		y+=1;
		xy_left[0]+=dx_left;xy_left[1]=y;
		xy_right[0]+=dx_right;xy_right[1]=y;
		r_left+=deltaR_left; g_left+=deltaG_left;b_left+=deltaB_left;
		r_right+=deltaR_right; g_right+=deltaG_right; b_right+=deltaB_right;
	}
}
bool Triangle::draw_flat_bottom_triangle(Vec2i p0,Vec2i p1,Vec2i p2,\
									  COLORREF v0_color,COLORREF v1_color,COLORREF v2_color,Vec2f winMin,Vec2f winMax)
{
	int minY=p1[1];
	int maxY=p2[1];
	//pnt0,pnt1,pnt2 are ordered by clock wise
	Vec2i pnt0=p0;
	Vec2i pnt1=p1;
	Vec2i pnt2=p2;
	COLORREF color0=v0_color;
	COLORREF color1=v1_color;
	COLORREF color2=v2_color;

	//clip the triangle
	if(pnt0[1]>winMax[1] || pnt2[1]<winMin[1] ||\
		(pnt0[0]<winMin[0] && pnt1[0]<winMin[0] && pnt2[0]<winMin[0])||\
		(pnt0[0]>winMax[0] && pnt1[0]>winMax[0] && pnt2[0]>winMax[0]))
	{
		return 0; //the triangle is invisible
	}

	double r0=GetR(color0);
	double g0=GetG(color0);
	double b0=GetB(color0);
	double r1=GetR(color1);
	double g1=GetG(color1);
	double b1=GetB(color1);
	double r2=GetR(color2);
	double g2=GetG(color2);
	double b2=GetB(color2);

	Vec2f xy_left,xy_right;

	double r_left,g_left,b_left;
	double r_right,g_right,b_right;

	double dx_left,dx_right;
	double deltaR_left,deltaG_left,deltaB_left;
	double deltaR_right,deltaG_right,deltaB_right;

	if(pnt2[0]<pnt1[0])
	{
		dx_left = double(pnt2[0] - pnt0[0])/double(pnt2[1]-pnt0[1]);
		dx_right = double(pnt2[0] - pnt1[0])/double(pnt2[1]-pnt1[1]);

		xy_left=Vec2f(pnt0[0]+double((pnt1[1]-pnt0[1])*(pnt2[0] - pnt0[0]))/double(pnt2[1]-pnt0[1]),pnt1[1]);
		xy_right = Vec2f(pnt1[0],pnt1[1]);

		r_left = r0+double((pnt1[1]-pnt0[1])*(r2-r0))/double(pnt2[1]-pnt0[1]); 
		g_left = g0+double((pnt1[1]-pnt0[1])*(g2-g0))/double(pnt2[1]-pnt0[1]); 
		b_left = b0+double((pnt1[1]-pnt0[1])*(b2-b0))/double(pnt2[1]-pnt0[1]);

		r_right = r1; 
		g_right = g1; 
		b_right = b1;

		deltaR_left = double(r2-r0)/double(pnt2[1]-pnt0[1]);
		deltaG_left = double(g2-g0)/double(pnt2[1]-pnt0[1]);
		deltaB_left = double(b2-b0)/double(pnt2[1]-pnt0[1]);

		deltaR_right = double(r2-r1)/double(pnt2[1]-pnt1[1]);
		deltaG_right= double(g2-g1)/double(pnt2[1]-pnt1[1]);
		deltaB_right = double(b2-b1)/double(pnt2[1]-pnt1[1]);
	}else 
	{
		dx_right = double(pnt2[0] - pnt0[0])/double(pnt2[1]-pnt0[1]);
		dx_left = double(pnt2[0] - pnt1[0])/double(pnt2[1]-pnt1[1]);

		xy_right=Vec2f(pnt0[0]+double((pnt1[1]-pnt0[1])*(pnt2[0] - pnt0[0]))/double(pnt2[1]-pnt0[1]),pnt1[1]);
		xy_left = Vec2f(pnt1[0],pnt1[1]);

		r_right = r0+double((pnt1[1]-pnt0[1])*(r2-r0))/double(pnt2[1]-pnt0[1]); 
		g_right = g0+double((pnt1[1]-pnt0[1])*(g2-g0))/double(pnt2[1]-pnt0[1]); 
		b_right = b0+double((pnt1[1]-pnt0[1])*(b2-b0))/double(pnt2[1]-pnt0[1]);
		r_left = r1; 
		g_left = g1; 
		b_left = b1;

		deltaR_right = double(r2-r0)/double(pnt2[1]-pnt0[1]);
		deltaG_right = double(g2-g0)/double(pnt2[1]-pnt0[1]);
		deltaB_right = double(b2-b0)/double(pnt2[1]-pnt0[1]);

		deltaR_left = double(r2-r1)/double(pnt2[1]-pnt1[1]);
		deltaG_left= double(g2-g1)/double(pnt2[1]-pnt1[1]);
		deltaB_left = double(b2-b1)/double(pnt2[1]-pnt1[1]);		
	}

	for(int y=minY;y<=maxY;)
	{
		//DDA_DrawLine(hDC,xy_left,xy_right,tri_flat_color,tri_flat_color);
		LineClip_LiangBarsky(winMin,winMax,xy_left,xy_right,RGB(r_left,g_left,b_left),RGB(r_right,g_right,b_right));
		//DDA_DrawLine(hDC,xy_left,xy_right,RGB(r_left,g_left,b_left),RGB(r_right,g_right,b_right));
		y+=1;
		xy_left[0]+=dx_left; xy_left[1]=y;
		xy_right[0]+=dx_right; xy_right[1]=y;
		r_left+=deltaR_left; g_left+=deltaG_left;b_left+=deltaB_left;
		r_right+=deltaR_right; g_right+=deltaG_right; b_right+=deltaB_right;
	}
}
bool Triangle::draw_triangle_gouraud(HDC hDC,Vec2f winMin,Vec2f winMax)
{
	
	Vec2i pnt0=Vec2i(pnt_trans[0].v[0],pnt_trans[0].v[1]);
	Vec2i pnt1=Vec2i(pnt_trans[1].v[0],pnt_trans[1].v[1]);
	Vec2i pnt2=Vec2i(pnt_trans[2].v[0],pnt_trans[2].v[1]);
	COLORREF color0=vertex_color[0];
	COLORREF color1=vertex_color[1];
	COLORREF color2=vertex_color[2];
	COLORREF color_intersect;

	float r0=GetR(color0);
	float g0=GetG(color0);
	float b0=GetB(color0);
	float r1=GetR(color1);
	float g1=GetG(color1);
	float b1=GetB(color1);
	float r2=GetR(color2);
	float g2=GetG(color2);
	float b2=GetB(color2);

	Vec2i a=pnt1-pnt0;
	Vec2i b=pnt2-pnt0;

	if (a[0]*b[1]-a[1]*b[0]==0)
	{
		return 0;// not a triangle
	}

	if (pnt0[1]>pnt1[1])
	{
		swap_pnt(pnt0,pnt1);
		swap_color(color0,color1);
	}
	if(pnt0[1]>pnt2[1])
	{
		swap_pnt(pnt0,pnt2);
		swap_color(color0,color2);
	}
	if(pnt1[1]>pnt2[1])
	{
		swap_pnt(pnt1,pnt2);
		swap_color(color1,color2);
	}
	//clip the triangle
	if(pnt0[1]>winMax[1] || pnt2[1]<winMin[1] ||\
		(pnt0[0]<winMin[0] && pnt1[0]<winMin[0] && pnt2[0]<winMin[0])||\
		(pnt0[0]>winMax[0] && pnt1[0]>winMax[0] && pnt2[0]>winMax[0]))
	{
		return 0; //the triangle is invisible
	}
	if (pnt0[1] == pnt1[1])
	{
		draw_flat_bottom_triangle(pnt0,pnt1,pnt2,color0,color1,color2,winMin,winMax);
	}else if (pnt1[1]==pnt2[1])
	{
		draw_flat_top_triangle(pnt0,pnt1,pnt2,color0,color1,color2,winMin,winMax);
	}else
	{		
		draw_flat_top_triangle(pnt0,pnt1,pnt2,color0,color1,color2,winMin,winMax);
		draw_flat_bottom_triangle(pnt0,pnt1,pnt2,color0,color1,color2,winMin,winMax);
	}
}
#endif
#if 1
bool Triangle::draw_flat_top_triangle(Vec2i p0,Vec2i p1,Vec2i p2,\
									  COLORREF v0_color,COLORREF v1_color,COLORREF v2_color,\
									  Vec2f winMin,Vec2f winMax,VTYPE *pVertex,long *pVertexNum)
{
	//int minY=p0[1];
	//int maxY=p1[1];
	//pnt0,pnt1,pnt2 are ordered by clock wise
	Vec2i pnt0=p0;
	Vec2i pnt1=p1;
	Vec2i pnt2=p2;
	COLORREF color0=v0_color;
	COLORREF color1=v1_color;
	COLORREF color2=v2_color;

	if (pnt2[1] == pnt1[1])
	{
		//flat bottom triangle
	}else if(pnt2[1] == pnt0[1])
	{
		swap_pnt(pnt0,pnt1);
		swap_color(color0,color1);
	}else if(pnt0[1]==pnt1[1])
	{
		swap_pnt(pnt0,pnt2);
		swap_color(color0,color2);
	}else
	{
		return 0;//not flat bottom triangle
	}
	if (pnt1[0]<pnt2[0])
	{
		swap_pnt(pnt1,pnt2);
		swap_color(color1,color2);
	}else if (pnt1[0]==pnt2[0])
	{
		return 0; //not a triangle
	}

	//clip the triangle
	if(pnt0[1]>winMax[1] || pnt2[1]<winMin[1] ||\
		(pnt0[0]<winMin[0] && pnt1[0]<winMin[0] && pnt2[0]<winMin[0])||\
		(pnt0[0]>winMax[0] && pnt1[0]>winMax[0] && pnt2[0]>winMax[0]))
	{
		return 0; //the triangle is invisible
	}

	float r0=GetR(color0);
	float g0=GetG(color0);
	float b0=GetB(color0);
	float r1=GetR(color1);
	float g1=GetG(color1);
	float b1=GetB(color1);
	float r2=GetR(color2);
	float g2=GetG(color2);
	float b2=GetB(color2);
#if 0
	double dx_left = double(pnt2[0] - pnt0[0])/double(pnt2[1]-pnt0[1]);
	double dx_right = double(pnt1[0] - pnt0[0])/double(pnt1[1]-pnt0[1]);
	Vec2f xy_left = Vec2f(pnt0[0],pnt0[1]);
	Vec2f xy_right = Vec2f(pnt0[0],pnt0[1]);

	float deltaR_left = float(r2-r0)/float(pnt2[1]-pnt0[1]);
	float deltaG_left = float(g2-g0)/float(pnt2[1]-pnt0[1]);
	float deltaB_left = float(b2-b0)/float(pnt2[1]-pnt0[1]);

	float deltaR_right = float(r1-r0)/float(pnt1[1]-pnt0[1]);
	float deltaG_right= float(g1-g0)/float(pnt1[1]-pnt0[1]);
	float deltaB_right = float(b1-b0)/float(pnt1[1]-pnt0[1]);

	float r_left = r0; float g_left = g0; float b_left = b0;
	float r_right = r0; float g_right = g0; float b_right = b0;

	for(int y=pnt0[1];y<=pnt2[1];)
		//for(int y=minY;y<=maxY;)
	{
		LineClip_LiangBarsky(winMin,winMax,xy_left,xy_right,RGB(r_left,g_left,b_left),RGB(r_right,g_right,b_right),pVertex,pVertexNum);
		//DDA_DrawLine(hDC,xy_left,xy_right,RGB(r_left,g_left,b_left),RGB(r_right,g_right,b_right));
		y+=1;
		xy_left[0]+=dx_left;xy_left[1]=y;
		xy_right[0]+=dx_right;xy_right[1]=y;
		r_left+=deltaR_left; g_left+=deltaG_left;b_left+=deltaB_left;
		r_right+=deltaR_right; g_right+=deltaG_right; b_right+=deltaB_right;
	}
#endif
#if 1
	double x_left,x_right;
	float r_left,r_right,g_left,g_right,b_left,b_right;
	float r_step,g_step,b_step,r,g,b;
	for(int y=pnt0[1];y<=pnt2[1];y++)
	{
		x_left =pnt0[0]+double((y-pnt0[1])*(pnt2[0] - pnt0[0]))/double(pnt2[1]-pnt0[1]);
		x_right =pnt0[0]+double((y-pnt0[1])*(pnt1[0] - pnt0[0]))/double(pnt1[1]-pnt0[1]);

		r_left=r0+double((y-pnt0[1])*(r2 - r0))/double(pnt2[1]-pnt0[1]);
		r_right=r0+double((y-pnt0[1])*(r1- r0))/double(pnt1[1]-pnt0[1]);

		g_left=g0+double((y-pnt0[1])*(g2 - g0))/double(pnt2[1]-pnt0[1]);
		g_right=g0+double((y-pnt0[1])*(g1- g0))/double(pnt1[1]-pnt0[1]);

		b_left=b0+double((y-pnt0[1])*(b2 - b0))/double(pnt2[1]-pnt0[1]);
		b_right=b0+double((y-pnt0[1])*(b1- b0))/double(pnt1[1]-pnt0[1]);

		if(x_left==x_right)
		{
			r_step=0.0;
			g_step=0.0;
			b_step=0.0;
		}else
		{
			r_step=double(r_right-r_left)/double(x_right-x_left);
			g_step=double(g_right-g_left)/double(x_right-x_left);
			b_step=double(b_right-b_left)/double(x_right-x_left);
		}

		for(double x=x_left,r=r_left,g=g_left,b=b_left;
			x<=x_right+1;//for accumulated error,so plus 1
			x++,r+=r_step,g+=g_step,b+=b_step)
		{
			if(r>255.0){r=255.0;}
			if(g>255.0){g=255.0;}
			if(b>255.0){b=255.0;}
			if(x>=winMin[0] && x<=winMax[0] && y>=winMin[1] && y<=winMax[1])
			{
				//pVertex[(*pVertexNum)++]=VTYPE(r/255.0,g/255.0,b/255.0,x,y,0.0);
				pVertex[(*pVertexNum)++]=VTYPE(GLubyte(r),GLubyte(g),GLubyte(b),GLubyte(0.0),x,y);
			}

		}
	}
#endif
}
bool Triangle::draw_flat_bottom_triangle(Vec2i p0,Vec2i p1,Vec2i p2,\
										 COLORREF v0_color,COLORREF v1_color,COLORREF v2_color,\
										 Vec2f winMin,Vec2f winMax,VTYPE *pVertex,long *pVertexNum)
{
	//int minY=p1[1];
	//int maxY=p2[1];
	//pnt0,pnt1,pnt2 are ordered by clock wise
	Vec2i pnt0=p0;
	Vec2i pnt1=p1;
	Vec2i pnt2=p2;
	COLORREF color0=v0_color;
	COLORREF color1=v1_color;
	COLORREF color2=v2_color;

	if (pnt0[1] == pnt1[1])
	{
		//flat bottom triangle
	}else if(pnt0[1] == pnt2[1])
	{
		swap_pnt(pnt2,pnt1);
		swap_color(color2,color1);
	}else if(pnt1[1]==pnt2[1])
	{
		swap_pnt(pnt0,pnt2);
		swap_color(color0,color2);
	}else
	{
		return 0;//not flat bottom triangle
	}

	if (pnt1[0]<pnt0[0])
	{
		swap_pnt(pnt1,pnt0);
		swap_color(color1,color0);
	}else if (pnt1[0]==pnt0[0])
	{
		return 0; //not a triangle
	}

	//clip the triangle
	if(pnt0[1]>winMax[1] || pnt2[1]<winMin[1] ||\
		(pnt0[0]<winMin[0] && pnt1[0]<winMin[0] && pnt2[0]<winMin[0])||\
		(pnt0[0]>winMax[0] && pnt1[0]>winMax[0] && pnt2[0]>winMax[0]))
	{
		return 0; //the triangle is invisible
	}

	float r0=GetR(color0);
	float g0=GetG(color0);
	float b0=GetB(color0);
	float r1=GetR(color1);
	float g1=GetG(color1);
	float b1=GetB(color1);
	float r2=GetR(color2);
	float g2=GetG(color2);
	float b2=GetB(color2);
#if 0
	double dx_left = double(pnt2[0] - pnt0[0])/double(pnt2[1]-pnt0[1]);
	double dx_right = double(pnt2[0] - pnt1[0])/double(pnt2[1]-pnt1[1]);
	Vec2f xy_left = Vec2f(pnt0[0],pnt0[1]);
	Vec2f xy_right = Vec2f(pnt1[0],pnt1[1]);

	float deltaR_left = float(r2-r0)/float(pnt2[1]-pnt0[1]);
	float deltaG_left = float(g2-g0)/float(pnt2[1]-pnt0[1]);
	float deltaB_left = float(b2-b0)/float(pnt2[1]-pnt0[1]);

	float deltaR_right = float(r2-r1)/float(pnt2[1]-pnt1[1]);
	float deltaG_right= float(g2-g1)/float(pnt2[1]-pnt1[1]);
	float deltaB_right = float(b2-b1)/float(pnt2[1]-pnt1[1]);

	float r_left = r0; float g_left = g0; float b_left = b0;
	float r_right = r1; float g_right = g1; float b_right = b1;

	for(int y=pnt0[1];y<=pnt2[1];)
		//for(int y=minY;y<=maxY;)
	{
		//DDA_DrawLine(hDC,xy_left,xy_right,tri_flat_color,tri_flat_color);
		LineClip_LiangBarsky(winMin,winMax,xy_left,xy_right,RGB(r_left,g_left,b_left),RGB(r_right,g_right,b_right),pVertex,pVertexNum);
		//DDA_DrawLine(hDC,xy_left,xy_right,RGB(r_left,g_left,b_left),RGB(r_right,g_right,b_right));
		y+=1;
		xy_left[0]+=dx_left; xy_left[1]=y;
		xy_right[0]+=dx_right; xy_right[1]=y;
		r_left+=deltaR_left; g_left+=deltaG_left;b_left+=deltaB_left;
		r_right+=deltaR_right; g_right+=deltaG_right; b_right+=deltaB_right;
	}
#endif
#if 1
	double x_left,x_right;
	float r_left,r_right,g_left,g_right,b_left,b_right;
	float r_step,g_step,b_step,r,g,b;
	for(int y=pnt0[1];y<=pnt2[1];y++)
	{
		x_left =pnt0[0]+double((y-pnt0[1])*(pnt2[0] - pnt0[0]))/double(pnt2[1]-pnt0[1]);
		x_right =pnt1[0]+double((y-pnt1[1])*(pnt2[0] - pnt1[0]))/double(pnt2[1]-pnt1[1]);

		r_left=r0+double((y-pnt0[1])*(r2 - r0))/double(pnt2[1]-pnt0[1]);
		r_right=r1+double((y-pnt1[1])*(r2- r1))/double(pnt2[1]-pnt1[1]);

		g_left=g0+double((y-pnt0[1])*(g2 - g0))/double(pnt2[1]-pnt0[1]);
		g_right=g1+double((y-pnt1[1])*(g2- g1))/double(pnt2[1]-pnt1[1]);

		b_left=b0+double((y-pnt0[1])*(b2 - b0))/double(pnt2[1]-pnt0[1]);
		b_right=b1+double((y-pnt1[1])*(b2- b1))/double(pnt2[1]-pnt1[1]);

		if(x_left==x_right)
		{
			r_step=0.0;
			g_step=0.0;
			b_step=0.0;
		}else
		{
			r_step=double(r_right-r_left)/double(x_right-x_left);
			g_step=double(g_right-g_left)/double(x_right-x_left);
			b_step=double(b_right-b_left)/double(x_right-x_left);
		}

		for(double x=x_left,r=r_left,g=g_left,b=b_left;
			x<=x_right+1;//for accumulated error,so plus 1
			x++,r+=r_step,g+=g_step,b+=b_step)
		{
			if(r>255.0){r=255.0;}
			if(g>255.0){g=255.0;}
			if(b>255.0){b=255.0;}
			if(x>=winMin[0] && x<=winMax[0] && y>=winMin[1] && y<=winMax[1])
			{
				//pVertex[(*pVertexNum)++]=VTYPE(r/255.0,g/255.0,b/255.0,x,y,0.0);
				pVertex[(*pVertexNum)++]=VTYPE(GLubyte(r),GLubyte(g),GLubyte(b),GLubyte(0.0),x,y);
			}		
		}
	}

#endif
}
bool Triangle::draw_triangle_gouraud(Vec2f winMin,Vec2f winMax)
{
	double maxX,minX,maxY,minY;
	Vec2i pnt0=Vec2i(pnt_trans[0].v[0],pnt_trans[0].v[1]);
	Vec2i pnt1=Vec2i(pnt_trans[1].v[0],pnt_trans[1].v[1]);
	Vec2i pnt2=Vec2i(pnt_trans[2].v[0],pnt_trans[2].v[1]);
	COLORREF color0=vertex_color[0];
	COLORREF color1=vertex_color[1];
	COLORREF color2=vertex_color[2];
	COLORREF color_intersect;

	float r0=GetR(color0);
	float g0=GetG(color0);
	float b0=GetB(color0);
	float r1=GetR(color1);
	float g1=GetG(color1);
	float b1=GetB(color1);
	float r2=GetR(color2);
	float g2=GetG(color2);
	float b2=GetB(color2);

	Vec2i a=pnt1-pnt0;
	Vec2i b=pnt2-pnt0;

	if (a[0]*b[1]-a[1]*b[0]==0)
	{
		return 0;// not a triangle
	}

	if (pnt0[1]>pnt1[1])
	{
		swap_pnt(pnt0,pnt1);
		swap_color(color0,color1);
	}
	if(pnt0[1]>pnt2[1])
	{
		swap_pnt(pnt0,pnt2);
		swap_color(color0,color2);
	}
	if(pnt1[1]>pnt2[1])
	{
		swap_pnt(pnt1,pnt2);
		swap_color(color1,color2);
	}
	//clip the triangle
	if(pnt0[1]>winMax[1] || pnt2[1]<winMin[1] ||\
		(pnt0[0]<winMin[0] && pnt1[0]<winMin[0] && pnt2[0]<winMin[0])||\
		(pnt0[0]>winMax[0] && pnt1[0]>winMax[0] && pnt2[0]>winMax[0]))
	{
		return 0; //the triangle is invisible
	}
	//calculate the bounding box of the triangle
	minY=pnt0[1];maxY=pnt2[1];
	minX=maxX=pnt0[0];
	if(minX>pnt1[0])
	{
		minX=pnt1[0];
	}
	if (minX>pnt2[0])
	{
		minX=pnt2[0];
	}
	if(maxX<pnt1[0])
	{
		maxX=pnt1[0];
	}
	if (maxX<pnt2[0])
	{
		maxX=pnt2[0];
	}
	//////////
	long nWidth=abs(maxX-minX)+2;
	long nHeight=abs(maxY-minY)+2;
	long nVertexNum=0;
	VTYPE *pVertex=new VTYPE[nWidth*nHeight];

#ifdef _DEBUG
	if (pVertex == NULL)
	{
		system("pause");
	}
#endif

	if (pnt0[1] == pnt1[1])
	{
		draw_flat_bottom_triangle(pnt0,pnt1,pnt2,color0,color1,color2,winMin,winMax,pVertex,&nVertexNum);
	}else if (pnt1[1]==pnt2[1])
	{
		draw_flat_top_triangle(pnt0,pnt1,pnt2,color0,color1,color2,winMin,winMax,pVertex,&nVertexNum);
	}else
	{
		//float x_intersect = pnt0[0]+(pnt1[1]-pnt0[1])*(pnt2[0]-pnt0[0])/(pnt2[1]-pnt0[1]);
		int x_intersect = pnt0[0]+double((pnt1[1]-pnt0[1])*(pnt2[0]-pnt0[0]))/double(pnt2[1]-pnt0[1]);
		Vec2i intersect_pnt(x_intersect,pnt1[1]);

		float r_in = r0+float((pnt1[1]-pnt0[1])*(r2-r0))/float(pnt2[1]-pnt0[1]);
		float g_in = g0+float((pnt1[1]-pnt0[1])*(g2-g0))/float(pnt2[1]-pnt0[1]);
		float b_in = b0+float((pnt1[1]-pnt0[1])*(b2-b0))/float(pnt2[1]-pnt0[1]);
		color_intersect = RGB(r_in,g_in,b_in);

		draw_flat_top_triangle(pnt0,pnt1,intersect_pnt,color0,color1,color_intersect,winMin,winMax,pVertex,&nVertexNum);
		draw_flat_bottom_triangle(intersect_pnt,pnt1,pnt2,color_intersect,color1,color2,winMin,winMax,pVertex,&nVertexNum);
	}

	//glInterleavedArrays(GL_C3F_V3F,0,pVertex);
	glInterleavedArrays(GL_C4UB_V2F,0,pVertex);
	glDrawArrays(GL_POINTS,0,nVertexNum);
	glFlush ();
	if (pVertex!=NULL)
	{
		delete []pVertex;
		pVertex=NULL;
	}
	
}
#endif
bool Triangle::draw_triangle_wireframe(Vec2f winMin,Vec2f winMax)
{
	if(!bHiddenFace)
	{
		
		double maxX,minX,maxY,minY;
		Vec2i pnt0 = Vec2i(pnt_trans[0].v[0],pnt_trans[0].v[1]);
		Vec2i pnt1 = Vec2i(pnt_trans[1].v[0],pnt_trans[1].v[1]);
		Vec2i pnt2 = Vec2i(pnt_trans[2].v[0],pnt_trans[2].v[1]);

		Vec2i a=pnt1-pnt0;
		Vec2i b=pnt2-pnt0;

		if (a[0]*b[1]-a[1]*b[0]==0)
		{
			return 0;// not a triangle
		}

		if (pnt0[1]>pnt1[1])
		{
			swap_pnt(pnt0,pnt1);

		}
		if(pnt0[1]>pnt2[1])
		{
			swap_pnt(pnt0,pnt2);
	
		}
		if(pnt1[1]>pnt2[1])
		{
			swap_pnt(pnt1,pnt2);

		}
		//clip the triangle
		if(pnt0[1]>winMax[1] || pnt2[1]<winMin[1] ||\
			(pnt0[0]<winMin[0] && pnt1[0]<winMin[0] && pnt2[0]<winMin[0])||\
			(pnt0[0]>winMax[0] && pnt1[0]>winMax[0] && pnt2[0]>winMax[0]))
		{
			return 0; //the triangle is invisible
		}

		//calculate the bounding box of the triangle
		minX=maxX=pnt0[0];
		minY=maxY=pnt0[1];
		if(minX>pnt1[0])
		{
			minX=pnt1[0];
		}
		if (minX>pnt2[0])
		{
			minX=pnt2[0];
		}
		if(maxX<pnt1[0])
		{
			maxX=pnt1[0];
		}
		if (maxX<pnt2[0])
		{
			maxX=pnt2[0];
		}
		////////
		if(minY>pnt1[1])
		{
			minY=pnt1[1];
		}
		if (minY>pnt2[1])
		{
			minY=pnt2[1];
		}
		if(maxY<pnt1[1])
		{
			maxY=pnt1[1];
		}
		if (maxY<pnt2[1])
		{
			maxY=pnt2[1];
		}
		/////////////
		long nWidth=abs(maxX-minX)+1;
		long nHeight=abs(maxY-minY)+1;
		long nVertexNum=0;
		VTYPE *pVertex=new VTYPE[nWidth*nHeight];

		//DDA_DrawLine(hDC,Vec2f(pnt_trans[0].v[0],pnt_trans[0].v[1]),Vec2f(pnt_trans[1].v[0],pnt_trans[1].v[1]),tri_color,tri_color);
		//DDA_DrawLine(hDC,Vec2f(pnt_trans[1].v[0],pnt_trans[1].v[1]),Vec2f(pnt_trans[2].v[0],pnt_trans[2].v[1]),tri_color,tri_color);
		//DDA_DrawLine(hDC,Vec2f(pnt_trans[2].v[0],pnt_trans[2].v[1]),Vec2f(pnt_trans[0].v[0],pnt_trans[0].v[1]),tri_color,tri_color);
		LineClip_LiangBarsky(winMin,winMax,Vec2f(pnt0[0],pnt0[1]),Vec2f(pnt1[0],pnt1[1]),tri_color,tri_color,pVertex,&nVertexNum);
		LineClip_LiangBarsky(winMin,winMax,Vec2f(pnt1[0],pnt1[1]),Vec2f(pnt2[0],pnt2[1]),tri_color,tri_color,pVertex,&nVertexNum);
		LineClip_LiangBarsky(winMin,winMax,Vec2f(pnt2[0],pnt2[1]),Vec2f(pnt0[0],pnt0[1]),tri_color,tri_color,pVertex,&nVertexNum);
	
		//glInterleavedArrays(GL_C3F_V3F,0,pVertex);
		glInterleavedArrays(GL_C4UB_V2F,0,pVertex);
		glDrawArrays(GL_POINTS,0,nVertexNum);
		glFlush ();
		if (pVertex!=NULL)
		{
			delete []pVertex;
			pVertex=NULL;
		}
	}
}

bool Triangle::draw_triangle_texture(Vec2f winMin,Vec2f winMax)
{
	double maxX,minX,maxY,minY;
	Vec2i pnt0=Vec2i(pnt_trans[0].v[0],pnt_trans[0].v[1]);
	Vec2i pnt1=Vec2i(pnt_trans[1].v[0],pnt_trans[1].v[1]);
	Vec2i pnt2=Vec2i(pnt_trans[2].v[0],pnt_trans[2].v[1]);
	Vec2f uv0=uv[0];
	Vec2f uv1=uv[1];
	Vec2f uv2=uv[2];

	float z0 = z[0];
	float z1 = z[1];
	float z2 = z[2];

	Vec2i a=pnt1-pnt0;
	Vec2i b=pnt2-pnt0;

	if (a[0]*b[1]-a[1]*b[0]==0)
	{
		return 0;// not a triangle
	}

	if (texture.bih.biWidth!=texture.bih.biHeight)
	{
		return 0;//not a cube texture
	}
	if (pnt0[1]>pnt1[1])
	{
		swap_pnt(pnt0,pnt1);
		//swap_color(color0,color1);
		swap_uv(uv0,uv1);
		swap_z(z0,z1);
	}
	if(pnt0[1]>pnt2[1])
	{
		swap_pnt(pnt0,pnt2);
		//swap_color(color0,color2);
		swap_uv(uv0,uv2);
		swap_z(z0,z2);
	}
	if(pnt1[1]>pnt2[1])
	{
		swap_pnt(pnt1,pnt2);
		//swap_color(color1,color2);
		swap_uv(uv1,uv2);
		swap_z(z1,z2);
	}
	//clip the triangle
	if(pnt0[1]>winMax[1] || pnt2[1]<winMin[1] ||\
		(pnt0[0]<winMin[0] && pnt1[0]<winMin[0] && pnt2[0]<winMin[0])||\
		(pnt0[0]>winMax[0] && pnt1[0]>winMax[0] && pnt2[0]>winMax[0]))
	{
		return 0; //the triangle is invisible
	}

	//calculate the bounding box of the triangle
	minY=pnt0[1];maxY=pnt2[1];
	minX=maxX=pnt0[0];
	if(minX>pnt1[0])
	{
		minX=pnt1[0];
	}
	if (minX>pnt2[0])
	{
		minX=pnt2[0];
	}
	if(maxX<pnt1[0])
	{
		maxX=pnt1[0];
	}
	if (maxX<pnt2[0])
	{
		maxX=pnt2[0];
	}
	//////////
	long nWidth=abs(maxX-minX)+1;
	long nHeight=abs(maxY-minY)+1;
	long nVertexNum=0;
	VTYPE *pVertex=new VTYPE[nWidth*nHeight];

#ifdef _DEBUG
	if (pVertex == NULL)
	{
		system("pause");
	}
#endif

	if (pnt0[1] == pnt1[1])
	{
		draw_flat_top_triangle_texture(pnt0,pnt1,pnt2,uv0,uv1,uv2,z0,z1,z2,winMin,winMax,pVertex,&nVertexNum);
	}else if (pnt1[1]==pnt2[1])
	{
		draw_flat_bottom_triangle_texture(pnt0,pnt1,pnt2,uv0,uv1,uv2,z0,z1,z2,winMin,winMax,pVertex,&nVertexNum);
	}else
	{
		draw_flat_bottom_triangle_texture(pnt0,pnt1,pnt2,uv0,uv1,uv2,z0,z1,z2,winMin,winMax,pVertex,&nVertexNum);
		draw_flat_top_triangle_texture(pnt0,pnt1,pnt2,uv0,uv1,uv2,z0,z1,z2,winMin,winMax,pVertex,&nVertexNum);
	}

	//glInterleavedArrays(GL_C3F_V3F,0,pVertex);
	glInterleavedArrays(GL_C4UB_V2F,0,pVertex);
	glDrawArrays(GL_POINTS,0,nVertexNum);
	glFlush ();
	if (pVertex!=NULL)
	{
		delete []pVertex;
		pVertex=NULL;
	}
}
bool Triangle::draw_flat_bottom_triangle_texture(Vec2i p0,Vec2i p1,Vec2i p2,Vec2f uv0,Vec2f uv1,Vec2f uv2,\
												 float z0,float z1,float z2,Vec2f winMin,Vec2f winMax,VTYPE *pVertex,long *pVertexNum)
{
	long minY =	p0[1];
	long maxY = p1[1];
	double x_intersect = p0[0]+double((p1[1]-p0[1])*(p2[0]-p0[0]))/double(p2[1]-p0[1]);
	double x_left,x_right; 
	double oneoverz_left, oneoverz_right; 
	double oneoverz,oneoverz_step;   
	double soverz_left, soverz_right; 
	double toverz_left, toverz_right; 
	double soverz, soverz_step; 
	double toverz, toverz_step; 
	double s, t; 
	int cubeEdgeSize;
	cubeEdgeSize=texture.bih.biHeight;

	for(double y=minY;y<maxY;y++)
	{
		if(x_intersect<p1[0])
		{
			x_left = p0[0]+double((y-p0[1])*(p2[0]-p0[0]))/double(p2[1]-p0[1]);
			x_right = p0[0]+double((y-p0[1])*(p1[0]-p0[0]))/double(p1[1]-p0[1]);

			oneoverz_left=1.0/z0 + double((1.0/z2 - 1.0/z0)*(y-p0[1]))/double(p2[1]-p0[1]);
			oneoverz_right=1.0/z0 + double((1.0/z1 - 1.0/z0)*(y-p0[1]))/double(p1[1]-p0[1]);
		
			soverz_left = uv0[0]/z0+double((y-p0[1])*(uv2[0]/z2-uv0[0]/z0))/double(p2[1]-p0[1]);
			soverz_right=uv0[0]/z0+double((y-p0[1])*(uv1[0]/z1-uv0[0]/z0))/double(p1[1]-p0[1]);
			
			toverz_left = uv0[1]/z0+double((y-p0[1])*(uv2[1]/z2-uv0[1]/z0))/double(p2[1]-p0[1]);
			toverz_right=uv0[1]/z0+double((y-p0[1])*(uv1[1]/z1-uv0[1]/z0))/double(p1[1]-p0[1]);
			
			if (x_right==x_left)
			{
				oneoverz_step=0;
				soverz_step=0;
				toverz_step=0;
			} 
			else
			{
				oneoverz_step = double(oneoverz_right-oneoverz_left)/double(x_right-x_left);
				soverz_step=double(soverz_right-soverz_left)/double(x_right-x_left);
				toverz_step=double(toverz_right-toverz_left)/double(x_right-x_left);
			}

		}else 
		{
			x_right = p0[0]+double((y-p0[1])*(p2[0]-p0[0]))/double(p2[1]-p0[1]);
			x_left = p0[0]+double((y-p0[1])*(p1[0]-p0[0]))/double(p1[1]-p0[1]);
			oneoverz_right=1.0/z0 + double((1.0/z2 - 1.0/z0)*(y-p0[1]))/double(p2[1]-p0[1]);
			oneoverz_left=1.0/z0 + double((1.0/z1 - 1.0/z0)*(y-p0[1]))/double(p1[1]-p0[1]);
			
			soverz_right = uv0[0]/z0+double((y-p0[1])*(uv2[0]/z2-uv0[0]/z0))/double(p2[1]-p0[1]);
			soverz_left=uv0[0]/z0+double((y-p0[1])*(uv1[0]/z1-uv0[0]/z0))/double(p1[1]-p0[1]);
			
			toverz_right = uv0[1]/z0+double((y-p0[1])*(uv2[1]/z2-uv0[1]/z0))/double(p2[1]-p0[1]);
			toverz_left=uv0[1]/z0+double((y-p0[1])*(uv1[1]/z1-uv0[1]/z0))/double(p1[1]-p0[1]);
			

			if (x_right==x_left)
			{
				oneoverz_step=0;
				soverz_step=0;
				toverz_step=0;
			} else
			{
				oneoverz_step = double(oneoverz_right-oneoverz_left)/double(x_right-x_left);
				soverz_step=double(soverz_right-soverz_left)/double(x_right-x_left);
				toverz_step=double(toverz_right-toverz_left)/double(x_right-x_left);
			}
		
		}
		for(double x=x_left,oneoverz=oneoverz_left,soverz=soverz_left,toverz=toverz_left;
			x<(x_right);x++,oneoverz+=oneoverz_step,soverz+=soverz_step,toverz+=toverz_step)
		{
			assert(oneoverz!=0);
			s=soverz/oneoverz;
			t=toverz/oneoverz;
			//currently only support cube texture mapping

			if (s>cubeEdgeSize && fabsf(s-cubeEdgeSize)<1.0)
			{
				s=cubeEdgeSize;
			} 
			if(t>cubeEdgeSize&& fabsf(t-cubeEdgeSize)<1.0)
			{
				t=cubeEdgeSize;
			}
			if (s<0&&fabsf(s)<1.0)
			{
				s=0.0;
			}
			if (t<0&&fabsf(t)<1.0)
			{
				t=0.0;
			}
			assert(s>=0 && s<=cubeEdgeSize && t>=0 && t<=cubeEdgeSize);
			COLORREF rgb = get_texture_color(s+0.5,t+0.5);
			float r,g,b;
			if(x>=winMin[0] && x<=winMax[0] && y>=winMin[1] && y<=winMax[1])
			{
				r=GetR(rgb);
				g=GetG(rgb);
				b=GetB(rgb);
				//glColor3f(r/255.0,g/255.0,b/255.0);
				//glVertex2d(x+0.5,y+0.5);
				//pVertex[(*pVertexNum)++]=VTYPE(r/255.0,g/255.0,b/255.0,x+0.5,y+0.5,0.0);
				pVertex[(*pVertexNum)++]=VTYPE(GLubyte(r),GLubyte(g),GLubyte(b),GLubyte(0.0),x,y);
				//SetPixelV(hDC,int(x+0.5),int(y+0.5),rgb);
			}
		}
	}
	return 1;
}
bool Triangle::draw_flat_top_triangle_texture(Vec2i p0,Vec2i p1,Vec2i p2,Vec2f uv0,Vec2f uv1,Vec2f uv2,float z0,float z1,float z2,\
											  Vec2f winMin,Vec2f winMax,VTYPE *pVertex,long *pVertexNum)
{
	long minY =	p1[1];
	long maxY = p2[1];
	double x_intersect = p0[0]+double((p1[1]-p0[1])*(p2[0]-p0[0]))/double(p2[1]-p0[1]);
	double x_left,x_right; 
	double oneoverz_left, oneoverz_right; 
	double oneoverz,oneoverz_step;   
	double soverz_left, soverz_right; 
	double toverz_left, toverz_right; 
	double soverz, soverz_step; 
	double toverz, toverz_step; 
	double s, t; 
	int cubeEdgeSize;
	cubeEdgeSize=texture.bih.biHeight;

	for(double y=minY;y<maxY;y++)
	{
		if(x_intersect<p1[0])
		{
			x_left = p0[0]+double((y-p0[1])*(p2[0]-p0[0]))/double(p2[1]-p0[1]);
			x_right = p1[0]+double((y-p1[1])*(p2[0]-p1[0]))/double(p2[1]-p1[1]);
			oneoverz_left=1.0/z0 + double((1.0/z2 - 1.0/z0)*(y-p0[1]))/double(p2[1]-p0[1]);
			oneoverz_right=1.0/z1 + double((1.0/z2 - 1.0/z1)*(y-p1[1]))/double(p2[1]-p1[1]);
			
			soverz_left = uv0[0]/z0+double((y-p0[1])*(uv2[0]/z2-uv0[0]/z0))/double(p2[1]-p0[1]);
			soverz_right=uv1[0]/z1+double((y-p1[1])*(uv2[0]/z2-uv1[0]/z1))/double(p2[1]-p1[1]);
			
			toverz_left = uv0[1]/z0+double((y-p0[1])*(uv2[1]/z2-uv0[1]/z0))/double(p2[1]-p0[1]);
			toverz_right=uv1[1]/z1+double((y-p1[1])*(uv2[1]/z2-uv1[1]/z1))/double(p2[1]-p1[1]);
			
		
			if (x_right==x_left)
			{
				oneoverz_step=0;
				soverz_step=0;
				toverz_step=0;
			} else
			{
				oneoverz_step = double(oneoverz_right-oneoverz_left)/double(x_right-x_left);
				soverz_step=double(soverz_right-soverz_left)/double(x_right-x_left);
				toverz_step=double(toverz_right-toverz_left)/double(x_right-x_left);
				
			}

		}else 
		{
			x_right = p0[0]+double((y-p0[1])*(p2[0]-p0[0]))/double(p2[1]-p0[1]);
			x_left = p1[0]+double((y-p1[1])*(p2[0]-p1[0]))/double(p2[1]-p1[1]);
			oneoverz_right=1.0/z0 + double((1.0/z2 - 1.0/z0)*(y-p0[1]))/double(p2[1]-p0[1]);
			oneoverz_left=1.0/z1 + double((1.0/z2 - 1.0/z1)*(y-p1[1]))/double(p2[1]-p1[1]);
			
			soverz_right = uv0[0]/z0+double((y-p0[1])*(uv2[0]/z2-uv0[0]/z0))/double(p2[1]-p0[1]);
			soverz_left=uv1[0]/z1+double((y-p1[1])*(uv2[0]/z2-uv1[0]/z1))/double(p2[1]-p1[1]);
			
			toverz_right = uv0[1]/z0+double((y-p0[1])*(uv2[1]/z2-uv0[1]/z0))/double(p2[1]-p0[1]);
			toverz_left=uv1[1]/z1+double((y-p1[1])*(uv2[1]/z2-uv1[1]/z1))/double(p2[1]-p1[1]);
			

			if (x_right==x_left)
			{
				oneoverz_step=0;
				soverz_step=0;
				toverz_step=0;
			}else
			{
				oneoverz_step = double(oneoverz_right-oneoverz_left)/double(x_right-x_left);
				soverz_step=double(soverz_right-soverz_left)/double(x_right-x_left);
				toverz_step=double(toverz_right-toverz_left)/double(x_right-x_left);			
			}
		
		}
		for(double x=x_left,oneoverz=oneoverz_left,soverz=soverz_left,toverz=toverz_left;
			x<(x_right);x++,oneoverz+=oneoverz_step,soverz+=soverz_step,toverz+=toverz_step)
		{
			assert(oneoverz!=0);
			s=soverz/oneoverz;
			t=toverz/oneoverz;
			if (s>cubeEdgeSize && fabsf(s-cubeEdgeSize)<1.0)
			{
				s=cubeEdgeSize;
			} 
			if(t>cubeEdgeSize&& fabsf(t-cubeEdgeSize)<1.0)
			{
				t=cubeEdgeSize;
			}
			if (s<0&&fabsf(s)<1.0)
			{
				s=0.0;
			}
			if (t<0&&fabsf(t)<1.0)
			{
				t=0.0;
			}

			assert(s>=0 && s<=cubeEdgeSize && t>=0 && t<=cubeEdgeSize);
			COLORREF rgb = get_texture_color(s+0.5,t+0.5);
			float r,g,b;
			if(x>=winMin[0] && x<=winMax[0] && y>=winMin[1] && y<=winMax[1])
			{
				r=GetR(rgb);
				g=GetG(rgb);
				b=GetB(rgb);
				//glColor3f(r/255.0,g/255.0,b/255.0);
				//glVertex2d(x+0.5,y+0.5);
				//pVertex[(*pVertexNum)++]=VTYPE(r/255.0,g/255.0,b/255.0,x+0.5,y+0.5,0.0);
				pVertex[(*pVertexNum)++]=VTYPE(GLubyte(r),GLubyte(g),GLubyte(b),GLubyte(0.0),x,y);
				//SetPixelV(hDC,int(x+0.5),int(y+0.5),rgb);
			}
		}
	}
	return 1;
}


bool Triangle::draw_triangle_normal(Material *pMaterial, Camera *pCamera,Light *pLights,int nLightNum,Vec2f winMin,Vec2f winMax)
{
	double maxX,minX,maxY,minY;
	Vec2i pnt0=Vec2i(pnt_trans[0].v[0],pnt_trans[0].v[1]);
	Vec2i pnt1=Vec2i(pnt_trans[1].v[0],pnt_trans[1].v[1]);
	Vec2i pnt2=Vec2i(pnt_trans[2].v[0],pnt_trans[2].v[1]);
	Vec3 normal0=Vec3(pnt_trans[0].n[0],pnt_trans[0].n[1],pnt_trans[0].n[2]);
	Vec3 normal1=Vec3(pnt_trans[1].n[0],pnt_trans[1].n[1],pnt_trans[1].n[2]);
	Vec3 normal2=Vec3(pnt_trans[2].n[0],pnt_trans[2].n[1],pnt_trans[2].n[2]);

	float z0 = z[0];
	float z1 = z[1];
	float z2 = z[2];

	Vec2i a=pnt1-pnt0;
	Vec2i b=pnt2-pnt0;

	if (a[0]*b[1]-a[1]*b[0]==0)
	{
		return 0;// not a triangle
	}

	if (pnt0[1]>pnt1[1])
	{
		swap_pnt(pnt0,pnt1);
		swap_normal(normal0,normal1);
		swap_z(z0,z1);
	}
	if(pnt0[1]>pnt2[1])
	{
		swap_pnt(pnt0,pnt2);
		swap_normal(normal0,normal2);
		swap_z(z0,z2);
	}
	if(pnt1[1]>pnt2[1])
	{
		swap_pnt(pnt1,pnt2);
		swap_normal(normal1,normal2);
		swap_z(z1,z2);
	}
	//clip the triangle
	if(pnt0[1]>winMax[1] || pnt2[1]<winMin[1] ||\
		(pnt0[0]<winMin[0] && pnt1[0]<winMin[0] && pnt2[0]<winMin[0])||\
		(pnt0[0]>winMax[0] && pnt1[0]>winMax[0] && pnt2[0]>winMax[0]))
	{
		return 0; //the triangle is invisible
	}

	//calculate the bounding box of the triangle
	minY=pnt0[1];maxY=pnt2[1];
	minX=maxX=pnt0[0];
	if(minX>pnt1[0])
	{
		minX=pnt1[0];
	}
	if (minX>pnt2[0])
	{
		minX=pnt2[0];
	}
	if(maxX<pnt1[0])
	{
		maxX=pnt1[0];
	}
	if (maxX<pnt2[0])
	{
		maxX=pnt2[0];
	}
	//////////
	long nWidth=abs(maxX-minX)+1;
	long nHeight=abs(maxY-minY)+1;
	long nVertexNum=0;
	VTYPE *pVertex=new VTYPE[nWidth*nHeight];

#ifdef _DEBUG
	if (pVertex == NULL)
	{
		system("pause");
	}
#endif

	if (pnt0[1] == pnt1[1])
	{
		draw_flat_top_triangle_normal(pnt0,pnt1,pnt2,normal0,normal1,normal2,z0,z1,z2,pMaterial,pCamera,pLights,nLightNum,winMin,winMax,pVertex,&nVertexNum);
	}else if (pnt1[1]==pnt2[1])
	{
		draw_flat_bottom_triangle_normal(pnt0,pnt1,pnt2,normal0,normal1,normal2,z0,z1,z2,pMaterial,pCamera,pLights,nLightNum,winMin,winMax,pVertex,&nVertexNum);
	}else
	{
		draw_flat_bottom_triangle_normal(pnt0,pnt1,pnt2,normal0,normal1,normal2,z0,z1,z2,pMaterial,pCamera,pLights,nLightNum,winMin,winMax,pVertex,&nVertexNum);
		draw_flat_top_triangle_normal(pnt0,pnt1,pnt2,normal0,normal1,normal2,z0,z1,z2,pMaterial,pCamera,pLights,nLightNum,winMin,winMax,pVertex,&nVertexNum);
	}

	//glInterleavedArrays(GL_C3F_V3F,0,pVertex);
	glInterleavedArrays(GL_C4UB_V2F,0,pVertex);
	glDrawArrays(GL_POINTS,0,nVertexNum);
	glFlush ();
	if (pVertex!=NULL)
	{
		delete []pVertex;
		pVertex=NULL;
	}
}
bool Triangle::draw_flat_bottom_triangle_normal(Vec2i p0,Vec2i p1,Vec2i p2,Vec3 normal0,Vec3 normal1,Vec3 normal2,float z0,float z1,float z2,\
																				Material *pMaterial, Camera *pCamera,Light *pLights,int nLightNum,Vec2f winMin,Vec2f winMax,VTYPE *pVertex,long *pVertexNum)
{
	long minY =	p0[1];
	long maxY = p1[1];
	double x_intersect = p0[0]+double((p1[1]-p0[1])*(p2[0]-p0[0]))/double(p2[1]-p0[1]);
	double x_left,x_right; 
	double oneoverz_left, oneoverz_right; 
	double oneoverz,oneoverz_step;   
	double nxoverz_left, nxoverz_right; 
	double nyoverz_left, nyoverz_right; 
	double nzoverz_left,nzoverz_right;
	double nxoverz, nxoverz_step; 
	double nyoverz, nyoverz_step; 
	double nzoverz, nzoverz_step; 

	double nx, ny,nz; 

	for(double y=minY;y<maxY;y++)
	{
		if(x_intersect<p1[0])
		{
			x_left = p0[0]+double((y-p0[1])*(p2[0]-p0[0]))/double(p2[1]-p0[1]);
			x_right = p0[0]+double((y-p0[1])*(p1[0]-p0[0]))/double(p1[1]-p0[1]);

			oneoverz_left=1.0/z0 + double((1.0/z2 - 1.0/z0)*(y-p0[1]))/double(p2[1]-p0[1]);
			oneoverz_right=1.0/z0 + double((1.0/z1 - 1.0/z0)*(y-p0[1]))/double(p1[1]-p0[1]);

			nxoverz_left = normal0[0]/z0+double((y-p0[1])*(normal2[0]/z2-normal0[0]/z0))/double(p2[1]-p0[1]);
			nxoverz_right=normal0[0]/z0+double((y-p0[1])*(normal1[0]/z1-normal0[0]/z0))/double(p1[1]-p0[1]);

			nyoverz_left = normal0[1]/z0+double((y-p0[1])*(normal2[1]/z2-normal0[1]/z0))/double(p2[1]-p0[1]);
			nyoverz_right=normal0[1]/z0+double((y-p0[1])*(normal1[1]/z1-normal0[1]/z0))/double(p1[1]-p0[1]);

			nzoverz_left = normal0[2]/z0+double((y-p0[1])*(normal2[2]/z2-normal0[2]/z0))/double(p2[1]-p0[1]);
			nzoverz_right=normal0[2]/z0+double((y-p0[1])*(normal1[2]/z1-normal0[2]/z0))/double(p1[1]-p0[1]);

			if (x_right==x_left)
			{
				oneoverz_step=0;
				nxoverz_step=0;
				nyoverz_step=0;
				nzoverz_step=0;
			} 
			else
			{
				oneoverz_step = double(oneoverz_right-oneoverz_left)/double(x_right-x_left);
				nxoverz_step=double(nxoverz_right-nxoverz_left)/double(x_right-x_left);
				nyoverz_step=double(nyoverz_right-nyoverz_left)/double(x_right-x_left);
				nzoverz_step=double(nzoverz_right-nzoverz_left)/double(x_right-x_left);
			}

		}else 
		{
			x_right = p0[0]+double((y-p0[1])*(p2[0]-p0[0]))/double(p2[1]-p0[1]);
			x_left = p0[0]+double((y-p0[1])*(p1[0]-p0[0]))/double(p1[1]-p0[1]);
			oneoverz_right=1.0/z0 + double((1.0/z2 - 1.0/z0)*(y-p0[1]))/double(p2[1]-p0[1]);
			oneoverz_left=1.0/z0 + double((1.0/z1 - 1.0/z0)*(y-p0[1]))/double(p1[1]-p0[1]);

			nxoverz_right = normal0[0]/z0+double((y-p0[1])*(normal2[0]/z2-normal0[0]/z0))/double(p2[1]-p0[1]);
			nxoverz_left=normal0[0]/z0+double((y-p0[1])*(normal1[0]/z1-normal0[0]/z0))/double(p1[1]-p0[1]);

			nyoverz_right = normal0[1]/z0+double((y-p0[1])*(normal2[1]/z2-normal0[1]/z0))/double(p2[1]-p0[1]);
			nyoverz_left=normal0[1]/z0+double((y-p0[1])*(normal1[1]/z1-normal0[1]/z0))/double(p1[1]-p0[1]);

			nzoverz_right = normal0[2]/z0+double((y-p0[1])*(normal2[2]/z2-normal0[2]/z0))/double(p2[1]-p0[1]);
			nzoverz_left=normal0[2]/z0+double((y-p0[1])*(normal1[2]/z1-normal0[2]/z0))/double(p1[1]-p0[1]);


			if (x_right==x_left)
			{
				oneoverz_step=0;
				nxoverz_step=0;
				nyoverz_step=0;
				nzoverz_step=0;
			} else
			{
				oneoverz_step = double(oneoverz_right-oneoverz_left)/double(x_right-x_left);
				nxoverz_step=double(nxoverz_right-nxoverz_left)/double(x_right-x_left);
				nyoverz_step=double(nyoverz_right-nyoverz_left)/double(x_right-x_left);
				nzoverz_step=double(nzoverz_right-nzoverz_left)/double(x_right-x_left);
			}

		}
		for(double x=x_left,oneoverz=oneoverz_left,nxoverz=nxoverz_left,nyoverz=nyoverz_left,nzoverz=nzoverz_left;
			x<(x_right);x++,oneoverz+=oneoverz_step,nxoverz+=nxoverz_step,nyoverz+=nyoverz_step,nzoverz+=nzoverz_step)
		{
			double x_vc=0.0;
			double y_vc=0.0;
			double z_vc=0.0;
			assert(oneoverz!=0);
			nx=nxoverz/oneoverz;
			ny=nyoverz/oneoverz;
			nz=nzoverz/oneoverz;
			z_vc=1.0/oneoverz;
			Vec3 N=Vec3(nx,ny,nz);
			N.normalize();

			float r,g,b;
			if(x>=winMin[0] && x<=winMax[0] && y>=winMin[1] && y<=winMax[1])
			{
				x_vc=(2*x*(-z_vc)*(tan(pCamera->fov/2)*(pCamera->aspect_ratio)))/(pCamera->viewport_width);
				y_vc=(2*y*(-z_vc)*tan(pCamera->fov/2))/(pCamera->viewport_hight);
				Vec3 pnt_vc = Vec3(x_vc,y_vc,z_vc);
				DWORD rgb=GetPixelColor_BlinnPhongShading(pnt_vc,N,pMaterial,pCamera,pLights,nLightNum);
				r=GetR(rgb);
				g=GetG(rgb);
				b=GetB(rgb);
				//glColor3f(r/255.0,g/255.0,b/255.0);
				//glVertex2d(x+0.5,y+0.5);
				//pVertex[(*pVertexNum)++]=VTYPE(r/255.0,g/255.0,b/255.0,x+0.5,y+0.5,0.0);
				pVertex[(*pVertexNum)++]=VTYPE(GLubyte(r),GLubyte(g),GLubyte(b),GLubyte(0.0),x,y);
				//SetPixelV(hDC,int(x+0.5),int(y+0.5),rgb);
			}
		}
	}
	return 1;
}
bool Triangle::draw_flat_top_triangle_normal(Vec2i p0,Vec2i p1,Vec2i p2,Vec3 normal0,Vec3 normal1,Vec3 normal2,float z0,float z1,float z2,
											Material *pMaterial, Camera *pCamera,Light *pLights,int nLightNum,Vec2f winMin,Vec2f winMax,VTYPE *pVertex,long *pVertexNum)
{
	long minY =	p1[1];
	long maxY = p2[1];
	double x_intersect = p0[0]+double((p1[1]-p0[1])*(p2[0]-p0[0]))/double(p2[1]-p0[1]);
	double x_left,x_right; 
	double oneoverz_left, oneoverz_right; 
	double oneoverz_top, oneoverz_bottom; 
	double oneoverz,oneoverz_step;   
	double nxoverz_left, nxoverz_right; 
	double nyoverz_left, nyoverz_right; 
	double nzoverz_left, nzoverz_right; 
	double nxoverz, nxoverz_step; 
	double nyoverz, nyoverz_step; 
	double nzoverz, nzoverz_step; 

	double nx, ny,nz; 


	for(double y=minY;y<maxY;y++)
	{
		if(x_intersect<p1[0])
		{
			x_left = p0[0]+double((y-p0[1])*(p2[0]-p0[0]))/double(p2[1]-p0[1]);
			x_right = p1[0]+double((y-p1[1])*(p2[0]-p1[0]))/double(p2[1]-p1[1]);
			oneoverz_left=1.0/z0 + double((1.0/z2 - 1.0/z0)*(y-p0[1]))/double(p2[1]-p0[1]);
			oneoverz_right=1.0/z1 + double((1.0/z2 - 1.0/z1)*(y-p1[1]))/double(p2[1]-p1[1]);

			nxoverz_left = normal0[0]/z0+double((y-p0[1])*(normal2[0]/z2-normal0[0]/z0))/double(p2[1]-p0[1]);
			nxoverz_right=normal1[0]/z1+double((y-p1[1])*(normal2[0]/z2-normal1[0]/z1))/double(p2[1]-p1[1]);

			nyoverz_left = normal0[1]/z0+double((y-p0[1])*(normal2[1]/z2-normal0[1]/z0))/double(p2[1]-p0[1]);
			nyoverz_right=normal1[1]/z1+double((y-p1[1])*(normal2[1]/z2-normal1[1]/z1))/double(p2[1]-p1[1]);

			nzoverz_left = normal0[2]/z0+double((y-p0[1])*(normal2[2]/z2-normal0[2]/z0))/double(p2[1]-p0[1]);
			nzoverz_right=normal1[2]/z1+double((y-p1[1])*(normal2[2]/z2-normal1[2]/z1))/double(p2[1]-p1[1]);

			if (x_right==x_left)
			{
				oneoverz_step=0;
				nxoverz_step=0;
				nyoverz_step=0;
				nzoverz_step=0;
			} else
			{
				oneoverz_step = double(oneoverz_right-oneoverz_left)/double(x_right-x_left);
				nxoverz_step=double(nxoverz_right-nxoverz_left)/double(x_right-x_left);
				nyoverz_step=double(nyoverz_right-nyoverz_left)/double(x_right-x_left);
				nzoverz_step=double(nzoverz_right-nzoverz_left)/double(x_right-x_left);

			}

		}else 
		{
			x_right = p0[0]+double((y-p0[1])*(p2[0]-p0[0]))/double(p2[1]-p0[1]);
			x_left = p1[0]+double((y-p1[1])*(p2[0]-p1[0]))/double(p2[1]-p1[1]);
			oneoverz_right=1.0/z0 + double((1.0/z2 - 1.0/z0)*(y-p0[1]))/double(p2[1]-p0[1]);
			oneoverz_left=1.0/z1 + double((1.0/z2 - 1.0/z1)*(y-p1[1]))/double(p2[1]-p1[1]);

			nxoverz_right = normal0[0]/z0+double((y-p0[1])*(normal2[0]/z2-normal0[0]/z0))/double(p2[1]-p0[1]);
			nxoverz_left=normal1[0]/z1+double((y-p1[1])*(normal2[0]/z2-normal1[0]/z1))/double(p2[1]-p1[1]);

			nyoverz_right = normal0[1]/z0+double((y-p0[1])*(normal2[1]/z2-normal0[1]/z0))/double(p2[1]-p0[1]);
			nyoverz_left=normal1[1]/z1+double((y-p1[1])*(normal2[1]/z2-normal1[1]/z1))/double(p2[1]-p1[1]);

			nzoverz_right = normal0[2]/z0+double((y-p0[1])*(normal2[2]/z2-normal0[2]/z0))/double(p2[1]-p0[1]);
			nzoverz_left=normal1[2]/z1+double((y-p1[1])*(normal2[2]/z2-normal1[2]/z1))/double(p2[1]-p1[1]);


			if (x_right==x_left)
			{
				oneoverz_step=0;
				nxoverz_step=0;
				nyoverz_step=0;
				nzoverz_step=0;
			}else
			{
				oneoverz_step = double(oneoverz_right-oneoverz_left)/double(x_right-x_left);
				nxoverz_step=double(nxoverz_right-nxoverz_left)/double(x_right-x_left);
				nyoverz_step=double(nyoverz_right-nyoverz_left)/double(x_right-x_left);	
				nzoverz_step=double(nzoverz_right-nzoverz_left)/double(x_right-x_left);
			}

		}
		for(double x=x_left,oneoverz=oneoverz_left,nxoverz=nxoverz_left,nyoverz=nyoverz_left,nzoverz=nzoverz_left;
			x<(x_right);x++,oneoverz+=oneoverz_step,nxoverz+=nxoverz_step,nyoverz+=nyoverz_step,nzoverz+=nzoverz_step)
		{
			double x_vc=0.0;
			double y_vc=0.0;
			double z_vc=0.0;
			assert(oneoverz!=0);
			nx=nxoverz/oneoverz;
			ny=nyoverz/oneoverz;
			nz=nzoverz/oneoverz;
			z_vc=1.0/oneoverz;
			Vec3 N=Vec3(nx,ny,nz);
			N.normalize();
			float r,g,b;
			if(x>=winMin[0] && x<=winMax[0] && y>=winMin[1] && y<=winMax[1])
			{
				x_vc=(2*x*(-z_vc)*(tan(pCamera->fov/2)*(pCamera->aspect_ratio)))/(pCamera->viewport_width);
				y_vc=(2*y*(-z_vc)*tan(pCamera->fov/2))/(pCamera->viewport_hight);
				Vec3 pnt_vc = Vec3(x_vc,y_vc,z_vc);
				DWORD rgb=GetPixelColor_BlinnPhongShading(pnt_vc,N,pMaterial,pCamera,pLights,nLightNum);
				r=GetR(rgb);
				g=GetG(rgb);
				b=GetB(rgb);
				//glColor3f(r/255.0,g/255.0,b/255.0);
				//glVertex2d(x+0.5,y+0.5);
				//pVertex[(*pVertexNum)++]=VTYPE(r/255.0,g/255.0,b/255.0,x+0.5,y+0.5,0.0);
				pVertex[(*pVertexNum)++]=VTYPE(GLubyte(r),GLubyte(g),GLubyte(b),GLubyte(0.0),x,y);
				//SetPixelV(hDC,int(x+0.5),int(y+0.5),rgb);
			}
		}
	}
	return 1;
}

COLORREF Triangle::GetPixelColor_BlinnPhongShading(Vec3 pntPos_vc,Vec3 normal_vc,Material *pMaterial,Camera *pCamera,Light *pLights,int nLightNum)
{
	float scene_a_R,scene_a_G,scene_a_B;
	Vec3 sumRGB=Vec3(0,0,0);
	COLORREF scene_a_color=RGB(50,50,50);

	scene_a_R = GetR(scene_a_color);
	scene_a_G = GetG(scene_a_color);
	scene_a_B = GetB(scene_a_color);
	Vec3 ambientFactorRGB = pMaterial->getAmbient();
	Vec3 diffuseFactorRGB = pMaterial->getDiffuse();
	Vec3 specularFactorRGB = pMaterial->getSpecular();
	float shininessFactor = pMaterial->getShininess();

	Vec3 BPhongModel_a = Vec3(scene_a_R*ambientFactorRGB[0],scene_a_G*ambientFactorRGB[1],scene_a_B*ambientFactorRGB[2]);
	Vec3 L,V,N,H;
	float distance=0;
	sumRGB+=BPhongModel_a;

	for(int lightIndex=0;lightIndex<nLightNum;lightIndex++)
	{
		
		if(pLights[lightIndex].bActive)
		{
			Vec4 lightPos_vc=pCamera->mat_WCToVC*(pLights[lightIndex].getLightPosition());
			Vec3 cameraPos_vc = Vec3(0.0,0.0,0.0);
			float lightR,lightG,lightB;
			lightR=GetR(pLights[lightIndex].getLightColor());
			lightG=GetG(pLights[lightIndex].getLightColor());
			lightB=GetB(pLights[lightIndex].getLightColor());

			if(pLights[lightIndex].type & LIGHT_TYPE_POINT)
			{
				L=Vec3(lightPos_vc[0],lightPos_vc[1],lightPos_vc[2])-pntPos_vc;
				distance=L.length();
				L.normalize();

				V=cameraPos_vc-pntPos_vc;
				V.normalize();

				H=(L+V);
				H.normalize();

				N=normal_vc;
				N.normalize();

				float dotLN=L*N;
				float dotHN=H*N;
				float attenuation=0.0;
				Vec3 factorRGB;
				if(dotLN>0)
				{
					factorRGB=(diffuseFactorRGB*max(dotLN,0.0) + specularFactorRGB *pow(max(dotHN,0.0),shininessFactor));
					attenuation=min(1.0,1.0/(pLights[lightIndex].kc + pLights[lightIndex].kl*distance + pLights[lightIndex].kq*distance*distance));
					//attenuation=1.0;
					factorRGB*=attenuation;
				}

				sumRGB[0]+=factorRGB[0]*lightR;
				sumRGB[1]+=factorRGB[1]*lightG;
				sumRGB[2]+=factorRGB[2]*lightB;
			}
		}
	}

		float finalR =  max(0.0f, min(255.0f, sumRGB[0]));
		float finalG = max(0.0f, min(255.0f, sumRGB[1]));
		float finalB = max(0.0f, min(255.0f, sumRGB[2]));

		return RGB(finalR,finalG,finalB);
}
