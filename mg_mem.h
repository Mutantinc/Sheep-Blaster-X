//-------------------------------------------------------------------
//MG_MEM.H - MEMORY, RESOURCES, FILES MANAGEMENT
//-------------------------------------------------------------------
#ifndef _MG_MEM_H_
#define _MG_MEM_H_



//-----------------------------------------------------------------------------------------
// DEFINITIONS & STRUCT - BEWARE: WORKS FOR MODs TOO
//-----------------------------------------------------------------------------------------
//DO NOT USE VALUE 0
#define		MG_DATA_FILE			1	//file
#define		MG_DATA_MEMORY			2	//memory
#define		MG_DATA_VISUAL			3	//resource
#define		MG_DATA_ASM				4	//asm data
#define		MG_DATA_ZLIB_FILE		5	//blob in file
#define		MG_DATA_ZLIB_MEMORY		6	//blob in memory

#define		RELEASE(x)	if( x!=0 ) { free(x); x = NULL; }


typedef struct
{
	uint length;		//length of the memory file
	uint pos;		//offset from begining of memory file
	void *data;		//datas
} MEMFILE;	



uint (*mgOpen)( char* name );
void (*mgClose)( uint handle);
int (*mgRead)( void* buffer, int size, uint handle );
void (*mgSeek)( uint handle, int pos, uint mode );
int (*mgTell)( uint handle );
uint8 (*mgGetc)( uint handle );
uint	mgDataAccessMode;




//-----------------------------------------------------------------------------------------
//FUNCTIONS ON FILES
//-----------------------------------------------------------------------------------------
#ifndef MG_USE_SMALLEST_CODE	//we don't want to have some LIBCT.LIB linkage for files i/o when requesting small code

uint mgFileOpen(char *name)
{
	return (uint)fopen(name, "rb");
}

void mgFileClose(uint handle)
{
	fclose((FILE *)handle);
}

int mgFileRead(void *buffer, int size, uint handle)
{
	return fread(buffer, 1, size, (FILE *)handle);
}

void mgFileSeek(uint handle, int pos, uint mode)
{
	fseek((FILE *)handle, pos, mode);
}

int mgFileTell(uint handle)
{
	return ftell((FILE *)handle);
}


uint8 mgFileGetc( uint handle)
{
	return (uint8)fgetc((FILE*)handle);
}

#endif



//-----------------------------------------------------------------------------------------
//FUNCTIONS ON MEMORY FILES : MEMOPEN PUTS A FILE IN MEMORY
//-----------------------------------------------------------------------------------------
#ifndef MG_USE_SMALLEST_CODE

uint mgMemOpen( char* name )
{
	MEMFILE *memfile;
	memfile = (MEMFILE *)malloc(sizeof(MEMFILE));

	FILE *fp;
	if( (fp = fopen(name, "rb")) == NULL)
		return 0;
	
	fseek(fp, 0, SEEK_END);
	memfile->length = ftell(fp);
	memfile->data = malloc(memfile->length);
	memfile->pos = 0;
	
	fseek(fp, 0, SEEK_SET);
	fread(memfile->data, 1, memfile->length, fp);
	fclose(fp);
	
	return (uint)memfile;
}
#endif


void mgMemClose(uint handle)
{
	MEMFILE* mf = (MEMFILE*)handle;
	RELEASE(mf);
}



int mgMemRead( void *buffer, int size, uint handle )
{
	MEMFILE *memfile = (MEMFILE *)handle;

	if (memfile->pos + size >= memfile->length)
		size = memfile->length - memfile->pos;
	
	memcpy(buffer, (char *)memfile->data + memfile->pos, size);
	memfile->pos += size;
	
	return size;
}



void mgMemSeek( uint handle, int off, uint mode )
{
	MEMFILE* memfile = (MEMFILE*)handle;

	if (mode == SEEK_SET) 
		memfile->pos = off;
	else if (mode == SEEK_CUR) 
		memfile->pos += off;
	else if (mode == SEEK_END)
		memfile->pos = memfile->length + off;
	
	if ( memfile->pos > memfile->length )
		memfile->pos = memfile->length;
}



int mgMemTell( uint handle )
{
	return ((MEMFILE*)handle)->pos;
}



uint8 mgMemGetc( uint handle )	//sans vérification
{
	MEMFILE* memfile = (MEMFILE*)handle;

	//	if (memfile->pos + 1 >= memfile->length)
	//		return NULL;
	
	uint8 buffer;
	buffer = *((uint8 *)memfile->data+memfile->pos);
	memfile->pos += 1;
	return buffer;
}




//-----------------------------------------------------------------------------------------
//FUNCTIONS ON ASM INCLUDED FILES
//-----------------------------------------------------------------------------------------
uint mgAsmOpen( char *name )
{
	MEMFILE		*memfile;
	memfile = (MEMFILE *)malloc(sizeof(MEMFILE));

	memfile->data = name + sizeof(uint32);	//+4 bytes (for the length of the following section)
	memfile->length = *(uint32*)name;
	memfile->pos = 0;

	return (uint)memfile;
}



//-----------------------------------------------------------------------------------------
//FUNCTIONS ON VISUAL STUDIO RESOURCES
//THE INCLUDED DATA MUST BE OF TYPE "RCDATA"
//"name" is the ID (the #define) of the resource
//-----------------------------------------------------------------------------------------
#ifndef MG_USE_SMALLEST_CODE

uint mgRcOpen( char *name )
{
	MEMFILE		*memfile;
	memfile = (MEMFILE *)malloc(sizeof(MEMFILE));
	HRSRC rec;

	if((rec = FindResource(NULL, MAKEINTRESOURCE(name), RT_RCDATA))==NULL)
		return NULL;
	
	HGLOBAL handle = LoadResource(NULL, rec);
	
	memfile->data = LockResource(handle);
	memfile->length = SizeofResource(NULL, rec);
	memfile->pos = 0;
	
	return (uint)memfile;
}


void mgRcClose( uint )
{/*la mémoire est en LockResource, on ne fait rien*/}

#endif



//-----------------------------------------------------------------------------------------
//FUNCTIONS ON ZLIB FILES
//-----------------------------------------------------------------------------------------
#ifndef MG_USE_SMALLEST_CODE

//*****TO TEST OR MODIFY TO SUPPORT ZLIB*****
int mgBlobFileRead( void* buffer, int nb, uint handle )	//buffer doit être un MEMFILE* à remplir, nb le n° du fichier dans le blob, handle un FILE* qc
{
	FILE *fileblob = (FILE*)handle;
	MEMFILE* memfile = (MEMFILE*)buffer;

	int first;
	int len1, len2;

	fread( &first, sizeof(int), 1, fileblob);
	first++;

	
	if (!nb)
	{
		fread( &len1, sizeof(int), 1, fileblob);
		fseek( fileblob, first, SEEK_SET );
	}
	else
	{
		fseek( fileblob, sizeof(int)*nb, SEEK_SET );
		fread( &len2, sizeof(int), 1, fileblob);
		fread( &len1, sizeof(int), 1, fileblob);
		len1 = len1 - len2;

		fseek( fileblob, first+len2, SEEK_SET );
	}

	memfile->data = (uint8*)malloc(len1);
	fread( memfile->data, 1, len1, fileblob);
	memfile->length = len1;
	memfile->pos = 0;

	return len1;	//évidemment, ne correspond pas du tout à nb...
}

#endif



//-----------------------------------------------------------------------------------------
//FUNCTIONS ON ZLIB FILES IN MEMORY
//-----------------------------------------------------------------------------------------
#ifndef MG_USE_SMALLEST_CODE

int mgBlobMemRead( void* buffer, int nb, uint handle )		//buffer doit être un MEMFILE* à remplir, nb le n° du fichier dans le blob, handle un MEMFILE* qc
{
	MEMFILE* memblob = (MEMFILE*)handle;
	MEMFILE* memfile = (MEMFILE*)buffer;

	int* data= (int*)memblob->data;
	int* first = data + data[0] + 1;	//on ne multiplie pas par 4 car on a un pointeur de "int", pas de "char"
	
	if (!nb)
	{
		memfile->data = first;
		memfile->length = data[1];
	}
	else
	{
		memfile->data = (uint8*)first + data[nb];
		memfile->length = data[nb+1] - data[nb];
	}

	memfile->pos = 0;

	return memfile->length;
}

#endif



//-----------------------------------------------------------------------------------------
// CALLBACK FUNCTIONS ATTRIBUTION TO BE ABLE TO SEEK DATA ANYWHERE
//-----------------------------------------------------------------------------------------
void mgSetDataAccessMode( uint mode )
{

	if( !mode )
	{
#ifdef MG_DATA_DEFAULT_ASM
		mode = MG_DATA_ASM;
#elif defined( MG_DATA_DEFAULT_FILE )
		mode = MG_DATA_FILE;
#endif
	}

	mgDataAccessMode = mode;

	switch(mode)
	{

	case MG_DATA_ASM:
		mgOpen = mgAsmOpen;
		mgClose = mgMemClose;
		mgRead = mgMemRead;
		mgSeek = mgMemSeek;
		mgTell = mgMemTell;
		mgGetc = mgMemGetc;
		break;

#ifndef MG_USE_SMALLEST_CODE
	case MG_DATA_FILE:
		mgOpen = mgFileOpen;
		mgClose = mgFileClose;
		mgRead = mgFileRead;
		mgSeek = mgFileSeek;
		mgTell = mgFileTell;
		mgGetc = mgFileGetc;
		break;

	case MG_DATA_MEMORY:
		mgOpen = mgMemOpen;
		mgClose = mgMemClose;
		mgRead = mgMemRead;
		mgSeek = mgMemSeek;
		mgTell = mgMemTell;
		mgGetc = mgMemGetc;
		break;

	case MG_DATA_VISUAL:
		mgOpen = mgRcOpen;
		mgClose = mgRcClose;
		mgRead = mgMemRead;;
		mgSeek = mgMemSeek;
		mgTell = mgMemTell;
		mgGetc = mgMemGetc;
		break;

	case MG_DATA_ZLIB_FILE:
		mgOpen = mgFileOpen;
		mgClose = mgFileClose;
		mgRead = mgBlobFileRead;
		mgSeek = mgFileSeek;
		mgTell = mgFileTell;
		mgGetc = mgFileGetc;
		break;

	case MG_DATA_ZLIB_MEMORY:
		mgOpen = mgMemOpen;
		mgClose = mgMemClose;
		mgRead = mgBlobMemRead;
		mgSeek = mgMemSeek;
		mgTell = mgMemTell;
		mgGetc = mgMemGetc;
		break;
#endif
	}
	
}


//-----------------------------------------------------------------------------------------
// FUNCTIONS TO IMPORT/EXPORT OF FILES
//-----------------------------------------------------------------------------------------

uint mgRead8( uint p )
{
	return (uint)mgGetc(p);
}


uint mgRead16( uint p )
{
	uint16 s;
	mgRead(&s, sizeof(uint16), p);
	return (uint)s;
}


uint mgRead32( uint p )
{
	uint32 i;
	mgRead(&i, sizeof(uint), p);
	return (uint)i;
}


float mgReadFlt( uint p )
{
	float f;
	mgRead(&f, sizeof(float), p);
	return f;
}


//-----------------------------------------------------------------------------------------

//FIN
#endif