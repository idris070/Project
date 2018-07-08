#ifndef HM_VEC_H
#define HM_VEC_H

#include <math.h>
#include "typedef.h"



#define floatCompareFuncs(T,epsilon)	\
										\
INLINE HMbool isZero(T x)				\
{return fabs(x)<epsilon;}				\
										\
INLINE HMbool notZero(T x)				\
{return fabs(x)>=epsilon;}				\
										\
INLINE HMbool equal(T x,T y)			\
{return isZero(x-y);}					\
										\
INLINE HMbool notEqual(T x,T y)			\
{return notZero(x-y);}					\
										\
INLINE HMbool lessThan(T x,T y)			\
{return x<=y-epsilon;}					\
										\
INLINE HMbool lessThanEqual(T x,T y)	\
{return x<y+epsilon;}					\
										\
INLINE HMbool greaterThan(T x,T y)		\
{return x>=y+epsilon;}					\
										\
INLINE HMbool greaterThanEqual(T x,T y)	\
{return x>y-epsilon;}

floatCompareFuncs(HMfloat,1e-4)
floatCompareFuncs(HMdouble,1e-8)
floatCompareFuncs(HMlong_double,1e-8)

#undef floatCompareFuncs


#define intCompareFuncs(T)				\
										\
INLINE HMbool isZero(T x)				\
{return x==0;}							\
										\
INLINE HMbool notZero(T x)				\
{return x!=0;}							\
										\
INLINE HMbool equal(T x,T y)			\
{return x==y;}							\
										\
INLINE HMbool notEqual(T x,T y)			\
{return x!=y;}							\
										\
INLINE HMbool lessThan(T x,T y)			\
{return x<y;}							\
										\
INLINE HMbool lessThanEqual(T x,T y)	\
{return x<=y;}							\
										\
INLINE HMbool greaterThan(T x,T y)		\
{return x>y;}							\
										\
INLINE HMbool greaterThanEqual(T x,T y)	\
{return x>=y;}

intCompareFuncs(HMbyte)
intCompareFuncs(HMubyte)
intCompareFuncs(HMint)
intCompareFuncs(HMuint)
intCompareFuncs(HMshort)
intCompareFuncs(HMushort)

#undef intCompareFuncs

INLINE HMbool    equal(HMbool x,HMbool y) {return x==y;}
INLINE HMbool notEqual(HMbool x,HMbool y) {return x!=y;}


class bvec3;
class bvec4;
template <class T> class Tvec3;
template <class T> class Tvec4;

////////////////////////////////////////////////////////////////////////////
//////////////////////// %   %  %%%%%   %%%    ###  ////////////////////////
//////////////////////// %   %  %      %   %  #   # ////////////////////////
//////////////////////// %   %  %%%    %        ##  ////////////////////////
////////////////////////  % %   %      %   %   #    ////////////////////////
////////////////////////   %    %%%%%   %%%   ##### ////////////////////////
////////////////////////////////////////////////////////////////////////////

class bvec2
{
public:

	union {HMbool x,r,s;};
	union {HMbool y,g,t;};

	typedef HMbool TYPE;//for external use only

	INLINE bvec2() {}
	INLINE bvec2(const bvec2 &v):x(v.x),y(v.y) {}
	INLINE explicit bvec2(const HMbool a):x(a),y(a) {}
	INLINE explicit bvec2(const bvec3 &v);
	INLINE explicit bvec2(const bvec4 &v);
	INLINE bvec2(const HMbool x,const HMbool y):x(x),y(y) {}

	INLINE HMbool &operator [] (const int i) {return (&x)[i];}
	INLINE const HMbool operator [] (const int i) const {return (&x)[i];}
	INLINE operator HMbool* () {return &x;}
	INLINE operator const HMbool* () const {return &x;}

	INLINE HMbool operator==(const bvec2 &v) const {return x==v.x && y==v.y;}
	INLINE HMbool operator!=(const bvec2 &v) const {return x!=v.x || y!=v.y;}

	INLINE const bvec2 &operator=(const bvec2 &v) {x=v.x; y=v.y; return *this;}
};


template <class T> class Tvec2
{
public:

	union {T x,r,s;};
	union {T y,g,t;};

	typedef T TYPE;//for external use only

	INLINE Tvec2() {}
	INLINE Tvec2(const Tvec2 &v):x(v.x),y(v.y) {}
	INLINE explicit Tvec2(const T a):x(a),y(a) {}
	INLINE explicit Tvec2(const Tvec3<T> &v);
	INLINE explicit Tvec2(const Tvec4<T> &v);
	INLINE Tvec2(const T x,const T y):x(x),y(y) {}

	INLINE T &operator [] (const int i) {return (&x)[i];}
	INLINE const T operator [] (const int i) const {return (&x)[i];}
	INLINE operator T* () {return &x;}
	INLINE operator const T* () const {return &x;}

	INLINE HMbool operator==(const Tvec2 &v) const {return equal(x,v.x) && equal(y,v.y);}
	INLINE HMbool operator!=(const Tvec2 &v) const {return notEqual(x,v.x) || notEqual(y,v.y);}

#define op(o) INLINE const Tvec2 operator o(const T a) const {return Tvec2(x o a, y o a);}
	op(+) op(-) op(*) op(/)
#undef op

#define op(o) INLINE const Tvec2 operator o(const Tvec2 &v) const {return Tvec2(x o v.x, y o v.y);}
	op(+) op(-) op(*) op(/)
#undef op

#define op(o) INLINE const Tvec2 &operator o(const T a) {x o a; y o a; return *this;}
	op(+=) op(-=) op(*=) op(/=)
#undef op

#define op(o) INLINE const Tvec2 &operator o(const Tvec2 &v) {x o v.x; y o v.y; return *this;}
	op(+=) op(-=) op(*=) op(/=) op(=)
#undef op
	/*
	Tvec2 &operator =(const Tvec2 &v) {x =v.x; y =v.y; return *this;}
	Tvec2 &operator+=(const Tvec2 &v) {x+=v.x; y+=v.y; return *this;}
	Tvec2 &operator-=(const Tvec2 &v) {x-=v.x; y-=v.y; return *this;}
	Tvec2 &operator*=(const Tvec2 &v) {x*=v.x; y*=v.y; return *this;}
	Tvec2 &operator/=(const Tvec2 &v) {x/=v.x; y/=v.y; return *this;}*/

	INLINE const Tvec2 operator-() const {return Tvec2(-x, -y);}
	INLINE const Tvec2 operator++() {return Tvec2(++x, ++y);}
	INLINE const Tvec2 operator--() {return Tvec2(--x, --y);}
	INLINE const Tvec2 operator++(int notused) {return Tvec2(x++, y++);}
	INLINE const Tvec2 operator--(int notused) {return Tvec2(x--, y--);}
};

#define op(o) template <class T>\
INLINE const Tvec2<T> operator o(const T a,const Tvec2<T> &v) {return Tvec2<T>(a o v.x, a o v.y);}
	op(+) op(-) op(*) op(/)
#undef op

////////////////////////////////////////////////////////////////////////////
//////////////////////// %   %  %%%%%   %%%   ####  ////////////////////////
//////////////////////// %   %  %      %   %      # ////////////////////////
//////////////////////// %   %  %%%    %        ##  ////////////////////////
////////////////////////  % %   %      %   %      # ////////////////////////
////////////////////////   %    %%%%%   %%%   ####  ////////////////////////
////////////////////////////////////////////////////////////////////////////

class bvec3
{
public:

	union {HMbool x,r,s;};
	union {HMbool y,g,t;};
	union {HMbool z,b,p;};

	typedef HMbool TYPE;//for external use only

	INLINE bvec3() {}
	INLINE bvec3(const bvec3 &v):x(v.x),y(v.y),z(v.z) {}
	INLINE explicit bvec3(const HMbool a):x(a),y(a),z(a) {}
	INLINE explicit bvec3(const bvec4 &v);
	INLINE bvec3(const bvec2 &xy,const HMbool z):x(xy.x),y(xy.y),z(z) {}
	INLINE bvec3(const HMbool x,const bvec2 &yz):x(x),y(yz.x),z(yz.y) {}
	INLINE bvec3(const HMbool x,const HMbool y,const HMbool z):x(x),y(y),z(z) {}

	INLINE HMbool &operator [] (const int i) {return (&x)[i];}
	INLINE const HMbool operator [] (const int i) const {return (&x)[i];}
	INLINE operator HMbool* () {return &x;}
	INLINE operator const HMbool* () const {return &x;}

#define swizzleCombo2(vec2,x,y)				\
	INLINE vec2 &x##y() {return *(vec2*)&x;}\
	INLINE const vec2 &x##y() const {return *(const vec2*)&x;}

#define swizzleCombos3(vec2,x,y,z)	\
	swizzleCombo2(vec2,x,y)			\
	swizzleCombo2(vec2,y,z)

#define componentsCombos3(vec2)	\
	swizzleCombos3(vec2,x,y,z)	\
	swizzleCombos3(vec2,r,g,b)	\
	swizzleCombos3(vec2,s,t,p)

	componentsCombos3(bvec2)

	INLINE HMbool operator==(const bvec3 &v) const {return x==v.x && y==v.y && z==v.z;}
	INLINE HMbool operator!=(const bvec3 &v) const {return x!=v.x || y!=v.y || z!=v.z;}

	INLINE const bvec3 &operator=(const bvec3 &v) {x=v.x; y=v.y; z=v.z; return *this;}
};

INLINE bvec2::bvec2(const bvec3 &v):x(v.x),y(v.y) {}


template <class T> class Tvec3
{
public:

	union {T x,r,s;};
	union {T y,g,t;};
	union {T z,b,p;};

	typedef T TYPE;//for external use only

	INLINE Tvec3() {}
	INLINE Tvec3(const Tvec3 &v):x(v.x),y(v.y),z(v.z) {}
	INLINE explicit Tvec3(const T a):x(a),y(a),z(a) {}
	INLINE explicit Tvec3(const Tvec4<T> &v);
	INLINE Tvec3(const Tvec2<T> &xy,const T z):x(xy.x),y(xy.y),z(z) {}
	INLINE Tvec3(const T x,const Tvec2<T> &yz):x(x),y(yz.x),z(yz.y) {}
	INLINE Tvec3(const T x,const T y,const T z):x(x),y(y),z(z) {}

	INLINE T &operator [] (const int i) {return (&x)[i];}
	INLINE const T operator [] (const int i) const {return (&x)[i];}
	INLINE operator T* () {return &x;}
	INLINE operator const T* () const {return &x;}

	componentsCombos3(Tvec2<T>)

#undef swizzleCombos3
#undef componentsCombos3

	INLINE HMbool operator==(const Tvec3 &v) const {return equal(x,v.x) && equal(y,v.y) && equal(z,v.z);}
	INLINE HMbool operator!=(const Tvec3 &v) const {return notEqual(x,v.x) || notEqual(y,v.y) || notEqual(z,v.z);}

#define op(o) INLINE const Tvec3 operator o(const T a) const {return Tvec3(x o a, y o a, z o a);}
	op(+) op(-) op(*) op(/)
#undef op

#define op(o) INLINE const Tvec3 operator o(const Tvec3 &v) const {return Tvec3(x o v.x, y o v.y, z o v.z);}
	op(+) op(-) op(*) op(/)
#undef op

#define op(o) INLINE const Tvec3 &operator o(const T a) {x o a; y o a; z o a; return *this;}
	op(+=) op(-=) op(*=) op(/=)
#undef op

#define op(o) INLINE const Tvec3 &operator o(const Tvec3 &v) {x o v.x; y o v.y; z o v.z; return *this;}
	op(+=) op(-=) op(*=) op(/=) op(=)
#undef op

	INLINE const Tvec3 operator-() const {return Tvec3(-x, -y, -z);}
	INLINE const Tvec3 operator++() {return Tvec3(++x, ++y, ++z);}
	INLINE const Tvec3 operator--() {return Tvec3(--x, --y, --z);}
	INLINE const Tvec3 operator++(int notused) {return Tvec3(x++, y++, z++);}
	INLINE const Tvec3 operator--(int notused) {return Tvec3(x--, y--, z--);}
};

#define op(o) template <class T>\
INLINE const Tvec3<T> operator o(const T a,const Tvec3<T> &v) {return Tvec3<T>(a o v.x, a o v.y, a o v.z);}
	op(+) op(-) op(*) op(/)
#undef op

template <class T>
INLINE Tvec2<T>::Tvec2(const Tvec3<T> &v):x(v.x),y(v.y) {}

////////////////////////////////////////////////////////////////////////////
//////////////////////// %   %  %%%%%   %%%      ## ////////////////////////
//////////////////////// %   %  %      %   %    # # ////////////////////////
//////////////////////// %   %  %%%    %       #  # ////////////////////////
////////////////////////  % %   %      %   %  ##### ////////////////////////
////////////////////////   %    %%%%%   %%%       # ////////////////////////
////////////////////////////////////////////////////////////////////////////

class bvec4
{
public:

	union {HMbool x,r,s;};
	union {HMbool y,g,t;};
	union {HMbool z,b,p;};
	union {HMbool w,a,q;};

	typedef HMbool TYPE;//for external use only

	INLINE bvec4() {}
	INLINE bvec4(const bvec4 &v):x(v.x),y(v.y),z(v.z),w(v.w) {}
	//1 arg
	INLINE explicit bvec4(const HMbool a):x(a),y(a),z(a),w(a) {}
	//2 arg
	INLINE bvec4(const bvec3 &xyz,const HMbool w):x(xyz.x),y(xyz.y),z(xyz.z),w(w)     {}
	INLINE bvec4(const HMbool x,const bvec3 &yzw):x(x)    ,y(yzw.x),z(yzw.y),w(yzw.z) {}
	INLINE bvec4(const bvec2 &xy,const bvec2 &zw):x(xy.x) ,y(xy.y) ,z(zw.x) ,w(zw.y)  {}
/*	INLINE bvec4(const bvec3 &xyz,const HMbool w):xyz(xyz),w(w) {}
	INLINE bvec4(const HMbool x,const bvec3 &yzw):x(x),yzw(yzw) {}
	INLINE bvec4(const bvec2 &xy,const bvec2 &zw):xy(xy),zw(zw) {}*/
	//3 arg
	INLINE bvec4(const bvec2 &xy,const HMbool z,const HMbool w):x(xy.x),y(xy.y),z(z)   ,w(w)    {}
	INLINE bvec4(const HMbool x,const bvec2 &yz,const HMbool w):x(x)   ,y(yz.x),z(yz.y),w(w)    {}
	INLINE bvec4(const HMbool x,const HMbool y,const bvec2 &zw):x(x)   ,y(y)   ,z(zw.x),w(zw.y) {}
/*	INLINE bvec4(const bvec2 &xy,const HMbool z,const HMbool w):xy(xy),z(z),w(w) {}
	INLINE bvec4(const HMbool x,const bvec2 &yz,const HMbool w):x(x),yz(yz),w(w) {}
	INLINE bvec4(const HMbool x,const HMbool y,const bvec2 &zw):x(x),y(y),zw(zw) {}*/
	//4 arg
	INLINE bvec4(const HMbool x,const HMbool y,const HMbool z,const HMbool w):x(x),y(y),z(z),w(w) {}

	INLINE HMbool &operator [] (const int i) {return (&x)[i];}
	INLINE const HMbool operator [] (const int i) const {return (&x)[i];}
	INLINE operator HMbool* () {return &x;}
	INLINE operator const HMbool* () const {return &x;}

#define swizzleCombo3(vec3,x,y,z)				\
	INLINE vec3 &x##y##z() {return *(vec3*)&x;}	\
	INLINE const vec3 &x##y##z() const {return *(const vec3*)&x;}

#define swizzleCombos4(vec2,vec3,x,y,z,w)	\
	swizzleCombo2(vec2,x,y)					\
	swizzleCombo2(vec2,y,z)					\
	swizzleCombo2(vec2,z,w)					\
	swizzleCombo3(vec3,x,y,z)				\
	swizzleCombo3(vec3,y,z,w)

#define componentsCombos4(vec2,vec3)	\
	swizzleCombos4(vec2,vec3,x,y,z,w)	\
	swizzleCombos4(vec2,vec3,r,g,b,a)	\
	swizzleCombos4(vec2,vec3,s,t,p,q)

	componentsCombos4(bvec2,bvec3)

	INLINE HMbool operator==(const bvec4 &v) const {return x==v.x && y==v.y && z==v.z && w==v.w;}
	INLINE HMbool operator!=(const bvec4 &v) const {return x!=v.x || y!=v.y || z!=v.z || w!=v.w;}

	INLINE const bvec4 &operator=(const bvec4 &v) {x=v.x; y=v.y; z=v.z; w=v.w; return *this;}
};

INLINE bvec2::bvec2(const bvec4 &v):x(v.x),y(v.y) {}
INLINE bvec3::bvec3(const bvec4 &v):x(v.x),y(v.y),z(v.z) {}


#if !defined(DISABLE_VECTORIZATION) && !defined(__INTEL_COMPILER)
#define DISABLE_VECTORIZATION
#endif

template <class T> class Tvec4
{
public:

#ifndef DISABLE_VECTORIZATION
	union
	{
		struct {T x,y,z,w;};
		struct {T r,g,b,a;};
		struct {T s,t,p,q;};
		T e[4];
	};
#else
	union {T x,r,s;};
	union {T y,g,t;};
	union {T z,b,p;};
	union {T w,a,q;};
#endif

	typedef T TYPE;//for external use only

	INLINE Tvec4() {}
	//1 arg
	INLINE explicit Tvec4(const T a):x(a),y(a),z(a),w(a) {}
	//2 arg
	INLINE Tvec4(const Tvec3<T> &xyz,const T w)        :x(xyz.x),y(xyz.y),z(xyz.z),w(w)     {}
	INLINE Tvec4(const T x,const Tvec3<T> &yzw)        :x(x)    ,y(yzw.x),z(yzw.y),w(yzw.z) {}
	INLINE Tvec4(const Tvec2<T> &xy,const Tvec2<T> &zw):x(xy.x) ,y(xy.y) ,z(zw.x) ,w(zw.y)  {}
/*	INLINE Tvec4(const Tvec3<T> &xyz,const T w):xyz(xyz),w(w) {}
	INLINE Tvec4(const T x,const Tvec3<T> &yzw):x(x),yzw(yzw) {}
	INLINE Tvec4(const Tvec2<T> &xy,const Tvec2<T> &zw):xy(xy),zw(zw) {}*/
	//3 arg
	INLINE Tvec4(const Tvec2<T> &xy,const T z,const T w):x(xy.x),y(xy.y),z(z)   ,w(w)    {}
	INLINE Tvec4(const T x,const Tvec2<T> &yz,const T w):x(x)   ,y(yz.x),z(yz.y),w(w)    {}
	INLINE Tvec4(const T x,const T y,const Tvec2<T> &zw):x(x)   ,y(y)   ,z(zw.x),w(zw.y) {}
/*	INLINE Tvec4(const Tvec2<T> &xy,const T z,const T w):xy(xy),z(z),w(w) {}
	INLINE Tvec4(const T x,const Tvec2<T> &yz,const T w):x(x),yz(yz),w(w) {}
	INLINE Tvec4(const T x,const T y,const Tvec2<T> &zw):x(x),y(y),zw(zw) {}*/
	//4 arg
	INLINE Tvec4(const T x,const T y,const T z,const T w):x(x),y(y),z(z),w(w) {}

#ifndef DISABLE_VECTORIZATION
	INLINE T &operator [] (const int i) {return e[i];}
	INLINE const T operator [] (const int i) const {return e[i];}
	INLINE operator T* () {return e;}
	INLINE operator const T* () const {return e;}
#else
	INLINE T &operator [] (const int i) {return (&x)[i];}
	INLINE const T operator [] (const int i) const {return (&x)[i];}
	INLINE operator T* () {return &x;}
	INLINE operator const T* () const {return &x;}
#endif

	componentsCombos4(Tvec2<T>,Tvec3<T>)

#undef swizzleCombo2
#undef swizzleCombo3
#undef swizzleCombos4
#undef componentsCombos4

	INLINE HMbool operator==(const Tvec4 &v) const {return equal(x,v.x) && equal(y,v.y) && equal(z,v.z) && equal(w,v.w);}
	INLINE HMbool operator!=(const Tvec4 &v) const {return notEqual(x,v.x) || notEqual(y,v.y) || notEqual(z,v.z) || notEqual(w,v.w);}

#ifdef DISABLE_VECTORIZATION

	INLINE Tvec4(const Tvec4 &v):x(v.x),y(v.y),z(v.z),w(v.w) {}

#define op(o) INLINE const Tvec4 operator o(const T a) const {return Tvec4(x o a, y o a, z o a, w o a);}
	op(+) op(-) op(*) op(/)
#undef op

#define op(o) INLINE const Tvec4 operator o(const Tvec4 &v) const {return Tvec4(x o v.x, y o v.y, z o v.z, w o v.w);}
	op(+) op(-) op(*) op(/)
#undef op

#define op(o) INLINE const Tvec4 &operator o(const T a) {x o a; y o a; z o a; w o a; return *this;}
	op(+=) op(-=) op(*=) op(/=)
#undef op

#define op(o) INLINE const Tvec4 &operator o(const Tvec4 &v) {x o v.x; y o v.y; z o v.z; w o v.w; return *this;}
	op(+=) op(-=) op(*=) op(/=) op(=)
#undef op

	INLINE const Tvec4 operator-() const {return Tvec4(-x, -y, -z, -w);}
	INLINE const Tvec4 operator++() {return Tvec4(++x, ++y, ++z, ++w);}
	INLINE const Tvec4 operator--() {return Tvec4(--x, --y, --z, --w);}
	INLINE const Tvec4 operator++(int notused) {return Tvec4(x++, y++, z++, w++);}
	INLINE const Tvec4 operator--(int notused) {return Tvec4(x--, y--, z--, w--);}
};

#define op(o) template <class T>\
INLINE const Tvec4<T> operator o(const T a,const Tvec4<T> &v) {return Tvec4<T>(a o v.x, a o v.y, a o v.z, a o v.w);}
	op(+) op(-) op(*) op(/)
#undef op

#else //ENABLE_VECTORIZATION

	//copy constructor
	INLINE Tvec4(const Tvec4 &v)
	{
#pragma ivdep
		for (int i=0;i<4;i++) e[i]=v[i];
	}

	//vector ~ a
#define op_begin(o) INLINE const Tvec4 operator o(const T a) const {Tvec4 r;
#define op_end(o) for (int i=0;i<4;i++) r[i]=e[i] o a; return r;}
	op_begin(+)
#pragma ivdep
	  op_end(+) op_begin(-)
#pragma ivdep
				  op_end(-) op_begin(*)
#pragma ivdep
							  op_end(*) op_begin(/)
#pragma ivdep
										  op_end(/)
#undef op_begin
#undef op_end

	//vector ~ vector
#define op_begin(o) INLINE const Tvec4 operator o(const Tvec4 &v) const {Tvec4 r;
#define op_end(o) for (int i=0;i<4;i++) r[i]=e[i] o v[i]; return r;}
	op_begin(+)
#pragma ivdep
#pragma vector always
	  op_end(+) op_begin(-)
#pragma ivdep
#pragma vector always
				  op_end(-) op_begin(*)
#pragma ivdep
#pragma vector always
							  op_end(*) op_begin(/)
#pragma ivdep
#pragma vector always
										  op_end(/)
#undef op_begin
#undef op_end

	//vector~=a
#define op_begin(o) INLINE const Tvec4 &operator o(const T a) {
#define op_end(o) for (int i=0;i<4;i++) e[i] o a; return *this;}
	op_begin(+=)
#pragma ivdep
#pragma vector always
	  op_end(+=) op_begin(-=)
#pragma ivdep
#pragma vector always
				   op_end(-=) op_begin(*=)
#pragma ivdep
#pragma vector always
								op_end(*=) op_begin(/=)
#pragma ivdep
#pragma vector always
											 op_end(/=)
#undef op_begin
#undef op_end

	//vector~=vector
#define op_begin(o) INLINE const Tvec4 &operator o(const Tvec4 &v) {
#define op_end(o) for (int i=0;i<4;i++) e[i] o v[i]; return *this;}
	op_begin(+=)
#pragma ivdep
#pragma vector always
	  op_end(+=) op_begin(-=)
#pragma ivdep
#pragma vector always
				   op_end(-=) op_begin(*=)
#pragma ivdep
#pragma vector always
								op_end(*=) op_begin(/=)
#pragma ivdep
#pragma vector always
											 op_end(/=) op_begin(=)
#pragma ivdep
														  op_end(=)
#undef op_begin
#undef op_end

	//-vector
	INLINE const Tvec4 operator-() const
	{	Tvec4 r; 
#pragma ivdep
		for (int i=0;i<4;i++) r[i]=-e[i];
		return r;	}
	//++vector & --vector
	INLINE const Tvec4 operator++()
	{
#pragma ivdep
#pragma vector always
		for (int i=0;i<4;i++) ++e[i];
		return *this;
	}
	INLINE const Tvec4 operator--()
	{
#pragma ivdep
#pragma vector always
		for (int i=0;i<4;i++) --e[i];
		return *this;
	}
	//vector++ & vector--
	INLINE const Tvec4 operator++(int notused)
	{	Tvec4 t(*this);
#pragma ivdep
#pragma vector always
		for (int i=0;i<4;i++) ++e[i];
		return t;
	}
	INLINE const Tvec4 operator--(int notused)
	{	Tvec4 t(*this);
#pragma ivdep
#pragma vector always
		for (int i=0;i<4;i++) --e[i];
		return t;
	}
};

//a ~ vector
#define op_begin(o) template <class T> INLINE const Tvec4<T> operator o(const T a,const Tvec4<T> &v) {Tvec4<T> r;
#define op_end(o) for (int i=0;i<4;i++) r[i]=a o v[i]; return r;}
	op_begin(+)
#pragma ivdep
	  op_end(+) op_begin(-)
#pragma ivdep
				  op_end(-) op_begin(*)
#pragma ivdep
							  op_end(*) op_begin(/)
#pragma ivdep
										  op_end(/)
#undef op_begin
#undef op_end

#endif //DISABLE_VECTORIZATION

template <class T>
INLINE Tvec2<T>::Tvec2(const Tvec4<T> &v):x(v.x),y(v.y) {}

template <class T>
INLINE Tvec3<T>::Tvec3(const Tvec4<T> &v):x(v.x),y(v.y),z(v.z) {}


#define typedef_vectors(dimension)							\
typedef Tvec##dimension<HMfloat>		vec##dimension;		\
typedef Tvec##dimension<HMint>			ivec##dimension;	\
typedef Tvec##dimension<HMdouble>		dvec##dimension;	\
typedef Tvec##dimension<HMlong_double>	ldvec##dimension;	\
typedef Tvec##dimension<HMshort>		svec##dimension;	\
typedef Tvec##dimension<HMushort>		usvec##dimension;	\
typedef Tvec##dimension<HMubyte>		ubvec##dimension;	\
typedef Tvec##dimension<HMbyte>			sbvec##dimension;

typedef_vectors(2)
typedef_vectors(3)
typedef_vectors(4)

#undef typedef_vectors

#endif //HM_VEC_H
