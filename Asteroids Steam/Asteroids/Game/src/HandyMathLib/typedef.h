#ifndef HM_TYPEDEF_H
#define HM_TYPEDEF_H

typedef bool HMbool;
typedef signed char HMbyte;
typedef signed short HMshort;
typedef signed int HMint;
typedef unsigned char HMubyte;
typedef unsigned short HMushort;
typedef unsigned int HMuint;
typedef float HMfloat;
typedef double HMdouble;
typedef long double HMlong_double;

#define HMtrue  true
#define HMfalse false

#if _MSC_VER > 1000
#define INLINE inline//__forceinline
#else
#define INLINE inline
#endif // _MSC_VER > 1000

#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
#pragma warning(disable:4068)
#pragma warning(disable:4244)
#endif

//Undefine fucking Milkosoft macroses
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif


template <class T> class limits
{
public:
	static INLINE const T max() {return T(0);}
};

#include <float.h>
#include <limits.h>

#define IMPLEMENT_LIMITS(T,MAX)					\
template <> class limits<T>						\
{												\
public:											\
	static INLINE const T max() {return MAX;}	\
};

IMPLEMENT_LIMITS(HMbyte,SCHAR_MAX)
IMPLEMENT_LIMITS(HMubyte,UCHAR_MAX)
IMPLEMENT_LIMITS(HMshort,SHRT_MAX)
IMPLEMENT_LIMITS(HMushort,USHRT_MAX)
IMPLEMENT_LIMITS(HMint,INT_MAX)
IMPLEMENT_LIMITS(HMuint,UINT_MAX)
IMPLEMENT_LIMITS(HMfloat,FLT_MAX)
IMPLEMENT_LIMITS(HMdouble,DBL_MAX)
IMPLEMENT_LIMITS(HMlong_double,LDBL_MAX)

#undef IMPLEMENT_LIMITS

#endif //HM_TYPEDEF_H
