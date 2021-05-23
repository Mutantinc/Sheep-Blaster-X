//-------------------------------------------------------------------
//MG_KEYB.H - LES GESTIONNAIRES DU CLAVIER
//-------------------------------------------------------------------
#ifndef _MG_KEYB_H_
#define _MG_KEYB_H_



//-------------------------------------------------------------------
//DEFAUT MANAGER (return false to stop program)
//-------------------------------------------------------------------
bool mgKeyBoardDefault(void)
{
	if (mgKeys[VK_ESCAPE])
	{
		return false;
	}


	if (mgKeys[VK_F1])					//passage fenêtré <-> fullscreen
	{
		mgKeys[VK_F1]=false;
		KillWindow();
		mgFullScreen=!mgFullScreen;
		if (!CreateWin())
		{
			return false;
		}
	}
				
				
#ifdef MG_USE_EXTSETUP
	if (mgKeys[VK_F2])
	{
		mgKeys[VK_F2]=false;
		KillWindow();
		mgExtSetupDlg( hInstance );
		if (!CreateWin())
		{
			return false;
		}
	}
#endif



	return true;
}








//-------------------------------------------------------------------
//MAP KEYBOARD DURING RUNTIME
//-------------------------------------------------------------------
bool mgKeyBoardRunTime(void)
{
	//gérer ici des touches supplémentaires

	return mgKeyBoardDefault();
}






//END
#endif
