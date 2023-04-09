//-----------------------------------------------------------
//MG_EFFECTS.H - THE HEART OF THE APP !
//-----------------------------------------------------------
#ifndef _MG_EFFECTS_H_
#define _MG_EFFECTS_H_


mgTimeLine CurTimeLine;


mgTexture boom01;
mgTexture boom02;
mgTexture boom03;
mgTexture cybmouton1;
mgTexture cybmouton2;
mgTexture fond;
mgTexture main_boom;
mgTexture main_place;
mgTexture main_repos;
mgTexture mine;
mgTexture mouton1;
mgTexture mouton2;
mgTexture title;
mgTexture grille1;
mgTexture grille2;
mgTexture grille3;
mgTexture over;

//-----------------------------------------------------------

uint bonus = 5;
uint score;
uint scorebase;
uint boundx = 710;
uint boundy = 510;
uint border = 35;


uint sheeps = 20;		//number of sheeps
bool sheepactive[20];	//what sheep is active ?
float sheeppos[20][2];	//sheeps' positions
bool sheepdir[20][2];	//1: left/top - 0: right/bottom
uint sheeptime[20];		//base time for move operations
uint sheeptimelen = 20;	//time before a sheep is allowed to move


uint bombs;				//number of bombs (initialized at the begining of a game)
bool bombactive = false;//a bomb is launched ?
float bombpos[2];		//bomb's position
float bombtarget[2];	//where the bomb wants to go
bool bombdir[2];		//1: left/top - 0: right/bottom
uint bombtime;			//base time for move operations
uint bombtimelen = 20;	//time before a bomb is allowed to move
uint bombtimeact;		//time at which the bomb was launched
uint bombtimeout = 10000;//max time the bomb can live
uint bombrange = 80;	//distance of blowing

bool exploding = false;	//is the bomb exploding ?
uint explotime;			//time at which the bomb started exploding

bool gameover = false;	//gameover ?


uint mousex, mousey;	//scaled mouse-coords

//-----------------------------------------------------------
//-----------------------------------------------------------

float dist( float v1[2], float v2[2])
{
	uint h1 = v1[0] - v2[0];
	uint h2 = v1[1] - v2[1];
	return sqrt( h1 * h1 + h2 * h2 );
}

void newbombtarget(void)
{
	bombtarget[0] = (float)mousex;
	bombtarget[1] = (float)mousey;
	if( bombtarget[0] < border )
		bombtarget[0] = border;
				
	if( bombtarget[0] > boundx )
		bombtarget[0] = boundx;
				
	if( bombtarget[1] < border )
		bombtarget[1] = border;
				
	if( bombtarget[1] > boundy )
		bombtarget[1] = boundy;
	bombdir[0] = ( bombtarget[0] < bombpos[0] );
	bombdir[1] = ( bombtarget[1] < bombpos[1] );		
}

//-----------------------------------------------------------

void _titleG(void)
{
	boom01.LoadPCX( boom01_, 0);
	boom01.SetColorKey(PURPLE);
	boom01.Bind(MG_RGBA);

	mine.LoadPCX( mine_, 0);
	mine.SetColorKey(PURPLE);
	mine.Bind(MG_RGBA);

	boom02.LoadPCX( boom02_, 0);
	boom02.SetColorKey(PURPLE);
	boom02.Bind(MG_RGBA);

	boom03.LoadPCX( boom03_, 0);
	boom03.SetColorKey(PURPLE);
	boom03.Bind(MG_RGBA);

	mouton1.LoadPCX( mouton1_, 0);
	mouton1.SetColorKey(PURPLE);
	mouton1.Bind(MG_RGBA);

	mouton2.LoadPCX( mouton2_, 0);
	mouton2.SetColorKey(PURPLE);
	mouton2.Bind(MG_RGBA);
	
	cybmouton1.LoadPCX( cybmouton1_, 0);
	cybmouton1.SetColorKey(PURPLE);
	cybmouton1.Bind(MG_RGBA);

	cybmouton2.LoadPCX( cybmouton2_, 0);
	cybmouton2.SetColorKey(PURPLE);
	cybmouton2.Bind(MG_RGBA);

	grille1.LoadPCX( grille1_, 0);
	grille1.SetColorKey(PURPLE);
	grille1.Bind(MG_RGBA);

	grille2.LoadPCX( grille2_, 0);
	grille2.SetColorKey(PURPLE);
	grille2.Bind(MG_RGBA);

	grille3.LoadPCX( grille3_, 0);
	grille3.SetColorKey(PURPLE);
	grille3.Bind(MG_RGBA);

	main_boom.LoadPCX( main_boom_, 0);
	main_boom.SetColorKey(PURPLE);
	main_boom.Bind(MG_RGBA);

	main_place.LoadPCX( main_place_, 0);
	main_place.SetColorKey(PURPLE);
	main_place.Bind(MG_RGBA);

	main_repos.LoadPCX( main_repos_, 0);
	main_repos.SetColorKey(PURPLE);
	main_repos.Bind(MG_RGBA);

	fond.LoadPCX( fond_, 0);
	fond.ScaleImage( 512, 512, MG_RGBA );
	fond.Bind(MG_RGBA);

	title.LoadPCX( title_, 0);
	title.ScaleImage( 512, 256, MG_RGBA );
	title.Bind(MG_RGBA);

	over.LoadPCX( gameover_, 0);
	over.SetColorKey(PURPLE);
	over.Filtering = false;
	over.Bind(MG_RGBA);


	mgRasterMode();
	ShowCursor(TRUE);
}


void _uninitG(void)
{
	boom01.Unbind();
	boom02.Unbind();
	boom03.Unbind();
	mouton1.Unbind();
	mouton2.Unbind();
	cybmouton1.Unbind();
	cybmouton2.Unbind();
	grille1.Unbind();
	grille2.Unbind();
	grille3.Unbind();
	mine.Unbind();
	fond.Unbind();
	title.Unbind();
	over.Unbind();
	main_repos.Unbind();
	main_place.Unbind();
	main_boom.Unbind();
}

//-----------------------------------------------------------

bool _titleE(void)
{
	mgCls(BLACK);
	mgDrawTex(0, 100, 800, 400, &title, 0, 0, 0, 0);

	if( mgLButton || mgRButton )
	{
		mgLButton = false;
		mgRButton = false;
		return true;
	}
	else
		return false;
}

//-----------------------------------------------------------
//-----------------------------------------------------------

void _gameG(void)
{
	if( mgFullScreen )
		ShowCursor(FALSE);

	bombs = 5;
	score = 0;
	scorebase = 0;
	gameover = false;

	for( uint i=0; i<sheeps; i++)
	{
		sheeppos[i][0] = ((float)rand()*boundx)/32767 + border;
		sheeppos[i][1] = ((float)rand()*boundy)/32767 + border;

		sheepdir[i][0] = (rand() < 16384);
		sheepdir[i][1] = (rand() < 16384);

		sheepactive[i] = true;
		sheeptime[i] = CurTimeLine.Time - rand()%sheeptimelen;
	}
}


//-----------------------------------------------------------


bool _gameE(void)
{
	uint i;

	//MOUSE THINGS
	mousex = (mgMouseX * mgWidth)/cursize.right;	//scaling between the screen pix and the viewport coords
	mousey = (mgMouseY * mgHeight)/cursize.bottom;


	//COMPUTE BACKGROUND & THINGS
	mgCls(BLACK);
	mgDrawTex(0, 0, 800, 600, &fond, 0, 0, 0, 0 );


	//PLACE THE BOMBS REMAINING
	for( i=0; i<bombs; i++)
		mgDrawTex( 10+37*i, 558, &mine );


	//THE SCORE
	glColor4ub(255, 255, 255, 180);
	Text2R( 200, 570, 0.5f, mgFontList, "Toasted:%d", score );

	
	//COMPUTE THE SHEEPS
	for( i=0; i<sheeps; i++)
	{
		if( sheepactive[i] )
		{
			if( !(exploding && dist( sheeppos[i], bombpos ) < bombrange ))	//when there is an exlosion and the sheep is in it, it should not move anymore
			{
				if( rand() < 200 )	//may change the horizontal direction
					sheepdir[i][0] = !sheepdir[i][0];
				
				if( rand() < 200 )	//may change the vertical direction
					sheepdir[i][1] = !sheepdir[i][1];
				
				if( mgTime - sheeptime[i] >= sheeptimelen  )	//allowed to move
				{
					sheeptime[i] = mgTime;
					
					if( rand()<16384 )						//allowed to move horizontally
					{
						if( sheepdir[i][0] )	//heads to the left
						{
							if( sheeppos[i][0] > border )
								sheeppos[i][0]--;
							else
								sheepdir[0][i] = !sheepdir[i][0];
						}
						else					//heads to the right
						{
							if( sheeppos[i][0] < boundx )
								sheeppos[i][0]++;
							else
								sheepdir[0][i] = !sheepdir[i][0];
						}
					}
					
					if( rand()<16384 )
					{
						if( sheepdir[i][1] )	//heads to the top
						{
							if( sheeppos[i][1] > border )
								sheeppos[i][1]--;
							else
								sheepdir[1][i] = !sheepdir[1][i];
						}
						else					//heads to the bottom
						{
							if( sheeppos[i][1] < boundy )
								sheeppos[i][1]++;
							else
								sheepdir[1][i] = !sheepdir[1][i];
						}
					}
				}
				
				if( mgTime%600 < 300 )
					mgDrawTex( sheeppos[i][0], sheeppos[i][1], &mouton1, sheepdir[i][0], false );
				else
					mgDrawTex( sheeppos[i][0], sheeppos[i][1], &mouton2, sheepdir[i][0], false );
			}
			else
			{
				if( mgTime - explotime > 1350 )
				{
					sheepactive[i] = false;
					score++;
					if( score - scorebase == bonus )
					{
						scorebase = score;
						bombs++;
					}
				}
				else if( mgTime - explotime > 1200 )
					mgDrawTex( (sint)sheeppos[i][0], (sint)sheeppos[i][1], &grille3 );

				else if( mgTime - explotime > 1050 )
					mgDrawTex( (sint)sheeppos[i][0], (sint)sheeppos[i][1], &grille2 );

				else if( mgTime - explotime > 900 )
					mgDrawTex( (sint)sheeppos[i][0], (sint)sheeppos[i][1], &grille1 );

				else if( mgTime%600 < 300 )
					mgDrawTex( sheeppos[i][0], sheeppos[i][1], &mouton1, sheepdir[i][0], false );

				else
					mgDrawTex( sheeppos[i][0], sheeppos[i][1], &mouton2, sheepdir[i][0], false );
			}
		}
	}


	//COMPUTE THE BOMB
	if( bombactive && !exploding )
	{
		if( mgTime - bombtime > bombtimelen )					//allowed to move
		{
			bombtime = mgTime;
			uint d = dist( bombpos, bombtarget );
			if( d > 3 )
			{
				bombpos[0] += 2*(bombtarget[0] - bombpos[0])/d;
				bombpos[1] += 2*(bombtarget[1] - bombpos[1])/d;
			}
		}

		if( mgLButton )										//DEFINE TARGET
		{
			newbombtarget();
		}


		//collision with sheeps
		bool proxi = false;
		for( i=0; i<sheeps; i++)
		{
			if( sheepactive[i] && dist( bombpos, sheeppos[i] ) < 12 )
			{
				proxi=true;
				break;
			}
		}


		if( mgRButton || (mgTime - bombtimeact > bombtimeout) || proxi )	//EXPLODE
		{
			exploding = true;
			explotime = mgTime;
		}


		if( mgTime%600 < 300 )
			mgDrawTex( bombpos[0], bombpos[1], &cybmouton1, bombdir[0], false );
		else
			mgDrawTex( bombpos[0], bombpos[1], &cybmouton2, bombdir[0], false );

		glColor4ub(255, 255, 255, 180);
		Text2R( 600, 570, 0.5f, mgFontList, "Countdown:%d", (bombtimeout-mgTime+bombtimeact)/1000 );
	}
	else if( !exploding && mgLButton && bombs != 0 )
	{
		bombactive = true;
		bombpos[0] = 715.f;
		bombpos[1] = 495.f;
		bombtimeact = mgTime;
		bombtime = mgTime;
		bombs--;
		newbombtarget();
	}
	

	//COMPUTE THE EXPLOSION
	if( exploding )
	{
		if( mgTime - explotime > 1500 )
		{
			bombactive = false;
			exploding = false;
			if( bombs == 0 )
				gameover = true;
		}
		else if( mgTime - explotime > 900 )
		{/*to have the bomb03's to disappear before mgTime - explotime > 1500 */}
		else if( mgTime - explotime > 750 )
		{
			mgDrawTex( (sint)bombpos[0]+64, (sint)bombpos[1]+32, &boom03 );
			mgDrawTex( (sint)bombpos[0]+64, (sint)bombpos[1]-32, &boom03 );
			mgDrawTex( (sint)bombpos[0]-64, (sint)bombpos[1]+32, &boom03 );
			mgDrawTex( (sint)bombpos[0]-64, (sint)bombpos[1]-32, &boom03 );
			mgDrawTex( (sint)bombpos[0]+32, (sint)bombpos[1]+64, &boom03 );
			mgDrawTex( (sint)bombpos[0]+32, (sint)bombpos[1]-64, &boom03 );
			mgDrawTex( (sint)bombpos[0]-32, (sint)bombpos[1]+64, &boom03 );
			mgDrawTex( (sint)bombpos[0]-32, (sint)bombpos[1]-64, &boom03 );
		}
		else if( mgTime - explotime > 600 )
		{
			mgDrawTex( (sint)bombpos[0]+32, (sint)bombpos[1]+32, &boom03 );
			mgDrawTex( (sint)bombpos[0]+32, (sint)bombpos[1]-32, &boom03 );
			mgDrawTex( (sint)bombpos[0]-32, (sint)bombpos[1]+32, &boom03 );
			mgDrawTex( (sint)bombpos[0]-32, (sint)bombpos[1]-32, &boom03 );

			mgDrawTex( (sint)bombpos[0]+64, (sint)bombpos[1], &boom03 );
			mgDrawTex( (sint)bombpos[0]-64, (sint)bombpos[1], &boom03 );
			mgDrawTex( (sint)bombpos[0], (sint)bombpos[1]+64, &boom03 );
			mgDrawTex( (sint)bombpos[0], (sint)bombpos[1]-64, &boom03 );
			
			mgDrawTex( (sint)bombpos[0]+64, (sint)bombpos[1]+32, &boom02 );
			mgDrawTex( (sint)bombpos[0]+64, (sint)bombpos[1]-32, &boom02 );
			mgDrawTex( (sint)bombpos[0]-64, (sint)bombpos[1]+32, &boom02 );
			mgDrawTex( (sint)bombpos[0]-64, (sint)bombpos[1]-32, &boom02 );
			mgDrawTex( (sint)bombpos[0]+32, (sint)bombpos[1]+64, &boom02 );
			mgDrawTex( (sint)bombpos[0]+32, (sint)bombpos[1]-64, &boom02 );
			mgDrawTex( (sint)bombpos[0]-32, (sint)bombpos[1]+64, &boom02 );
			mgDrawTex( (sint)bombpos[0]-32, (sint)bombpos[1]-64, &boom02 );
		}
		else if( mgTime - explotime > 450 )
		{
			mgDrawTex( (sint)bombpos[0]+32, (sint)bombpos[1], &boom03 );
			mgDrawTex( (sint)bombpos[0]-32, (sint)bombpos[1], &boom03 );
			mgDrawTex( (sint)bombpos[0], (sint)bombpos[1]+32, &boom03 );
			mgDrawTex( (sint)bombpos[0], (sint)bombpos[1]-32, &boom03 );

			mgDrawTex( (sint)bombpos[0]+32, (sint)bombpos[1]+32, &boom02 );
			mgDrawTex( (sint)bombpos[0]+32, (sint)bombpos[1]-32, &boom02 );
			mgDrawTex( (sint)bombpos[0]-32, (sint)bombpos[1]+32, &boom02 );
			mgDrawTex( (sint)bombpos[0]-32, (sint)bombpos[1]-32, &boom02 );

			mgDrawTex( (sint)bombpos[0]+64, (sint)bombpos[1], &boom02 );
			mgDrawTex( (sint)bombpos[0]-64, (sint)bombpos[1], &boom02 );
			mgDrawTex( (sint)bombpos[0], (sint)bombpos[1]+64, &boom02 );
			mgDrawTex( (sint)bombpos[0], (sint)bombpos[1]-64, &boom02 );
			
			mgDrawTex( (sint)bombpos[0]+64, (sint)bombpos[1]+32, &boom01 );
			mgDrawTex( (sint)bombpos[0]+64, (sint)bombpos[1]-32, &boom01 );
			mgDrawTex( (sint)bombpos[0]-64, (sint)bombpos[1]+32, &boom01 );
			mgDrawTex( (sint)bombpos[0]-64, (sint)bombpos[1]-32, &boom01 );
			mgDrawTex( (sint)bombpos[0]+32, (sint)bombpos[1]+64, &boom01 );
			mgDrawTex( (sint)bombpos[0]+32, (sint)bombpos[1]-64, &boom01 );
			mgDrawTex( (sint)bombpos[0]-32, (sint)bombpos[1]+64, &boom01 );
			mgDrawTex( (sint)bombpos[0]-32, (sint)bombpos[1]-64, &boom01 );
		}
		else if( mgTime - explotime > 300 )
		{
			mgDrawTex( (sint)bombpos[0], (sint)bombpos[1], &boom03 );

			mgDrawTex( (sint)bombpos[0]+32, (sint)bombpos[1], &boom02 );
			mgDrawTex( (sint)bombpos[0]-32, (sint)bombpos[1], &boom02 );
			mgDrawTex( (sint)bombpos[0], (sint)bombpos[1]+32, &boom02 );
			mgDrawTex( (sint)bombpos[0], (sint)bombpos[1]-32, &boom02 );

			mgDrawTex( (sint)bombpos[0]+32, (sint)bombpos[1]+32, &boom01 );
			mgDrawTex( (sint)bombpos[0]+32, (sint)bombpos[1]-32, &boom01 );
			mgDrawTex( (sint)bombpos[0]-32, (sint)bombpos[1]+32, &boom01 );
			mgDrawTex( (sint)bombpos[0]-32, (sint)bombpos[1]-32, &boom01 );

			mgDrawTex( (sint)bombpos[0]+64, (sint)bombpos[1], &boom01 );
			mgDrawTex( (sint)bombpos[0]-64, (sint)bombpos[1], &boom01 );
			mgDrawTex( (sint)bombpos[0], (sint)bombpos[1]+64, &boom01 );
			mgDrawTex( (sint)bombpos[0], (sint)bombpos[1]-64, &boom01 );
		}
		else if( mgTime - explotime > 150 )
		{
			mgDrawTex( (sint)bombpos[0], (sint)bombpos[1], &boom02 );

			mgDrawTex( (sint)bombpos[0]+32, (sint)bombpos[1], &boom01 );
			mgDrawTex( (sint)bombpos[0]-32, (sint)bombpos[1], &boom01 );
			mgDrawTex( (sint)bombpos[0], (sint)bombpos[1]+32, &boom01 );
			mgDrawTex( (sint)bombpos[0], (sint)bombpos[1]-32, &boom01 );
		}
		else
		{
			mgDrawTex( (sint)bombpos[0], (sint)bombpos[1], &boom01 );
		}
	}


	//COMPUTE GAME OVER
	if(gameover)
	{
		mgDrawTex(0, 160, 800, 256, &over, 0, 0, 0, 0 );
		
		if( mgLButton || mgRButton )
		{
			mgLButton = false;
			mgRButton = false;
			return true;
		}
		else
			return false;
	}


	//COMPUTE THE HAND
	if( mgLButton )
		mgDrawTex( mousex, mousey, &main_place, 0xD0, false, true );
	else if( mgRButton )
		mgDrawTex( mousex, mousey, &main_boom, 0xD0, false, true );
	else
		mgDrawTex( mousex, mousey, &main_repos, 0xD0, false, true );

	return false;
}



void _jump(void)
{
	CurTimeLine.CurrentEffect -= 2;
}


//-----------------------------------------------------------
//-----------------------------------------------------------


//EFFECTS' LIST
//ONE SEQUENCE PER LINE
mgEffect CurEffects[]={{1000, -1, -1, mgEffectNull, mgEffectTrue, mgEffectFalse },
/*....BEGIN....*/


{-1, -1, -1, _titleG, mgEffectFalse, _titleE },
{-1, -1, -1, _gameG, mgEffectTrue, _gameE },
{-1, -1, -1, _jump, mgEffectTrue, mgEffectFalse },

/*....END....*/
{0,-1,-1,_uninitG,mgEffectFalse,NULL}};



//END
#endif