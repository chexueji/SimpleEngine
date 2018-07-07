//mat.h
#ifndef _MATRIX_H_
#define _MATRIX_H_

class Vec2;
class Vec3;
class Vec4;

class Mat3 {
public:
	// matrix elements
	float	n[9];

	Mat3();
	Mat3( float n0, float n1, float n2, float n3, float n4, float n5, float n6, float n7, float n8 );
	Mat3( const Mat3 & m );
	~Mat3();

	Mat3 & operator =( const Mat3 & m );
	Mat3 & operator +=( const Mat3 & m );
	Mat3 & operator -=( const Mat3 & m );
	Mat3 & operator *=( const float d );
	Mat3 & operator /=( const float d );

	float* operator []( int i ); 
	const float* operator []( int i ) const; 

	Mat3 transpose() const;

	Mat3 operator +( const Mat3 & a);
	Mat3 operator -( const Mat3 & a );
	Mat3 operator *( const Mat3 & a );
	Vec3 operator *(const Vec3 & a);
	Mat3 operator *( const float d );
	Mat3 operator /( const float d );
};


class Mat4 {
public:
	// matrix elements
	float	n[16];

	Mat4();
	Mat4( float n0, float n1, float n2, float n3, float n4, float n5, float n6, float n7, float n8, float n9, float n10, float n11, float n12, float n13, float n14, float n15 );  
	Mat4( const Mat4 & m );
	Mat4( const Vec4 & v0, const Vec4 & v1, const Vec4 & v2, const Vec4 & v3 );
	~Mat4();

	Mat4 & operator =( const Mat4 & m );
	Mat4 & operator +=( const Mat4 & m );
	Mat4 & operator -=( const Mat4 & m );
	Mat4 & operator *=( const float d );
	Mat4 & operator /=( const float d );
	
	float* operator []( int i );
	const float* operator []( int i ) const;

	Mat4 transpose() const;

	Mat4 operator +( const Mat4 & a);
	Mat4 operator -( const Mat4 & a);
	Mat4 operator *( const Mat4 & a);
	Vec4 operator *( const Vec4 & a);
	Mat4 operator *(const float d );
	Mat4 operator /( const float d );
};
#endif  //_MATRIX_H_