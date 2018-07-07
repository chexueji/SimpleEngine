#include "stdafx.h"
#include <cstring>
#include <cmath>
#include "mat.h"
#include "vec.h"
using namespace std;

/////////////////////////////////////////////////////////////
//mat3
Mat3::Mat3()
{
	memset(n,0,9*sizeof(float)); n[0]=1; n[4]=1; n[8]=1; 
}

Mat3::Mat3( float n0, float n1, float n2, float n3, float n4, float n5, float n6, float n7, float n8 )
{	
	n[0]=n0; n[1]=n1; n[2]=n2;
	n[3]=n3; n[4]=n4; n[5]=n5;
	n[6]=n6; n[7]=n7; n[8]=n8; 
}

Mat3::Mat3( const Mat3 & m )
{ 
	memcpy(n,m.n,9*sizeof(float)); 
}

Mat3::~Mat3()
{
}

Mat3 & Mat3::operator =( const Mat3 & m )
{ 
	memcpy(n,m.n,9*sizeof(float)); return *this; 
}

Mat3 & Mat3::operator +=( const Mat3 & m )
{ 
	n[0]+=m.n[0]; n[1]+=m.n[1]; n[2]+=m.n[2]; n[3]+=m.n[3]; n[4]+=m.n[4]; n[5]+=m.n[5]; n[6]+=m.n[6]; n[7]+=m.n[7]; n[8]+=m.n[8]; 
	return *this; 
}

Mat3 & Mat3::operator -=( const Mat3 & m )
{ 
	n[0]-=m.n[0]; n[1]-=m.n[1]; n[2]-=m.n[2]; n[3]-=m.n[3]; n[4]-=m.n[4]; n[5]-=m.n[5]; n[6]-=m.n[6]; n[7]-=m.n[7]; n[8]-=m.n[8]; 
	return *this; 
}

Mat3 & Mat3::operator *=( const float d )
{ 
	n[0]*=d; n[1]*=d; n[2]*=d; n[3]*=d; n[4]*=d; n[5]*=d; n[6]*=d; n[7]*=d; n[8]*=d; 
	return *this;
}

Mat3 & Mat3::operator /=( const float d )
{ 
	n[0]/=d; n[1]/=d; n[2]/=d; n[3]/=d; n[4]/=d; n[5]/=d; n[6]/=d; n[7]/=d; n[8]/=d; 
	return *this; 
}

float* Mat3::operator []( int i ) 
{ 
	return &n[i*3]; 
}

const float* Mat3::operator []( int i ) const
{
	return &n[i*3]; 
}

Mat3 Mat3::transpose() const 
{ 
	return Mat3 (n[0],n[3],n[6],n[1],n[4],n[7],n[2],n[5],n[8]); 
}

Mat3 Mat3::operator -( const Mat3 & a )
{
	return Mat3 ( n[0]-a.n[0], n[1]-a.n[1], n[2]-a.n[2],
		n[3]-a.n[3], n[4]-a.n[4], n[5]-a.n[5],
		n[6]-a.n[6], n[7]-a.n[7], n[8]-a.n[8]);
}

Mat3 Mat3::operator +( const Mat3 & a )
{
	return Mat3 (n[0]+a.n[0], n[1]+a.n[1], n[2]+a.n[2],
		n[3]+a.n[3], n[4]+a.n[4], n[5]+a.n[5],
		n[6]+a.n[6], n[7]+a.n[7], n[8]+a.n[8]);
}

Mat3 Mat3::operator *( const Mat3 & a )
{
	return Mat3 ( n[0]*a.n[0]+n[1]*a.n[3]+n[2]*a.n[6],
		n[0]*a.n[1]+n[1]*a.n[4]+n[2]*a.n[7],
		n[0]*a.n[2]+n[1]*a.n[5]+n[2]*a.n[8],
		n[3]*a.n[0]+n[4]*a.n[3]+n[5]*a.n[6],
		n[3]*a.n[1]+n[4]*a.n[4]+n[5]*a.n[7],
		n[3]*a.n[2]+n[4]*a.n[5]+n[5]*a.n[8],
		n[6]*a.n[0]+n[7]*a.n[3]+n[8]*a.n[6],
		n[6]*a.n[1]+n[7]*a.n[4]+n[8]*a.n[7],
		n[6]*a.n[2]+n[7]*a.n[5]+n[8]*a.n[8] );
}
Vec3 Mat3::operator *(const Vec3 & a)
{
	return Vec3 (
		a.n[0]*n[0]+a.n[1]*n[1]+a.n[2]*n[2],
		a.n[0]*n[3]+a.n[1]*n[4]+a.n[2]*n[5],
		a.n[0]*n[6]+a.n[1]*n[7]+a.n[2]*n[8]); 
}
Mat3 Mat3::operator *( const float d )
{
	return Mat3 ( n[0]*d, n[1]*d, n[2]*d,
				n[3]*d, n[4]*d, n[5]*d,
				n[6]*d, n[7]*d, n[8]*d );
}

Mat3 Mat3::operator /( const float d )
{
	return Mat3 ( n[0]/d, n[1]/d, n[2]/d,
		n[3]/d, n[4]/d, n[5]/d,
		n[6]/d, n[7]/d, n[8]/d );
}
/////////////////////////////////////////////////////////////
//mat4
Mat4::Mat4()
{ 
	memset(n,0,16*sizeof(float)); n[0]=1;n[5]=1;n[10]=1;n[15]=1; 
}

Mat4::Mat4( float n0, float n1, float n2, float n3, float n4, float n5, float n6, float n7, float n8, float n9, float n10, float n11, float n12, float n13, float n14, float n15 )
{
	n[ 0]=n0 ; n[ 1]=n1 ; n[ 2]=n2 ; n[ 3]=n3 ;
	n[ 4]=n4 ; n[ 5]=n5 ; n[ 6]=n6 ; n[ 7]=n7 ;
	n[ 8]=n8 ; n[ 9]=n9 ; n[10]=n10; n[11]=n11;
	n[12]=n12; n[13]=n13; n[14]=n14; n[15]=n15; 
}	

Mat4::Mat4( const Mat4 & m )
{ 
	memcpy(n,m.n,16*sizeof(float)); 
}

Mat4::Mat4( const Vec4 & v0, const Vec4 & v1, const Vec4 & v2, const Vec4 & v3 ) {
	n[ 0] = v0[0]; n[ 1] = v0[1]; n[ 2] = v0[2]; n[ 3] = v0[3];
	n[ 4] = v1[0]; n[ 5] = v1[1]; n[ 6] = v1[2]; n[ 7] = v1[3];
	n[ 8] = v2[0]; n[ 9] = v2[1]; n[10] = v2[2]; n[11] = v2[3];
	n[12] = v3[0]; n[13] = v3[1]; n[14] = v3[2]; n[15] = v3[3];
}

Mat4::~Mat4()
{
}

Mat4 & Mat4::operator =( const Mat4 & m )
{
	memcpy(n,m.n,16*sizeof(float)); 
	return *this; 
}

Mat4 & Mat4::operator +=( const Mat4 & m )
{ 
	n[ 0]+=m.n[ 0]; n[ 1]+=m.n[ 1]; n[ 2]+=m.n[ 2]; n[ 3]+=m.n[ 3];
	n[ 4]+=m.n[ 4]; n[ 5]+=m.n[ 5]; n[ 6]+=m.n[ 6]; n[ 7]+=m.n[ 7];
	n[ 8]+=m.n[ 8]; n[ 9]+=m.n[ 9]; n[10]+=m.n[10]; n[11]+=m.n[11];
	n[12]+=m.n[12]; n[13]+=m.n[13]; n[14]+=m.n[14]; n[15]+=m.n[15]; 
	return *this; 
}

Mat4 & Mat4::operator -=( const Mat4 & m )
{ 
	n[ 0]-=m.n[ 0]; n[ 1]-=m.n[ 1]; n[ 2]-=m.n[ 2]; n[ 3]-=m.n[ 3];
	n[ 4]-=m.n[ 4]; n[ 5]-=m.n[ 5]; n[ 6]-=m.n[ 6]; n[ 7]-=m.n[ 7];
	n[ 8]-=m.n[ 8]; n[ 9]-=m.n[ 9]; n[10]-=m.n[10]; n[11]-=m.n[11];
	n[12]-=m.n[12]; n[13]-=m.n[13]; n[14]-=m.n[14]; n[15]-=m.n[15]; 
	return *this; 
}

Mat4 & Mat4::operator *=( const float d )
{ 
	n[ 0]*=d; n[ 1]*=d; n[ 2]*=d; n[ 3]*=d;
	n[ 4]*=d; n[ 5]*=d; n[ 6]*=d; n[ 7]*=d;
	n[ 8]*=d; n[ 9]*=d; n[10]*=d; n[11]*=d;
	n[12]*=d; n[13]*=d; n[14]*=d; n[15]*=d; 
	return *this;
}

Mat4 & Mat4::operator /=( const float d )
{
	n[ 0]/=d; n[ 1]/=d; n[ 2]/=d; n[ 3]/=d;
	n[ 4]/=d; n[ 5]/=d; n[ 6]/=d; n[ 7]/=d;
	n[ 8]/=d; n[ 9]/=d; n[10]/=d; n[11]/=d;
	n[12]/=d; n[13]/=d; n[14]/=d; n[15]/=d; 
	return *this;
}

float* Mat4::operator []( int i )
{ 
	return &n[i*4]; 
}

const float* Mat4::operator []( int i ) const
{ 
	return &n[i*4]; 
}

Mat4 Mat4::transpose() const
{
	return Mat4 (n[ 0],n[ 4],n[ 8],n[12],
		n[ 1],n[ 5],n[ 9],n[13],  
		n[ 2],n[ 6],n[10],n[14],
		n[ 3],n[ 7],n[11],n[15]); 
}

Mat4 Mat4::operator +( const Mat4 & a) 
{
	return Mat4 (
		n[ 0]+a.n[ 0], n[ 1]+a.n[ 1], n[ 2]+a.n[ 2], n[ 3]+a.n[ 3],
		n[ 4]+a.n[ 4], n[ 5]+a.n[ 5], n[ 6]+a.n[ 6], n[ 7]+a.n[ 7],
		n[ 8]+a.n[ 8], n[ 9]+a.n[ 9], n[10]+a.n[10], n[11]+a.n[11],
		n[12]+a.n[12], n[13]+a.n[13], n[14]+a.n[14], n[15]+a.n[15] );
}

Mat4 Mat4::operator -( const Mat4 & a) 
{
	return Mat4 ( 
		n[ 0]-a.n[ 0], n[ 1]-a.n[ 1], n[ 2]-a.n[ 2], n[ 3]-a.n[ 3],
		n[ 4]-a.n[ 4], n[ 5]-a.n[ 5], n[ 6]-a.n[ 6], n[ 7]-a.n[ 7],
		n[ 8]-a.n[ 8], n[ 9]-a.n[ 9], n[10]-a.n[10], n[11]-a.n[11],
		n[12]-a.n[12], n[13]-a.n[13], n[14]-a.n[14], n[15]-a.n[15] );
}

Mat4 Mat4::operator *( const Mat4 & a) {
	return Mat4 ( 
		n[ 0]*a.n[ 0]+n[ 1]*a.n[ 4]+n[ 2]*a.n[ 8]+n[ 3]*a.n[12],
		n[ 0]*a.n[ 1]+n[ 1]*a.n[ 5]+n[ 2]*a.n[ 9]+n[ 3]*a.n[13],
		n[ 0]*a.n[ 2]+n[ 1]*a.n[ 6]+n[ 2]*a.n[10]+n[ 3]*a.n[14],
		n[ 0]*a.n[ 3]+n[ 1]*a.n[ 7]+n[ 2]*a.n[11]+n[ 3]*a.n[15],
		n[ 4]*a.n[ 0]+n[ 5]*a.n[ 4]+n[ 6]*a.n[ 8]+n[ 7]*a.n[12],
		n[ 4]*a.n[ 1]+n[ 5]*a.n[ 5]+n[ 6]*a.n[ 9]+n[ 7]*a.n[13],
		n[ 4]*a.n[ 2]+n[ 5]*a.n[ 6]+n[ 6]*a.n[10]+n[ 7]*a.n[14],
		n[ 4]*a.n[ 3]+n[ 5]*a.n[ 7]+n[ 6]*a.n[11]+n[ 7]*a.n[15],
		n[ 8]*a.n[ 0]+n[ 9]*a.n[ 4]+n[10]*a.n[ 8]+n[11]*a.n[12],
		n[ 8]*a.n[ 1]+n[ 9]*a.n[ 5]+n[10]*a.n[ 9]+n[11]*a.n[13],
		n[ 8]*a.n[ 2]+n[ 9]*a.n[ 6]+n[10]*a.n[10]+n[11]*a.n[14],
		n[ 8]*a.n[ 3]+n[ 9]*a.n[ 7]+n[10]*a.n[11]+n[11]*a.n[15],
		n[12]*a.n[ 0]+n[13]*a.n[ 4]+n[14]*a.n[ 8]+n[15]*a.n[12],
		n[12]*a.n[ 1]+n[13]*a.n[ 5]+n[14]*a.n[ 9]+n[15]*a.n[13],
		n[12]*a.n[ 2]+n[13]*a.n[ 6]+n[14]*a.n[10]+n[15]*a.n[14],
		n[12]*a.n[ 3]+n[13]*a.n[ 7]+n[14]*a.n[11]+n[15]*a.n[15] );
}

Vec4 Mat4::operator *( const Vec4 & a)
{
	return Vec4 (a.n[0]*n[0]+a.n[1]*n[1]+a.n[2]*n[2]+a.n[3]*n[3],
		a.n[0]*n[4]+a.n[1]*n[5]+a.n[2]*n[6]+a.n[3]*n[7],
		a.n[0]*n[8]+a.n[1]*n[9]+a.n[2]*n[10]+a.n[3]*n[11],
		a.n[0]*n[12]+a.n[1]*n[13]+a.n[2]*n[14]+a.n[3]*n[15]);
}

Mat4 Mat4::operator *(const float d )
{
	return Mat4 ( 
		n[ 0]*d, n[ 1]*d, n[ 2]*d, n[ 3]*d,
		n[ 4]*d, n[ 5]*d, n[ 6]*d, n[ 7]*d,
		n[ 8]*d, n[ 9]*d, n[10]*d, n[11]*d,
		n[12]*d, n[13]*d, n[14]*d, n[15]*d );
}

Mat4 Mat4::operator /(const float d )
{
	return Mat4 ( 
		n[ 0]/d, n[ 1]/d, n[ 2]/d, n[ 3]/d,
		n[ 4]/d, n[ 5]/d, n[ 6]/d, n[ 7]/d,
		n[ 8]/d, n[ 9]/d, n[10]/d, n[11]/d,
		n[12]/d, n[13]/d, n[14]/d, n[15]/d );
}


