#include "stdafx.h"
#include <cmath>
#include "vec.h"
#include "mat.h"
using namespace std;

////////////////////////////////////////////////////////////////////////////
//vec2
//Vec2::Vec2() 
//{ 
//	n[0] = 0.0; n[1] = 0.0; 
//}
//
//Vec2::Vec2( const int x, const int y )
//{ 
//	n[0] = x; n[1] = y; 
//}
//
//Vec2::Vec2( const  Vec2 & v )
//{ 
//	n[0] = v.n[0]; n[1] = v.n[1]; 
//}
//
//Vec2::~Vec2()
//{
//}
//
//Vec2 & Vec2::operator = ( const  Vec2& v )
//{ 
//	n[0] = v.n[0]; n[1] = v.n[1]; return *this;
//}
//
//Vec2 & Vec2::operator += ( const  Vec2 & v )
//{
//	n[0] += v.n[0]; n[1] += v.n[1]; return *this; 
//}
//
//Vec2 & Vec2::operator -= ( const  Vec2 & v )
//{
//	n[0] -= v.n[0]; n[1] -= v.n[1]; return *this;
//}
//
//Vec2 & Vec2::operator *= ( const double d )
//{ 
//	n[0] *= d; n[1] *= d; return *this; 
//}
//
//Vec2 & Vec2::operator /= ( const double d )
//{
//	n[0] /= d; n[1] /= d; return *this; 
//}
//
//int & Vec2::operator []( int i )
//{ 
//	return n[i]; 
//}
//int Vec2::operator []( int i ) const 
//{ 
//	return n[i]; 
//}
//
//Vec2  Vec2::operator-( const  Vec2 & a ) const
//{ 
//	return  Vec2 (n[0]-a.n[0],n[1]-a.n[1]); 
//}
//Vec2  Vec2::operator+( const  Vec2 & a ) const
//{ 
//	return  Vec2 (n[0]+a.n[0],n[1]+a.n[1]); 
//}
//int Vec2::operator*(const Vec2 & a) const
//{
//	return double (n[0]*a.n[0]+n[1]*a.n[1]); 
//}
//Vec2 Vec2::operator*(const double d) const
//{ 
//	return Vec2(n[0]*d,n[1]*d); 
//}
//Vec2 Vec2::operator/(const double d) const
//{ 
//	return Vec2(n[0]/d,n[1]/d); 
//}
//
//
//double Vec2::length() const 
//{
//	return sqrt( n[0]*n[0] + n[1]*n[1]); 
//}
//
//void Vec2::normalize() 
//{ 
//	double len = length();
//	assert(len != 0);
//	n[0] /= len; n[1] /= len;
//}
////////////////////////////////////////////////////////////////////////////
//vec3
Vec3::Vec3() 
{ 
	n[0] = 0.0; n[1] = 0.0; n[2] = 0.0;
}
Vec3::Vec3( const double x, const double y, const double z )
{ 
	n[0] = x; n[1] = y; n[2] = z; 
}
Vec3::Vec3( const  Vec3 & v )
{
	n[0] = v.n[0]; n[1] = v.n[1]; n[2] = v.n[2]; 
}
Vec3::~Vec3()
{
}

Vec3 &  Vec3::operator=( const  Vec3 & v )
{
	n[0] = v.n[0]; n[1] = v.n[1]; n[2] = v.n[2]; return *this; 
}

Vec3 & Vec3::operator +=( const  Vec3 & v )
{
	n[0] += v.n[0]; n[1] += v.n[1]; n[2] += v.n[2]; return *this;
}

Vec3 & Vec3::operator -= ( const  Vec3 & v )
{
	n[0] -= v.n[0]; n[1] -= v.n[1]; n[2] -= v.n[2]; return *this; 
}

Vec3 & Vec3::operator *= ( const double d )
{ 
	n[0] *= d; n[1] *= d; n[2] *= d; return *this;
}

Vec3 & Vec3::operator /= ( const double d )
{ 
	n[0] /= d; n[1] /= d; n[2] /= d; return *this; 
}

double & Vec3::operator []( int i )
{ 
	return n[i]; 
}
double Vec3::operator []( int i ) const 
{ 
	return n[i]; 
}

Vec3  Vec3::operator-( const  Vec3 & a ) const 
{
	return  Vec3 (n[0]-a.n[0],n[1]-a.n[1],n[2]-a.n[2]); 
}

Vec3 Vec3::operator+( const  Vec3 & a ) const 
{ 
	return  Vec3 (n[0]+a.n[0],n[1]+a.n[1],n[2]+a.n[2]); 
}

double Vec3::operator*( const Vec3 & a ) const
{ 
	return  double (n[0]*a.n[0]+n[1]*a.n[1]+n[2]*a.n[2]);
}

Vec3 Vec3::operator*( const Mat3 & m ) const
{
	return Vec3 (
		n[0]*m.n[0]+n[1]*m.n[3]+n[2]*m.n[6],
		n[0]*m.n[1]+n[1]*m.n[4]+n[2]*m.n[7],
		n[0]*m.n[2]+n[1]*m.n[5]+n[2]*m.n[8]); 
}

Vec3 Vec3::operator*( const double d ) const
{ 
	return Vec3 (n[0]*d,n[1]*d,n[2]*d); 
}

Vec3 Vec3::operator/( const double d ) const
{ 
	return Vec3 (n[0]/d,n[1]/d,n[2]/d); 
}

Vec3 Vec3::operator ^(const Vec3 & b) const
{
	return Vec3 ( n[1]*b.n[2] - n[2]*b.n[1],
		n[2]*b.n[0] - n[0]*b.n[2],
		n[0]*b.n[1] - n[1]*b.n[0] );
}

double Vec3::length() const
{ 
	return sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]); 
}

void Vec3::normalize() 
{ 
	double len = length();
	assert(len != 0);
	n[0] /= len; n[1] /= len; n[2] /= len;
}
////////////////////////////////////////////////////////////////////////////
//vec4
Vec4::Vec4() 
{
	n[0] = 0.0; n[1] = 0.0; n[2] = 0.0; n[3] = 1.0; 
}

Vec4::Vec4( const double x, const double y, const double z, const double w )
{
	n[0] = x; n[1] = y; n[2] = z; n[3] = w; 
}

Vec4::Vec4( const Vec4 & v )
{ 
	n[0] = v.n[0]; n[1] = v.n[1]; n[2] = v.n[2]; n[3] = v.n[3]; 
}

Vec4::~Vec4()
{
}

Vec4 & Vec4::operator =( const Vec4 & v )
{ 
	n[0] = v.n[0]; n[1] = v.n[1]; n[2] = v.n[2]; n[3] = v.n[3];return *this; 
}

Vec4 & Vec4::operator +=( const Vec4 & v )
{
	n[0] += v.n[0]; n[1] += v.n[1]; n[2] += v.n[2]; n[3] += v.n[3];return *this; 
}

Vec4 & Vec4::operator -= ( const Vec4 & v )
{
	n[0] -= v.n[0]; n[1] -= v.n[1]; n[2] -= v.n[2]; n[3] -= v.n[3];return *this; 
}
Vec4 & Vec4::operator +=( const Vec3 & v )
{
	n[0] += v.n[0]; n[1] += v.n[1]; n[2] += v.n[2];return *this;
}
Vec4 & Vec4::operator -= ( const Vec3 & v )
{
	n[0] -= v.n[0]; n[1] -= v.n[1]; n[2] -= v.n[2];return *this;
}
Vec4 & Vec4::operator *= ( const double d )
{
	n[0] *= d; n[1] *= d; n[2] *= d; n[3] *= d; return *this; 
}

Vec4 & Vec4::operator /= ( const double d )
{ 
	n[0] /= d; n[1] /= d; n[2] /= d; n[3] /= d; return *this; 
}

double & Vec4::operator []( int i ) 
{ 
	return n[i]; 
}

double Vec4::operator []( int i ) const
{ 
	return n[i]; 
}

Vec4 Vec4::operator-( const Vec4 & a ) 
{ 
	return Vec4 (n[0]-a.n[0],n[1]-a.n[1],n[2]-a.n[2],n[3]-a.n[3]);
}

Vec4 Vec4::operator+( const Vec4 & a ) 
{ 
	return Vec4 (a.n[0]+n[0],a.n[1]+n[1],a.n[2]+n[2],a.n[3]+n[3]);
}

Vec4 Vec4::operator-( const Vec3 & a ) 
{ 
	return Vec4 (n[0]-a.n[0],n[1]-a.n[1],n[2]-a.n[2],n[3]);
}

Vec4 Vec4::operator+( const Vec3 & a ) 
{ 
	return Vec4 (a.n[0]+n[0],a.n[1]+n[1],a.n[2]+n[2],n[3]);
}

double Vec4::operator*( const Vec4 & a )
{
	return double (n[0]*a.n[0]+n[1]*a.n[1]+n[2]*a.n[2]+n[3]*a.n[3]);
}

Vec4 Vec4::operator*(const Mat4 & a)
{
	return Vec4 (n[0]*a.n[0]+n[1]*a.n[4]+n[2]*a.n[8]+n[3]*a.n[12],
						n[0]*a.n[1]+n[1]*a.n[5]+n[2]*a.n[9]+n[3]*a.n[13],
						n[0]*a.n[2]+n[1]*a.n[6]+n[2]*a.n[10]+n[3]*a.n[14],
						n[0]*a.n[3]+n[1]*a.n[7]+n[2]*a.n[11]+n[3]*a.n[15]);
	
}

Vec4 Vec4::operator*( const double d ) 
{ 
	return Vec4 (n[0]*d,n[1]*d,n[2]*d,n[3]*d); 
}

Vec4 Vec4::operator/( const double d ) 
{ 
	return Vec4 (n[0]/d,n[1]/d,n[2]/d,n[3]/d); 
}

double Vec4::length() const
{ 
	return sqrt( n[0]*n[0] + n[1]*n[1] + n[2]*n[2] ); //Attention!:not include n[3]
}

void Vec4::normalize() 
{
	double len = length();
	assert(len != 0);
	n[0] /= len; n[1] /= len; n[2] /= len;//Attention!:not include n[3]
}