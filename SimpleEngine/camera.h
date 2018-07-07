//camera.h
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vec.h"
#include "mat.h"
#include "cmath"

#define PI 3.14159265359
#define	 DEFAULT_VIEWPORT_WIDTH 600
#define DEFAUTL_VIEWPORT_HIGHT 600

class Camera
{
public:
    Camera();
	~Camera();

    //void setLookAt( const Vec3 &eye,const Vec3 &center,const Vec3 &upDir );
	void Init();
	void setWCToVCTransform(const Vec3 &eye,const Vec3 &center,const Vec3 &upDir );
	void setVCToNCTransform(double theta,double aspect,double znear,double zfar);
	void setNCToVPTransform(double vp_centerX,double vp_centerY,double vpWidth,double vpHeight);
	const Vec3& getEyePos() const	{ return eye_pos; }
	const Vec3& getLookAt() const	{ return look_at; }
	const Vec3 & getUpDir() const { return up_dir; }
	void update();   
public:                            
    //Mat3 mat_Rotate; // rotation matrix
	Mat4 mat_WCToVC;
	Mat4 mat_VCToNC;
	Mat4 mat_NCToVP;

    double aspect_ratio;  //aspectRatio=Width/Height
	double viewport_centerX;
	double viewport_centerY;
	double viewport_width;
	double viewport_hight;
	double fov;
	double z_near_clip;
	double z_far_clip;
    
    Vec3 eye_pos;
    Vec3 look_at; 
	Vec3 up_dir;
};

#endif //_CAMERA_H_