//-------------------------------------------------------------------
//MG_VECTOR.H - ABOUT THE VECTORS
//-------------------------------------------------------------------
#ifndef _MG_VECTOR_H_
#define _MG_VECTOR_H_



class mgMatrix;			//because crossed calls



class mgVector
{
	
public:	
	union
	{
		struct {float x,y,z,w;};
		struct {float r,g,b,a;};
		float longvec[4];
	};	

	
//-------------------------------------------------------------------

	inline mgVector() {}
	
	inline mgVector(const float x, const float y, const float z)
	{
		this->x=x; 
		this->y=y; 
		this->z=z;
	}
	
	inline mgVector(uint c)
	{
		r = (float)((c&0xff)*(1.0f/255.0f));
		g = (float)(((c>>8)&0xff)*(1.0f/255.0f));
		b = (float)(((c>>16)&0xff)*(1.0f/255.0f));
	}

	inline mgVector(uint8 r8, uint8 g8, uint8 b8 )
	{
		r = (float)(r8*(1.0f/255.0f));
		g = (float)(g8*(1.0f/255.0f));
		b = (float)(b8*(1.0f/255.0f));
	}

//-------------------------------------------------------------------

	void ReadC(uint p)
	{
		*this = mgVector( mgGetc(p), mgGetc(p), mgGetc(p));
	}

	void ReadV(uint p)
	{
		*this = mgVector( mgReadFlt(p), mgReadFlt(p), mgReadFlt(p));
	}

	float* Fltpt(void)
	{
		return (float*)this;
	}
//-------------------------------------------------------------------

	inline float& operator[](const uint i)
	{
		if(i==0) return x;
		if(i==1) return y;
		return z;
	}
	
	inline void Set(float x, float y, float z)
	{
		this->x = x; 
		this->y = y;
		this->z = z;
	}
	
	inline mgVector Abs ()
	{
		mgVector v;
		if (x<0) v.x = -x; else v.x = x;
		if (y<0) v.y = -y; else v.y = y;
		if (z<0) v.z = -z; else v.z = z;
		return v;
	}
	
	inline uint AsRGB32()
	{
		return ((int)(b*255.f)<<16)|((int)(g*255.f)<<8)|(int)(r*255.f);
	}
	
	inline void Null()
	{
		x=y=z=0.f;
	}
	
	inline void Negate()
	{
		x=-x;
		y=-y;
		z=-z;
	}
	
	inline mgVector Negated()
	{
		return mgVector(-x, -y, -z);
	}
	
	inline void Square()
	{
		x*=x;
		y*=y;
		z*=z;
	}
	
	inline mgVector Squared()
	{
		return mgVector(x*x, y*y, z*z);
	}
	
	inline void Invert()
	{
		x=1.f/x;
		y=1.f/y;
		z=1.f/z;
	}
	
	inline mgVector Inverted()
	{
		return mgVector(1.f/x, 1.f/y, 1.f/z);
	}
	
	inline void clampf () {
		if (r>1) r = 1;
		if (g>1) g = 1;
		if (b>1) b = 1;
	}
	
	inline void Scale(mgVector s)
	{
		x*=s.x;
		y*=s.y;
		z*=s.z;
	}
	
	inline mgVector Scaled(mgVector s)
	{
		return mgVector(x*=s.x, y*=s.y, z*=s.z);
	}
	
	inline float Magnitude()
	{
		return sqrt(x*x+y*y+z*z);
	}
	
	inline float MagnitudeSquare() {
		return (float)(x*x+y*y+z*z);
	}
	
	inline void Normalise()
	{
		float invlen = 1.f/Magnitude();
		x*=invlen;
		y*=invlen;
		z*=invlen;	
	}
	
	inline void Normalise_Aligned()
	{						
		Normalise ();
	}
	
	inline void AddCompMul(mgVector a, mgVector b)
	{		
		x+=a.x*b.x;
		y+=a.y*b.y;
		z+=a.z*b.z;		
	}
	
	inline mgVector Reflect(mgVector n) 
	{ 
		mgVector r;
		float d=-2.0f*(n.x*x+n.y*y+n.z*z);
		r.x=(d * n.x)+x;
		r.y=(d * n.y)+y;
		r.z=(d * n.z)+z;
		return r;
	}
	
//-------------------------------------------------------------------

	inline bool operator==(const mgVector &a)
	{		
		return( x==a.x && y==a.y && z==a.z);
	}

	inline mgVector &operator+=(const mgVector &a)
	{		
		x+=a.x;
		y+=a.y;
		z+=a.z;
		return *this;
	}
	
	inline mgVector &operator-=(const mgVector &a)
	{
		x-=a.x;
		y-=a.y;
		z-=a.z;
		return *this;
	}
	
	inline mgVector &operator*=(const float f)
	{
		x*=f;
		y*=f;
		z*=f;
		return *this;
	}
	
	inline mgVector &operator*=(const mgMatrix m);
	inline mgVector &operator/=(const mgMatrix m);
	
};


//-------------------------------------------------------------------


inline mgVector operator-(const mgVector &a)
{
	return mgVector( -a.x, -a.y, -a.z);
}

inline mgVector operator+(const mgVector &a, const mgVector &b)
{
	return mgVector(a.x+b.x,a.y+b.y,a.z+b.z);
}

inline mgVector operator-(const mgVector &a, const mgVector &b)
{
	return mgVector(a.x-b.x,a.y-b.y,a.z-b.z);
}

inline mgVector operator*(const mgVector &a, const float b)
{
	return mgVector(a.x*b, a.y*b, a.z*b);
}

inline mgVector operator*(float a, const mgVector &b)
{
	return mgVector(a*b.x, a*b.y, a*b.z);
}

inline mgVector operator/(const mgVector &a, const float b)
{
	float ib=1.f/b;
	return mgVector(a.x*ib, a.y*ib, a.z*ib);
}

inline float operator*(const mgVector &a, const mgVector &b)	//dot product
{
	return a.x*b.x+a.y*b.y+a.z*b.z;
}

inline mgVector operator%(const mgVector &a, const mgVector &b)	//vect product
{
	return mgVector( a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x );
}

inline float operator|(mgVector &a, mgVector &b)
{
	float l=(a.x*a.x+a.y*a.y+a.z*a.z)*(b.x*b.x+b.y*b.y+b.z*b.z);
	if (l)
		l=1.f/(float)sqrt(l);
	return (a.x*b.x+a.y*b.y+a.z*b.z)*l;
}

//-------------------------------------------------------------------

typedef		mgVector		mgColor;

//-------------------------------------------------------------------

//END
#endif

