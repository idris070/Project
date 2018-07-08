#ifndef HM_AABB_H
#define HM_AABB_H

#include "func.h"



template <class Tvec> class Taabb
{
public:

	Tvec min,max;

	typedef Tvec TYPE;//for external use only

	INLINE Taabb() {}
	INLINE Taabb(const Taabb &b):min(b.min),max(b.max) {}
	INLINE explicit Taabb(const typename Tvec::TYPE a)
	{
		if (a)
		{
			min=Tvec(-a);
			max=Tvec( a);
		}
		else
		{
#if _MSC_VER==1200
#define typename
#endif
			min=Tvec( limits<typename Tvec::TYPE>::max());
			max=Tvec(-limits<typename Tvec::TYPE>::max());
#ifdef typename
#undef typename
#endif
		}
	}
	INLINE Taabb(const typename Tvec::TYPE min,const typename Tvec::TYPE max):min(min),max(max) {}
	INLINE Taabb(const Tvec &min,const Tvec &max):min(min),max(max) {}

	INLINE Taabb(const HMuint count,const Tvec *vertices)
	{
		if (count)
		{
			min=max=vertices[0];

			for (HMuint i=1;i<count;i++)
				(*this)+=vertices[i];
		}
		else
			*this=Taabb(0);
	}

	INLINE HMbool operator==(const Taabb &b) const {return min==b.min && max==b.max;}
	INLINE HMbool operator!=(const Taabb &b) const {return min!=b.min || max!=b.max;}

	INLINE const Taabb operator+(const Tvec &v) const
	{return Taabb(::min(min,v),::max(max,v));}

	INLINE const Taabb operator+(const Taabb &b) const
	{return Taabb(::min(min,b.min),::max(max,b.max));}

	INLINE const Taabb operator*(const Taabb &b) const
	{
		if (intersect(*this,b))
			return Taabb(::max(min,b.min),::min(max,b.max));
		else
			return Taabb(0);
	}

	INLINE const Taabb &operator+=(const Tvec &v)
	{
		for (int i=0;i<dimension(min);i++)
		{
			if (v[i]<min[i]) min[i]=v[i];
			if (v[i]>max[i]) max[i]=v[i];
		}
		return *this;
	}

	INLINE const Taabb &operator+=(const Taabb &b)
	{
		for (int i=0;i<dimension(min);i++)
		{
			if (b.min[i]<min[i]) min[i]=b.min[i];
			if (b.max[i]>max[i]) max[i]=b.max[i];
		}
		return *this;
	}

	INLINE const Taabb &operator*=(const Taabb &b)
	{
		if (intersect(*this,b))
			for (int i=0;i<dimension(min);i++)
			{
				if (b.min[i]>min[i]) min[i]=b.min[i];
				if (b.max[i]<max[i]) max[i]=b.max[i];
			}
		else
			*this=Taabb(0);

		return *this;
	}

	INLINE const Taabb &operator=(const Taabb &b) {min=b.min; max=b.max; return *this;}
};

template <class T>
INLINE const Taabb<T> operator+(const T &v,const Taabb<T> &b)
{return Taabb<T>(::min(b.min,v),::max(b.max,v));}

template <class T> INLINE HMbool intersect(const Taabb<T> &b0,const Taabb<T> &b1)
{
	for (int i=0;i<dimension(b0.min);i++)
		if (!(b0.min[i]<b1.max[i] && b0.max[i]>b1.min[i])) return HMfalse;

	return HMtrue;
}


typedef Taabb<vec3>		aabb;
typedef Taabb<dvec3>	daabb;
typedef Taabb<ldvec3>	ldaabb;
typedef Taabb<ivec3>	iaabb;
typedef Taabb<svec3>	saabb;

typedef Taabb<vec2>		rect;
typedef Taabb<dvec2>	drect;
typedef Taabb<ldvec2>	ldrect;
typedef Taabb<ivec2>	irect;
typedef Taabb<svec2>	srect;

#endif //HM_AABB_H
