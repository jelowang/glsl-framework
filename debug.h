
//	A simple , GLSL-like Programing framework
//	Use for debuging of GLSL codes in runtime

//	JELO
//	(C) Quatum Dynamics Lab.
//	http://tok.cc

#ifndef _DEBUG_H
#define _DEBUG_H

#include "stdio.h"
#include "memory.h"
#include "malloc.h"
#include "common.h"

typedef struct tagBITMAPFILEHEADERTEST {
	unsigned long bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned long bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADERTEST {
	unsigned long biSize;
	long biWidth;
	long biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned long biCompression;
	unsigned long biSizeILengthe;
	long biXPelsPerMeter;
	long biYPelsPerMeter;
	unsigned long biClrUsed;
	unsigned long biClrImportant;
} BITMAPINFOHEADER;

class DebugDraw 
{
public :
	DebugDraw (){}
	~DebugDraw (){}

	static void init ( int w , int h ) {	
		width = w ;
		height = h ;
		pannel = (unsigned char*)malloc (width*height*4) ; 
	}
	static void draw ( int x , int y , float red , float green , float blue , float alpha ) {

		unsigned char r = 0 ;
		unsigned char g = 0 ;
		unsigned char b = 0 ;
		unsigned char a = 0 ;

		r = ( unsigned char )(GlslClampf ( blue , 0.0f , 1.0f ) * 255.0f ) ;
		g = ( unsigned char )(GlslClampf ( green , 0.0f , 1.0f ) * 255.0f );
		b = ( unsigned char )(GlslClampf ( red , 0.0f , 1.0f ) * 255.0f ) ;
		a = ( unsigned char )(GlslClampf ( alpha , 0.0f , 1.0f ) * 255.0f ) ;

		//y = (height-1) - y ;

		int row = y * width * 4;
		int col = x ;

		pannel[row + col * 4 ] = r ;
		pannel[row + col * 4 + 1] = g ;
		pannel[row + col * 4 + 2] = b ;
		pannel[row + col * 4 + 3] = a ;
	
	}

	static void save ( char* path ) {

		unsigned short fileType;

		BITMAPFILEHEADER bmpHeader;
		BITMAPINFOHEADER bmpInfo;

		tagBITMAPFILEHEADERTEST *pbmpHeader;

		pbmpHeader = (tagBITMAPFILEHEADERTEST *)malloc ( sizeof ( BITMAPINFOHEADER ) );

		tagBITMAPINFOHEADERTEST *pbmpInfo;

		pbmpInfo = (BITMAPINFOHEADER *)malloc ( sizeof ( BITMAPINFOHEADER ) );

		FILE *testfile;

		testfile = fopen ( path , "wb" );
		if ( !testfile ) {
			free ( pbmpHeader ) ;
			free ( pbmpInfo ) ;
			return ;
		}

		fileType = 0x4D42;
		fwrite ( &fileType , sizeof( unsigned short ) , 1 , testfile );

		bmpHeader.bfOffBits = 54;
		bmpHeader.bfReserved1 = 0;
		bmpHeader.bfReserved2 = 0;

		bmpInfo.biSize = 40;
		bmpInfo.biWidth = width;
		bmpInfo.biHeight = height;
		bmpInfo.biPlanes = 1;
		bmpInfo.biBitCount = 4 * 8;
		bmpInfo.biCompression = 0;
		bmpInfo.biSizeILengthe = width * height * 4;
		bmpInfo.biXPelsPerMeter = 0;
		bmpInfo.biYPelsPerMeter = 0;
		bmpInfo.biClrUsed = 0;
		bmpInfo.biClrImportant = 0;

		bmpHeader.bfSize = 54 + bmpInfo.biSizeILengthe;

		fwrite ( &bmpHeader , sizeof( BITMAPFILEHEADER ) , 1 , testfile );

		fwrite ( &bmpInfo , sizeof( BITMAPINFOHEADER ) , 1 , testfile );

		fwrite ( pannel , bmpInfo.biSizeILengthe , 1 , testfile );

		fclose ( testfile );
	}

	static int width ;
	static int height ;
	static unsigned char* pannel ;

} ;

int DebugDraw::width = 0 ;
int DebugDraw::height = 0 ;
unsigned char* DebugDraw::pannel = 0 ;

#endif