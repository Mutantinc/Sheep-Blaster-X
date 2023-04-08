//-------------------------------------------------------------------
//MINIMAL.CPP - SOURCE DE BASE: INCLUDES + WINMAIN
//-------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------------------------
#include <windows.h>				//for Windows
#include "mg_defines.h"				//all defines

#ifndef MG_USE_SMALLEST_CODE
#include <stdio.h>					//I/O
#else
#include "mg_crt.h"					//crt functions replacement
#endif

#include "rtlib2.h"					//maths functions replacement
#include "resource.h"				//resources declarations

//-----------------------------------------------------------

#include "mg_types.h"				//basic types
#include "mg_global.h"				//global vars
#include "mg_mem.h"					//files and memory management
#include "mg_add.h"					//bazaar & misc
#include "mg_impexp.h"

//-----------------------------------------------------------

#ifdef MG_USE_FMUSIC
#include "mg_fmod.h"

#ifdef MG_USE_FMOD					//sound & melody
#include "fmod.h"
#elif defined(MG_USE_MINIFMOD)
#include "minifmod.h"
#endif

#endif

//-----------------------------------------------------------
#include "mg_vector.h"
//-----------------------------------------------------------

#ifdef MG_USE_OPENGL
#include <gl/gl.h>					//OpenGL 32
#include <gl/glu.h>					//GLu32
#include <gl/glaux.h>				//GLaux
#ifdef MG_USE_OPENGL_EXT
#include "glext.h"					//extensions openGL 1.2 (multitexturing...)
#endif
#include "mg_ogl.h"
#endif

//#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "glu32.lib")
//#pragma comment(lib, "glaux.lib")

//-----------------------------------------------------------

#include "mg_texture.h"				//graphic files & textures management
#include "mg_material.h"
#include "mg_ogl2D.h"				//additional 2D-only OpenGL functions

//-----------------------------------------------------------

#include "mg_time.h"				//functions to manage time
#include "mg_timeline.h"			//effects' structure

#ifdef MG_USE_TEMPO
#include "mg_tempo.h"				//effets' scheduler
#endif

//-----------------------------------------------------------

#ifdef MG_USE_SETUP
#include "mg_setup.h"				//setup dialog
#endif


#ifdef MG_USE_EXTSETUP
#include "mg_extsetup.h"			//runtime setup
#endif

//-----------------------------------------------------------

#include "mg_keyb.h"				//keyboard management

//-----------------------------------------------------------

#include "data.h"					//included data big setup
#include "mg_effects.h"				//the effects temselves

//-----------------------------------------------------------


//WINMAIN
#ifndef MG_USE_SMALLEST_CODE
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
#else
int WINAPI WinMainCRTStartup()
#endif
{

//*****
//#ifdef MG_USE_SMALLEST_CODE
//	mgIcon = LoadIcon(NULL, IDI_APPLICATION);
//#else
	mgIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MPICON));	//loads the icon
//#endif

	//***** pas propre (la var globale s'initialise pas correctement va savoir pourquoi)
	//***** donc à comprendre et fixer
	winsize.right = mgWidth;
	winsize.bottom = mgHeight;
	fulsize.right = mgWidth;
	fulsize.bottom = mgHeight;

	MSG msg;
	active=true;

	
	//KEYBOARD INITIALIZATION
	memset(mgKeys, 0, sizeof(mgKeys));		//clears keyboard buffer
	mgKeyBoard = &mgKeyBoardDefault;				//default keyboard manager

	
	//IMAGE INITIALIZATION
#ifdef MG_USE_SETUP
	if ( mgSetupDlg(hInstance, &mgFullScreen) )
	{
		if ( !CreateWin() )
			KillWindow();
	}
	else
	{
		done = true;
	}
	
#else
	if ( !CreateWin() )
		KillWindow();
	
#endif


	//FONT INITIALIZATION (hDC must exist, so placed after CreateWin)
	mgFontInit(hDC, mgFontName, mgFontHeight, mgFontWeight, &mgFontList, mgFontTex);


	//SOUND INITIALIZATION
#ifdef MG_USE_FMUSIC
	mgSoundInit(0);
#endif


	//TIMELINE INITIALIZATION
	CurTimeLine.Effects = CurEffects;
	CurTimeLine.Start();


	//END OF INITIALIZATION
	//-----------------------------------------------------------

	//BEGINING OF THE MESSAGES' PUMP
	while( !done )
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				done = true;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (active && !CurTimeLine.Run())		//draws the scene
			{
				done = true;						//if failed
			}
			else
			{
				if(!mgKeyBoard()) {done = true;}
			}
		}
	}
	

	//SHUTDOWN EVERYTHING
#ifdef MG_USE_FMUSIC
	mgSoundKill();
#endif
	KillWindow();
	mgFontKill(mgFontList);

	return (msg.wParam);
}





//END

