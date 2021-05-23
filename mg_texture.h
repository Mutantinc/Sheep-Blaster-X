//-------------------------------------------------------------------
//MG_TEXTURE.H
//-------------------------------------------------------------------
// Attention si utilisation de DirectX:
// les PITCHes (le nb d'octets par scan > RGB(A)*Width) des surfaces DX
// sont alignées zarbi et il faut les retrouver avec GetSurfaceDesc puis ddsd.lPitch
//-------------------------------------------------------------------
// Attention si utilisation d'OpenGL:
// les PITCHes sont alignés par défaut sur 4 pixels
// sont alignées zarbi et il faut les retrouver avec GetSurfaceDesc puis ddsd.lPitch
//-------------------------------------------------------------------
//ALPHA: plus il est bas plus le fragment est rendu transparent
//-------------------------------------------------------------------

#ifndef _MG_TEXTURE_H_
#define _MG_TEXTURE_H_


typedef  AUX_RGBImageRec	mgImage;			//using OpenGL type

#define MG_RGB	GL_RGB
#define MG_RGBA GL_RGBA

//-------------------------------------------------------------------


class mgTexture
{
public:
	mgImage* Image;		//raw datas are there
	uint ID;			//handle when transfered into gfx card memory
	bool UTiling;
	bool VTiling;
	bool Filtering;
	
	
	inline mgTexture()
	{
		UTiling = false;
		VTiling = false;
		Filtering = true;
	}
	
//-------------------------------------------------------------------
	
	inline void ScaleImage( sint width, sint height, uint mode )
	{
		sint depth;	//same depth for in and out
		if( mode == MG_RGB )
			depth = 3;
		else	// if( mode == MG_RGBA )
			depth = 4;
		
		unsigned char* newimage = (unsigned char*)malloc(width*height*depth);
		gluScaleImage( mode, Image->sizeX, Image->sizeY, GL_UNSIGNED_BYTE, Image->data, width, height, GL_UNSIGNED_BYTE, newimage );
		RELEASE(Image->data);
		Image->data = newimage;
		Image->sizeX = width;
		Image->sizeY = height;
	}


	inline void BindNoRelease(uint mode)
	{
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);
		glTexImage2D( GL_TEXTURE_2D, 0, mode == MG_RGB ? 3 : 4, Image->sizeX, Image->sizeY, 0, mode, GL_UNSIGNED_BYTE, Image->data);
		
		if( Filtering)
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		}
		else
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		}
		
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, UTiling ? GL_REPEAT : GL_CLAMP );
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, VTiling ? GL_REPEAT : GL_CLAMP );
	}
	

	inline void Bind(uint mode)
	{
		BindNoRelease(mode);
		RELEASE( Image->data );
		//RELEASE( Image );
	}

	inline void Unbind(void)	//releases gfx card datas only
	{
		glDeleteTextures(1, &ID);
	}
	
	
//-------------------------------------------------------------------
//Import functions	
//-------------------------------------------------------------------
	inline void Import(uint fp)
	{
		uint16 textype = mgRead16(fp);
		if( textype == MG_IMPEXP_TEXTURE )
		{
			if( mgRead16(fp) == MG_IMPEXP_STRING )
			{
				uint texnamesize = (uint)mgRead16(fp);
				char* texname = (char*)malloc(texnamesize);
				mgRead(texname, texnamesize, fp);
				Load(texname, MG_DATA_FILE);
				RELEASE(texname);
			}
			else 
			{}	//*****si ressource ou autre ?
		}
		else if( textype == MG_IMPEXP_TEXPCD )
		{
			PCD();
		}
		
		UTiling = (mgRead8(fp) != 0);
		VTiling = (mgRead8(fp) != 0);
		Filtering = (mgRead8(fp) != 0);
		Bind(MG_RGB);
	}
	

	inline void Load(char* texname, uint mode)
	{
		//*****si besoin sauver le "data access mode" avant de trafiquer et le remettre après
		char* ext = texname+lstrlen(texname)-3;

		if( strcmp(ext, "bmp") == 0)
			LoadBMP(texname, mode);
		else if( strcmp(ext, "pcx") == 0)
			LoadPCX(texname, mode);
		else if( strcmp(ext, "tga") == 0)
			LoadTGA(texname, mode);
		else if( strcmp(ext, "jpg") == 0)
		{}
	}
	
	
//-------------------------------------------------------------------
//BMP Loader (24 -> 24)
//-------------------------------------------------------------------
	inline void LoadBMP(char* name, uint mode)	//only MG_DATA_FILE
	{
#ifdef MG_USE_SMALLEST_CODE
		Image = auxDIBImageLoad(name);
#else
		mgSetDataAccessMode( mode );
		uint handle = (uint)mgOpen( name );

		if( handle == NULL)
		{
			Error( strcat( "GFX file not found: ", name ));
			return; //NULL;
		}
		else
		{
			mgClose( handle );
			//return auxDIBImageLoad(name);
			Image = auxDIBImageLoad(name);
		}
#endif
	}
	
	
	
	
	
//-------------------------------------------------------------------
//PCX Loader (265 cols -> 32 bits)
//-------------------------------------------------------------------
	inline void LoadPCX(char* name, uint mode)
	{
		mgSetDataAccessMode(mode);
		mgImage* image = new mgImage;
		sint val1, val2;
		sint i, j, k;
		
#ifdef MG_USE_SMALLEST_CODE
		uint fp = mgOpen(name);
#else
		uint fp;
		if( (fp=mgOpen(name)) == NULL )
		{
			Error(strcat("GFX file not found: ", name));
			return; //NULL;
		}
#endif

		mgSeek(fp,2,SEEK_SET);	uint8 comp=mgGetc(fp);
		mgSeek(fp,10,SEEK_SET);	uint off = mgRead16(fp);	mgSeek(fp,6,SEEK_SET);	image->sizeY=off-mgRead16(fp)+1;
		off = mgRead16(fp);	mgSeek(fp,4,SEEK_SET);	image->sizeX=off-mgRead16(fp)+1;
		mgSeek(fp,66,SEEK_SET);	sint scan = mgRead16(fp);	//even
		
		image->data = (unsigned char*)malloc( (image->sizeX*image->sizeY+1)*4 );	//+4 bytes (+1*4) because the last pixel may be the pixel from the edge of the last scanline and it would overwrite the int just after the malloc
		
		uint8 pal[769];	//256*(r,g,b)+1 (+1 because i always read the byte after the color to have a 32 bits color which can lead to reading a byte outside the pal buffer for the last color)
		mgSeek(fp,-768,SEEK_END);
		mgRead(pal,768,fp);
		
		mgSeek(fp,0x80L,SEEK_SET);	//header size:128
		
		if(comp)
		{
			for( k=0; k<image->sizeY; k++ )
			{
				for( i=image->sizeX*k; i<image->sizeX*k+scan; )	//recalculating i here ensures we are on the good pixel even if there are artefzcts due to scan
				{
					val1 = mgGetc(fp);
					if( (val1 & 0xC0)==0xC0 )
					{
						val1 -= 0xC0;
						val2 = mgRead8(fp)*3;
						for( j=0; j<val1; j++ )
							((uint32*)image->data)[i++]=*((uint32*)&pal[val2]) | 0xFF000000;
					}
					else
						((uint32*)image->data)[i++]=*((uint32*)&pal[val1*3]) | 0xFF000000;
				}
			}
		}
		else	
		{
			for( k=0; k<image->sizeY; k++ )
				for( i=image->sizeX*k; i<image->sizeX*k+scan; )
					((uint32*)image->data)[i++]=*((uint32*)&pal[mgRead8(fp)*3]) | 0xFF000000;
		}
		
		mgClose(fp);
		//return image;
		Image = image;
	}
	
	
//-------------------------------------------------------------------
//TGA Loader (24/32 bits -> 24/32 bits)
//-------------------------------------------------------------------
	inline void LoadTGA(char* name, uint mode)
	{
		mgSetDataAccessMode(mode);
		mgImage* image = new mgImage;
		uint cnt, val;
		int i,j,k;
		
#ifdef MG_USE_SMALLEST_CODE
		uint fp = mgOpen(name);
#else
		uint fp;
		if ((fp=mgOpen(name))==NULL)
		{
			Error(strcat("GFX file not found: ", name));
			return; //NULL;
		}
#endif
		
		uint head = mgRead8(fp);
		mgSeek(fp,1,SEEK_CUR);	uint comp = mgRead8(fp);
		mgSeek(fp,9,SEEK_CUR);
		image->sizeX = mgRead16(fp);
		image->sizeY = mgRead16(fp);
		uint depth = mgRead8(fp)>>3;	// >>3 is /8
		
		image->data = (unsigned char*)malloc(image->sizeX*image->sizeY*depth+1);	//+1 because of the last alpha value written
		
		
		if(head==0)
			mgSeek(fp,18,SEEK_SET);
		else
			mgSeek(fp,head,SEEK_SET);
		
		
		if( depth == 3 )	//RGB
		{
			if( comp==10 )
			{	
				k=image->sizeY-1;
				i=3*image->sizeX*k;
				
				while( k>=0 )
				{
					cnt = mgGetc(fp);
					if( (cnt&0x80)==0x80 )
					{
						cnt-=0x80;
						
						((uint8*)&val)[2] = mgGetc(fp); ((uint8*)&val)[1] = mgGetc(fp); ((uint8*)&val)[0] = mgGetc(fp);
						for( j=cnt; j>=0 ;j-- )
						{
							*((uint32*)&image->data[i]) = val;
							i+=3;
							
							if( i>=3*image->sizeX*(k+1) )
							{
								k-=1;
								i=3*image->sizeX*k;
							}
						}
					}
					else
					{
						for( j=cnt; j>=0; j--)
						{
							((uint8*)&val)[2] = mgGetc(fp); ((uint8*)&val)[1] = mgGetc(fp); ((uint8*)&val)[0] = mgGetc(fp);
							*((uint32*)&image->data[i]) = val;
							i+=3;
							
							if( i>=3*image->sizeX*(k+1) )
							{
								k-=1;
								i=3*image->sizeX*k;
							}
						}
					}
				}
			}
			else
			{
				for( k=image->sizeY-1; k>=0; k-- )
				{
					for( i=3*image->sizeX*k; i<3*image->sizeX*(k+1); )
					{
						((uint8*)&val)[2] = mgGetc(fp); ((uint8*)&val)[1] = mgGetc(fp); ((uint8*)&val)[0] = mgGetc(fp);
						*((uint32*)&image->data[i]) = val;
						i+=3;
					}
				}
			}
		}
		
		else if( depth == 4 )	//RGBA
		{
			if( comp==10 )
			{	
				k=image->sizeY-1;
				i=4*image->sizeX*k;
				
				while( k>=0 )
				{
					cnt=mgGetc(fp);
					if((cnt&0x80)==0x80)
					{
						cnt-=0x80;
						
						((uint8*)&val)[3] = mgGetc(fp); ((uint8*)&val)[2] = mgGetc(fp); ((uint8*)&val)[1] = mgGetc(fp); ((uint8*)&val)[0] = mgGetc(fp);
						for( j=cnt; j>=0 ;j-- )
						{
							*((uint32*)&image->data[i]) = val;
							i+=4;
							
							if( i>=4*(image->sizeX+1)*k )
							{
								k-=1;
								i=4*image->sizeX*k;
							}
						}
					}
					else
					{
						for( j=cnt; j>=0 ;j-- )
						{
							((uint8*)&val)[3] = mgGetc(fp); ((uint8*)&val)[2] = mgGetc(fp); ((uint8*)&val)[1] = mgGetc(fp); ((uint8*)&val)[0] = mgGetc(fp);
							*((uint32*)&image->data[i]) = val;
							i+=4;
							
							if( i>=4*(image->sizeX+1)*k )
							{
								k-=1;
								i=4*image->sizeX*k;
							}
						}
					}
				}
			}
			else
			{
				for( k=image->sizeY-1; k>=0; k-- )
				{
					for( i=4*image->sizeX*k; i<4*image->sizeX*(k+1); )
					{
						((uint8*)&val)[2] = mgGetc(fp); ((uint8*)&val)[1] = mgGetc(fp); ((uint8*)&val)[0] = mgGetc(fp); ((uint8*)&val)[3] = 0xFF-mgGetc(fp);
						*((uint32*)&image->data[i]) = val;
						i+=4;
					}
				}
			}
		}

		else
		{
			return; //NULL;
		}
		
		mgClose(fp);
		//return Image;
		Image = image;
	}


//-------------------------------------------------------------------
//Procedural generation of textures
//-------------------------------------------------------------------

//can be several functions PCD_WhatYouWant
inline void PCD(void)
{ /******TODO*/ }




//-------------------------------------------------------------------
//Add Alpha layer to image
//-------------------------------------------------------------------

inline void ToRGBA(/*mgImage* Image*/)
{
	uint32* newdata = (uint32*)malloc(Image->sizeX*Image->sizeY*4);
	
	for( sint i=0; i<Image->sizeX*Image->sizeY; i++)
		newdata[i] = *((uint32*)&(Image->data[3*i])) | 0xFF000000;
	
	RELEASE(Image->data);
	Image->data = (unsigned char*)newdata;
}


//-------------------------------------------------------------------
//COLORKEYING
//-------------------------------------------------------------------

inline void SetColorKey(/*mgImage* Image,*/ uint32 c )	//allows to set several colorkeys. c is written by the coder as ABGR (RGBA in memory).
{
	for( sint i=0; i<Image->sizeX*Image->sizeY; i++)
	{
		if( (((uint32*)Image->data)[i] & 0x00FFFFFF) == (c & 0x00FFFFFF) )
			((uint32*)Image->data)[i] &= 0x00FFFFFF;
		
		//if( (((uint32*)Image->data)[i] | 0xFF000000) == (c | 0xFF000000) )
		//	((uint32*)Image->data)[i] |= 0xFF000000;
	}
}


//end of the class

};



//END
#endif
