//-------------------------------------------------------------------
//MG_CRT.H - ADDITIONAL CRT REPLACEMENT FUNCTIONS
//-------------------------------------------------------------------
#ifndef _MG_CRT_H_
#define _MG_CRT_H_


extern "C" const int _fltused = 0;	//some OpenGL functions use this thing (only for alpha or ppc usually (?))


//-------------------------------------------------------------------
//STARTUP & SHUTDOWN CODE
//-------------------------------------------------------------------
int __cdecl atexit(void (__cdecl *)())
{
	return 0;
}



//-------------------------------------------------------------------
//MALLOC & FREE
//-------------------------------------------------------------------

#include <malloc.h>

extern "C" void * __cdecl malloc(size_t size)
{
    return HeapAlloc( GetProcessHeap(), 0, size );
}


extern "C" void __cdecl free(void * p)
{
    HeapFree( GetProcessHeap(), 0, p );
}


extern "C" void * __cdecl realloc(void * p, size_t size)
{
    if ( p )
        return HeapReAlloc( GetProcessHeap(), 0, p, size );
    else    // 'p' is 0, and HeapReAlloc doesn't act like realloc() here
        return HeapAlloc( GetProcessHeap(), 0, size );
}


extern "C" void * __cdecl calloc(size_t nitems, size_t size)
{
    return HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, nitems * size );
}


//-------------------------------------------------------------------
//NEW & DELETE
//-------------------------------------------------------------------
void * __cdecl operator new(unsigned int s)
{
    return HeapAlloc( GetProcessHeap(), 0, s );
}

void __cdecl operator delete( void * p )
{
    HeapFree( GetProcessHeap(), 0, p );
}


//-------------------------------------------------------------------
//FORMATTED IN/OUT
//-------------------------------------------------------------------
#include <stdio.h>
#include <stdarg.h>

#define EOF     (-1)

// Force the linker to include USER32.LIB
#pragma comment(linker, "/defaultlib:user32.lib")


extern "C" int __cdecl printf(const char * format, ...)
{
    char szBuff[1024];
    int retValue;
    DWORD cbWritten;
    va_list argptr;
          
    va_start( argptr, format );
    retValue = wvsprintf( szBuff, format, argptr );
    va_end( argptr );

    WriteFile(  GetStdHandle(STD_OUTPUT_HANDLE), szBuff, retValue,
                &cbWritten, 0 );

    return retValue;
}


extern "C" int __cdecl sprintf(char *buffer, const char * format, ...)
{
    int retValue;
    va_list argptr;
          
    va_start( argptr, format );
    retValue = wvsprintf( buffer, format, argptr );
    va_end( argptr );

    return retValue;
}


extern "C" int __cdecl puts(const char * s)
{
    DWORD cbWritten;
    HANDLE hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );

    WriteFile( hStdOut, s, lstrlen(s), &cbWritten, 0 );
    WriteFile( hStdOut, "\r\n", 2, &cbWritten, 0 );

    return (int)(cbWritten ? cbWritten : EOF);
}



//-------------------------------------------------------------------
//STRINGS
//-------------------------------------------------------------------
#include <string.h>

extern "C" int __cdecl _strcmpi(const char *s1, const char *s2)
{
    return lstrcmpi( s1, s2 );
}

extern "C" int __cdecl _stricmp(const char *s1, const char *s2)
{
    return lstrcmpi( s1, s2 );
}


//-------------------------------------------------------------------
//MISC
//-------------------------------------------------------------------
int __cdecl rand()
{
	static int seed = 31337303;
	seed=(seed*15625+1)&32767;
	return seed;
}




//END
#endif