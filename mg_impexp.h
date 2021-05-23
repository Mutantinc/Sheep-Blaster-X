//-------------------------------------------------------------------
//MG_IMPEXP.H
//-------------------------------------------------------------------
#ifndef _MG_IMPEXP_H_
#define _MG_IMPEXP_H_

//-------------------------------------------------------------------

//size of ids					16
//size of "numbers of"			16
//size of string lengths		16

//-------------------------------------------------------------------

#define MG_IMPEXP_OBJECT		110

#define MG_IMPEXP_OBJPCD_BOX	120	//x1,y1,z1,x2,y2,z2
#define MG_IMPEXP_OBJPCD_SPHERE	121	//x,y,z,r
#define MG_IMPEXP_OBJPCD_CYL	122	//x,y,z,r,h
#define MG_IMPEXP_OBJPCD_TORUS	123	//x,y,z,R,r
#define MG_IMPEXP_OBJPCD_CONE	124	//x,y,z,R,r
#define MG_IMPEXP_OBJPCD_TUBE	125	//x,y,z,r
#define MG_IMPEXP_OBJPCD_PYRAM	126	//x1,y1,z1,x2,y2,z2,h
#define MG_IMPEXP_OBJPCD_PLANE	127	//x1,y1,z1,x2,y2,z2
#define MG_IMPEXP_OBJECT_INTER	128	//{x,y,z}, h, to interpolate
#define MG_IMPEXP_OBJECT_LATHE	129	//{x,y,z},xa,ya,za (axis for lathe)


#define MG_IMPEXP_BONE			200


#define MG_IMPEXP_MATERIAL		310
#define MG_IMPEXP_TEXTURE		320 //bitmap texture + id
#define MG_IMPEXP_TEXPCD		330	//procedural texture + id


#define MG_IMPEXP_LIGHT			400


#define MG_IMPEXP_CAMERA		510
#define MG_IMPEXP_TARGET		520


#define MG_IMPEXP_STRING		700


#define MG_IMPEXP_LINPOSTRK		810
#define MG_IMPEXP_LINROTTRK		820
#define MG_IMPEXP_LINSCATRK		830

#define MG_IMPEXP_BEZPOSTRK		840
#define MG_IMPEXP_BEZROTTRK		850
#define MG_IMPEXP_BEZSCATRK		860

#define MG_IMPEXP_TCBPOSTRK		870
#define MG_IMPEXP_TCBROTTRK		880
#define MG_IMPEXP_TCBSCATRK		890


#define MG_IMPEXP_ROLLTRK		910
#define MG_IMPEXP_FOVTRK		920
#define MG_IMPEXP_MESHTRK		930
#define MG_IMPEXP_IKTRK			940


//END
#endif

