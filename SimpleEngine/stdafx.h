// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <glut.h>

#define GetR(c)	((BYTE)((c) & 0x0000FF))
#define GetG(c)	((BYTE)(((c) & 0x00FF00) >> 8))	
#define GetB(c)	((BYTE)(((c) & 0xFF0000) >> 16))

#define GetR_Inverse(c)	((BYTE)(((c) & 0xFF0000) >> 16))	
#define GetB_Inverse(c)	((BYTE)((c) & 0x0000FF))
#define GetG_Inverse(c)	((BYTE)(((c) & 0x00FF00) >> 8))	

#define BYTESALIGN(n)  (n%4 == 0?n:(n+=(4-(n%4))))
#define PIXELBYTES 32

//class  VTYPE
//{
//public:
//	VTYPE(){}
//	VTYPE(GLfloat red,GLfloat green,GLfloat blue,GLfloat _x,GLfloat _y,GLfloat _z)
//	{ r=red;g=green;b=blue;x=_x;y=_y;z=_z;}
//	~VTYPE(){}
//	VTYPE &operator =(const VTYPE & v)
//	{
//		r=v.r;g=v.g;b=v.b;x=v.x;y=v.y;z=v.z;
//		return *this;
//	}
//public:
//	GLfloat r;
//	GLfloat g;
//	GLfloat b;
//	GLfloat x;
//	GLfloat y;
//	GLfloat z;
//};
class  VTYPE
{
public:
	VTYPE(){}
	VTYPE(GLubyte red,GLubyte green,GLubyte blue,GLubyte alpha,GLfloat _x,GLfloat _y)
	{ r=red;g=green;b=blue;a=alpha;x=_x;y=_y;}
	~VTYPE(){}
	VTYPE &operator =(const VTYPE & v)
	{
		r=v.r;g=v.g;b=v.b;a=v.a;x=v.x;y=v.y;
		return *this;
	}
public:
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
	GLfloat x;
	GLfloat y;
	//GLfloat z;
};

// TODO: reference additional headers your program requires here
