
//	A simple , GLSL-like Programing framework
//	Using for debug GLSL codes in runtime

//	author : jelo
//	(C) Quatum Dynamics Lab.
//	http://tok.cc

#ifndef _COMMON_H
#define _COMMON_H

#include <math.h>
#include <algorithm>
using namespace std ;

#define GlslTolerance	2e-37f
#define epsino 0.000001
#define GlslMathSqrt(x) sqrt(x)
#define GlslMathZero(x) (-epsino <= x && x <= epsino)
#define GlslPi 3.14159265f
#define Glsl2Pi 2.0 * GlslPi 
#define Glsl1OverPi 1.0f / GlslPi 
#define Glsl1Over2Pi 1.0f / Glsl2Pi

inline float GlslClampf ( float value , float min_inclusive , float max_inclusive ) {

	#define SWAP(x, y, type)    \
		{ type temp = ( x );        \
		x = y; y = temp;        \
	}
	if ( min_inclusive > max_inclusive ) {
		SWAP ( min_inclusive , max_inclusive , float );
	}
	return value < min_inclusive ? min_inclusive : value < max_inclusive ? value : max_inclusive;

}

inline float Smooth ( float edge0 , float edge1 , float x ) {

	float t ;   
	t = GlslClampf ( ( x - edge0 ) / ( edge1 - edge0 ) , 0.0 , 1.0 ) ;
	return t * t * ( 3.0 - 2.0 * t ) ;

}

#define Glslcosf(x) GlslCosine(x)
#define Glslsinf(x) GlslSine(x)
#define GlslToDegree(__ANGLE__) ((__ANGLE__) * 57.29577951f) // PI * 180
#define GlslToRadian(__ANGLE__) ((__ANGLE__) * 0.01745329252f) // PI / 180
#define Glslabs(x) (0 > x) ? -x : (x) 

#endif