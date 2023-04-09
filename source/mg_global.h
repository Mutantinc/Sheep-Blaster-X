//-------------------------------------------------------------------
//MG_GLOBAL.H - LES VARS GLOBALES DE L'APPLI
//-------------------------------------------------------------------
#ifndef _MG_GLOBAL_H_
#define _MG_GLOBAL_H_


//-------------------------------------------------------------------
// VARIABLES
//-------------------------------------------------------------------
//***** prévoir une portion afichage et des portions params
//***** gérer aspectratio et fixedsize

uint		mgWidth = 800;
uint		mgHeight = 600;
uint		mgDepth = 32;

#ifdef MG_USE_FULLSCREEN
bool		mgFullScreen = true;
#else
bool		mgFullScreen = false;
#endif

//bool		mgAspectRatio = false;
//bool		mgFixedSize = false;

void		(*mgViewPortMode)();					//how to initialize the screen when the viewport is changed

//-------------------------------------------------------------------

const char*		mgClassName = "MENGI";				//internal name (useful to window RegisterClass ad others)

#ifdef MG_USE_SMALLEST_CODE
#define		mgDesc		""
#define		mgTitle		""
#else
#define		mgDesc		"Mutant inc. application"	//description of the application (<64 chars)
#define		mgTitle		"Mutant OpenGL Viewer"		//window title
#endif

HICON		mgIcon;									//handle of the EXE icon and the top-left corner of the window

//-------------------------------------------------------------------

#define		mgFontName		"Arial"					//font grabbed in windows
#define		mgFontHeight	120						//font height
#define		mgFontWeight	FW_SEMIBOLD	//FW_NORMAL	//font width

//-------------------------------------------------------------------

bool		mgKeys[256];							//keyboard switches
bool		(*mgKeyBoard)();						//function pointer of the keyboard manager

//-------------------------------------------------------------------

uint		mgMouseX, mgMouseY;
bool		mgLButton, mgRButton;

//-------------------------------------------------------------------

#ifdef MG_USE_SMALLEST_CODE
#define		MG_DATA_DEFAULT_ASM						//file storage method
#else
#define		MG_DATA_DEFAULT_FILE
//#define		MG_DATA_DEFAULT_ASM
#endif

//-------------------------------------------------------------------

//#define		mgBlobName		"mp2.dat"
//#define		mgStreamName	"Daft punk - One More Time.mp3"
//#define		mgModName		"mp2.xm"
//#define		scene1			"meuf.mut"

//-------------------------------------------------------------------

//FIN
#endif