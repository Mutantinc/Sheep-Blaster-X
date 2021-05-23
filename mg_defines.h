//-------------------------------------------------------------------

// MG_DEFINES.H - TOUS LES DEFINES
//-------------------------------------------------------------------
#ifndef _MG_DEFINES_H_
#define _MG_DEFINES_H_


//-------------------------------------------------------------------
//WINDOWS
//-------------------------------------------------------------------
#define WIN32_LEAN_AND_MEAN



//-------------------------------------------------------------------
//GRAPHICS
//-------------------------------------------------------------------
#undef MG_USE_FULLSCREEN
#define MG_USE_FULLSCREEN

#undef MG_USE_OPENGL
#define MG_USE_OPENGL
#undef MG_USE_OPENGL_EXT
//#define MG_USE_OPENGL_EXT

#undef MG_USE_TRIANGLES_ONLY
#define MG_USE_TRIANGLES_ONLY			//don't touch, i didn't implement the other case !

#undef MG_USE_VERTEX_COLORS
//#define MG_USE_VERTEX_COLORS

#undef MG_USE_MULTITEXTURING			//Maximum 6 textures in this engine
//#define MG_USE_MULTITEXTURING

#undef MG_USE_ENVIRONMENT_MAPPING
//#define MG_USE_ENVIRONMENT_MAPPING

#undef MG_USE_BUMP_MAPPING
//#define MG_USE_BUMP_MAPPING

#ifndef MG_USE_OPENGL_EXT
#undef MG_USE_MULTITEXTURING
#endif



//-------------------------------------------------------------------
//SOUND
//-------------------------------------------------------------------
#undef MG_USE_FMOD
#undef MG_USE_MINIFMOD
//#define MG_USE_FMOD		//to use Fmod (defaut if both defined)
//#define MG_USE_MINIFMOD	//to use MiniFmod

//-------------------------------------------------------------------

#undef MG_USE_FMUSIC
#ifdef MG_USE_MINIFMOD
#define MG_USE_FMUSIC
#endif

#ifdef MG_USE_FMOD
#define MG_USE_FMUSIC
#endif

#ifdef MG_USE_FMOD
#ifdef MG_USE_MINIFMOD
#undef MG_USE_MINIFMOD
#endif
#endif



//-------------------------------------------------------------------
//SETUP
//-------------------------------------------------------------------
#undef MG_USE_SETUP
//#define MG_USE_SETUP

#undef MG_USE_EXTSETUP
//#define MG_USE_EXTSETUP



//-------------------------------------------------------------------
//MISC
//-------------------------------------------------------------------
#undef MG_USE_SMALLEST_CODE
#define MG_USE_SMALLEST_CODE		//THE thing to change to make intros

#ifdef _DEBUG
#undef MG_USE_SMALLEST_CODE
#endif

#undef MG_USE_FPS_COUNTER
#ifdef _DEBUG
//#define MG_USE_FPS_COUNTER		//FPS counter...
#endif


#undef MG_USE_TEMPO
//#define MG_USE_TEMPO

//-------------------------------------------------------------------

//END
#endif