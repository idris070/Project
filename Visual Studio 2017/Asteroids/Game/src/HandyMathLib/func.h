#ifndef HM_FUNC_H
#define HM_FUNC_H

#include "vec.h"
#include "mat.h"
#include "quat.h"
#include "aabb.h"



//For internal use only
				   INLINE int dimension(const HMlong_double) {return 1;}
template <class T> INLINE int dimension(const Tvec2<T> &) {return 2;}
template <class T> INLINE int dimension(const Tvec3<T> &) {return 3;}
template <class T> INLINE int dimension(const Tvec4<T> &) {return 4;}
template <class T> INLINE int dimension(const Tmat2<T> &) {return 2;}
template <class T> INLINE int dimension(const Tmat3<T> &) {return 3;}
template <class T> INLINE int dimension(const Tmat4<T> &) {return 4;}

	   INLINE HMlong_double sum(const HMlong_double x)   {return x;}
template <class T> INLINE T sum(const Tvec2<T> &v) {return v.x+v.y;}
template <class T> INLINE T sum(const Tvec3<T> &v) {return v.x+v.y+v.z;}
template <class T> INLINE T sum(const Tvec4<T> &v) {return v.x+v.y+v.z+v.w;}

	   INLINE HMlong_double dereference(const HMlong_double x,const int)  {return x;}
template <class T> INLINE T dereference(const Tvec2<T> &v,const int i) {return v[i];}
template <class T> INLINE T dereference(const Tvec3<T> &v,const int i) {return v[i];}
template <class T> INLINE T dereference(const Tvec4<T> &v,const int i) {return v[i];}

//Type Cast Functions

#if _MSC_VER > 1000
	#pragma warning(push)
	#pragma warning(disable:4800)
#endif // _MSC_VER > 1000

#if defined(_MSC_VER) && _MSC_VER<=1200 //for MSVC++ 6.0 and earlier
	#define dst dstT::TYPE
#else
	#define dst typename dstT::TYPE
#endif

template <class dstT,class srcT> INLINE const dstT vec_cast(const Tvec2<srcT> &v) {return dstT((dst)v.x, (dst)v.y);}
template <class dstT,class srcT> INLINE const dstT vec_cast(const Tvec3<srcT> &v) {return dstT((dst)v.x, (dst)v.y, (dst)v.z);}
template <class dstT,class srcT> INLINE const dstT vec_cast(const Tvec4<srcT> &v) {return dstT((dst)v.x, (dst)v.y, (dst)v.z, (dst)v.w);}

template <class dstT> INLINE const dstT vec_cast(const bvec2 &v) {return dstT((dst)v.x, (dst)v.y);}
template <class dstT> INLINE const dstT vec_cast(const bvec3 &v) {return dstT((dst)v.x, (dst)v.y, (dst)v.z);}
template <class dstT> INLINE const dstT vec_cast(const bvec4 &v) {return dstT((dst)v.x, (dst)v.y, (dst)v.z, (dst)v.w);}


template <class dstT,class srcT>
INLINE const dstT mat_cast(const Tmat4<srcT> &m) {return dstT((dst)m.e[ 0], (dst)m.e[ 1], (dst)m.e[ 2], (dst)m.e[ 3],
															  (dst)m.e[ 4], (dst)m.e[ 5], (dst)m.e[ 6], (dst)m.e[ 7],
															  (dst)m.e[ 8], (dst)m.e[ 9], (dst)m.e[10], (dst)m.e[11],
															  (dst)m.e[12], (dst)m.e[13], (dst)m.e[14], (dst)m.e[15]);}

template <class dstT,class srcT>
INLINE const dstT mat_cast(const Tmat3<srcT> &m) {return dstT((dst)m.e[0], (dst)m.e[1], (dst)m.e[2],
															  (dst)m.e[3], (dst)m.e[4], (dst)m.e[5],
															  (dst)m.e[6], (dst)m.e[7], (dst)m.e[8]);}

template <class dstT,class srcT>
INLINE const dstT mat_cast(const Tmat2<srcT> &m) {return dstT((dst)m.e[0], (dst)m.e[1],
															  (dst)m.e[2], (dst)m.e[3]);}

template <class dstT,class srcT> INLINE const dstT quat_cast(const Tquat<srcT> &q)
{return dstT((dst)q.x, (dst)q.y, (dst)q.z, (dst)q.w);}

template <class dstT,class srcT> INLINE const dstT aabb_cast(const Taabb<srcT> &b)
{return dstT(vec_cast<dst>(b.min), vec_cast<dst>(b.max));}

template <class dstT,class srcT> INLINE const dstT rect_cast(const Taabb<srcT> &r)
{return dstT(vec_cast<dst>(r.min), vec_cast<dst>(r.max));}

#undef dst

#if _MSC_VER > 1000
	#pragma warning(pop)
#endif // _MSC_VER > 1000

//Angle Functions
const HMlong_double PI=3.1415926535897932384626433832795L;
template <class T> INLINE const T radians(const T &deg) {return deg*T(0.017453292519943295769236907684886L);}
template <class T> INLINE const T degrees(const T &rad) {return rad*T(57.295779513082320876798154814105L);}

//Exponential Functions
template <class T> INLINE const T exp2(const T &x) {return exp(x*T(0.69314718055994530941723212145818L));}
template <class T> INLINE const T log2(const T &x) {return log(x)*T(1.4426950408889634073599246810019L);}
template <class T> INLINE const T inversesqrt(const T &x) {return T(1)/sqrt(x);}

//Pseudorandom Number Generating Functions
static HMuint HMRandomSeed=0;

INLINE void rseed(const HMuint newSeed)
{HMRandomSeed=newSeed;}

#if _MSC_VER>=1300 //for MSVC++ >7.0
__declspec(noinline)
#endif
INLINE const HMuint random(const HMuint randMax)
{return (HMRandomSeed=1664525L*HMRandomSeed+1013904223L)%(randMax+1);}

INLINE const HMuint random(const HMuint randMin,const HMuint randMax)
{return random(randMax-randMin)+randMin;}

INLINE const HMint random(const HMint randMax) {return (HMint)random(HMuint(randMax));}
INLINE const HMint random(const HMint randMin,const HMint randMax)
{return (HMint)random(HMuint(randMin),HMuint(randMax));}

#if _MSC_VER>=1300 //for MSVC++ >7.0
__declspec(noinline)
#endif
INLINE const HMlong_double random(const HMlong_double randMax)
{return (HMRandomSeed=1664525L*HMRandomSeed+1013904223L)*(randMax/limits<HMuint>::max());}

INLINE const HMlong_double random(const HMlong_double randMin,const HMlong_double randMax)
{return random(randMax-randMin)+randMin;}

INLINE const HMfloat random(const HMfloat randMax) {return (HMfloat)random((HMlong_double)randMax);}
INLINE const HMfloat random(const HMfloat randMin,const HMfloat randMax) {return (HMfloat)random((HMlong_double)randMin,(HMlong_double)randMax);}
INLINE const HMdouble random(const HMdouble randMax) {return (HMdouble)random((HMlong_double)randMax);}
INLINE const HMdouble random(const HMdouble randMin,const HMdouble randMax) {return (HMdouble)random((HMlong_double)randMin,(HMlong_double)randMax);}

//Common Functions
#if defined(_MSC_VER) && _MSC_VER<=1200 //for MSVC++ 6.0 and earlier
#define T_TYPE T::TYPE
#else
#define T_TYPE typename T::TYPE
#endif

template <class T> INLINE const T square(const T &a) {return a*a;}
template <class T> INLINE const T cube(const T &a) {return a*a*a;}
template <class T> INLINE const T sign(const T &x) {return x>0 ? T(1) : (x<0 ? T(-1) : 0);}
template <class T> INLINE const T fract(const T &x) {return x-floor(x);}
INLINE const HMbyte mod(const HMbyte x,const HMbyte y) {return x%y;}
INLINE const HMshort mod(const HMshort x,const HMshort y) {return x%y;}
INLINE const HMint mod(const HMint x,const HMint y) {return x%y;}
INLINE const HMubyte mod(const HMubyte x,const HMubyte y) {return x%y;}
INLINE const HMushort mod(const HMushort x,const HMushort y) {return x%y;}
INLINE const HMuint mod(const HMuint x,const HMuint y) {return x%y;}
template <class T> INLINE const T mod(const T &x,const T &y) {return x - y*floor(x/y);}
template <class T> INLINE const T wrap(const T &x,const T &maxVal) {return mod(x,maxVal);}
template <class T> INLINE const T wrap(const T &x,const T &minVal,const T &maxVal)
{return mod(x-minVal,maxVal-minVal)+minVal;}

#define minmaxFunc(funcName,sign,type)	\
INLINE const type funcName(const type x,const type y) {return x sign y ? x : y;}
#define minmaxFuncs(funcName,sign)		\
minmaxFunc(funcName,sign,HMbyte)		\
minmaxFunc(funcName,sign,HMshort)		\
minmaxFunc(funcName,sign,HMint)			\
minmaxFunc(funcName,sign,HMubyte)		\
minmaxFunc(funcName,sign,HMushort)		\
minmaxFunc(funcName,sign,HMuint)		\
minmaxFunc(funcName,sign,HMfloat)		\
minmaxFunc(funcName,sign,HMdouble)		\
minmaxFunc(funcName,sign,HMlong_double)	\
template <class T> INLINE const T_TYPE funcName(const T &v)			\
{																	\
	T_TYPE res=v[0];												\
	for (int i=1;i<dimension(v);i++)								\
		if (v[i] sign res) res=v[i];								\
	return res;														\
}																	\
template <class T> INLINE const T funcName(const T &x,const T &y)	\
{																	\
	T res;															\
	for (int i=0;i<dimension(x);i++)								\
		res[i]=x[i] sign y[i] ? x[i] : y[i];						\
	return res;														\
}																	\
template <class T> INLINE const T funcName(const T &x,const T &y,const T &z) {return funcName##2(funcName##2(x,y),z);}	\
template <class T> INLINE const T funcName(const T &x,const T &y,const T &z,const T &w) {return funcName##2(funcName##2(x,y),funcName##2(z,w));}

minmaxFuncs(min,<)
minmaxFuncs(max,>)

#undef minmaxFunc
#undef minmaxFuncs
//template <class T> INLINE const T min(const T &x,const T &y) {return min2(x,y);}
//template <class T> INLINE const T max(const T &x,const T &y) {return max2(x,y);}
/*template <class T> INLINE const Tvec2<T> funcName##2(const Tvec2<T> &x,const Tvec2<T> &y)	\
{return Tvec2<T>(x[0] sign y[0] ? x[0] : y[0],												\
				 x[1] sign y[1] ? x[1] : y[1]);}											\
template <class T> INLINE const Tvec3<T> funcName##2(const Tvec3<T> &x,const Tvec3<T> &y)	\
{return Tvec3<T>(x[0] sign y[0] ? x[0] : y[0],												\
				 x[1] sign y[1] ? x[1] : y[1],												\
				 x[2] sign y[2] ? x[2] : y[2]);}											\
template <class T> INLINE const Tvec4<T> funcName##2(const Tvec4<T> &x,const Tvec4<T> &y)	\
{return Tvec4<T>(x[0] sign y[0] ? x[0] : y[0],												\
				 x[1] sign y[1] ? x[1] : y[1],												\
				 x[2] sign y[2] ? x[2] : y[2],												\
				 x[3] sign y[3] ? x[3] : y[3]);}											\*/
/*template <class T> INLINE const T max2(const T &x,const T &y) {return x>y ? x : y;}
template <class T> INLINE const T max3(const T &x,const T &y,const T &z) {return max2(max2(x,y),z);}
template <class T> INLINE const T max4(const T &x,const T &y,const T &z,const T &w) {return max2(max2(x,y),max2(z,w));}*/
template <class T> INLINE const T clamp(const T &x,const T &minVal,const T &maxVal)
{return x<minVal ? minVal : (x>maxVal ? maxVal : x);}//min(max(x,minVal),maxVal);}

template <class T,class Ta> INLINE const T  mix(const T &x,const T &y,const Ta &a) {return x + (y-x)*a;}
template <class T,class Ta> INLINE const T lerp(const T &x,const T &y,const Ta &a) {return mix(x,y,a);}
template <class T,class Ta> INLINE const T cerp(const T &x,const T &y,const Ta &a) {return mix(x,y,(Ta(1)-cos(a*PI))/Ta(2));}
template <class T,class Ta> INLINE const T herp3(const T &x,const T &y,const Ta &a) {return mix(x,y,a*a*(Ta(3)-Ta(2)*a));}
template <class T,class Ta> INLINE const T herp5(const T &x,const T &y,const Ta &a) {return mix(x,y,a*a*a*(Ta(10)+a*(Ta(6)*a-Ta(15))));}

template <class Ta,class T> INLINE const T step(const Ta &edge,const T &x) {return x<edge ? T(0) : T(1);}
template <class Ta,class T> INLINE const T smoothstep(const Ta &edge0,const Ta &edge1,const T &x)
{
	T t;
	t=clamp((x-edge0) / (edge1-edge0),T(0),T(1));
	return t*t*(T(3)-T(2)*t);
}

//Geometric Functions
template <class T> INLINE const T dot(const Tvec2<T> &v0,const Tvec2<T> &v1) {return v0.x*v1.x+v0.y*v1.y;}
template <class T> INLINE const T dot(const Tvec3<T> &v0,const Tvec3<T> &v1) {return v0.x*v1.x+v0.y*v1.y+v0.z*v1.z;}
template <class T> INLINE const T dot(const Tvec4<T> &v0,const Tvec4<T> &v1) {return v0.x*v1.x+v0.y*v1.y+v0.z*v1.z+v0.w*v1.w;}

/*template <class T> INLINE const T sqlen(const Tvec2<T> &v) {return dot(v,v);}
template <class T> INLINE const T sqlen(const Tvec3<T> &v) {return dot(v,v);}
template <class T> INLINE const T sqlen(const Tvec4<T> &v) {return dot(v,v);}*/
template <class T> INLINE const T_TYPE sqlen(const T &v) {return dot(v,v);}

//template <class T> INLINE const typename T::TYPE length(const T &v) {return sqrt(sqlen(v));}
template <class T> INLINE const T length(const Tvec2<T> &v) {return sqrt(sqlen(v));}
template <class T> INLINE const T length(const Tvec3<T> &v) {return sqrt(sqlen(v));}
template <class T> INLINE const T length(const Tvec4<T> &v) {return sqrt(sqlen(v));}

//template <class T,class Tr> INLINE const Tr distance(const T &p0,const T &p1) {return length(p0-p1);}
template <class T> INLINE const T_TYPE distance(const T &p0,const T &p1) {return length(p0-p1);}

template <class T> INLINE const Tvec3<T> cross(const Tvec3<T> &v0,const Tvec3<T> &v1)
{return Tvec3<T>(v0.y*v1.z-v0.z*v1.y, v0.z*v1.x-v0.x*v1.z, v0.x*v1.y-v0.y*v1.x);}

template <class T> INLINE const T normalize(const T &v) {return v/length(v);}
/*template <class T> INLINE const Tvec2<T> normalize(const Tvec2<T> &v) {return v/length(v);}
template <class T> INLINE const Tvec3<T> normalize(const Tvec3<T> &v) {return v/length(v);}
template <class T> INLINE const Tvec4<T> normalize(const Tvec4<T> &v) {return v/length(v);}*/

template <class T> INLINE const Tvec2<T> perp(const Tvec2<T> &v) {return Tvec2<T>(-v.y,v.x);}

template <class T> INLINE const Tvec3<T> perp(const Tvec3<T> &v)
{
	if (fabs(v.x)<fabs(v.y))
		return Tvec3<T>(0,-v.z,v.y);//cross(Tvec3<T>(1,0,0),n);
	else
		return Tvec3<T>(v.z,0,-v.x);//cross(Tvec3<T>(0,1,0),n);
}

template <class T> INLINE const Tvec3<T> slerp(const Tvec3<T> &v1,const Tvec3<T> &v2,const T t)
{
	T cos_a=dot(v1,v2)/sqrt(sqlen(v1)*sqlen(v2));
	if (greaterThanEqual(cos_a,T(1))) return lerp(v1,v2,t);
	if (lessThanEqual(cos_a,T(1))) return rotate(radians(t*180),perp(v1),v1);
	T a=acos(cos_a),b=a*t;
	T cos_b=cos(b),cos_ab=cos(a-b);
	Tvec3<T> v1_v2=cross(v1,v2);

	T t1=v1_v2.y*v2.z-v1_v2.z*v2.y,
	  t2=v1_v2.x*v2.z-v1_v2.z*v2.x,
	  t3=v1_v2.x*v2.y-v1_v2.y*v2.x;
	T det=v1.x*t1-v1.y*t2+v1.z*t3;

	return Tvec3<T>(cos_b*t1+cos_ab*(v1.y*v1_v2.z-v1_v2.y*v1.z),
				   -cos_b*t2-cos_ab*(v1.x*v1_v2.z-v1_v2.x*v1.z),
					cos_b*t3+cos_ab*(v1.x*v1_v2.y-v1_v2.x*v1.y))/det;
}

template <class T> INLINE const T faceforward(const T &N,const T &I,const T &Nref)
{if (dot(Nref,I)<0) return N; else return -N;}

template <class T> INLINE const Tvec2<T> reflect(const Tvec2<T> &V,const Tvec2<T> &N) {return V-2*dot(V,N)*N;}
template <class T> INLINE const Tvec3<T> reflect(const Tvec3<T> &V,const Tvec3<T> &N) {return V-2*dot(V,N)*N;}
template <class T> INLINE const Tvec3<T> reflect(const Tvec3<T> &V,const Tvec4<T> &Plane)
{return V-2*(dot(V,Plane.xyz())+Plane.w)*Plane.xyz();}
template <class T> INLINE const Tvec4<T> reflect(const Tvec4<T> &V,const Tvec4<T> &Plane)
{return Tvec4<T>(V.xyz()-2*dot(V,Plane)*Plane.xyz(),V.w);}

template <class T> INLINE const T refract(const T &V,const T &N,const T_TYPE eta)
{
	T_TYPE d=dot(N,V),
		   k=1 - eta*eta*(1-d*d);
	if (k<0)
		return T(0);
	else
		return V*eta - N*(eta*d+sqrt(k));
}

#undef T_TYPE

//Matrix Functions
template <class T> INLINE const T matrixCompMult(const T &x,const T &y)
{
	const int n=dimension(x)*dimension(x);
	T res;
	for (int i=0;i<n/*sizeof(res.e)/sizeof(T::TYPE)*/;i++)
		res.e[i]=x.e[i]*y.e[i];
	return res;
}

template <class T> INLINE const Tmat2<T> inverse(const Tmat2<T> &m)
{
	return Tmat2<T>(m[1][1], -m[0][1], -m[1][0], m[0][0])
					/ (m[0][0]*m[1][1] - m[0][1]*m[1][0]);
}

template <class T> INLINE const Tmat3<T> inverse(const Tmat3<T> &m)
{
	T A00 =			m[1][1]*m[2][2] - m[1][2]*m[2][1],
	  A10 = 		m[0][2]*m[2][1] - m[0][1]*m[2][2],
	  A20 =			m[0][1]*m[1][2] - m[0][2]*m[1][1];

	return Tmat3<T>(A00,
					A10,
					A20,
					m[1][2]*m[2][0] - m[1][0]*m[2][2],
					m[0][0]*m[2][2] - m[0][2]*m[2][0],
					m[0][2]*m[1][0] - m[0][0]*m[1][2],
					m[1][0]*m[2][1] - m[1][1]*m[2][0],
					m[0][1]*m[2][0] - m[0][0]*m[2][1],
					m[0][0]*m[1][1] - m[0][1]*m[1][0])
					/ (m[0][0] * A00 + m[1][0] * A10 + m[2][0] * A20);
}

template <class T> INLINE const Tmat4<T> inverse(const Tmat4<T> &m)
{
	T det0112 = m[0][1]*m[1][2] - m[0][2]*m[1][1],
	  det0113 = m[0][1]*m[1][3] - m[0][3]*m[1][1],
	  det0122 = m[0][1]*m[2][2] - m[0][2]*m[2][1],
	  det0123 = m[0][1]*m[2][3] - m[0][3]*m[2][1],
	  det0132 = m[0][1]*m[3][2] - m[0][2]*m[3][1],
	  det0133 = m[0][1]*m[3][3] - m[0][3]*m[3][1],
	  det0213 = m[0][2]*m[1][3] - m[0][3]*m[1][2],
	  det0223 = m[0][2]*m[2][3] - m[0][3]*m[2][2],
	  det0233 = m[0][2]*m[3][3] - m[0][3]*m[3][2],
	  det1122 = m[1][1]*m[2][2] - m[1][2]*m[2][1],
	  det1123 = m[1][1]*m[2][3] - m[1][3]*m[2][1],
	  det1132 = m[1][1]*m[3][2] - m[1][2]*m[3][1],
	  det1133 = m[1][1]*m[3][3] - m[1][3]*m[3][1],
	  det1223 = m[1][2]*m[2][3] - m[1][3]*m[2][2],
	  det1233 = m[1][2]*m[3][3] - m[1][3]*m[3][2],
	  det2132 = m[2][1]*m[3][2] - m[2][2]*m[3][1],
	  det2133 = m[2][1]*m[3][3] - m[2][3]*m[3][1],
	  det2233 = m[2][2]*m[3][3] - m[2][3]*m[3][2];

	T A00 =			m[1][1] * det2233 - m[2][1] * det1233 + m[3][1] * det1223,
	  A10 =			m[2][1] * det0233 - m[0][1] * det2233 - m[3][1] * det0223,
	  A20 =			m[0][1] * det1233 - m[1][1] * det0233 + m[3][1] * det0213,
	  A30 =			m[1][1] * det0223 - m[0][1] * det1223 - m[2][1] * det0213;

	return Tmat4<T>(A00,
					A10,
					A20,
					A30,
					m[2][0] * det1233 - m[1][0] * det2233 - m[3][0] * det1223,
					m[0][0] * det2233 - m[2][0] * det0233 + m[3][0] * det0223,
					m[1][0] * det0233 - m[0][0] * det1233 - m[3][0] * det0213,
					m[0][0] * det1223 - m[1][0] * det0223 + m[2][0] * det0213,
					m[1][0] * det2133 - m[2][0] * det1133 + m[3][0] * det1123,
					m[2][0] * det0133 - m[0][0] * det2133 - m[3][0] * det0123,
					m[0][0] * det1133 - m[1][0] * det0133 + m[3][0] * det0113,
					m[1][0] * det0123 - m[0][0] * det1123 - m[2][0] * det0113,
					m[2][0] * det1132 - m[1][0] * det2132 - m[3][0] * det1122,
					m[0][0] * det2132 - m[2][0] * det0132 + m[3][0] * det0122,
					m[1][0] * det0132 - m[0][0] * det1132 - m[3][0] * det0112,
					m[0][0] * det1122 - m[1][0] * det0122 + m[2][0] * det0112)
					/ (m[0][0] * A00 + m[1][0] * A10 + m[2][0] * A20 + m[3][0] * A30);
}

template <class T> INLINE const Tmat2<T> transpose(const Tmat2<T> &m)
{
	return Tmat2<T>(m[0][0], m[1][0],
					m[0][1], m[1][1]);
}

template <class T> INLINE const Tmat3<T> transpose(const Tmat3<T> &m)
{
	return Tmat3<T>(m[0][0], m[1][0], m[2][0],
					m[0][1], m[1][1], m[2][1],
					m[0][2], m[1][2], m[2][2]);
}

template <class T> INLINE const Tmat4<T> transpose(const Tmat4<T> &m)
{
	return Tmat4<T>(m[0][0], m[1][0], m[2][0], m[3][0],
					m[0][1], m[1][1], m[2][1], m[3][1],
					m[0][2], m[1][2], m[2][2], m[3][2],
					m[0][3], m[1][3], m[2][3], m[3][3]);
}

template <class T> INLINE const Tmat3<T> orthonormalize(const Tmat3<T> &m)
{
	Tvec3<T> u=m[0],v=m[1],n=m[2];
	v=cross(n,u);
	u=normalize(u);
	v=normalize(v);
	n=cross(u,v);
	return Tmat3<T>(u,v,n);
}

template <class T> INLINE const Tmat4<T> scalingMat(const Tvec3<T> &s)
{
	return Tmat4<T>(s.x,   0,   0, 0,
					  0, s.y,   0, 0,
					  0,   0, s.z, 0,
					  0,   0,   0, 1);
}

template <class T> INLINE const Tmat4<T> scalingMat4(const Tvec3<T> &s) {return scalingMat(s);}

template <class T> INLINE const Tmat3<T> scalingMat3(const Tvec3<T> &s)
{
	return Tmat3<T>(s.x,   0,   0,
					  0, s.y,   0,
					  0,   0, s.z);
}

template <class T> INLINE const Tmat3<T> scalingMat(const Tvec2<T> &s)
{
	return Tmat3<T>(s.x,   0, 0,
					  0, s.y, 0,
					  0,   0, 1);
}

template <class T> INLINE const Tmat3<T> scalingMat3(const Tvec2<T> &s) {return scalingMat(s);}

template <class T> INLINE const Tmat2<T> scalingMat2(const Tvec2<T> &s)
{
	return Tmat2<T>(s.x,   0,
					  0, s.y);
}

template <class T> INLINE const Tmat4<T> translationMat(const Tvec3<T> &tr)
{
	return Tmat4<T>(   1,    0,    0, 0,
					   0,    1,    0, 0,
					   0,    0,    1, 0,
					tr.x, tr.y, tr.z, 1);
}

template <class T> INLINE const Tmat4<T> translationMat4(const Tvec3<T> &tr) {return translationMat(tr);}

template <class T> INLINE const Tmat3<T> translationMat(const Tvec2<T> &tr)
{
	return Tmat3<T>(   1,    0, 0,
					   0,    1, 0,
					tr.x, tr.y, 1);
}

template <class T> INLINE const Tmat3<T> translationMat3(const Tvec2<T> &tr) {return translationMat(tr);}

template <class T> INLINE const Tmat3<T> crossMat(const Tvec3<T> &n)
{
	return Tmat3<T>(   0,  n.z, -n.y,
					-n.z,    0,  n.x,
					 n.y, -n.x,    0);
}

template <class T> INLINE const Tmat3<T> crossMat3(const Tvec3<T> &n) {return crossMat(n);}

template <class T> INLINE const Tmat4<T> crossMat4(const Tvec3<T> &n)
{
	return Tmat4<T>(   0,  n.z, -n.y, 0,
					-n.z,    0,  n.x, 0,
					 n.y, -n.x,    0, 0,
					   0,    0,    0, 1);
}

//Vector Relational Functions
#define ComponentWiseCompareUnary(funcName)							\
																	\
template <class T> INLINE const bvec2 funcName(const Tvec2<T> &v)	\
{return bvec2(funcName(v.x),funcName(v.y));}						\
																	\
template <class T> INLINE const bvec3 funcName(const Tvec3<T> &v)	\
{return bvec3(funcName(v.x),funcName(v.y),funcName(v.z));}			\
																	\
template <class T> INLINE const bvec4 funcName(const Tvec4<T> &v)	\
{return bvec4(funcName(v.x),funcName(v.y),funcName(v.z),funcName(v.w));}

ComponentWiseCompareUnary(isZero)
ComponentWiseCompareUnary(notZero)

#undef ComponentWiseCompareUnary

#define ComponentWiseCompareBinary(funcName)										\
																					\
template <class T> INLINE const bvec2 funcName(const Tvec2<T> &a,const Tvec2<T> &b)	\
{return bvec4(funcName(a.x,b.x),funcName(a.y,b.y));}								\
																					\
template <class T> INLINE const bvec3 funcName(const Tvec3<T> &a,const Tvec3<T> &b)	\
{return bvec4(funcName(a.x,b.x),funcName(a.y,b.y),funcName(a.z,b.z));}				\
																					\
template <class T> INLINE const bvec4 funcName(const Tvec4<T> &a,const Tvec4<T> &b)	\
{return bvec4(funcName(a.x,b.x),funcName(a.y,b.y),funcName(a.z,b.z),funcName(a.w,b.w));}

ComponentWiseCompareBinary(lessThan)
ComponentWiseCompareBinary(lessThanEqual)
ComponentWiseCompareBinary(greaterThan)
ComponentWiseCompareBinary(greaterThanEqual)
ComponentWiseCompareBinary(equal)
ComponentWiseCompareBinary(notEqual)

#undef ComponentWiseCompareBinary

INLINE HMbool any(const bvec2 &v) {return v.x||v.y;}
INLINE HMbool any(const bvec3 &v) {return v.x||v.y||v.z;}
INLINE HMbool any(const bvec4 &v) {return v.x||v.y||v.z||v.w;}

INLINE HMbool all(const bvec2 &v) {return v.x&&v.y;}
INLINE HMbool all(const bvec3 &v) {return v.x&&v.y&&v.z;}
INLINE HMbool all(const bvec4 &v) {return v.x&&v.y&&v.z&&v.w;}

#ifdef __GNUC__
INLINE const bvec2 operator not(const bvec2 &v) {return bvec2(!v.x,!v.y);}
INLINE const bvec3 operator not(const bvec3 &v) {return bvec3(!v.x,!v.y,!v.z);}
INLINE const bvec4 operator not(const bvec4 &v) {return bvec4(!v.x,!v.y,!v.z,!v.w);}
#else
INLINE const bvec2 not(const bvec2 &v) {return bvec2(!v.x,!v.y);}
INLINE const bvec3 not(const bvec3 &v) {return bvec3(!v.x,!v.y,!v.z);}
INLINE const bvec4 not(const bvec4 &v) {return bvec4(!v.x,!v.y,!v.z,!v.w);}
#endif //__GNUC__

//Noise Functions

static INLINE const HMuint blockMul(const HMuint x,const HMuint y)
{
	return x*y
		+ (x >> 16)*(y >> 16)
		+ ((((x >> 16) + 1)*(((y & 0xFFFF) >> 1)) >> 16) - 2)
		+ ((((x & 0xFFFF) >> 1)*(((y >> 16) + 1)) >> 16) - 2);
}

const HMuint noiseNum[4][3]={{3705202873U,2747025859U,0U},
							 {3025910477U,3546377269U,2394005989U},
							 {2707923697U,2926984589U,4102909219U},
							 {2561027561U,3518370727U,3466353563U}};

const HMuint noiseK[4][2][4]={3056977171U,3269783761U,2658032953U,3344787169U,
							  4037351789U,2609616677U,2464778807U,2766689203U,
							  3834873673U,2376325447U,2320361767U,3156823877U,
							  3363093571U,3333579463U,2308363033U,3134625413U,
							  2496754387U,2529548431U,2627810863U,2537233241U,
							  3447375511U,2269853797U,3091444841U,2335077293U,
							  3804676669U,3901832071U,3300504203U,4050898273U,
							  3048137039U,2649152113U,2778566341U,2984586859U};

template <class Tres,class baseT,int sizeofT,class T> INLINE const Tres noise_(const T &v,const baseT (*ferp)(const baseT &x,const baseT &y,const baseT &a))
{
	int i,j,k,n;
	HMuint iv[sizeofT];
	baseT a[sizeofT];
	for (i=0;i<sizeofT;i++)
	{
		iv[i]=(HMuint)floor(((baseT*)(&v))[i]);
		a[i]=fract(((baseT*)(&v))[i]);
	}

	static bool cacheValid=false;
	static HMuint civ[sizeofT];
	static baseT cf[sizeof(Tres)/sizeof(baseT)][1<<sizeofT];

	bool takeFromCache=true;
	if (cacheValid)
	{
		for (i=0;i<sizeofT;i++)
			if (iv[i]!=civ[i])
			{
				for (;i<sizeofT;i++)
					civ[i]=iv[i];

				takeFromCache=false;
				break;
			}
	}
	else
	{
		for (i=0;i<sizeofT;i++)
			civ[i]=iv[i];

		cacheValid=true;
		takeFromCache=false;
	}

	Tres r;
	for (i=0;i<sizeof(Tres)/sizeof(baseT);i++)
	{
		baseT f[1<<sizeofT];

		if (takeFromCache)
		{
			for (j=0;j<(1<<sizeofT);j++)
				f[j]=cf[i][j];
		}
		else
		{
			HMuint ivC[sizeofT][2][2];//[][min/max][+/^]
			for (j=0;j<sizeofT;j++)
				for (k=0;k<2;k++)
				{
					HMuint K=noiseK[i][k][j];
					ivC[j][0][k]=blockMul(iv[j],K);
					ivC[j][1][k]=blockMul(iv[j]+1,K);
				}

			for (j=0;j<(1<<sizeofT);j++)
			{
				HMuint t=noiseNum[i][0];
				for (k=0;k<sizeofT;k++)
					t^=ivC[k][(j>>k)&1][1];

				t=blockMul(t,noiseNum[i][1])+noiseNum[i][2];

				for (k=0;k<sizeofT;k++)
					t+=ivC[k][(j>>k)&1][0];

				cf[i][j]=f[j]=t*(baseT(2)/baseT(limits<HMuint>::max()))-baseT(1);
			}
		}

		for (n=1<<(sizeofT-1),k=sizeofT-1;n;n>>=1,k--)
			for (j=0;j<n;j++)
				f[j]=ferp(f[j],f[j+n],a[k]);

		((baseT*)(&r))[i]=f[0];
	}

	return r;
/*	int i,j;
	HMuint t[sizeofT];
	for (i=0;i<sizeofT;i++)
		t[i]=HMuint(fract(((baseT*)(&v))[i])*baseT(limits<HMuint>::max()));

	Tres r;
	for (i=0;i<dimension(Tres());i++)
	{
		HMuint a=noiseNum[i][0];
		for (j=0;j<sizeofT;j++)
			a^=blockMul(t[j],noiseK[i][1][j]);

		HMuint ir=blockMul(a,noiseNum[i][1])+noiseNum[i][2];

		for (j=0;j<sizeofT;j++)
			ir+=blockMul(t[j],noiseK[i][0][j]);

		((baseT*)(&r))[i]=baseT(ir);
	}

	return r*baseT(2)/baseT(limits<HMuint>::max())-baseT(1);*/
}

template <class Tres,class T> INLINE const Tres inoise_(const T &v)//Integer version
{
	int i,j;

	Tres r;
	for (i=0;i<sizeof(Tres)/sizeof(HMint);i++)
	{
		HMuint a=noiseNum[i][0];
		for (j=0;j<sizeof(T)/sizeof(HMint);j++)
			a^=blockMul(((HMuint*)(&v))[j],noiseK[i][1][j]);

		a=blockMul(a,noiseNum[i][1])+noiseNum[i][2];

		for (j=0;j<sizeof(T)/sizeof(HMint);j++)
			a+=blockMul(((HMuint*)(&v))[j],noiseK[i][0][j]);

		((HMuint*)(&r))[i]=a;
	}

	return r;
}

#define noiseFuncs11(T)																	\
INLINE const T noise1(const T &v,const T (*ferp)(const T &,const T &,const T &)=herp5)	\
{return noise_<T,T,1>(v,ferp);}

#define noiseFuncs1N(N,T)																		\
INLINE const T noise1(const Tvec##N<T> &v,const T (*ferp)(const T &,const T &,const T &)=herp5)	\
{return noise_<T,T,N>(v,ferp);}

#define noiseFuncsN1(N,T)																			\
INLINE const Tvec##N<T> noise##N(const T &v,const T (*ferp)(const T &,const T &,const T &)=herp5)	\
{return noise_<Tvec##N<T>,T,1>(v,ferp);}

#define noiseFuncsMN(M,N,T)																					\
INLINE const Tvec##M<T> noise##M(const Tvec##N<T> &v,const T (*ferp)(const T &,const T &,const T &)=herp5)	\
{return noise_<Tvec##M<T>,T,N>(v,ferp);}

#define noiseFuncs(T)	\
noiseFuncs11(T)			\
noiseFuncs1N(2,T)		\
noiseFuncs1N(3,T)		\
noiseFuncs1N(4,T)		\
noiseFuncsN1(2,T)		\
noiseFuncsN1(3,T)		\
noiseFuncsN1(4,T)		\
noiseFuncsMN(2,2,T)		\
noiseFuncsMN(2,3,T)		\
noiseFuncsMN(2,4,T)		\
noiseFuncsMN(3,2,T)		\
noiseFuncsMN(3,3,T)		\
noiseFuncsMN(3,4,T)		\
noiseFuncsMN(4,2,T)		\
noiseFuncsMN(4,3,T)		\
noiseFuncsMN(4,4,T)

noiseFuncs(HMfloat)
noiseFuncs(HMdouble)
noiseFuncs(HMlong_double)

#undef noiseFuncs11
#undef noiseFuncs1N
#undef noiseFuncsN1
#undef noiseFuncsMN

#define noiseFuncs11(T)				\
INLINE const T noise1(const T &v)	\
{return inoise_<T>(v);}

#define noiseFuncs1N(N,T)					\
INLINE const T noise1(const Tvec##N<T> &v)	\
{return inoise_<T>(v);}

#define noiseFuncsN1(N,T)					\
INLINE const Tvec##N<T> noise##N(const T &v)\
{return inoise_<Tvec##N<T> >(v);}

#define noiseFuncsMN(M,N,T)								\
INLINE const Tvec##M<T> noise##M(const Tvec##N<T> &v)	\
{return inoise_<Tvec##M<T> >(v);}

noiseFuncs(HMint)

#undef noiseFuncs11
#undef noiseFuncs1N
#undef noiseFuncsN1
#undef noiseFuncsMN
#undef noiseFuncs

//Pseudo-plane Functions
template <class T> INLINE const Tvec3<T> planeNormal(const Tvec3<T> &p0,const Tvec3<T> &p1,const Tvec3<T> &p2)
{return cross(p1-p0,p2-p0);}

template <class T> INLINE const Tvec4<T> plane(const Tvec3<T> &p0,const Tvec3<T> &p1,const Tvec3<T> &p2)
{
	Tvec3<T> n=planeNormal(p0,p1,p2);
	return Tvec4<T>(n,-dot(n,p0));
}

template <class T> INLINE const Tvec4<T> plane(const Tvec3<T> &n,const Tvec3<T> &p)
{return Tvec4<T>(n,-dot(n,p));}

template <class T> INLINE const Tvec4<T> unitPlane(const Tvec3<T> &p0,const Tvec3<T> &p1,const Tvec3<T> &p2)
{return plane(normalize(planeNormal(p0,p1,p2)),p0);}

template <class T> INLINE const Tvec4<T> unitPlane(const Tvec3<T> &n,const Tvec3<T> &p)
{return plane(normalize(n),p);}

template <class T> INLINE const Tvec4<T> normalizePlane(const Tvec4<T> &Plane)
{return Plane/length(Plane.xyz());}

template <class T> INLINE const Tvec4<T> translatePlane(const Tvec4<T> &Plane,const Tvec3<T> &tr)
{return Tvec4<T>(Plane.xyz(),Plane.w-dot(Plane.xyz(),tr));}

template <class T> INLINE const Tvec3<T> planePoint(const Tvec4<T> &Plane)
{return Plane.xyz()*(-Plane.w/sqlen(Plane.xyz()));}

template <class T> INLINE const Tvec3<T> unitPlanePoint(const Tvec4<T> &Plane)
{return Plane.xyz()*(-Plane.w);}

template <class T> INLINE const Tvec4<T> reflectPlane(const Tvec4<T> &Plane,const Tvec4<T> &R)
{return plane(reflect(Plane.xyz(),R.xyz()),reflect(planePoint(Plane),R));}

/*template <class T> INLINE const Tvec4<T> modifyPlaneNormal(const Tvec4<T> &Plane,const Tvec3<T> &newN)
{return Tvec4<T>(newN,dot(newN,Plane.xyz())*Plane.w/sqlen(Plane.xyz()));}

template <class T> INLINE const Tvec4<T> modifyUnitPlaneNormal(const Tvec4<T> &Plane,const Tvec3<T> &newN)
{return Tvec4<T>(newN,dot(newN,Plane.xyz())*Plane.w);}*/

template <class T> INLINE const T planeDist(const Tvec4<T> &Plane,const Tvec3<T> &point)
{return (dot(Plane.xyz(),point)+Plane.w)/length(Plane.xyz());}

template <class T> INLINE const T planeDist(const Tvec4<T> &Plane,const Tvec4<T> &point)
{return dot(Plane,point)/(length(Plane.xyz())*point.w);}

template <class T> INLINE const T unitPlaneDist(const Tvec4<T> &Plane,const Tvec3<T> &point)
{return dot(Plane.xyz(),point)+Plane.w;}

template <class T> INLINE const T unitPlaneDist(const Tvec4<T> &Plane,const Tvec4<T> &point)
{return dot(Plane,point)/point.w;}

template <class T> INLINE HMbool onPlane(const Tvec4<T> &Plane,const Tvec3<T> &point)
{return isZero(dot(Plane.xyz(),point)+Plane.w);}

template <class T> INLINE HMbool outOfPlane(const Tvec4<T> &Plane,const Tvec3<T> &point)
{return notZero(dot(Plane.xyz(),point)+Plane.w);}

template <class T> INLINE HMbool abovePlane(const Tvec4<T> &Plane,const Tvec3<T> &point)
{return greaterThan(dot(Plane.xyz(),point),-Plane.w);}

template <class T> INLINE HMbool onPlaneOrAbove(const Tvec4<T> &Plane,const Tvec3<T> &point)
{return greaterThanEqual(dot(Plane.xyz(),point),-Plane.w);}

template <class T> INLINE HMbool belowPlane(const Tvec4<T> &Plane,const Tvec3<T> &point)
{return lessThan(dot(Plane.xyz(),point),-Plane.w);}

template <class T> INLINE HMbool onPlaneOrBelow(const Tvec4<T> &Plane,const Tvec3<T> &point)
{return lessThanEqual(dot(Plane.xyz(),point),-Plane.w);}

template <class T>
INLINE HMbool rayPlaneIntn(T &t,const Tvec3<T> &rayStart,const Tvec3<T> &rayDirn,const Tvec4<T> &Plane)
{
	T d=dot(Plane.xyz(),rayDirn);
	if (notZero(d))
	{
		t=-unitPlaneDist(Plane,rayStart)/d;
		return HMtrue;
	}
	return HMfalse;
}

template <class T>
INLINE HMbool rayPlaneIntn(Tvec3<T> &intn,const Tvec3<T> &rayStart,const Tvec3<T> &rayDirn,const Tvec4<T> &Plane,const T min=0)
{
	T t;	
	if (rayPlaneIntn(t,rayStart,rayDirn,Plane))
		if (t>=min)
		{
			intn=rayStart+rayDirn*t;
			return HMtrue;
		}

	return HMfalse;
}

template <class T>
INLINE HMbool rayPlaneIntn(Tvec3<T> &intn,const Tvec3<T> &rayStart,const Tvec3<T> &rayDirn,const Tvec4<T> &Plane,const T min,const T max)
{
	T t;	
	if (rayPlaneIntn(rayStart,rayDirn,Plane,t))
		if (t>=min && t<=max)
		{
			intn=rayStart+rayDirn*t;
			return HMtrue;
		}

	return HMfalse;
}

template <class T>
INLINE HMbool segPlaneIntn(T &t,const Tvec3<T> &segStart,const Tvec3<T> &segEnd,const Tvec4<T> &Plane)
{
	return rayPlaneIntn(t,segStart,segEnd-segStart,Plane);
}

template <class T>
INLINE HMbool segPlaneIntn(Tvec3<T> &intn,const Tvec3<T> &segStart,const Tvec3<T> &segEnd,const Tvec4<T> &Plane,const T min=0)
{
	return rayPlaneIntn(intn,segStart,segEnd-segStart,Plane,min);
}

template <class T>
INLINE HMbool segPlaneIntn(Tvec3<T> &intn,const Tvec3<T> &segStart,const Tvec3<T> &segEnd,const Tvec4<T> &Plane,const T min,const T max)
{
	return rayPlaneIntn(intn,segStart,segEnd-segStart,Plane,min,max);
}

//LookAt Functions

template <class T> INLINE const Tmat4<T> lookAtDirnMat(const Tvec3<T> &eye,const Tvec3<T> &dirn,const Tvec3<T> &up)
{
	Tvec3<T> n=-normalize(dirn);
	Tvec3<T> u=cross(up,n);
	if (u==Tvec3<T>(0))
	{
#define calcU													\
		if (fabs(n.x)<T(.7))									\
			u=Tvec3<T>(0,-n.z,n.y);/*cross(Tvec3<T>(1,0,0),n);*/\
		else													\
			u=Tvec3<T>(n.z,0,-n.x);/*cross(Tvec3<T>(0,1,0),n);*/

		calcU
	}
	u=normalize(u);
	Tvec3<T> v=cross(n,u);
	return Tmat4<T>(        u.x,         v.x,         n.x, 0,
					        u.y,         v.y,         n.y, 0,
					        u.z,         v.z,         n.z, 0,
					-dot(u,eye), -dot(v,eye), -dot(n,eye), 1);
}

template <class T> INLINE const Tmat4<T> lookAtMat(const Tvec3<T> &eye,const Tvec3<T> &center,const Tvec3<T> &up)
{
	return lookAtDirnMat(eye,center-eye,up);
}

template <class T> INLINE const Tmat4<T> lookAtDirnMat(const Tvec3<T> &eye,const Tvec3<T> &dirn)
{
	Tvec3<T> n=-normalize(dirn);
	calcU
	u=normalize(u);
	Tvec3<T> v=cross(n,u);
	return Tmat4<T>(        u.x,         v.x,         n.x, 0,
					        u.y,         v.y,         n.y, 0,
					        u.z,         v.z,         n.z, 0,
					-dot(u,eye), -dot(v,eye), -dot(n,eye), 1);
}

template <class T> INLINE const Tmat4<T> lookAtMat(const Tvec3<T> &eye,const Tvec3<T> &dirn)
{
	return lookAtDirnMat(eye,center-eye);
}

template <class T> INLINE const Tmat4<T> lookAtDirnMat(const Tvec3<T> &dirn)
{
	Tvec3<T> n=-normalize(dirn);
	calcU
#undef calcU
	u=normalize(u);
	Tvec3<T> v=cross(n,u);
	return Tmat4<T>(u.x, v.x, n.x, 0,
					u.y, v.y, n.y, 0,
					u.z, v.z, n.z, 0,
					  0,   0,   0, 1);
}

//Auxiliary Functions

template <class T> INLINE const Tvec3<T> rotateX(const T angle,const Tvec3<T> &vec)
{
	T sn=sin(angle),cs=cos(angle);
	return Tvec3<T>(vec.x,
					vec.y*cs-vec.z*sn,
					vec.y*sn+vec.z*cs);
}

template <class T> INLINE const Tmat3<T> rotationXMat(const T angle)
{
	T sn=sin(angle),cs=cos(angle);
	return Tmat3<T>(1,   0,  0,
					0,  cs, sn,
					0, -sn, cs);
}

template <class T> INLINE const Tmat3<T> rotationXMat3(const T angle) {return rotationXMat(angle);}

template <class T> INLINE const Tmat4<T> rotationXMat4(const T angle) {return Tmat4<T>(rotationXMat(angle));}

template <class T> INLINE const Tvec3<T> rotateY(const T angle,const Tvec3<T> &vec)
{
	T sn=sin(angle),cs=cos(angle);
	return Tvec3<T>(vec.x*cs+vec.z*sn,
					vec.y,
					vec.z*cs-vec.x*sn);
}

template <class T> INLINE const Tmat3<T> rotationYMat(const T angle)
{
	T sn=sin(angle),cs=cos(angle);
	return Tmat3<T>(cs, 0, -sn,
					 0, 1,   0,
					sn, 0,  cs);
}

template <class T> INLINE const Tmat3<T> rotationYMat3(const T angle) {return rotationYMat(angle);}

template <class T> INLINE const Tmat4<T> rotationYMat4(const T angle) {return Tmat4<T>(rotationYMat(angle));}

template <class T> INLINE const Tvec3<T> rotateZ(const T angle,const Tvec3<T> &vec)
{
	T sn=sin(angle),cs=cos(angle);
	return Tvec3<T>(vec.x*cs-vec.y*sn,
					vec.x*sn+vec.y*cs,
					vec.z);
}

template <class T> INLINE const Tmat3<T> rotationZMat(const T angle)
{
	T sn=sin(angle),cs=cos(angle);
	return Tmat3<T>( cs, sn, 0,
					-sn, cs, 0,
					  0,  0, 1);
}

template <class T> INLINE const Tmat3<T> rotationZMat3(const T angle) {return rotationZMat(angle);}

template <class T> INLINE const Tmat4<T> rotationZMat4(const T angle) {return Tmat4<T>(rotationZMat(angle));}

template <class T> INLINE const Tvec3<T> rotate(const T angle,const Tvec3<T> &axis,const Tvec3<T> &vec)
{
	if (notZero(angle))
	{
		Tvec3<T> ax=normalize(axis);
		Tvec3<T> proj=ax*dot(ax,vec);
		return proj+cross(ax,vec)*sin(angle)+(vec-proj)*cos(angle);
	} else return vec;
}

template <class T> INLINE const Tmat3<T> rotationMat(const T angle,const Tvec3<T> &axis)
{
	if (notZero(angle))
	{
		Tvec3<T> n=normalize(axis);
		T sn=sin(angle),cs=cos(angle),_1_cs=1-cs,
		  nxny1_cs=n.x*n.y*_1_cs,nxnz1_cs=n.x*n.z*_1_cs,nynz1_cs=n.y*n.z*_1_cs;
		Tvec3<T> nsn=n*sn;
		return Tmat3<T>(cs+n.x*n.x*_1_cs,   nxny1_cs+nsn.z,   nxnz1_cs-nsn.y,
						  nxny1_cs-nsn.z, cs+n.y*n.y*_1_cs,   nynz1_cs+nsn.x,
						  nxnz1_cs+nsn.y,   nynz1_cs-nsn.x, cs+n.z*n.z*_1_cs);
	} else return Tmat3<T>(1);
}

template <class T> INLINE const Tmat3<T> rotationMat3(const T angle,const Tvec3<T> &axis)
{return rotationMat(angle,axis);}

template <class T> INLINE const Tmat4<T> rotationMat4(const T angle,const Tvec3<T> &axis)
{return Tmat4<T>(rotationMat(angle,axis));}
/*	if (notZero(angle))
	{
		Tvec3<T> n=normalize(axis);
		T sn=sin(angle),cs=cos(angle),_1_cs=1-cs,
		  nxny1_cs=n.x*n.y*_1_cs,nxnz1_cs=n.x*n.z*_1_cs,nynz1_cs=n.y*n.z*_1_cs;
		Tvec3<T> nsn=n*sn;
		return Tmat4<T>(cs+n.x*n.x*_1_cs,   nxny1_cs+nsn.z,   nxnz1_cs-nsn.y, 0,
						  nxny1_cs-nsn.z, cs+n.y*n.y*_1_cs,   nynz1_cs+nsn.x, 0,
						  nxnz1_cs+nsn.y,   nynz1_cs-nsn.x, cs+n.z*n.z*_1_cs, 0,
									   0,				 0,				   0, 1);
	} else return Tmat4<T>(1);
}*/

template <class T> INLINE const Tmat4<T> reflectionMat(const Tvec4<T> &Plane)
{
	return Tmat4<T>(1) - T(2)*Tmat4<T>(Tvec4<T>(Plane.xyz()*Plane.x,0),
									   Tvec4<T>(Plane.xyz()*Plane.y,0),
									   Tvec4<T>(Plane.xyz()*Plane.z,0),
									   Tvec4<T>(Plane.xyz()*Plane.w,0));
}

//template <class T> INLINE const T project(const T &V,const T &N) {return V-N*dot(V,N);}
template <class T> INLINE const Tvec3<T> project(const Tvec3<T> &V,const Tvec4<T> &Plane)
{return V-Plane.xyz()*(dot(V,Plane.xyz())+Plane.w);}
template <class T> INLINE const Tvec4<T> project(const Tvec4<T> &V,const Tvec4<T> &Plane)
{return Tvec4<T>(V.xyz()-Plane.xyz()*dot(V,Plane),V.w);}

template <class T> INLINE const Tmat4<T> projectionMat(const Tvec4<T> &Plane)
{
	return Tmat4<T>(1) - Tmat4<T>(Tvec4<T>(Plane.xyz()*Plane.x,0),
								  Tvec4<T>(Plane.xyz()*Plane.y,0),
								  Tvec4<T>(Plane.xyz()*Plane.z,0),
								  Tvec4<T>(Plane.xyz()*Plane.w,0));
}

//Quaternion Functions

template <class T> INLINE const Tquat<T> conjugate(const Tquat<T> &q) {return Tquat<T>(-q.x,-q.y,-q.z,q.w);}

template <class T> INLINE const T dot(const Tquat<T> &q0,const Tquat<T> &q1) {return q0.x*q1.x+q0.y*q1.y+q0.z*q1.z+q0.w*q1.w;}
template <class T> INLINE const T norm(const Tquat<T> &q) {return dot(q,q);}
template <class T> INLINE const T magnitude(const Tquat<T> &q) {return sqrt(norm(q));}
template <class T> INLINE const T length(const Tquat<T> &q) {return magnitude(q);}

template <class T> INLINE const Tquat<T> inverse(const Tquat<T> &q) {return conjugate(q)/norm(q);}

template <class T> INLINE const Tquat<T> shortestArcQuat(const Tvec3<T> &from,const Tvec3<T> &to)
{
	Tquat<T> q;
	q.xyz()=cross(from,to);
	q.w=dot(from,to);
	q.w+=length(q);//Reducing rotating angle of quaternion to a half
	if (lessThanEqual(q.w,T(0))) return Tquat<T>(perp(from));
	return q;
}

template <class T> INLINE const Tquat<T> shortestArcUnitQuat(const Tvec3<T> &from,const Tvec3<T> &to)
{
	Tquat<T> q;
	q.xyz()=cross(from,to);
	q.w=dot(from,to);
	T len=length(q);
	q.w+=len;//Reducing rotating angle of quaternion to a half
	if (lessThanEqual(q.w,T(0))) return Tquat<T>(perp(from));
	return q/sqrt(2*len*q.w);
}
//{return normalize(shortestArcQuat(from,to));}

template <class T> INLINE const Tmat3<T> quatToMat (const Tquat<T> &q) {return Tmat3<T>(q,HMfalse);}
template <class T> INLINE const Tmat3<T> quatToMat3(const Tquat<T> &q) {return Tmat3<T>(q,HMfalse);}
template <class T> INLINE const Tmat4<T> quatToMat4(const Tquat<T> &q) {return Tmat4<T>(q,HMfalse);}

template <class T> INLINE const Tmat3<T> unitQuatToMat (const Tquat<T> &q) {return Tmat3<T>(q,HMtrue);}
template <class T> INLINE const Tmat3<T> unitQuatToMat3(const Tquat<T> &q) {return Tmat3<T>(q,HMtrue);}
template <class T> INLINE const Tmat4<T> unitQuatToMat4(const Tquat<T> &q) {return Tmat4<T>(q,HMtrue);}

template <class T> INLINE const T quatToAxisAngle(Tvec3<T> &axis,const Tquat<T> &q)
{
	T ll=sqlen(q.xyz());
	if (greaterThan(ll,T(0)))
	{
		T len=sqrt(ll);
		axis=q.xyz()/len;
		return 2*(T)acos(q.w/sqrt(ll+q.w*q.w));
	}
	else
	{
		axis=Tvec3<T>(0);
		return 0;
	}
}

template <class T> INLINE const T unitQuatToAxisAngle(Tvec3<T> &axis,const Tquat<T> &q)
{
	if (lessThan(T(fabs(q.w)),T(1)))
	{
		T len=sqrt(1-q.w*q.w);
		axis=q.xyz()/len;
		return 2*(T)atan2(len,q.w);
	}
	else
	{
		axis=Tvec3<T>(0);
		return 0;
	}
}

template <class T> INLINE const Tvec3<T> quatToAxisAngle(const Tquat<T> &q)
{
	T ll=sqlen(q.xyz());
	if (greaterThan(ll,T(0)))
	{
		T len=sqrt(ll);
		return q.xyz()*(2*(T)acos(q.w/sqrt(ll+q.w*q.w))/len);
	}
	else
		return Tvec3<T>(0);
}

template <class T> INLINE const Tvec3<T> unitQuatToAxisAngle(const Tquat<T> &q)
{
	if (lessThan(T(fabs(q.w)),T(1)))
	{
		T len=sqrt(1-q.w*q.w);
		return q.xyz()*(2*(T)atan2(len,q.w)/len);
	}
	else
		return Tvec3<T>(0);
}

template <class T> INLINE const T quatAngle(const Tquat<T> &q)
{return 2*(T)acos(q.w/length(q));}

template <class T> INLINE const T unitQuatAngle(const Tquat<T> &q)
{return 2*(T)acos(q.w);}

template <class T> INLINE const Tquat<T> matToQuat(const Tmat3<T> &M) {return Tquat<T>(M,HMfalse);}
template <class T> INLINE const Tquat<T> matToQuat(const Tmat4<T> &M) {return Tquat<T>(M,HMfalse);}

template <class T> INLINE const Tquat<T> matToUnitQuat(const Tmat3<T> &M) {return Tquat<T>(M,HMtrue);}
template <class T> INLINE const Tquat<T> matToUnitQuat(const Tmat4<T> &M) {return Tquat<T>(M,HMtrue);}

template <class T> INLINE const Tquat<T> slerp(const Tquat<T> &from,const Tquat<T> &to,const T t)
{
	T fromLen=length(from),toLen=length(to);
	T d=dot(from,to)/(fromLen*toLen),k1,k2;

	if (d<0) {toLen=-toLen; d=-d;}

	if (greaterThanEqual(d,T(1)))
	{
		k1=(1-t)/fromLen;
		k2=   t /  toLen;
	}
	else
	{
		T a=acos(d),sn=sqrt(1-d*d);
		k1=sin((1-t)*a)/(sn*fromLen);
		k2=sin(   t *a)/(sn*  toLen);
	}

	return from*k1+to*k2;
}

template <class T> INLINE const Tquat<T> squad(const Tquat<T> &p,
											   const Tquat<T> &a,
											   const Tquat<T> &b,
											   const Tquat<T> &q,
											   const T t)
{return slerp(slerp(p,q,t), slerp(a,b,t), 2*t*(1-t));}

#endif //HM_FUNC_H
