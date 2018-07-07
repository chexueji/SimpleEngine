//vec.h
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <assert.h>

class Mat3;
class Mat4;

//class Vec2 {
//
//public:
//	// x, y
//	int n[2];
//	Vec2();
//	Vec2( const int x, const int y );
//	Vec2( const  Vec2 & v );
//	~Vec2();
//
//	Vec2 & operator = ( const  Vec2& v );
//	Vec2 & operator += ( const  Vec2 & v );
//	Vec2 & operator -= ( const  Vec2 & v );
//	Vec2 & operator *= ( const double d );
//	Vec2 & operator /= ( const double d );
//
//	int & operator []( int i );
//	int operator []( int i ) const;
//
//	Vec2  operator-( const  Vec2 & a ) const;
//	Vec2  operator+( const  Vec2 & a ) const;
//	int operator*(const Vec2 & a) const;
//	Vec2 operator*(const double d) const;
//	Vec2 operator/(const double d) const;
//
//	double length() const;
//	void normalize();
//};

class Vec2i {

public:
	// x, y
	int n[2];
	Vec2i(){n[0]=0.0;n[1]=0.0;};
	Vec2i( const int x, const int y ){n[0]=x;n[1]=y;}
	Vec2i( const  Vec2i & v ){n[0]=v.n[0];n[1]=v.n[1];};
	~Vec2i(){};
	Vec2i & operator = ( const  Vec2i& v ){n[0] = v.n[0]; n[1] = v.n[1]; return *this;};
	Vec2i operator-( const  Vec2i & a ) const{return  Vec2i (n[0]-a.n[0],n[1]-a.n[1]);}
	Vec2i operator+( const  Vec2i & a ) const{return  Vec2i (n[0]+a.n[0],n[1]+a.n[1]);}
	int & operator []( int i ){return n[i];}
	int operator []( int i ) const{return n[i];};
};

class Vec2f {

public:
	// x, y
	double n[2];
	Vec2f(){n[0]=0.0;n[1]=0.0;};
	Vec2f( const double x, const double y ){n[0]=x;n[1]=y;}
	Vec2f( const  Vec2f & v ){n[0]=v.n[0];n[1]=v.n[1];};
	~Vec2f(){};
	Vec2f & operator = ( const  Vec2f& v ){n[0] = v.n[0]; n[1] = v.n[1]; return *this;};
	Vec2f operator-( const  Vec2f & a ) const{return  Vec2f (n[0]-a.n[0],n[1]-a.n[1]);}
	Vec2f operator+( const  Vec2f & a ) const{return  Vec2f (n[0]+a.n[0],n[1]+a.n[1]);}
	double & operator []( int i ){return n[i];}
	double operator []( int i ) const{return n[i];};
};

class Vec3 {

public:
	// x, y, z
	double	n[3];
	Vec3();
	Vec3( const double x, const double y, const double z );
	Vec3( const  Vec3 & v );
	~Vec3();

	 Vec3 & operator=( const  Vec3 & v );
	 Vec3 & operator +=( const  Vec3 & v );
	 Vec3 & operator -= ( const  Vec3 & v );
	 Vec3 & operator *= ( const double d );
	 Vec3 & operator /= ( const double d );

	 double & operator []( int i );
	 double operator []( int i ) const;

	 Vec3  operator-( const  Vec3 & a ) const; 
	 Vec3 operator+( const  Vec3 & a ) const; 
	 double operator*( const Vec3 & a ) const;
	 Vec3 operator*( const Mat3 & m ) const;
	 Vec3 operator*( const double d ) const;
	 Vec3 operator/( const double d ) const;
	 Vec3 operator ^(const Vec3 & b) const;
	 
	double length() const;
	void normalize();
};

class Vec4 {
public:
	// x, y, z, w
	double	n[4];

	Vec4();
	Vec4( const double x, const double y, const double z, const double w );
	Vec4( const Vec4 & v );
	~Vec4();

	Vec4 & operator =( const Vec4 & v );
	Vec4 & operator +=( const Vec4 & v );
	Vec4 & operator -= ( const Vec4 & v );
	Vec4 & operator +=( const Vec3 & v );
	Vec4 & operator -= ( const Vec3 & v );
	Vec4 & operator *= ( const double d );
	Vec4 & operator /= ( const double d );

	double & operator []( int i ); 
	double operator []( int i ) const;

	Vec4 operator-( const Vec4 & a ); 
	Vec4 operator+( const Vec4 & a ); 
	Vec4 operator-( const Vec3 & a ); 
	Vec4 operator+( const Vec3 & a ); 
	double operator*( const Vec4 & a );
	Vec4 operator*(const Mat4 & a);
	Vec4 operator*( const double d );
	Vec4 operator/( const double d );

	double length() const;
	void normalize(); 
};

class Vertex3D
{
public:
	Vertex3D(){}
	~Vertex3D(){}
public:
	Vec4 v;
	Vec4 n;
	Vec2f uv;
	Vertex3D & operator =( const Vertex3D & vertex )
	{
		v=vertex.v;n=vertex.n;
		uv=vertex.uv;
		return *this;
	}
};

#endif //_VECTOR_H_