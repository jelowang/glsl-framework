
//	A simple , GLSL-like Programing framework
//	Use for debugging GLSL codes in runtime

//	author : jelo
//	(C) Quatum Dynamics Lab.
//	http://tok.cc

#ifndef _MAT3_H
#define _MAT3_H

#include "common.h"
#include "vec3.h"

class mat3 {
public:
	mat3 () {
		m00 = 1.0f ;
		m01 = 0.0f ;
		m10 = 0.0f ;
		m11 = 1.0f ;
	}

	mat3 
	( 
		float mat00 , float mat01 , float mat02 , 
		float mat10 , float mat11 , float mat12 ,
		float mat20 , float mat21 , float mat22 
	) {

		m00 = mat00 ; m01 = mat01 ; m02 = mat02 ;
		m10 = mat10 ; m11 = mat11 ; m12 = mat12 ;
		m20 = mat20 ; m21 = mat21 ; m22 = mat22 ;

	}
	~mat3 () {}

	float m00 ; float m01 ; float m02 ;	
	float m10 ; float m11 ; float m12 ;
	float m20 ; float m21 ; float m22 ;
	
} ;

inline vec3 operator * ( const vec3& v , const mat3& m ) {

	vec3 r ;
	r.x = m.m00 * v.x + m.m10 * v.x + m.m20 * v.x ;
	r.y = m.m01 * v.y + m.m11 * v.y + m.m21 * v.y ;
	r.z = m.m02 * v.z + m.m12 * v.z + m.m22 * v.z ;
	return r ;
}

inline void operator *= ( vec3& v , const mat3& m ) {

	v.x = m.m00 * v.x + m.m10 * v.x + m.m20 * v.x ;
	v.y = m.m01 * v.y + m.m11 * v.y + m.m21 * v.y ;
	v.z = m.m02 * v.z + m.m12 * v.z + m.m22 * v.z ;
	return ;
}

#endif