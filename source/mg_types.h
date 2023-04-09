//-------------------------------------------------------------------
//MG_TYPES.H - TOUS LES TYPES DE BASE
//-------------------------------------------------------------------
#ifndef _MG_TYPES_H_
#define _MG_TYPES_H_


//#include <exception>

//#include <string>					//gestion des chaines de caractères
//#include <vector>					//gestion des listes d'objets
//using namespace std;				//on se passe d'écrire std:: partout

#pragma warning (disable : 4244)	// possible loss of data...
#pragma warning (disable : 4503)	// STL: Decorated name length exceeded, name was truncated
#pragma warning (disable : 4786)	// STL: too long indentifier
#pragma warning (disable : 4290)	// throw() not implemented warning


//Check the STLPort presence
//#ifndef __SGI_STL_PORT
//#  error "You need STLPort to compile this project (www.stlport.org)"
//#endif // __SGI_STL_PORT


//Standard types
//correct numeric types:       sint8, uint8, sint16, uint16, sint32, uint32, sint64, uint64, sint, uint
//correct char types:          char, string, ucchar, ucstring
//correct misc types:          void, bool, float, double

//Integral Types
typedef signed		__int8		sint8;
typedef unsigned	__int8		uint8;
typedef signed		__int16		sint16;
typedef unsigned	__int16		uint16;
typedef signed		__int32		sint32;
typedef unsigned	__int32		uint32;
typedef signed		__int64		sint64;
typedef unsigned	__int64		uint64;

typedef signed		int			sint;                   // at least 32bits (depend of processor)
typedef unsigned	int			uint;                   // at least 32bits (depend of processor)

typedef				uint16		ucchar;


//END
#endif
