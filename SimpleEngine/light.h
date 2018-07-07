#ifndef _LIGHT_H_
#define _LIGHT_H_
#include "vec.h"


typedef enum light_type
{
	LIGHT_TYPE_POINT=0x01,
	LIGHT_TYPE_DIRECT=0x02,
	LIGHT_TYPE_AMBIENT=0x04,
}LIGHT_TYPE;

typedef enum shade_mode
{
  SHADE_MODE_NONE=0x01,
  SHADE_MODE_CONSTANT=0x02,
  SHADE_MODE_FLAT=0x04,
  SHADE_MODE_GOURAUD=0x08,
  SHADE_MODE_NORMAL=0x10,
  SHADE_MODE_TEXTURE=0x20,
}SHADE_MODE;


class Light 
{
public:
	Light();
	Light(int lightType,Vec4 lightPos,float ka,float kd,float ks,float ns,float kc,float kl,float kq,\
			bool bActive,COLORREF a_color,COLORREF light_color );
	~Light();
	void setAttenuationFactor(float k_c,float k_l,float k_q){kc= k_c;kl=k_l;kq=k_q;}
	void setLightColor(COLORREF color){light_color=color;}
	void setLightPosition(Vec4 pos){light_pos = pos;}
	COLORREF getLightColor(){return light_color;}
	Vec4 getLightPosition(){return light_pos;}
	bool isLightActive(){return bActive;}

public:
	int type;
	Vec4 light_pos;
//	float K_a,K_d,K_s,N_s;//ambient,diffuse,specular
	float kc,kl,kq;
	bool bActive;
	//COLORREF ambient_color;
	COLORREF light_color;

};
#endif