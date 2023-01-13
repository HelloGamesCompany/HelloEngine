#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <windows.h>
#include <stdio.h>
#include <time.h> 

#ifdef _DEBUG
#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);
#else
#define LOG(x);
#endif

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

#define IN_RANGE( value, min, max ) ( ((value) >= (min) && (value) <= (max)) ? 1 : 0 )
#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )
#define TO_BOOL( a )  ( (a != 0) ? true : false )

// Deletes a buffer
#define RELEASE( x ) \
    {                        \
    if( x != nullptr )        \
        {                      \
      delete x;            \
      x = nullptr;              \
        }                      \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{							  \
	if( x != nullptr )			  \
		{							\
	  delete[] x;				\
	  x = nullptr;					\
		}							\
	}

#ifdef GET_FROM_API
#define TO_API __declspec(dllimport)
#else
#define TO_API __declspec(dllexport)
#endif

template <class VALUE_TYPE> void SWAP(VALUE_TYPE& a, VALUE_TYPE& b)
{
	VALUE_TYPE tmp = a;
	a = b;
	b = tmp;
};

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

enum class UpdateStatus
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "HelloEngine"

#endif // !__GLOBALS_H__