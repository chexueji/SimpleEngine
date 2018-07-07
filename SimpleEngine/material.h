#ifndef  _MATERIAL_H_
#define _MATERIAL_H_
#include "vec.h"
class Material
{
public:

	Material():ka(Vec3(0,0,0)),
		kd(Vec3(0,0,0)),
		ks(Vec3(0,0,0)),
		ns(1.0){}
	Material(Vec3 k_a,Vec3 k_d,Vec3 k_s,float n_s):
		ka(k_a),kd(k_d),ks(k_s),ns(n_s){}

	void setAmbient( const Vec3& k_a ){ ka=k_a; }
	void setSpecular( const Vec3& k_s ){ ks=k_s; }
	void setDiffuse( const Vec3& k_d ){ kd=k_d; }
	void setShininess( float n_s){ ns=n_s;}
	Vec3 getAmbient(){return ka;}
	Vec3 getSpecular(){return ks;}
	Vec3 getDiffuse(){return kd;}
	float getShininess(){return ns;}
private:
	Vec3 ka;
	Vec3 kd;
	Vec3 ks;
	float ns;//shininess

};
#endif//_MATERIAL_H_