#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_
#include "vec.h"
#include "2D_DDALine.h"
#include "texture.h"
#include "camera.h"
#include "material.h"
#include "light.h"


class Triangle
{
public:
	Triangle();
	Triangle(Vec3 v1,Vec3 v2,Vec3 v3);
	~Triangle();
	//Vec4 & operator[](int i);
	//Vec4 Triangle::operator[](int i) const;
	Triangle & operator =(const Triangle &tri);

	bool draw_triangle_gouraud(Vec2f winMin,Vec2f winMax);
	bool draw_flat_bottom_triangle(Vec2i p0,Vec2i p1,Vec2i p2,
								COLORREF v0_color,COLORREF v1_color,COLORREF v2_color,Vec2f winMin,Vec2f winMax,VTYPE *pVertex,long *pVertexNum);
	bool draw_flat_top_triangle(Vec2i p0,Vec2i p1,Vec2i p2,
								COLORREF v0_color,COLORREF v1_color,COLORREF v2_color,Vec2f winMin,Vec2f winMax,VTYPE *pVertex,long *pVertexNum);
	bool draw_triangle_wireframe(Vec2f winMin,Vec2f winMax);
	bool draw_triangle_texture(Vec2f winMin,Vec2f winMax);
	bool draw_flat_top_triangle_texture(Vec2i p0,Vec2i p1,Vec2i p2,Vec2f uv0,Vec2f uv1,Vec2f uv2,\
										float z0,float z1,float z2,Vec2f winMin,Vec2f winMax,VTYPE *pVertex,long *pVertexNum);
	bool draw_flat_bottom_triangle_texture(Vec2i p0,Vec2i p1,Vec2i p2,Vec2f uv0,Vec2f uv1,Vec2f uv2,\
										float z0,float z1,float z2,Vec2f winMin,Vec2f winMax,VTYPE *pVertex,long *pVertexNum);

	bool draw_triangle_normal(Material *pMaterial, Camera *pCamera,Light *pLights,int nLightNum,Vec2f winMin,Vec2f winMax);
	bool draw_flat_bottom_triangle_normal(Vec2i p0,Vec2i p1,Vec2i p2,Vec3 normal0,Vec3 normal1,Vec3 normal2,float z0,float z1,float z2,\
																	Material *pMaterial, Camera *pCamera,Light *pLights,int nLightNum,Vec2f winMin,Vec2f winMax,VTYPE *pVertex,long *pVertexNum);
	bool draw_flat_top_triangle_normal(Vec2i p0,Vec2i p1,Vec2i p2,Vec3 normal0,Vec3 normal1,Vec3 normal2,float z0,float z1,float z2,
																Material *pMaterial, Camera *pCamera,Light *pLights,int nLightNum,Vec2f winMin,Vec2f winMax,VTYPE *pVertex,long *pVertexNum);

	COLORREF GetPixelColor_BlinnPhongShading(Vec3 pntPos_vc,Vec3 normal_vc,Material *pMaterial,
																	Camera *pCamera,Light *pLights,int nLightNum);
	COLORREF get_texture_color(int x,int y);
	void swap_pnt(Vec2i &pnt1,Vec2i &pnt2);
	void swap_color(COLORREF &color1,COLORREF &color2);
	void swap_uv(Vec2f &uv1,Vec2f &uv2);
	void swap_normal(Vec3 &nml1,Vec3 &nml2);
	void swap_z(float &z1,float &z2);
public:
	Vertex3D pnt_local[3];//local vertexes in world coordinate
	Vertex3D pnt_trans[3];//translated by the world-view-projection matrix
	float z[3];

	Vec4 tri_normal;
	int VertexIdx[3];
	Vec2f uv[3];
	bool bHiddenFace;
	Texture texture;
	COLORREF tri_color;
	COLORREF tri_flat_color;
	COLORREF vertex_color[3];
};

#endif //_TRIANGLE_H_