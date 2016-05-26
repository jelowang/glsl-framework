
#include "stdio.h"
#include "vec2.h"
#include "vec3.h"
#include "mat2.h"
#include "mat3.h"
#include "util.h"
#include "debug.h"

#define WIDTH 256.0
#define HEIGHT 256.0

//	ocean begin
const int ITER_GEOMETRY = 3;
const int ITER_FRAGMENT = 5;
const float SEA_HEIGHT = 0.9;
const float SEA_CHOPPY = 3.0;
const float SEA_SPEED = 1.0;
const float SEA_FREQ = 0.16;
const vec3 SEA_BASE = vec3 ( 0.1 , 0.19 , 0.22 );
const vec3 SEA_WATER_COLOR = vec3 ( 0.8 , 0.9 , 0.6 );
float SEA_TIME = 0.;
mat2 octave_m = mat2 ( 1.6 , 1.2 , -1.2 , 1.6 );

mat3 euler2mat ( vec3 ang ) {
	mat3 m ;
	vec2 a1 = vec2 ( sinf ( GlslToRadian ( ang.x ) ) , cosf ( GlslToRadian ( ang.x ) ) ) ;
	vec2 a2 = vec2 ( sinf ( GlslToRadian ( ang.y ) ) , cosf ( GlslToRadian ( ang.y ) ) ) ;
	vec2 a3 = vec2 ( sinf ( GlslToRadian ( ang.z ) ) , cosf ( GlslToRadian ( ang.z ) ) ) ;
	m.m00 = a1.y*a3.y + a1.x*a2.x*a3.x ;	m.m01 = -a2.y*a1.x ;	m.m02 = a3.y*a1.x*a2.x + a1.y*a3.x ;
	m.m10 = a1.y*a2.x*a3.x + a3.y*a1.x ;	m.m11 = a1.y*a2.y ;		m.m12 = a1.x*a3.x - a1.y*a3.y*a2.x ;
	m.m20 = -a2.y*a3.x ;					m.m21 = a2.x ;			m.m22 = a2.y*a3.y ;
	return m ;
}

float hash2 ( vec2 p ) {

//	float h = Dot ( p , vec2 ( 127.1 , 311.7 ) ) ;
//	float r = sinf ( h )*43758.5453123 ;
//	return r - floor(r) ; 
	float h = Dot ( p , vec2 ( 127.1 , 311.7 ) ) ;
	return Fract ( sinf ( h )*43758.5453123 ) ;
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

vec3 oceanRender ( vec2 uv ) {

	float EPSILON_NRM = 0.1 / WIDTH ;

	// ray
	vec3 ang = vec3 ( 0.0 , 0.0 , 0.0 );
	vec3 ori = vec3 ( 0.0 , 10.0 , 5.0 );
	vec3 dir = Normalize ( vec3 ( uv.x ,uv.y , -1.0 ) );
	dir.z += Length(uv) * 0.15;
	dir = Normalize ( dir ) * euler2mat ( ang );

	// tracing
	vec3 p;
	float height = heightMapTracing ( ori , dir , p );
	vec3 dist = p - ori;
	vec3 n = getNormal ( p , Dot ( dist , dist ) * EPSILON_NRM );
	vec3 light = Normalize ( vec3 ( 0.0 , 1.0 , 0.8 ) );

	// color
	vec3 color = Mix (
	getSkyColor ( dir ) ,
	getSeaColor ( p , n , light , dir , dist ) ,
	pow ( Smooth ( 0.0 , -0.05 , dir.y ) , 0.3 ) );

	return color ;

}
//	ocean end

//	sphere begin
float sphere ( vec3 ray , vec3 dir , vec3 center , float radius )
{
	vec3 rc = ray - center;
	float c = Dot ( rc , rc ) - ( radius*radius );
	float b = Dot ( dir , rc );
	float d = b*b - c;
	float t = -b - sqrt ( abs ( d ) );
	float st = Step ( 0.0 , min ( t , d ) );
	return Mix ( -1.0 , t , st );
}
vec3 background ( float t , vec3 rd )
{
	vec3 light = Normalize ( vec3 ( sin ( t ) , 0.6 , cos ( t ) ) );
	float sun = max ( 0.0f , Dot ( rd , light ) );
	float sky = max ( 0.0f , Dot ( rd , vec3 ( 1.0 , 1.0 , 0.0 ) ) );
	float ground = max ( 0.0f , -Dot ( rd , vec3 ( 1.0 , 1.0 , 1.0 ) ) );
	return
		( pow ( sun , 256.0 ) + 0.2*pow ( sun , 2.0 ) )*vec3 ( 2.0 , 1.6 , 1.0 ) +
		pow ( ground , 0.5 )*vec3 ( 0.4 , 0.3 , 0.2 ) +
		pow ( sky , 1.0 )*vec3 ( 0.5 , 0.6 , 0.7 );
}
vec3 sphereRender ( vec2 uv ) {

	vec3 ro = vec3 ( 0.0 , 0.0 , -3.0 );
	vec3 rd = Normalize ( vec3 ( uv.x , uv.y , 1.0 ) );
	vec3 p = vec3 ( 0.0 , 0.0 , 0.0 );
	float t = sphere ( ro , rd , p , 1.0 );
	vec3 nml = Normalize ( p - ( ro + rd*t ) );
	vec3 bgCol = background ( 49.5 , rd );
	rd = Reflect ( rd , nml );
	vec3 col = background ( 2.0 , rd ) * vec3 ( 0.9 , 0.8 , 1.0 );
	return Mix ( bgCol , col , Step ( 0.0 , t ) ) ;

}
//	sphere end

//	triangle begin

#define Resolution				resolution
#define Time					time

#define HorizontalAmplitude		0.20
#define VerticleAmplitude		0.10
#define HorizontalSpeed			0.90
#define VerticleSpeed			1.0
#define ParticleMinSize			1.76
#define ParticleMaxSize			1.51
#define ParticleBreathingSpeed		0.30
#define ParticleColorChangeSpeed	0.70
#define ParticleCount			1.0
#define ParticleColor2			vec3(9.0, 5.0, 3.0)
#define ParticleColor1			vec3(1.0, 3.0, 9.0)


float hash3 ( float x )
{
	return Fract ( sin ( x ) * 43758.5453 );
}

float noise3 ( vec2 uv )  // Thanks Inigo Quilez
{
	vec3 x = vec3 ( uv.x , uv.y , 0.0 );

	vec3 p = Floor ( x );
	vec3 f = Fract ( x );

	f = Smooth ( f ) ;

	float offset = 57.0;

	float n = Dot ( p , vec3 ( 1.0 , offset , offset*2.0 ) );

	return Mix ( Mix ( Mix ( hash3 ( n + 0.0 ) , hash3 ( n + 1.0 ) , f.x ) ,
		Mix ( hash3 ( n + offset ) , hash3 ( n + offset + 1.0 ) , f.x ) , f.y ) ,
		Mix ( Mix ( hash3 ( n + offset*2.0 ) , hash3 ( n + offset*2.0 + 1.0 ) , f.x ) ,
		Mix ( hash3 ( n + offset*3.0 ) , hash3 ( n + offset*3.0 + 1.0 ) , f.x ) , f.y ) , f.z );
}

float snoise ( vec2 uv )
{
	return noise ( uv ) * 2.0 - 1.0;
}


float perlinNoise ( vec2 uv )
{
	float n = noise3 ( uv * 1.0 ) 	* 128.0 +
		noise3 ( uv * 2.0 ) 	* 64.0 +
		noise3 ( uv * 4.0 ) 	* 32.0 +
		noise3 ( uv * 8.0 ) 	* 16.0 +
		noise3 ( uv * 16.0 ) 	* 8.0 +
		noise3 ( uv * 32.0 ) 	* 4.0 +
		noise3 ( uv * 64.0 ) 	* 2.0 +
		noise3 ( uv * 128.0 ) * 1.0;

	float noiseVal = n / ( 1.0 + 2.0 + 4.0 + 8.0 + 16.0 + 32.0 + 64.0 + 128.0 );
	noiseVal = abs ( noiseVal * 2.0 - 1.0 );

	return 	noiseVal;
}

float fBm ( vec2 uv , float lacunarity , float gain )
{
	float sum = 0.0;
	float amp = 7.0;

	for ( int i = 0; i < 2; ++i )
	{
		sum += ( perlinNoise ( uv ) ) * amp;
		amp *= gain;
		uv *= lacunarity;
	}

	return sum;
}

vec3 particles ( vec2 pos )
{

	vec3 c = vec3 ( 0 , 0 , 0 );

	float noiseFactor = fBm ( pos , 0.01 , 0.1 );

	for ( float i = 1.0; i < ParticleCount + 1.0; ++i )
	{
		float cs = cos ( 1.0 * HorizontalSpeed * ( i / ParticleCount ) + noiseFactor ) * HorizontalAmplitude;
		float ss = sin ( 1.0 * VerticleSpeed   * ( i / ParticleCount ) + noiseFactor ) * VerticleAmplitude;
		vec2 origin = vec2 ( cs , ss );

		float t = sin ( 1.0 * ParticleBreathingSpeed * i ) * 0.5 + 0.5;
		float particleSize = Mix ( ParticleMinSize , ParticleMaxSize , t );
		float d = GlslClampf ( sin ( Length ( pos - origin ) + particleSize ) , 0.0 , particleSize );

		float t2 = sin ( 1.0 * ParticleColorChangeSpeed * i ) * 0.5 + 0.5;
		vec3 color = Mix ( ParticleColor1 , ParticleColor2 , t2 );
		c += color * pow ( d , 10.0 );
	}

	return c;
}


float line ( vec2 a , vec2 b , vec2 p )
{
	vec2 aTob = b - a;
	vec2 aTop = p - a;

	float t = Dot ( aTop , aTob ) / Dot ( aTob , aTob );

	t = GlslClampf ( t , 0.0 , 1.0 );

	float d = Length ( p - ( a + aTob * t ) );
	d = 1.0 / d;

	return GlslClampf ( d , 0.0 , 1.0 );
}

vec3 triangleRender ( vec2 uv ) {

	vec2 signedUV = uv ;

	float freqA = Mix ( 0.4 , 1.2 , sin ( 1.0 + 30.0 ) * 0.5 + 0.5 );
	float freqB = Mix ( 0.4 , 1.2 , sin ( 1.0 + 20.0 ) * 0.5 + 0.5 );
	float freqC = Mix ( 0.4 , 1.2 , sin ( 1.0 + 10.0 ) * 0.5 + 0.5 );


	float scale = 100.0;
	const float v = 70.0;
	vec3 finalColor = vec3 ( 0.0 , 0.0 , 0.0 );

	finalColor = ( particles ( Sin ( Abs ( signedUV ) ) ) * Length ( signedUV ) ) * 0.20;

	float t = line ( vec2 ( -v , -v ) , vec2 ( 0.0 , v ) , signedUV * scale );
	finalColor += vec3 ( 8.0 * t , 2.0 * t , 4.0 * t ) * freqA;
	t = line ( vec2 ( 0.0 , v ) , vec2 ( v , -v ) , signedUV * scale );
	finalColor += vec3 ( 2.0 * t , 8.0 * t , 4.0 * t ) * freqB;
	t = line ( vec2 ( -v , -v ) , vec2 ( v , -v ) , signedUV * scale );
	finalColor += vec3 ( 2.0 * t , 4.0 * t , 8.0 * t ) * freqC;

	return finalColor ;

}
//	triangle end

int main(int argc, char* argv[])
{

	float r = Frac ( -123.434 ) ;
	float rr = Fract (-123.434) ;

	SEA_TIME = 1000.0f ;
	
	DebugDraw::init ( ( int )WIDTH , ( int ) HEIGHT ) ;

	int y = 0 ;
	float YStep = 1.0f / HEIGHT ;
	float XStep = 1.0f / WIDTH ;

	for ( int i = 0 ; i < 1 ; i++ ) {

		for ( float looper = 0.0f; looper < 1.0f; looper = looper + YStep ) {
			int x = 0 ;
			for ( float counter = 0.0f; counter < 1.0f; counter = counter + XStep ) {
				vec2 uv ( counter , looper ) ;
				uv = uv * 2.0f - 1.0f ;
				uv.x = uv.x * WIDTH / HEIGHT ;
				vec3 color ;
				color = oceanRender ( uv ) ;
				DebugDraw::draw ( x , y , color.x , color.y , color.z , 1.0f ) ;
				x = ( int ) ( WIDTH * counter ) ;
				printf ( "Rendering : %d %%\n" , (int )(looper * 100.0f) );
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

