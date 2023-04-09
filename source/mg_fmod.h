//-------------------------------------------------------------------
//MG_FMOD.H - TO MAKE THINGS EASIER WITH FMOD !
//-------------------------------------------------------------------
#ifndef _MG_FMOD_H_
#define _MG_FMOD_H_

//-------------------------------------------------------------------

#ifdef MG_USE_FMUSIC
#ifdef MG_USE_FMOD
	FSOUND_STREAM * CurStream;
#endif
	FMUSIC_MODULE* CurMod;
#endif

//-------------------------------------------------------------------

// this is if you want to replace the samples with your own (in case you have compressed them)
void sampleloadcallback(void *buff, int lenbytes, int numbits, int instno, int sampno)
{/* nothing */}



void mgSoundSetCalls( uint mode )
{
#ifdef MG_USE_FMUSIC
	mgSetDataAccessMode( mode );
	FSOUND_File_SetCallbacks(mgOpen, mgClose, mgRead, mgSeek, mgTell);
#endif
}


//-------------------------------------------------------------------

void mgSoundInit(uint mode)
{
	mgSoundSetCalls(mode);

#ifdef MG_USE_SMALLEST_CODE

#ifdef MG_USE_FMOD
	FSOUND_Init(44100, 1, 0);
#else	//MG_USE_FMOD
	FSOUND_Init(44100, 0);
#endif	//MG_USE_FMOD

#else	//MG_USE_SMALLEST_CODE

#ifdef MG_USE_FMOD
	if (!FSOUND_Init(44100, 1, 0))
#else	//MG_USE_FMOD
	if (!FSOUND_Init(44100, 0))
#endif	//MG_USE_FMOD
		Error("Player couldn't be initialized");
#endif	//MG_USE_SMALLEST_CODE
}


void mgSoundFree()
{
#ifdef MG_USE_FMOD
	FSOUND_Stream_Close(CurStream);
	CurStream = NULL;
#endif
	
	FMUSIC_FreeSong(CurMod);
	CurMod = NULL;
}


void mgSoundKill()
{
	mgSoundFree();
	FSOUND_Close();
}



//-------------------------------------------------------------------

void mgSoundPlay()
{
	if( CurMod )
		FMUSIC_PlaySong(curMod);
#ifdef MG_USE_FMOD
	else if( CurStream )
		FSOUND_Stream_Play( 0, stream );
#endif
}


//-------------------------------------------------------------------

#ifdef MG_USE_FMOD
uint mgSoundGetVolume()
{
	if( CurMod )
		return FMUSIC_GetMasterVolume(CurMod);
	else if( CurStream )
		return FSOUND_GetSFXMasterVolume();
	else
		return -1;
}


void mgSoundSetVolume(uint volume)
{
	if( CurMod )
		FMUSIC_SetMasterVolume(CurMod, volume);
	else if( CurStream )
		FSOUND_SetSFXMasterVolume(volume);

}
#endif	//MG_USE_FMOD

//-------------------------------------------------------------------

#define mgSoundLoadModule(name)			mgSoundFree(); CurMod = FMUSIC_LoadSong(name)
#define mgSoundCloseModule(stream)		FMUSIC_FreeSong(mod);

#ifdef MG_USE_FMOD
#define	mgSoundLoadStream(name)			mgSoundFree(); CurStream = FSOUND_Stream_OpenFile(name)
#define mgSoundCloseStream(stream)		FSOUND_Stream_Close(stream)
#endif	//MG_USE_FMOD

//-------------------------------------------------------------------

#define mgSoundGetRow(mod)				FMUSIC_GetRow(mod)
#define mgSoundGetOrder(mod)			FMUSIC_GetOrder(mod)


#ifdef MG_USE_FMOD
#define mgSoundSetOrder(mod,order)		FMUSIC_SetOrder(mod,order)		//while( mgSoundGetOrder(mod) != order ) {}		//if FMod too slow to refesh its vars


void mgSoundSetRow(FMUSIC_MODULE* mod, sint row)
{
	uint volsave = mgSoundGetVolume();
	mgSoundSetVolume(0);
	while( FMUSIC_GetRow(CurMod) < row ) {}
	mgSoundSetVolume(volsave);		
}
#endif	//MG_USE_FMOD

//-------------------------------------------------------------------

#define mgSoundGetTime()	mgGetTime()


#ifdef MG_USE_FMOD
#define mgSoundSetTime(stream,ms)		FSOUND_Stream_SetTime(stream,ms)
#endif

//-------------------------------------------------------------------


//END FMOD ADDON
#endif