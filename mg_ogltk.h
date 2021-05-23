//--------------------------------------------------------------------
// MG_OGLTK.H - LA BOITE A OUTILS OPENGL
//--------------------------------------------------------------------
#ifndef _MG_OGLTK_H_
#define _MG_OGLTK_H_




//---------------------------------------------------------------------------------------------
//FUNCTIONS ENABLE/DISABLE
//---------------------------------------------------------------------------------------------
#define mgFogEnable()		glEnable(GL_FOG)
#define mgFogDisable()		glDisable(GL_FOG)



//clears zbuffer only
#define mgClearZbuffer()		glClear( GL_DEPTH_BUFFER_BIT )



//clears screen and zbuffer
#define mgClearBuffers()		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT )



//swaps back et front buffer
void mgFlip()
{
	glFinish();
	SwapBuffers( hDC );
}



//copies back buffer to front buffer
void mgCopyBackToFrontBuffer()
{ glCopyPixels( 0, 0, cursize.right-1, cursize.bottom-1, GL_COLOR ); }





//---------------------------------------------------------------------------------------------
// FUNCTIONS TO USE SYSTEM FONTS
//---------------------------------------------------------------------------------------------
GLuint	mgFontTex[96];
GLuint mgFontList;

void mgFontInit(HDC hdc, char* fontname, int height, int weight, GLuint* base, GLuint* texture)
{
	int i;	//index des boucles

	HFONT font = CreateFont(
							-height,
							0,
							0,
							0,
							weight,
							FALSE,
							FALSE,
							FALSE,
							ANSI_CHARSET,
							OUT_TT_PRECIS,
							CLIP_DEFAULT_PRECIS,
							ANTIALIASED_QUALITY,
							FF_DONTCARE|DEFAULT_PITCH,
							fontname);				//on crée un objet GDI

	SelectObject(hdc, font);						//on e séléctionne comme un crayon

	
	(*base) = glGenLists(96);						//on génère 96 macros de rendu
	glGenTextures(96, texture);						//pour 96 textures

	GLYPHMETRICS glyphmetrics;						//unités renvoyées par GetGlyphOutline
	DWORD glyphsize;								//taille d'un graph renvoyé par GetGlyphOutline
	const GLint glyphb2w = 32;						//largeur d'une texture
	const GLint glyphb2h = 32;						//hauteur d'une texture
	int maxheight;									//hauteur maxi d'un caractère (< height)
	GLfloat boxX = 0.5f;							//pour l'aspect ratio
	GLfloat boxY = 1.0f;							//ici on initialise pour l'espace
	GLfloat cellX = 0.5f;							//la largeur du caractère + espacement avec le caractère suivant
	GLfloat origX = 0.0f;							//la largeur dont il faut se décaler pour avoir les lettres bien centrées
	GLfloat origY = 0.0f;							//la hauteur dont il faut se décaler pour avoir les lettres à la bonne hauteur
	void *glyphbuf1 = malloc(height * height);		//on ne connait même pas width, mais comme width<height...on majore!
	void *glyphbuf2 = malloc(glyphb2w * glyphb2h);	//pour la texture carrée à binder
	const MAT2 glyphmat2 = {{0, 1}, {0, 0}, {0, 0}, {0, 1}};	//matrice de transformation des glyphs (ici identité)

	glPixelTransferf(GL_INDEX_SHIFT, 0.0f);
	glPixelTransferf(GL_INDEX_OFFSET, 0.0f);
	glPixelStoref(GL_UNPACK_LSB_FIRST, 0.0f);

	GLfloat map[256];								//les glyphs arrivent avec une couleur entre 0 et 65 et nous on a besoin entre 0 et 255 -> conversion
	for(i=0; i<=64; i++)							//on n'a besoin que des 65 premiers, le reste on le laisse indéterminé
		map[i] = (float)i/64.0f;

	glPixelMapfv(GL_PIXEL_MAP_I_TO_A, 256, map);	//mapping des indexes sur l'alpha (on fait une fonte transparente)


	//on trouve la hauteur maxi d'un char (on a choisi ! pour ça)
	GetGlyphOutline(hdc, 33 /* ! */, GGO_GRAY8_BITMAP, &glyphmetrics, 0, NULL, &glyphmat2);
	maxheight = glyphmetrics.gmBlackBoxY;

	//d'abord l'espace
	ZeroMemory(glyphbuf2, glyphb2w*glyphb2h);
	//ratiox = 0.5f;	//deja mis ces valeurs à l'init
	//ratioy = 1.0f;
	//...et les autres aussi

	for(i=0; i<96; i++)
	{
		//paramètres de la texture
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		//fabrication de la texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, glyphb2w, glyphb2h, 0, GL_COLOR_INDEX, GL_UNSIGNED_BYTE, glyphbuf2);
		
		//compilation de l'affichage de la texture
		glNewList(*base+i, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glTranslatef(origX, origY, 0.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f, boxY, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( boxX, boxY, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( boxX, 0.0f, 0.0f);
		glEnd();
		glTranslatef(-origX+cellX, -origY, 0.0f);
		glEndList();

		//génération du caractère suivant (+33 au lieu de +32). on met la charue avant les boeufs mais c'est pour optimiser
		glyphsize = GetGlyphOutline(hdc, (unsigned char)(i+33), GGO_GRAY8_BITMAP, &glyphmetrics, 0, NULL, &glyphmat2);
		GetGlyphOutline(hdc, (unsigned char)(i+33), GGO_GRAY8_BITMAP, &glyphmetrics, glyphsize, glyphbuf1, &glyphmat2);
		gluScaleImage(GL_COLOR_INDEX, glyphmetrics.gmBlackBoxX, glyphmetrics.gmBlackBoxY, GL_UNSIGNED_BYTE, glyphbuf1, glyphb2w, glyphb2h, GL_UNSIGNED_BYTE, glyphbuf2);

		//calcul des décalages et ratios suivants (il le fera pour i=96 aussi mais c'est pas grave)
		boxX = (GLfloat)glyphmetrics.gmBlackBoxX/maxheight;
		boxY = (GLfloat)glyphmetrics.gmBlackBoxY/maxheight;
		cellX = (GLfloat)glyphmetrics.gmCellIncX/maxheight;
		origX = (GLfloat)glyphmetrics.gmptGlyphOrigin.x/maxheight;
		origY = 1.0f - (GLfloat)glyphmetrics.gmptGlyphOrigin.y/maxheight;
	}

	RELEASE(glyphbuf1);
	RELEASE(glyphbuf2);
	DeleteObject(font);
}



void mgFontKill(GLuint base)
{
	glDeleteLists(base, 96);
	glDeleteTextures(96, mgFontTex);
}


//---------------------------------------------------------------------------------------------
//displays a 2D text in raster mode, top-left corner is (0,0)
//it's possible to print ~20 chars / line with scale 1 in 640*480
//-------------------------------------------------------------------
//need to be in raster mode, and especially...
//glEnable(GL_TEXTURE_2D);
//glEnable(GL_BLEND);
//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//---------------------------------------------------------------------------------------------
void Text2R(GLint x, GLint y, GLfloat scale, GLuint base, char* format, ...)
{
	va_list args;
	char buffer[255];
	
	va_start(args, format);
	wvsprintf(buffer, format, args);
	va_end(args);

	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//glLoadIdentity();
	//gluOrtho2D(0, cursize.right, cursize.bottom, 0);	//if the viewport changes as the window is resized, then this line is useless

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

    //glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT);
    //glEnable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);
	//glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTranslatef((GLfloat)x, (GLfloat)y, 0.0f);
    glScalef(32.0f*scale, 32.0f*scale, 32.0f*scale);

	glListBase(base - 32);
	glListBase(mgFontList - 32);
	glCallLists(lstrlen(buffer), GL_UNSIGNED_BYTE, buffer);

	//glPopAttrib();

	glPopMatrix();
	//glMatrixMode(GL_PROJECTION);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);
}




//---------------------------------------------------------------------------------------------
//displays a 2D text with coords between 0 et 1, top-left corner is (0,0)
//it's possible to print ~20 chars / line with scale 1
//---------------------------------------------------------------------------------------------
void Text2D(GLfloat x, GLfloat y, GLfloat scale, GLuint base, char* format, ...)
{
	va_list args;
	char buffer[255];
	
	va_start(args, format);
	wvsprintf(buffer, format, args);
	va_end(args);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1, 1, 0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

    glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT);
    glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTranslatef(x, y, 0.0f);
    glScalef(scale/20.0f, scale/20.0f, scale/20.0f);

	glListBase(base - 32);
	glListBase(mgFontList - 32);
	glCallLists(lstrlen(buffer), GL_UNSIGNED_BYTE, buffer);

	glPopAttrib();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}



//---------------------------------------------------------------------------------------------
//displays a 3D text where we are positionned...like a quad
//---------------------------------------------------------------------------------------------
void Text3D(GLfloat scale, GLuint base, char* format, ...)
{
	va_list args;
	char buffer[255];
	
	va_start(args, format);
	wvsprintf(buffer, format, args);
	va_end(args);

    glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT);
    glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glScalef(scale, -scale, scale);

	glListBase(base - 32);
	glCallLists(lstrlen(buffer), GL_UNSIGNED_BYTE, buffer);

	glPopAttrib();
}


//---------------------------------------------------------------------------------------------
//raster line independant from the window size
//-------------------------------------------------------------------
//need to be in raster mode, and especially...
//glEnable(GL_BLEND);
//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//---------------------------------------------------------------------------------------------
void Line2R(GLint x1, GLint y1, GLint x2, GLint y2)
{
	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//glLoadIdentity();
	//gluOrtho2D(0, cursize.right, cursize.bottom, 0);

	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glLoadIdentity();

    glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_LINES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();

	glPopAttrib();

	//glPopMatrix();
	//glMatrixMode(GL_PROJECTION);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);
}




//---------------------------------------------------------------------------------------------
//proportional line with coords between 0.0 and 1.0
//---------------------------------------------------------------------------------------------
void Line2D(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1, 1, 0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

    glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();

	glPopAttrib();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


//END
#endif