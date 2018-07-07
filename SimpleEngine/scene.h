#ifndef _SCENE_H_
#define _SCENE_H_

#include "vec.h"
#include "mat.h"
#include "camera.h"
#include "object3D.h"
#include "light.h"
#include "material.h"

//#define GetR(c)	((BYTE)((c) & 0x0000FF))
//#define GetG(c)	((BYTE)(((c) & 0x00FF00) >> 8))	
//#define GetB(c)	((BYTE)(((c) & 0xFF0000) >> 16))	

class Scene
{
public:
	Scene();
	~Scene();
	Object3D *LoadObject(char *pObjectName);
	void DrawObject3D(HDC hDC,Object3D *pObject3D,Camera *pCamera,Light *pLights,int nLightNum,float angle);
	void ResetObject3DInfo(Object3D *pObject3D);
	void RemoveObject3D(Object3D *pObject3D,Camera *pCamera);
	void RemoveHidenfaces(Object3D *pObject3D,Camera *pCamera);
	void PhongShading(Object3D *pObject3D,Camera *pCamera,Light *pLights,int nLightNum);
	Texture*  LoadTexture(char  *pTextureName);
	Mat4 Translate3D(float tx,float ty,float tz);
	Mat4 Scale3D(float sx,float sy,float sz);
	bool Rotate3D(Object3D *pObject3D,float angle,float x,float y,float z);
	void Destroy_Camera(Camera *pCamera);
	void Destroy_Texture(Texture * pTextureInfo);
	void Destroy_Object3D(Object3D * pObject3D);
	HWND GetWnd(){return hWnd;}	
	SHADE_MODE GetShadeMode(){return shade_mode;}
	COLORREF GetAmbientColor(){return scene_a_color;}
	void SetWnd(HWND _hWnd){ hWnd = _hWnd;}
	void SetShadeMode(SHADE_MODE _shade_mode){shade_mode = _shade_mode;}
	void SetAmbientColor(COLORREF _ambient_color){scene_a_color = _ambient_color;}
	void SetWinWidthAndHeight(double vp_centerX,double vp_centerY,double vpWidth,double vpHeight);

private:
	HWND hWnd;
	SHADE_MODE shade_mode;
	COLORREF scene_a_color;//ambient color
	Vec2f winMin;
	Vec2f winMax;
};
#endif //_SCENE_H_ 