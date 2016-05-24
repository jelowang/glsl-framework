
//	A simple , GLSL-like Programing framework
//	Using for debug GLSL codes in runtime

//	author : jelo
//	(C) Quatum Dynamics Lab.
//	http://tok.cc

#ifndef _VEC3_H
#define _VEC3_H

#include "common.h"

class vec3 {
public:
	vec3 ( ) {
		x = 0. ;
		y = 0. ;
		z = 0. ;
	}

	vec3 ( float xx , float yy , float zz ) {
		x = xx ;
		y = yy ;
		z = zz ;
	}

	~vec3 ( ) {}

	inline vec3 operator += ( float v2 ) {
		x = x + v2 ;
		y = y + v2 ;
		z = z + v2 ;
		return *this ;
	}

	inline vec3 operator += ( const vec3& v2 ) {
		x = x + v2.x ;
		y = y + v2.y ;
		z = z + v2.z ;
		return *this ;
	}

	float x ;
	float y ;
	float z ;

} ;

//	inline functions are bellow

inline vec3 operator - ( float v , const vec3& vec ) {
	vec3 r ;
	r.x = v - vec.x ;
	r.y = v - vec.y ;
	r.z = v - vec.z ;
	return r ;
}

inline vec3 operator - ( const vec3& vec , float v ) {
	vec3 r ;
	r.x = vec.x - v ;
	r.y = vec.y - v ;
	r.z = vec.z - v ;
	return r ;
}

inline vec3 operator - ( const vec3& v , const vec3 v2 ) {
	vec3 r;
	r.x = v.x - v2.x ;
	r.y = v.y - v2.y ;
	r.z = v.z - v2.z ;
	return r;
}

inline vec3 operator + ( float v , const vec3& vec ) {
	vec3 r ;
	r.x = v + vec.x ;
	r.y = v + vec.y ;
	r.z = v + vec.z ;
	return r ;
}

inline vec3 operator + ( const vec3& vec , float v ) {
	vec3 r ;
	r.x = v + vec.x ;
	r.y = v + vec.y ;
	r.z = v + vec.z ;
	return r ;
}

inline vec3 operator + ( const vec3& v , const vec3& v2 ) {
	vec3 r ;
	r.x = v.x + v2.x ;
	r.y = v.y + v2.y ;
	r.z = v.z + v2.z ;
	return r ;
}

inline vec3 operator / ( const vec3& v , float v2 ) {
	vec3 r;
	r.x = v.x / v2 ;
	r.y = v.y / v2 ;
	r.z = v.z / v2 ;
	return r;
}

inline vec3 operator * ( float f , const vec3& v ) {
	vec3 r ;
	r.x = v.x * f ;
	r.y = v.y * f ;
	r.z = v.z * f ;
	return r ;
}

inline vec3 operator * ( const vec3& v , float f ) {
	vec3 r ;
	r.x = v.x * f ;
	r.y = v.y * f ;
	r.z = v.z * f ;
	return r ;
}

//	not dot product neither cross product , just product of component-wise
inline vec3 operator * ( const vec3& v , const vec3& v2 ) {
	vec3 r ;
	r.x = v.x * v2.x ;
	r.y = v.y * v2.y ;
	r.z = v.y * v2.y ;
	return r ;

}
inline vec3 Floor ( const vec3& v ) {
	vec3 r;
	r.x = floor ( v.x );
	r.y = floor ( v.y );
	r.z = floor ( v.z );
	return r;
}

inline vec3 Fract ( const vec3& v ) {
	vec3 r;
	vec3 r2;
	r = v;
	r2 = Floor ( v );
	r2 = r - r2;
	return r2;
}

inline vec3 Smooth ( const vec3& v ) {
	vec3 r ;
	r = v*v*( 3.0 - 2.0*v ) ;
	return r ;
}

inline float Length ( const vec3& v ) {
	float r = 0.0f ;
	r = sqrtf ( v.x*v.x + v.y*v.y + v.z*v.z ) ;
	if ( GlslMathZero ( r ) )
		return 0.0f ;
	return r ;
}

inline vec3 Normalize ( const vec3& v ) {

	vec3 results ;
	float r = 0.0f ;

	r = Length ( v ) ;

	if ( GlslMathZero ( r ) ) return results ;

	results = v / r ;

	return results ;
}

inline float Dot ( const vec3& v , const vec3& v2 ) {

	float r = 0.0f ;

	r = v.x*v2.x + v.y*v2.y + v.z*v2.z ;

	return r ;
}

inline vec3 Sin ( vec3 a ) {

	vec3 r ;

	r.x = sinf ( a.x ) ;
	r.y = sinf ( a.y ) ;
	r.z = sinf ( a.z ) ;

	return r ;
}

inline vec3 Cos ( vec3 a ) {

	vec3 r ;

	r.x = cosf ( a.x ) ;
	r.y = cosf ( a.y ) ;
	r.z = cosf ( a.z ) ;

	return r ;
}

inline vec3 Mix ( vec3 x , vec3 y , vec3 a ) {

	vec3 r ;

	r.x = x.x * ( 1.0f - a.x ) + y.x * a.x ;
	r.y = x.y * ( 1.0f - a.y ) + y.y * a.y ;
	r.z = x.z * ( 1.0f - a.z ) + y.z * a.z ;

	return r ;

}

inline vec3 Mix ( vec3 x , vec3 y , float a ) {

	vec3 r ;

	r.x = x.x * ( 1.0f - a ) + y.x * a ;
	r.y = x.y * ( 1.0f - a ) + y.y * a ;
	r.z = x.z * ( 1.0f - a ) + y.z * a ;

	return r ;

}


inline vec3 Abs ( vec3 a ) {

	vec3 r ;

	r.x = Glslabs ( a.x );
	r.y = Glslabs ( a.y );
	r.z = Glslabs ( a.z );

	return r ;
}

inline vec3 Reflect ( vec3 I , vec3 N ) {

	vec3 r ;

	r = I - 2.0f * Dot ( N , I ) * N ;

	return r ;

}

#endif