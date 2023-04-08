//--------------------------------------------------------------------
// MG_OGL.H - EVERYTHING NEEDED TO DISPLAY SOME STUFF
//--------------------------------------------------------------------
#ifndef _MG_OGL_H_
#define _MG_OGL_H_

//including minimal multitexture extensions
#ifdef MG_USE_OPENGL_EXT
PFNGLACTIVETEXTUREARBPROC	glActiveTextureARB = NULL;
PFNGLMULTITEXCOORD2FARBPROC	glMultiTexCoord2fARB = NULL;
#endif


HINSTANCE	hInstance;								//application handle
HWND		hWnd;									//window handle
HDC			hDC;									//device context
HGLRC		hRC;									//OpenGL rendering context

RECT		winsize = {0, 0, mgWidth, mgHeight};	//saves window size
RECT		fulsize = {0, 0, mgWidth, mgHeight};	//saves fullscreen size
RECT		cursize = {0, 0, mgWidth, mgHeight};	//current size of display

bool		active = false;							//minimized?
bool		done = false;							//signals the end of the application

//******rendre ça plus propre
#include "mg_ogltk.h"								//openGL useful functions



//---------------------------------------------------------------------------------------------
//RESIZING THE OPENGL VIEWPORT
//---------------------------------------------------------------------------------------------
GLvoid ReSizeScene(int width, int height)			//resize and initialize the GL window
{
	if (height==0) {height=1;}						//prevent a divide by zero by making height equal one
	
	glViewport(0, 0, width, height);				//reset the current viewport

	if( mgViewPortMode )
		mgViewPortMode();
}



//---------------------------------------------------------------------------------------------
//OPENGL EXTENSIONS
//---------------------------------------------------------------------------------------------
#ifdef MG_USE_OPENGL_EXT
bool GetExtensions()
{
	if(glActiveTextureARB && glMultiTexCoord2fARB) return true;
	
#ifndef MG_USE_SMALLEST_CODE
	char* str = (char*)glGetString(GL_EXTENSIONS);

	if(!str)
	{
		Error( "OpenGL extensions not present");
		return false;
	}
	
	if( !strstr(str, "GL_ARB_multitexture") )
	{
		Error( "GL_ARB_multitexture OpenGL extension not present" );
		return false;
	}
	
	/*
	if( !strstr(str, "EXT_texture_env_combine") )
	{
	Error( "EXT_texture_env_combine OpenGL extension not present" );
	return false;
	}
	*/
#endif
	
	glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
	glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");

#ifndef MG_USE_SMALLEST_CODE
	if( !(glActiveTextureARB && glMultiTexCoord2fARB) )
	{
		Error( "OpenGL Multitexturing extension pointers not present" );
		return false;
	}
#endif
	
	return true;
}
#endif









//---------------------------------------------------------------------------------------------
//OPENGL DEFAULT INITIALIZATION
//As there are dynamic Render modes now almost everything has been commented
//---------------------------------------------------------------------------------------------

_inline int InitDisplay()
{

	//retrieving the extensions
#ifdef MG_USE_OPENGL_EXT
	if (!GetExtensions())
		return false;
#endif
	
	
	//Setup zbuffer
	glDepthMask(GL_TRUE);							//enables zbuffer writing
	glDepthFunc(GL_LEQUAL);							//zbuffer test type
	//glEnable(GL_DEPTH_TEST);						//zbuffer activation

	
	//Setup Ombrage
	glShadeModel(GL_SMOOTH);						//pretty shading

	
	//Setup Texturing & Blanding
	//glEnable(GL_TEXTURE_2D);						//enables 2D mapping
	//glEnable(GL_BLEND);							//enables transparency
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	
	//Projection
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//GLfloat nearz = 0.001f;						//near zclipping limit
	//GLfloat farz = 10000.0f;						//far zclipper limit
	//glOrtho(0, 1, 0, 1, nearz, farz );			//basic ortho projection
	//gluPerspective( fovy, (GLfloat)mgWidth/(GLfloat)mgHeight, nearz, farz);	//or perspective projection


	//Modelview
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	glHint( GL_PERSPECTIVE_CORRECTION_HINT , GL_NICEST );	//nice perspective correction
	
	
	//Setup Line
	glLineWidth( 2.0f );							//thichness of 1
	glEnable( GL_LINE_SMOOTH );						//antialiased lines
	
	
	//Setup Fog
	glFogi( GL_FOG_MODE, GL_LINEAR  );				//equation used for the fog
	//glFogfv(GL_FOG_COLOR, fogColor);				//fog color
	glFogf(GL_FOG_DENSITY, 0.35f);					//fog density
	glHint(GL_FOG_HINT, GL_DONT_CARE );				//nice fog
	glFogf(GL_FOG_START, 1.0f);						//begining of the fog
	glFogf(GL_FOG_END, 0.7f);						//end of the fog
	glDisable(GL_FOG);								//no fog as defaut

	glClearDepth(1.0f);								//zbuffer value when emptied
	return true;
}










//---------------------------------------------------------------------------------------------
//KILLS CURRENT DISPLAY
//---------------------------------------------------------------------------------------------
void KillWindow(void)								//properly kill the window
{

	if (mgFullScreen)								//are we in fullscreen mode?
	{
		GetWinP(hWnd, &fulsize);
		GetWinS(hWnd, &fulsize);

		ChangeDisplaySettings(NULL,0);				//if so switch back to the desktop...and it calls WM_SIZE !!
		ShowCursor(TRUE);							//show mouse pointer
	}
	else
	{
		GetWinP(hWnd, &winsize);
		GetWinS(hWnd, &winsize);
	}

	
#ifdef MG_USE_SMALLEST_CODE
	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(hRC);
	hRC = NULL;
	ReleaseDC(hWnd,hDC);
	hDC = NULL;
	DestroyWindow(hWnd);
	hWnd = NULL;
	UnregisterClass(mgClassName,hInstance);
	hInstance=NULL;
#else

	if (hRC)										//do we have a rendering context?
	{
		if (!wglMakeCurrent(NULL,NULL))				//are we able to release the DC and RC contexts?
		{
			Error("Release of DC and RC failed");
		}
		
		if (!wglDeleteContext(hRC))					//are we able to delete the RC?
		{
			Error("Release Rendering Context failed");
		}
		hRC = NULL;									//set RC to NULL
	}
	
	if (hDC && !ReleaseDC(hWnd,hDC))				//are we able to release the DC
	{
		Error("Release Device Context failed");
		hDC=NULL;									//set DC to NULL
	}
	
	if (hWnd && !DestroyWindow(hWnd))				//are we able to destroy the window?
	{
		Error("Could not release hWnd");
		hWnd=NULL;									//set hWnd to NULL
	}
	
	if (!UnregisterClass(mgClassName,hInstance))	//are we able to unregister class ?
	{
		//Error("Could not unregister window class.");	//anyway an application always destroys registered windows instances when stopping
		hInstance=NULL;
	}
#endif

}










//---------------------------------------------------------------------------------------------
//MAIN MESSAGE LOOP DEFINED
//---------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );










//---------------------------------------------------------------------------------------------
//OPENS AN OPENGL DISPLAY
//---------------------------------------------------------------------------------------------
bool CreateWin()
{
	GLuint		PixelFormat;						//holds the results after searching for a match
	WNDCLASS	wc;									//Windows Class structure
	DWORD		dwExStyle;							//Window Extended Style
	DWORD		dwStyle;							//Window Style
	RECT		WindowRect;


	WindowRect.left = 0;
	WindowRect.top = 0;

	if (mgFullScreen)
	{
		WindowRect.right = fulsize.right;
		WindowRect.bottom = fulsize.bottom;
	}
	else
	{
		WindowRect.right = winsize.right;
		WindowRect.bottom = winsize.bottom;
	}

	
	hInstance			= GetModuleHandle(NULL);				//grab an instance for our window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	//redraw on size, and own DC for window.
	wc.lpfnWndProc		=  (WNDPROC) WndProc;					//WndProc handles messages
	wc.cbClsExtra		= 0;									//no extra window data
	wc.cbWndExtra		= 0;									//no extra window data
	wc.hInstance		= hInstance;							//set the instance
	wc.hIcon			= mgIcon;								//load the default icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			//load the arrow pointer
	wc.hbrBackground	= NULL;									//no background required for GL
	wc.lpszMenuName		= NULL;									//we don't want a menu
	wc.lpszClassName	= mgClassName;							//set the class name

#ifdef MG_USE_SMALLEST_CODE
	RegisterClass(&wc);
#else
	if (!RegisterClass(&wc))									//attempt to register the window class
	{
		Error("Failed to register the window class");
		return false;
	}
#endif

	if (mgFullScreen)
	{
		DEVMODE dmScreenSettings;								//device mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	//makes sure memory's cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		//size of the devmode structure
		dmScreenSettings.dmPelsWidth	= mgWidth;				//selected screen width
		dmScreenSettings.dmPelsHeight	= mgHeight;				//selected screen height
		dmScreenSettings.dmBitsPerPel	= mgDepth;				//selected bits per pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
		
		//try to set selected mode and get results.  NOTE: CDS_FULLSCREEN gets rid of start bar.
#ifdef MG_USE_SMALLEST_CODE
		ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN);
#else
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			//if the mode fails, offer two options.  quit or use windowed mode.
			if (MsgYesNo("The requested fullscreen mode is not supported by\nyour video card. Use windowed mode instead?"))
			{
				mgFullScreen = false;
			}
			else
			{
				//pop up a message box letting user know the program is closing.
				Error("Program will now close.");
				return false;
			}
		}
#endif
	}
	
	if (mgFullScreen)								//are we still in fullscreen mode?
	{
		dwExStyle=WS_EX_APPWINDOW;					//window extended style
		dwStyle=WS_POPUP;							//windows style
		ShowCursor(FALSE);							//hide mouse pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;//window extended style
		dwStyle=WS_OVERLAPPEDWINDOW;				//windows style
	}
	
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);	//adjust window to TRUE requested size
	
	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,						//extended style for the window
		mgClassName,								//Class name
		mgTitle,									//window title
		dwStyle |									//defined window style
		WS_CLIPSIBLINGS |							//required window style
		WS_CLIPCHILDREN,							//required window style
		mgFullScreen?fulsize.left:winsize.left, mgFullScreen?fulsize.top:winsize.top,			// Window Position
		WindowRect.right-WindowRect.left,			//calculate window width
		WindowRect.bottom-WindowRect.top,			//calculate window height
		NULL,										//no parent window
		NULL,										//no menu
		hInstance,									//instance
		NULL)))										//don't pass anything to WM_CREATE
	{
		KillWindow();								//reset the display
#ifndef MG_USE_SMALLEST_CODE
		Error("Window creation error");
#endif
		return false;
	}
	
	PIXELFORMATDESCRIPTOR pfd=						//pfd tells Windows how we want things to be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				//size of this pixel format descriptor
			1,										//version number
			PFD_DRAW_TO_WINDOW |					//format must support window
			PFD_SUPPORT_OPENGL |					//format must support OpenGL
			PFD_DOUBLEBUFFER,						//must support double buffering
			PFD_TYPE_RGBA,							//request an RGBA format
			mgDepth,								//select our color depth
			8, 0, 8, 0, 8, 0,						//color bits
			8,										//8 bits for the alpha buffer, which DOES enable it.
			0,										//shift bit ignored
			0,										//no accumulation buffer
			0, 0, 0, 0,								//accumulation bits ignored
			16,										//16bit zbuffer (depth buffer)  
			0,										//no stencil buffer
			0,										//no auxiliary buffer
			PFD_MAIN_PLANE,							//main drawing layer
			0,										//reserved
			0, 0, 0									//layer masks ignored
	};

	
#ifdef MG_USE_SMALLEST_CODE
	hDC=GetDC(hWnd);
	PixelFormat=ChoosePixelFormat(hDC,&pfd);
	SetPixelFormat(hDC,PixelFormat,&pfd);
	hRC=wglCreateContext(hDC);
	wglMakeCurrent(hDC,hRC);
#else
	if (!(hDC=GetDC(hWnd)))							//did we get a device context?
	{
		KillWindow();								//reset the display
		Error("Can't create a GL Device Context");
		return false;
	}
	
	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	//did Windows find a matching pixel format?
	{
		KillWindow();								//reset the display
		Error("Can't find a suitable PixelFormat");
		return false;
	}
	
	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		//are we able to set the pixel format?
	{
		KillWindow();								//reset the display
		Error("Can't set the PixelFormat");
		return false;
	}
	
	if (!(hRC=wglCreateContext(hDC)))				//are we able to get a rendering context?
	{
		KillWindow();								//reset the display
		Error("Can't create a GL Rendering Context");
		return false;
	}
	
	if(!wglMakeCurrent(hDC,hRC))					//try to activate the rendering context
	{
		KillWindow();								//reset the display
		Error("Can't activate the GL Rendering Context");
		return false;
	}
#endif	
	ShowWindow(hWnd,SW_SHOW);						//displays the window
	SetForegroundWindow(hWnd);						//slightly higher priority
	SetFocus(hWnd);									//sets keyboard focus to the window

	
	/****** may not be necessary because ShowWindow generates a WM_SIZE message... */
	//---------------------------------------------------
	mgFullScreen ? cursize = fulsize : cursize = winsize;
	ReSizeScene(cursize.right, cursize.bottom);
	//---------------------------------------------------

#ifdef MG_USE_SMALLEST_CODE
	InitDisplay();
#else
	if (!InitDisplay())								//initialize our newly created GL window
	{
		KillWindow();								//reset the display
		Error("Initialization failed");
		return false;
	}
#endif

	return true;									//success
}










//---------------------------------------------------------------------------------------------
// corps de la boucle de messages principale
//---------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch (uMsg)
	{
	case WM_ACTIVATE:
		{
			if (!HIWORD(wParam))
				active = true;
			else
				active = false;
			
			return 0;
		}
		
	case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
			case SC_SCREENSAVE:
			case SC_MONITORPOWER:
				return 0;
			}
			break;
		}
		
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		
	case WM_KEYDOWN:
		{
			mgKeys[wParam] = true;
			return 0;
		}
		
	case WM_KEYUP:
		{
			mgKeys[wParam] = false;
			return 0;
		}

	case WM_MOUSEMOVE:
		{
			mgMouseX = LOWORD(lParam);
			mgMouseY = HIWORD(lParam);
			return 0;
		}

	case WM_LBUTTONDOWN:
		{
			mgLButton = true;
			return 0;
		}

	case WM_LBUTTONUP:
		{
			mgLButton = false;
			return 0;
		}

	case WM_RBUTTONDOWN:
		{
			mgRButton = true;
			return 0;
		}

	case WM_RBUTTONUP:
		{
			mgRButton = false;
			return 0;
		}

	case WM_MOVE:
		{
			GetWinP(hWnd, &cursize);
			return 0;
		}

	case WM_SIZE:
		{
			GetWinS(hWnd, &cursize);
			ReSizeScene(LOWORD(lParam), HIWORD(lParam));
			return 0;
		}
	}
	
	//Pass aAll unhandled messages to DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}





//FIN
#endif
