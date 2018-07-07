#include "stdafx.h"
#include "light.h"

Light::Light()
{
	type=LIGHT_TYPE_POINT;
	light_pos = Vec4(0,0,0,1);
	//K_a=0.5;
 //   K_d=0.5;
	//K_s=0.5;
	//N_s=2.0;
	kc=0.1;
	kl=0.1;
	kq=0.01;
	bActive = false;
	//ambient_color=RGB(0,0,0);
	light_color=RGB(255,255,255);
}
Light::Light(int lightType,Vec4 lightPos,float ka,float kd,float ks,float ns,float kc,float kl,float kq,bool bActive,COLORREF a_color,COLORREF light_color )
{
	type=lightType;
	light_pos = lightPos;
	//K_a=ka;
	//K_d=kd;
	//K_s=ks;
	//N_s=ns;
	kc=kc;
	kl=kl;
	kq=kq;
	bActive = bActive;
	//ambient_color=a_color;
	light_color=light_color;
}
Light::~Light()
{
}