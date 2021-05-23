//-------------------------------------------------------------------
//MG.OGL2D.H - 2D SPECIFIC RASTER FUNCTIONS
//-------------------------------------------------------------------
#ifndef _MG_OGL2D_H_
#define _MG_OGL2D_H_


#define MG_RGB	GL_RGB
#define MG_RGBA	GL_RGBA


//-------------------------------------------------------------------
//SWITCHES TO 2D MODE - (0,0) ON TOP-LEFT OR 3D MODE
//-------------------------------------------------------------------

void mgRasterMode(void)
{
	mgViewPortMode = mgRasterMode;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	uint32 white_ = WHITE;
	glColor4ubv((unsigned char*)&white_);
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	

	glDisable(GL_DEPTH_TEST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	//1 seems to be the size of 1 int (?), so aligned

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (float)mgWidth, (float)mgHeight, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void mg3DMode(void)
{
	mgViewPortMode = mg3DMode;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	uint32 white_ = WHITE;
	glColor4ubv((unsigned char*)&white_);
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//*****set primary prespective here

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//-------------------------------------------------------------------
//RASTERIZATION FUNCTIONS
//-------------------------------------------------------------------
//need to be in raster mode, and especially...
//glEnable(GL_TEXTURE_2D);
//glEnable(GL_BLEND);
//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//-------------------------------------------------------------------

void mgDrawTex(sint xd, sint yd, sint wd, sint hd, mgTexture* tex, sint xs, sint ys, sint ws, sint hs, uint opa, bool hflip, bool vflip )
{
	glBindTexture(GL_TEXTURE_2D, tex->ID);

	if( !opa )		//nothing to draw
		return;
	
	if( !wd || !hd )
	{ wd=tex->Image->sizeX; hd=tex->Image->sizeY; }
	
	if( !ws || !hs )
	{ xs=0; ys=0; ws=tex->Image->sizeX; hs=tex->Image->sizeY; }

	glBegin(GL_QUADS);
	glColor4ub(255,255,255,opa);
	uint x1 = (float)xs/(float)tex->Image->sizeX;
	uint x2 = (float)(xs+ws)/(float)tex->Image->sizeX;
	uint y1 = (float)ys/(float)tex->Image->sizeY;
	uint y2 = (float)(ys+hs)/(float)tex->Image->sizeY;
	uint txy;

	if(hflip)
	{
		txy = x1;
		x1 = x2;
		x2 = txy;
	}

	if(vflip)
	{
		txy = y1;
		y1 = y2;
		y2 = txy;
	}

	glTexCoord2f(x1, y2);
	glVertex2i( xd, yd );
	
	glTexCoord2f(x2, y2);
	glVertex2i( xd+wd, yd );
	
	glTexCoord2f(x2, y1);
	glVertex2i( xd+wd, yd+hd );
	
	glTexCoord2f(x1, y1);
	glVertex2i( xd, yd+hd );
	
	glEnd();
}


//coords opa
void mgDrawTex(sint xd, sint yd, sint wd, sint hd, mgTexture* tex, sint xs, sint ys, sint ws, sint hs, uint opa )
{
	mgDrawTex(xd, yd, wd, hd, tex, xs, ys, ws, hs, opa, false, false );
}

//coords flip
void mgDrawTex(sint xd, sint yd, sint wd, sint hd, mgTexture* tex, sint xs, sint ys, sint ws, sint hs, bool hflip, bool vflip )
{
	mgDrawTex(xd, yd, wd, hd, tex, xs, ys, ws, hs, 0xFF, hflip, vflip );
}

//coords
void mgDrawTex(sint xd, sint yd, sint wd, sint hd, mgTexture* tex, sint xs, sint ys, sint ws, sint hs )
{
	mgDrawTex(xd, yd, wd, hd, tex, xs, ys, ws, hs, 0xFF, false, false );
}

//opa flip
void mgDrawTex(sint xd, sint yd, mgTexture* tex, uint opa, bool hflip, bool vflip )
{
	mgDrawTex(xd, yd, 0, 0, tex, 0, 0, 0, 0, opa, hflip, vflip );
}

//opa
void mgDrawTex(sint xd, sint yd, mgTexture* tex, uint opa )
{
	mgDrawTex(xd, yd, 0, 0, tex, 0, 0, 0, 0, opa, false, false );
}

//flip
void mgDrawTex(sint xd, sint yd, mgTexture* tex, bool hflip, bool vflip )
{
	mgDrawTex(xd, yd, 0, 0, tex, 0, 0, 0, 0, 0xFF, hflip, vflip );
}

//no transformation
void mgDrawTex(sint xd, sint yd, mgTexture* tex )
{
	mgDrawTex(xd, yd, 0, 0, tex, 0, 0, 0, 0, 0xFF, false, false );
}



//---------------------------------------------------------------------------------------------
//CLEAR THE SCREEN
//---------------------------------------------------------------------------------------------

void mgCls( uint32 col )
{
	mgVector v = mgVector(col);
	glClearColor( v.r, v.g, v.b, v.a );
	glClear(GL_COLOR_BUFFER_BIT);
}



void mgClsRectEx(sint xd, sint yd, sint wd, sint hd, uint opa, uint32 col)
{
	mgTexture tex;
	mgImage* image = new mgImage;
	image->sizeX = image->sizeY = 1;
	image->data = (unsigned char*)&col;
	tex.Image = image;
	tex.BindNoRelease(MG_RGBA);
	mgDrawTex(xd, yd, wd, hd, &tex, 0, 0, 0, 0, opa);
	tex.Unbind();
}



void mgClsRect(sint xd, sint yd, sint wd, sint hd, uint32 col)
{
	mgClsRectEx(xd, yd, wd, hd, 0xFF, col);
}


/*
//------------------------------------------------------------
//FADE IN/OUT
//------------------------------------------------------------
void mgDIf(int xd, int yd, mgImage* image, int opa)
{
}





//---------------------------------------------------------------------------------------------
//TV DISPLAY (2 normal, 2 dimed...)
//---------------------------------------------------------------------------------------------
void mgTV(int xd, int yd, int wd, int hd)
{
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	dsurf->GetSurfaceDesc(&ddsd);

	BYTE* surfad=Lock(dsurf);
	Unlock(dsurf);

	int off1=(yd+1)*ddsd.lPitch+4*xd, off2;
	
	for (int tvy=0; tvy<hd; tvy+=4)
	{
		off2=off1;

		for (int tvx=0; tvx<wd; tvx++)
		{
			surfad[off2]=surfad[off2]/2;
			surfad[off2+1]=surfad[off2+1]/2;
			surfad[off2+2]=surfad[off2+2]/2;
			off2+=4;
		}

		off1+=ddsd.lPitch;
		off2=off1;

		for (int tvx2=0; tvx2<wd; tvx2++)
		{
			surfad[off2]=surfad[off2]/2;
			surfad[off2+1]=surfad[off2+1]/2;
			surfad[off2+2]=surfad[off2+2]/2;
			off2+=4;
		}

		off1+=3*ddsd.lPitch;
	}
}




//---------------------------------------------------------------------------------------------
//SNOW
//---------------------------------------------------------------------------------------------
void mgSnow(int x, int y, int w, int h)
{
	int i,j;
	int da,dai;
	char s;
	BYTE* surfad=Lock(dsurf);
	Unlock(dsurf);

	dai=4*(pitch*y+x);
	for (j=y;j<y+h;j++)
	{
		da=dai;

		for (i=x;i<x+w;i++)
		{
			s=rand()%256;
			surfad[da]=s;
			surfad[da+1]=s;
			surfad[da+2]=s;
			//surfad[da++]=0;
			da+=4;
		}
		dai+=4*pitch;
	}
}


//---------------------------------------------------------------------------------------------
//MOSAIC : TO DISPLAY THE SAME PATTERN EVERYWHERE ON THE SCREEN
//---------------------------------------------------------------------------------------------
void mgMosaic(int x, int y, mgImage* image)
{

	int nw=width/ddsd.dwWidth;
	if(x%ddsd.dwWidth != 0)
		nw++;
	if((width-x)%ddsd.dwWidth != 0)
		nw++;

	int nh=height/ddsd.dwHeight;
	if(y%ddsd.dwHeight != 0)
		nh++;
	if((height-y)%ddsd.dwHeight != 0)
		nh++;

	int xo=(x+ddsd.dwWidth-1)%ddsd.dwWidth-ddsd.dwWidth+1;
	int yo=(y+ddsd.dwHeight-1)%ddsd.dwHeight-ddsd.dwHeight+1;


	int i,j;
	for(j=0;j<nh;j++)
		for(i=0;i<nw;i++)
			dI(dsurf,xo+i*ddsd.dwWidth,yo+j*ddsd.dwHeight,ddsd.dwWidth,ddsd.dwHeight,image,0,0,ddsd.dwWidth,ddsd.dwHeight);
}

*/


//END
#endif
