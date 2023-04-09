//---------------------------------------------------------------------------------------------
//MG_TIME.H - FUNCTIONS THAT DEAL WITH TIME
//---------------------------------------------------------------------------------------------
#ifndef _MG_TIME_H_
#define _MG_TIME_H_


#include <mmsystem.h>				//for timeGetTime()
//#pragma comment(lib, "winmm.lib")



uint mgGetTime(void)
{
#ifdef MG_USE_FMUSIC
	if( CurMod )
		return FMUSIC_GetTime(CurMod);
#ifdef MG_USE_FMOD
	else if( CurStream )
		return FSOUND_GetTime(CurStream);
#endif
	else
#endif
		return timeGetTime();
}


#endif