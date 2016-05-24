
//	A simple , GLSL-like Programing framework
//	Using for debug GLSL codes in runtime

//	author : jelo
//	(C) Quatum Dynamics Lab.
//	http://tok.cc

#ifndef _MAT2_H
#define _MAT2_H

#include "common.h"
#include "vec2.h"

class mat2 {
public:
	mat2 () {
		m00 = 1.0f ;
		m01 = 0.0f ;
		m10 = 0.0f ;
		m11 = 1.0f ;
	}
	mat2 ( float mat00 , float mat01 , float mat10 , float mat11 ) {
		m00 = mat00 ;
		m01 = mat01 ;
		m10 = mat10 ;
		m11 = mat11 ;
	}
	~mat2 () {}
	
	float m00 ;
	float m01 ;
	float m10 ;
	float m11 ;
} ;

inline vec2 operator * ( const vec2& v , const mat2& m ) {
	
	vec2 r ;
	r.x = m.m00 * v.x + m.m10 * v.y ;
	r.y = m.m10 * v.x + m.m11 * v.y ;
	return r ;
}

inline vec2 operator *= ( const vec2& v , const mat2& m ) {

	vec2 r ;
	r.x = m.m00 * v.x + m.m10 * v.y ;
	r.y = m.m10 * v.x + m.m11 * v.y ;
	return r ;
}

#endif