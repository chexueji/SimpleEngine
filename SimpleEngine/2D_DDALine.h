#ifndef _2D_DDALINE_H_
#define _2D_DDALINE_H_

#include "vec.h"

void DDA_DrawLine(const Vec2f &StartPnt,const Vec2f &EndPnt,COLORREF Color1,COLORREF Color2,VTYPE *pVertex,long *pVertexNum); 
bool ClipTest(double p,double q,double *u1,double *u2);
void LineClip_LiangBarsky(Vec2f winMin,Vec2f winMax,const Vec2f pnt_1,const Vec2f pnt_2,const COLORREF Color1,const COLORREF Color2,VTYPE *pVertex,long *pVertexNum);

#endif //_2D_DDALINE_