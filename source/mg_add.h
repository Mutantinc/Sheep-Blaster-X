//-------------------------------------------------------------------
//MG_ADD.H - MORE BAZAAR
//-------------------------------------------------------------------
#ifndef _MG_ADD_H_
#define _MG_ADD_H_


//---------------------------------------------------------------------------------------------
// VARIOUS
//---------------------------------------------------------------------------------------------
#define mgPi 3.1415926535897932384626433832795f


//---------------------------------------------------------------------------------------------
// COLORS
//---------------------------------------------------------------------------------------------
//						0xAABBGGRR
#define		BLACK		0xFF000000
#define		RED			0xFF0000FF
#define		GREEN		0xFF00FF00
#define		BLUE		0xFFFF0000
#define		YELLOW		0xFF00FFFF
#define		PURPLE		0xFFFF00FF
#define		CYAN		0xFFFFFF00
#define		WHITE		0xFFFFFFFF



//---------------------------------------------------------------------------------------------
// MESSAGES & ERRORS
//---------------------------------------------------------------------------------------------
#ifndef MG_USE_SMALLEST_CODE

void Msg(char *str)
{
	MessageBox( NULL, str, "Message...", MB_OK|MB_TASKMODAL );
}


bool MsgYesNo(char *str)
{
	if (MessageBox( NULL, str, "Yes/No", MB_YESNO|MB_ICONEXCLAMATION ) == IDYES)
		return true;
	else
		return false;
}


void Error(char *str)
{
	MessageBox( NULL, str, "Boulette!", MB_ICONERROR|MB_OK|MB_TASKMODAL );
}

#endif

//---------------------------------------------------------------------------------------------
// WINDOWS SIZES
// NB: only modifies the two strictly necessary values
//---------------------------------------------------------------------------------------------
void GetWinP(HWND hWnd, RECT* rect)	//position de la fenêtre (Window coords)
{
	RECT tmprect;
	GetWindowRect(hWnd, &tmprect);
	(*rect).left = tmprect.left;
	(*rect).top = tmprect.top;
}


void GetWinS(HWND hWnd, RECT* rect)	//taille de la fenêtre (Client coords)
{
	RECT tmprect;
	GetClientRect(hWnd, &tmprect);
	(*rect).right = tmprect.right;
	(*rect).bottom = tmprect.bottom;
}





//END
#endif