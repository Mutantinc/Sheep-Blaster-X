//-------------------------------------------------------------------
//MG_MATERIAL.H
//-------------------------------------------------------------------
#ifndef _MG_MATERIAL_H_
#define _MG_MATERIAL_H_



class mgMaterial
{
public:
	int Parameters;	//*****c'est quoi déja ça ?

	mgColor AmbientColor;
	mgColor DiffuseColor;
	mgColor SpecularColor;
	//mgColor EmissionColor;

	float Shininess;
	float Transparency;

#ifdef MG_USE_MULTITEXTURING
	mgTexture Mapping0;
	mgTexture Mapping1;
	mgTexture Mapping2;
	mgTexture Mapping3;
	mgTexture Mapping4;
	mgTexture Mapping5;
#else
	mgTexture Mapping;
#endif


#ifdef MG_USE_BUMP_MAPPING
	mgTexture Bump;	//***** 3 textures normalement ?
#endif
 
#ifdef MG_USE_ENVIRONMENT_MAPPING
	mgTexture Environment;
#endif

//-------------------------------------------------------------------

	inline mgMaterial()
	{}


	inline void Set(void)
	{
		glMaterialfv( GL_FRONT, GL_AMBIENT, AmbientColor.Fltpt());
		glMaterialfv( GL_FRONT, GL_DIFFUSE, DiffuseColor.Fltpt());
		glMaterialfv( GL_FRONT, GL_SPECULAR, SpecularColor.Fltpt());
		glMaterialf( GL_FRONT, GL_SHININESS, Shininess );
		if( Mapping.ID != 0 )
			glBindTexture( GL_TEXTURE_2D, Mapping.ID );
	}

//-------------------------------------------------------------------
	
	inline void Import(uint fp)
	{
		uint nbmat = (uint)mgRead16(fp);
		for( uint i=0; i<nbmat; i++ )
		{
			AmbientColor.ReadC(fp);
			DiffuseColor.ReadC(fp);
			SpecularColor.ReadC(fp);

			Shininess = mgRead32(fp);
			Transparency = mgRead32(fp);
			
			uint nbtex = (uint)mgRead16(fp);
			for( uint j=0; j<nbtex; j++ )	//*****a modifier pour que ça gère le multitexturing
				Mapping.Import(fp);
		}
	}
};



//END
#endif
