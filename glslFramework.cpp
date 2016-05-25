
#include "stdio.h"
#include "vec2.h"
#include "vec3.h"
#include "mat2.h"
#include "mat3.h"
#include "util.h"
#include "debug.h"

#define WIDTH 256.0f
#define HEIGHT 256.0f

const int ITER_GEOMETRY = 1;
const int ITER_FRAGMENT = 5;
const float SEA_HEIGHT = 0.6;
const float SEA_CHOPPY = 5.0;
const float SEA_SPEED = 1.0;
const float SEA_FREQ = 0.16;
const vec3 SEA_BASE = vec3 ( 0.1 , 0.19 , 0.22 );
const vec3 SEA_WATER_COLOR = vec3 ( 0.8 , 0.9 , 0.6 );
float SEA_TIME = 0.;
mat2 octave_m = mat2 ( 1.6 , 1.2 , -1.2 , 1.6 );

float hash2 ( vec2 p ) {
	float h = Dot ( p , vec2 ( 127.1 , 311.7 ) ) ;
	float r = sinf ( h )*43758.5453123 ;
	return r - floor(r) ;
}

float noise ( vec2 p ) {
	vec2 i = Floor ( p );
	vec2 f = Fract ( p );	
	vec2 u = Smooth ( f ) ;

	return -1.0 + 2.0*Mix ( Mix ( hash2 ( i + vec2 ( 0.0 , 0.0 ) ) ,
		hash2 ( i + vec2 ( 1.0 , 0.0 ) ) , u.x ) ,
		Mix ( hash2 ( i + vec2 ( 0.0 , 1.0 ) ) ,
		hash2 ( i + vec2 ( 1.0 , 1.0 ) ) , u.x ) , u.y );
}

float sea_octave ( vec2 uuvv , float choppy ) {
	vec2 uv = uuvv + noise(uuvv) ;
	vec2 wv = 1.0 - Abs ( Sin ( uv ) ) ;
	vec2 swv = Abs ( Cos ( uv ) ) ;
	wv = Mix ( wv , swv , wv ) ;
	return pow ( 1.0f - pow ( wv.x * wv.y , 0.65f ) , choppy ) ;
}

float map ( vec3 p ) {
	float freq = SEA_FREQ;
	float amp = SEA_HEIGHT;
	float choppy = SEA_CHOPPY;
	vec2 uv (p.x,p.z); 
	uv.x *= 0.75;

	float d , h = 0.0;
	for ( int i = 0; i < ITER_GEOMETRY; i++ ) {
		d = sea_octave ( ( uv + SEA_TIME )*freq , choppy );
		d += sea_octave ( ( uv - SEA_TIME )*freq , choppy );
		h += d * amp;
		uv *= octave_m; 
		freq *= 1.9; 
		amp *= 0.22;
		choppy = Mix ( choppy , 1.0 , 0.2 );
	}
	return p.y - h;
}

float map_detailed ( vec3 p ) {
	float freq = SEA_FREQ;
	float amp = SEA_HEIGHT;
	float choppy = SEA_CHOPPY;
	vec2 uv ( p.x , p.z );
	uv.x *= 0.75;

	float d , h = 0.0;
	for ( int i = 0; i < ITER_FRAGMENT; i++ ) {
		d = sea_octave ( ( uv + SEA_TIME )*freq , choppy );
		d += sea_octave ( ( uv - SEA_TIME )*freq , choppy );
		h += d * amp;
		uv *= octave_m;
		freq *= 1.9;
		amp *= 0.22;
		choppy = Mix ( choppy , 1.0 , 0.2 );
	}
	return p.y - h;
}

float heightMapTracing ( vec3 ori , vec3 dir , vec3& p ) {
	float tm = 0.0;
	float tx = 1000.0;
	float hx = map ( ori + dir * tx );
	if ( hx > 0.0 ) return tx;
	float hm = map ( ori + dir * tm );
	float tmid = 0.0;
	for ( int i = 0; i < 16; i++ ) {
		tmid = Mix ( tm , tx , hm / ( hm - hx ) );
		p = ori + dir * tmid;
		float hmid = map ( p );
		if ( hmid < 0.0 ) {
			tx = tmid;
			hx = hmid;
		}
		else {
			tm = tmid;
			hm = hmid;
		}
	}
	return tmid;
}

vec3 getNormal ( vec3 p , float eps ) {
	vec3 n;
	n.y = map_detailed ( p );
	n.x = map_detailed ( vec3 ( p.x + eps , p.y , p.z ) ) - n.y;
	n.z = map_detailed ( vec3 ( p.x , p.y , p.z + eps ) ) - n.y;
	n.y = eps;
	return Normalize ( n );
}

float diffuse ( vec3 n , vec3 l , float p ) {
	return pow ( Dot ( n , l ) * 0.4f + 0.6f , p ) ;
}
float specular ( vec3 n , vec3 l , vec3 e , float s ) {
	float nrm = ( s + 8.0 ) / ( 3.1415 * 8.0 );
	return pow ( max ( Dot ( Reflect ( e , n ) , l ) , 0.0f ) , s ) * nrm ;
}

vec3 getSkyColor ( vec3 e ) {
	e.y = max ( e.y , 0.0f );
	vec3 ret;
	ret.x = pow ( 1.0 - e.y , 2.0 );
	ret.y = 1.0 - e.y;
	ret.z = 0.6 + ( 1.0 - e.y )*0.4;
	return ret;
}

vec3 getSeaColor ( vec3 p , vec3 n , vec3 l , vec3 eye , vec3 dist ) {
	float fresnel = 1.0 - max ( Dot ( n , -1.0f * eye ) , 0.0f );
	fresnel = pow ( fresnel , 3.0 ) * 0.65;

	vec3 reflected = getSkyColor ( Reflect ( eye , n ) );
	vec3 refracted = SEA_BASE + diffuse ( n , l , 80.0 ) * SEA_WATER_COLOR * 0.12;

	vec3 color = Mix ( refracted , reflected , fresnel );

	float atten = max ( 1.0f - Dot ( dist , dist ) * 0.001f , 0.0f );
	color += SEA_WATER_COLOR * ( p.y - SEA_HEIGHT ) * 0.18 * atten;

	float spec = specular ( n , l , eye , 60.0 ) ;
	color += vec3 ( spec , spec , spec );

	return color;
}


int main(int argc, char* argv[])
{

	SEA_TIME = 1000.0f ;
	float EPSILON_NRM = 0.1 / WIDTH ;

	DebugDraw::init ( ( int )WIDTH , ( int ) HEIGHT ) ;
	
	int y = 0 ;
	float YStep = 1.0f / HEIGHT ;
	float XStep = 1.0f / WIDTH ;

	for ( int i = 0 ; i < 1 ; i++ ) {

		printf ( "%d\n" , i );

		for ( float looper = 0.0f; looper < 1.0f; looper = looper + YStep ) {
			int x = 0 ;
			for ( float counter = 0.0f; counter < 1.0f; counter = counter + XStep ) {
				vec2 uv ( counter , looper ) ;
				uv = uv * 2.0f - 1.0f ;
				uv.x = uv.x * WIDTH / HEIGHT ;

				// ray
				vec3 ori = vec3 ( 0.0 , 10.0 , 5.0 );
				vec3 dir = Normalize ( vec3 ( uv.x , uv.y , -2.0 ) );
				//dir.z += length(uv) * 0.15;
				//dir = normalize(dir) * fromEuler(ang);

				// tracing
				vec3 p;
				heightMapTracing ( ori , dir , p );
				vec3 dist = p - ori;
				vec3 n = getNormal ( p , Dot ( dist , dist ) * EPSILON_NRM );
				vec3 light = Normalize ( vec3 ( 0.0 , 1.0 , 0.8 ) );

				// color
				vec3 color = Mix (
					getSkyColor ( dir ) ,
					getSeaColor ( p , n , light , dir , dist ) ,
					pow ( Smooth ( 0.0 , -0.05 , dir.y ) , 0.3 ) );

				DebugDraw::draw ( x , y , color.x , color.y , color.z , 1.0f ) ;
				x = ( int ) ( WIDTH * counter ) ;
			}
			y = ( int ) ( WIDTH * looper ) ;
		}

		char path[64] = {0} ;
		sprintf ( path , "c:\\%d.bmp" , i ) ;
		DebugDraw::save ( path ) ;

		SEA_TIME = SEA_TIME + 0.46f ;

	}

	

	return 0;
}

