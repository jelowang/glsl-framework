
//	A simple , GLSL-like Programing framework
//	Use for debugging GLSL codes in runtime

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

inline float GlslClampf ( float value , float minv , float maxv ) {

	if ( minv > maxv ) {
		float temp = minv ;
		minv = maxv ;
		maxv = temp ;
	}
	return value < minv ? minv : value < maxv ? value : maxv;

}

inline float Smooth ( float edge0 , float edge1 , float x ) {

	float t ;   
	t = GlslClampf ( ( x - edge0 ) / ( edge1 - edge0 ) , 0.0 , 1.0 ) ;
	return t * t * ( 3.0 - 2.0 * t ) ;

}

inline float Step ( float edge , float x ) {
	return x < edge ? 0.0f : 1.0f ;
}

const float fp_amount = (float)( 0xFFFF );
const float fp_amount_inv = 1.f / fp_amount;

inline float Frac ( float a_X )
{
	return ( (int)( a_X * fp_amount ) & 0xFFFF ) * fp_amount_inv;
}

inline float Fract ( float value ) {

	//if ( value >= 0.0 )
	//	return value - floor ( value ) ;
	//else
	//	return value - ceil ( value ) ;
	double param , fractpart , intpart ;
	param = value ;
	fractpart = modf ( param , &intpart ) ;
	return fractpart ;
	//return Frac(value) ;
}

inline double drand48 () {

	static unsigned long long seed = 1 ;

	#define MNWZ 0x100000000
	#define ANWZ 0x5DEECE66D
	#define CNWZ 0xB16

	seed = ( ANWZ * seed + CNWZ ) & 0xFFFFFFFFFFFFLL;
	unsigned int x = seed >> 16;
	return ( (double)x / (double)MNWZ );
}

inline void srand48 ( unsigned int i ) {
	static unsigned long long seed = 1 ;
	seed = ( ( ( long long int )i ) << 16 ) | rand ();
}


#define Glslcosf(x) GlslCosine(x)
#define Glslsinf(x) GlslSine(x)
#define GlslToDegree(__ANGLE__) ((__ANGLE__) * 57.29577951f) // PI * 180
#define GlslToRadian(__ANGLE__) ((__ANGLE__) * 0.01745329252f) // PI / 180
#define Glslabs(x) (0 > x) ? -x : (x) 

#endif