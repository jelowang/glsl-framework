
//	A simple , GLSL-like Programing framework
//	Using for debug GLSL codes in runtime

//	author : jelo
//	(C) Quatum Dynamics Lab.
//	http://tok.cc

#ifndef _VEC2_H
#define _VEC2_H

#include "common.h"

class vec2 {
public:
	vec2 ( ) {
		x = 0. ;
		y = 0. ;
	}

	vec2 ( float xx , float yy ) {
		x = xx ;
		y = yy ;
	}

	~vec2 ( ) {}

	inline vec2 operator += ( float v2 ) {
		x = x + v2 ;
		y = y + v2 ;
		return *this ;
	}

	inline vec2 operator += ( const vec2& v2 ) {
		x = x + v2.x ;
		y = y + v2.y ;
		return *this ;
	}

	float x ;
	float y ;
} ;

//	inline functions are bellow

inline vec2 operator - ( float v , const vec2& vec ) {
	vec2 r ;
	r.x = v - vec.x ;
	r.y = v - vec.y ;
	return r ;
}

inline vec2 operator - ( const vec2& vec , float v ) {
	vec2 r ;
	r.x = vec.x - v ;
	r.y = vec.y - v ;
	return r ;
}

inline vec2 operator - ( const vec2& v , const vec2 v2 ) {
	vec2 r;
	r.x = v.x - v2.x ;
	r.y = v.y - v2.y ;
	return r;
}

inline vec2 operator + ( float v , const vec2& vec ) {
	vec2 r ;
	r.x = v + vec.x ;
	r.y = v + vec.y ;
	return r ;
}

inline vec2 operator + ( const vec2& vec , float v ) {
	vec2 r ;
	r.x = v + vec.x ;
	r.y = v + vec.y ;
	return r ;
}

inline vec2 operator + ( const vec2& v , const vec2& v2 ) {
	vec2 r ;
	r.x = v.x + v2.x ;
	r.y = v.y + v2.y ;
	return r ;
}

inline vec2 operator / ( const vec2& v , float v2 ) {
	vec2 r;
	r.x = v.x / v2 ;
	r.y = v.y / v2 ;
	return r;
}

inline vec2 operator * ( float f , const vec2& v ) {
	vec2 r ;
	r.x = v.x * f ;
	r.y = v.y * f ;
	return r ;
}

inline vec2 operator * ( const vec2& v , float f ) {
	vec2 r ;
	r.x = v.x * f ;
	r.y = v.y * f ;
	return r ;
}

//	not dot product neither cross product , just product of component-wise
inline vec2 operator * ( const vec2& v , const vec2& v2 ) {
	vec2 r ;
	r.x = v.x * v2.x ;
	r.y = v.y * v2.y ;
	return r ;

}
inline vec2 Floor ( const vec2& v ) {
	vec2 r;
	r.x = floor ( v.x );
	r.y = floor ( v.y );
	return r;
}

inline vec2 Fract ( const vec2& v ) {
	vec2 r;
	vec2 r2;
	r = v;
	r2 = Floor ( v );
	r2 = r - r2;
	return r2;
}

inline vec2 Smooth ( const vec2& v ) {
	vec2 r ;
	r = v*v*( 3.0 - 2.0*v ) ;
	return r ;
}

inline float Length ( const vec2& v ) {
	float r = 0.0f ;
	r = sqrtf ( v.x*v.x + v.y*v.y ) ;
	if ( GlslMathZero ( r ) ) 
		return 0.0f ;
	return r ;
}

inline vec2 Normalize ( const vec2& v ) {

	vec2 results ;
	float r = 0.0f ;

	r = Length ( v ) ;

	if ( GlslMathZero ( r ) ) return results ;
	
	results = v / r ;

	return results ;
}

inline float Dot ( const vec2& v , const vec2& v2 ) {

	float r = 0.0f ;

	r = v.x*v2.x + v.y*v2.y ;

	return r ;
}

inline vec2 Sin ( vec2 a ) {

	vec2 r ;

	r.x = sinf ( a.x ) ;
	r.y = sinf ( a.y ) ;

	return r ;
}

inline vec2 Cos ( vec2 a ) {

	vec2 r ;

	r.x = cosf ( a.x ) ;
	r.y = cosf ( a.y ) ;

	return r ;
}

inline float Mix ( float x , float y , float a ) {
	float r1 = x * ( 1.0f - a );
	float r2 = y * ( a ) ;
	return r1 + r2 ;
}

inline vec2 Mix ( vec2 x , vec2 y , vec2 a ) {
	
	vec2 r ;

	r.x = x.x * ( 1.0f - a.x ) + y.x * a.x ;
	r.y = x.y * ( 1.0f - a.y ) + y.y * a.y ;

	return r ;
	
}

inline vec2 Abs ( vec2 a ) {

	vec2 r ;

	r.x = Glslabs ( a.x );
	r.y = Glslabs ( a.y );

	return r ;
}

#endif