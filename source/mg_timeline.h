//---------------------------------------------------------------------------------------------
//MG_TIMELINE.H - EFFECTS' CLASS
//---------------------------------------------------------------------------------------------
#ifndef _MG_TIMELINE_H_
#define _MG_TIMELINE_H_



typedef struct
{
	sint Time;				//Time the effect lasts	( < 0 is a neverending effect )
	sint Order;				//Order of the module when the effect starts
	sint Row;				//Row of the Module when the effect starts
	void (*Gfx)();			//Specific graphic function (load/free scene)
	bool (*Sfx)();			//Specific sound function (load/free module), returns true if TimeBase has to be refreshed
	bool (*Efx)();			//The effect itself, returns true if the Effect MUST end.
	bool Initialized;		//true if initialization is complete
	bool Ended;

} mgEffect;




class mgTimeLine
{
public:
	uint FrameTotal;		//total number of frames displayed since the begining of the demo
	uint Frame;				//number of frames displayed in this effect
	
	uint Time;									//time since the begining of the effect (or something else)
	uint TimeBase;			//time at which the demo started (ms since 1970...) for the 1st effect, then when the latest effect should have ended
	uint TimeGap;			//delay between the previous frame and the current one

	uint CurrentEffect;		//effect number
	mgEffect* Effects;		//the list of effects



	mgTimeLine()
	{
		FrameTotal = 0;
		Frame = 0;
		CurrentEffect = 0;
	}
//------------------------------------------------------------------------------------------

	inline void Start(void)
	{
		InitEffect(0);
		ResetTime();
	}

	inline void ResetTime(void)
	{
		Time = 0;
		TimeBase = mgGetTime();
		TimeGap = 0;
	}

//------------------------------------------------------------------------------------------

	inline void InitEffect(uint nb)
	{
		Effects[nb].Initialized = true;
		Effects[nb].Ended = false;

		if( Effects[nb].Gfx )
			(*(Effects[nb].Gfx))();

		if( Effects[nb].Sfx )
			if( (*(Effects[nb].Sfx))() )
				ResetTime();
	}
	
	//------------------------------------------------------------------------------------------
	
	bool Run(void)
	{
		if( Effects[CurrentEffect].Efx )
		{
			TimeGap = mgGetTime() - TimeBase - Time;
			Time = Time + TimeGap;
			
			sint maxtime = Effects[CurrentEffect].Time;
			bool same;

			if( Effects[CurrentEffect].Ended )
				same = false;
			else if( maxtime < 0 )
				same = true;
			else
			{
				
#ifdef MG_USE_FMUSIC
				
				sint order = mgGetOrder(CurMod);
				sint row = mgGetRow(CurMod);
				
				if( Effects[EffectNumber].Order < 0 || Effects[EffectNumber].Row < 0 )	//then time must be used. do not put values when using a mp3 or midi...
					same = ( (signed)Time < maxtime );		//do not replace by <= in case thistm=0; to change the effect
				else
					same = ( order != Effects[EffectNumber].Order || row < Effects[EffectNumber].Row );
#else
				same = (signed)Time < maxtime;
#endif
			}
			

			if( same )	//haven't we reached the end of the effect ?
			{
				Effects[CurrentEffect].Ended = (*(Effects[CurrentEffect].Efx))();
				
				//framecounter + various infos
#ifdef MG_USE_FPS_COUNTER
				uint fps;
				char infos[48];
				
				if( TimeGap )
				{
					fps = 1000/TimeGap;
					sprintf( infos, "fps:%-4d frames:%-4d order:%-4d row:%-4d", fps, FramesTotal, order, row );
				}
				else
					sprintf( infos, "fps:%4s frames:%-4d order:%-4d row:%-4d", "", FramesTotal, order, row );
				
				//***** fonction pour afficher du texte sur l'écran
#endif
				mgFlip();
				FrameTotal++;
				Frame++;
				return true;
			}
			else	//go to next effect
			{
				Effects[CurrentEffect].Ended = true;
				TimeBase += maxtime;
				CurrentEffect++;
				InitEffect(CurrentEffect);
				return true;
			}
			
		}
		else
			return false;

	}
	
	
	

//------------------------------------------------------------------------------------------
//Function to start at some effect with synchonized music (needs Fmod Api)
//------------------------------------------------------------------------------------------
#ifdef MG_USE_FMOD
	void GotoEffect( uint nb )
	{
		uint i=0;

		Start();
		while( Effects[i].Efx && i < nb )	//we must get to the post-init of the asked effect
		{
			TimeBase += Effects[i].Time;
			i++;
			InitEffect(i);
		}
		CurrentEffect = i;


		if( CurMod && Effects[i].Order >= 0 && Effects[i].Row >= 0 )
		{
			mgSoundSetOrder(CurMod,Effects[i].Order);
			mgSoundSetRow(CurMod,Effects[i].Row);
		}
		else if( CurStream )
		{
			mgSoundSetTime(TimeBase);
		}

		//else, 
		//either there is no sound,
		//or there is a time-synch, it's not proper and the music just starts here...
	}
#endif



};



//-------------------------------------------------------------------
//INIT AND UNINIT DUMMY EFFECTS
//-------------------------------------------------------------------
#define mgTime		CurTimeLine.Time
#define mgTimeBase	CurTimeLine.TimeBase
#define mgTimeGap	CurTimeLine.TimeGap


bool mgEffectTrue(void) { return true; }
bool mgEffectFalse(void) { return false; }
void mgEffectNull(void) {}



//END
#endif