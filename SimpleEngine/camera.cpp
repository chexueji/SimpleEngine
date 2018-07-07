#include "stdafx.h"
#include "camera.h"
 
using namespace std;

Camera::Camera()
{ 
    eye_pos = Vec3(0,0,0);
	up_dir = Vec3(0,1,0);
	look_at = Vec3(0,0,-1);
	aspect_ratio = 1.0; 
	viewport_centerX = 0.0;
	viewport_centerY = 0.0;
	viewport_width = DEFAULT_VIEWPORT_WIDTH;
	viewport_hight = DEFAUTL_VIEWPORT_HIGHT;
	fov = PI/2.0;
	z_near_clip = 1.0;
	z_far_clip = 500.0;
	mat_WCToVC=Mat4( 1,0,0,0,
										0,1,0,0,
										0,0,1,0,
										0,0,0,1);

	mat_VCToNC = Mat4(float(1.0/(tan(fov/2)*aspect_ratio)),0,0,0,
									0,float(1/tan(fov/2)),0,0,
									0,0,float((-z_near_clip-z_far_clip)/(-z_near_clip+z_far_clip)),float((-2*z_near_clip*z_far_clip)/(-z_near_clip+z_far_clip)),
									0,0,-1,0);

	mat_NCToVP=Mat4(viewport_width/2.0,0,0,viewport_centerX,
											0,viewport_hight/2.0,0,viewport_centerY,
											0,0,0.5,0.5,
											0,0,0,1);
}

Camera::~Camera()
{

}

//void Camera::setLookAt( const Vec3 &eye,const Vec3 &center,const Vec3 &upDir )
//{
//	eye_pos = eye;
//	Vec3 n = eye-center;  
//	Vec3 v = upDir; 
//	v.normalize();
//	n.normalize();
//    Vec3 u = v ^ n;  
//	u.normalize();
//    mat_Rotate = Mat3 ( u[0],u[1],u[2],v[0],v[1],v[2],n[0],n[1],n[2] ).transpose();
//
//    update();
//}

void Camera::setWCToVCTransform(const Vec3 &eye,const Vec3 &center,const Vec3 &upDir )
{
	Vec3 n=eye-center; 
	n.normalize();
	Vec3 v = up_dir;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
	v.normalize();
	Vec3 u=v^n;
	u.normalize();
	//adjust
	v=n^u;
	v.normalize();

	eye_pos=eye;
	look_at=Vec3(0,0,0)-n;
	up_dir=v;

	 Mat4 matWCToVC(u[0],u[1],u[2],-(u*eye_pos),
					v[0],v[1],v[2],-(v*eye_pos),
					n[0],n[1],n[2],-(n*eye_pos),
					0,0,0,1);
	mat_WCToVC = matWCToVC;
}

void Camera::setVCToNCTransform(double theta,double aspect,double znear,double zfar)
{

	fov = theta / (180.0 / PI);
	z_near_clip = znear;
	z_far_clip = zfar;
	aspect_ratio = aspect;
	Mat4 matVCToNC(float(1.0/(tan(fov/2)*aspect_ratio)),0,0,0,
									0,float(1/tan(fov/2)),0,0,
									0,0,float((-z_near_clip-z_far_clip)/(-z_near_clip+z_far_clip)),float((-2*z_near_clip*z_far_clip)/(-z_near_clip+z_far_clip)),
									0,0,-1,0);
	mat_VCToNC = matVCToNC;
}

void Camera::setNCToVPTransform(double vp_centerX,double vp_centerY,double vpWidth,double vpHeight)
{
	viewport_centerX = vp_centerX;
	viewport_centerY = vp_centerY;
	viewport_width = vpWidth;
	viewport_hight = vpHeight;
	
	double xvmax,yvmax;
	double xvmin,yvmin;
	xvmin=viewport_centerX-viewport_width/2.0;
	yvmin=viewport_centerY-viewport_hight/2.0;
	xvmax=xvmin+viewport_width;
	yvmax=yvmin+viewport_hight;
	//1.viewport center is (xvmin,yvmin)
	Mat4 matNCToViewPort((xvmax-xvmin)/2.0,0,0,(xvmax+xvmin)/2.0,
											0,(yvmax-yvmin)/2.0,0,(yvmax+yvmin)/2.0,
											0,0,0.5,0.5,
											0,0,0,1);
	mat_NCToVP = matNCToViewPort;
}








