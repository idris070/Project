#ifndef HM_MAT_H
#define HM_MAT_H

#include "vec.h"



template <class T> class Tmat3;
template <class T> class Tmat4;
template <class T> class Tquat;

////////////////////////////////////////////////////////////////////////////
//////////////////////// %   %    %    %%%%%   ###  ////////////////////////
//////////////////////// %% %%   % %     %    #   # ////////////////////////
//////////////////////// %% %%  %   %    %      ##  ////////////////////////
//////////////////////// % % %  %%%%%    %     #    ////////////////////////
//////////////////////// %   %  %   %    %    ##### ////////////////////////
////////////////////////////////////////////////////////////////////////////

template <class T> class Tmat2
{
public:

#define operator_x(d,op,arg,secondOperand)				\
	INLINE const Tmat##d operator op(const arg) const	\
	{													\
		Tmat##d res;									\
														\
		for (int i=0;i<d*d;i++)							\
			res.e[i]=e[i] op secondOperand;				\
														\
		return res;										\
	}
#define operator_xx(d,op,arg,secondOperand)				\
	INLINE const Tmat##d &operator op(const arg)		\
	{for (int i=0;i<d*d;i++) e[i] op secondOperand; return *this;}

#define operators4(d)														\
	union																	\
	{																		\
		T m[d][d];															\
		T e[d*d];															\
	};																		\
	INLINE Tmat##d() {}														\
	INLINE Tvec##d<T> &operator[](const int i) {return *(Tvec##d<T>*)m[i];}	\
	INLINE const Tvec##d<T> &operator[](const int i) const {return *(const Tvec##d<T>*)m[i];}\
	INLINE operator T* () {return e;}										\
	INLINE operator const T* () const {return e;}							\
	INLINE HMbool operator==(const Tmat##d &M) const						\
	{	for (int i=0;i<d*d;i++)												\
			if (notEqual(e[i],M.e[i])) return HMfalse;						\
		return HMtrue;   }													\
	INLINE HMbool operator!=(const Tmat##d &M) const						\
	{	for (int i=0;i<d*d;i++)												\
			if (notEqual(e[i],M.e[i])) return HMtrue;						\
		return HMfalse;   }													\
	operator_x(d, +, T a, a)												\
	operator_x(d, -, T a, a)												\
	operator_x(d, *, T a, a)												\
	operator_x(d, /, T a, a)												\
	operator_x(d, +, Tmat##d &M, M.e[i])									\
	operator_x(d, -, Tmat##d &M, M.e[i])									\
	operator_x(d, /, Tmat##d &M, M.e[i])									\
	operator_xx(d, +=, T a, a)												\
	operator_xx(d, -=, T a, a)												\
	operator_xx(d, *=, T a, a)												\
	operator_xx(d, /=, T a, a)												\
	operator_xx(d, +=, Tmat##d &M, M.e[i])									\
	operator_xx(d, -=, Tmat##d &M, M.e[i])									\
	operator_xx(d, /=, Tmat##d &M, M.e[i])									\
	INLINE const Tmat##d &operator*=(const Tmat##d &M) {return *this=(*this)*M;}\
	INLINE const Tmat##d operator-() const {Tmat##d res; for (int i=0;i<d*d;i++) res.e[i]=-e[i]; return res;}\
	INLINE const Tmat##d operator++() {for (int i=0;i<d*d;i++) ++e[i]; return *this;}\
	INLINE const Tmat##d operator--() {for (int i=0;i<d*d;i++) --e[i]; return *this;}\
	INLINE const Tmat##d operator++(int notused) {Tmat##d t(*this); for (int i=0;i<d*d;i++) ++e[i]; return t;}\
	INLINE const Tmat##d operator--(int notused) {Tmat##d t(*this); for (int i=0;i<d*d;i++) --e[i]; return t;}
#define operators(d)														\
	INLINE Tmat##d(const Tmat##d &M) {for (int i=0;i<d*d;i++) e[i]=M.e[i];}	\
	operators4(d)															\
	operator_xx(d, =, Tmat##d &M, M.e[i])

	operators(2)

	typedef T TYPE;//for external use only

	INLINE explicit Tmat2(const T a)
	{
		m[0][0]=a; m[0][1]=0;
		m[1][0]=0; m[1][1]=a;
	}

	INLINE explicit Tmat2(const Tmat3<T> &M);
	INLINE explicit Tmat2(const Tmat4<T> &M);

	INLINE Tmat2(const Tvec2<T> &m0,const Tvec2<T> &m1)
	{
		(*this)[0]=m0;
		(*this)[1]=m1;
	}

	INLINE Tmat2(const T m00, const T m01,
				 const T m10, const T m11)
	{
		m[0][0]=m00; m[0][1]=m01;
		m[1][0]=m10; m[1][1]=m11;
	}

	INLINE const Tvec2<T> operator*(const Tvec2<T> &v) const
	{
#ifndef D3D_MAT_STYLE
		return Tvec2<T>(v[0] * m[0][0] + v[1] * m[1][0],
						v[0] * m[0][1] + v[1] * m[1][1]);
#else
		return Tvec2<T>(v[0] * m[0][0] + v[1] * m[0][1],
						v[0] * m[1][0] + v[1] * m[1][1]);
#endif
	}

	INLINE const Tmat2 operator*(const Tmat2 &M) const
	{
		Tmat2 r;
		int j;
		T c0,c1;

#ifndef D3D_MAT_STYLE
#define column(i)					\
		c0=M.e[i*2+0];				\
		c1=M.e[i*2+1];				\
									\
		for (j=0;j<2;j++)			\
			r.e[i*2+j]=e[0*2+j]*c0	\
					  +e[1*2+j]*c1;
#else
#define column(i)					\
		c0=e[i*2+0];				\
		c1=e[i*2+1];				\
									\
		for (j=0;j<2;j++)			\
			r.e[i*2+j]=M.e[0*2+j]*c0\
					  +M.e[1*2+j]*c1;
#endif

		column(0) column(1)

#undef column
		return r;
	}
};

template <class T>
INLINE const Tvec2<T> operator*(const Tvec2<T> &v,const Tmat2<T> &M)
{
#ifndef D3D_MAT_STYLE
	return Tvec2<T>(v[0] * M[0][0] + v[1] * M[0][1],
					v[0] * M[1][0] + v[1] * M[1][1]);
#else
	return Tvec2<T>(v[0] * M[0][0] + v[1] * M[1][0],
					v[0] * M[0][1] + v[1] * M[1][1]);
#endif
}

#define operator_xxx(d,op)											\
template <class T>													\
INLINE const Tmat##d<T> operator op(const T a,const Tmat##d<T> &M)	\
{																	\
	Tmat##d<T> res;													\
																	\
	for (int i=0;i<d*d;i++)											\
		res.e[i]=a op M.e[i];										\
																	\
	return res;														\
}

#define operators_xxx(d)\
operator_xxx(d, +)		\
operator_xxx(d, -)		\
operator_xxx(d, *)		\
operator_xxx(d, /)		\

operators_xxx(2)

////////////////////////////////////////////////////////////////////////////
//////////////////////// %   %    %    %%%%%  ####  ////////////////////////
//////////////////////// %% %%   % %     %        # ////////////////////////
//////////////////////// %% %%  %   %    %      ##  ////////////////////////
//////////////////////// % % %  %%%%%    %        # ////////////////////////
//////////////////////// %   %  %   %    %    ####  ////////////////////////
////////////////////////////////////////////////////////////////////////////

template <class T> class Tmat3
{
public:

	operators(3)

	typedef T TYPE;//for external use only

	INLINE explicit Tmat3(const T a)
	{
		m[0][0]=a; m[0][1]=0; m[0][2]=0;
		m[1][0]=0; m[1][1]=a; m[1][2]=0;
		m[2][0]=0; m[2][1]=0; m[2][2]=a;
	}

	INLINE explicit Tmat3(const Tmat4<T> &M);
	INLINE explicit Tmat3(const Tquat<T> &q,HMbool quatIsUnit=HMfalse);

	INLINE explicit Tmat3(const Tmat2<T> &M)
	{
		(*this)[0]=Tvec3<T>(M[0],0);
		(*this)[1]=Tvec3<T>(M[1],0);
		(*this)[2]=Tvec3<T>( 0,0,1);
	}

	INLINE Tmat3(const Tvec2<T> &m0,const Tvec2<T> &m1)
	{
		(*this)[0]=Tvec3<T>( m0,0);
		(*this)[1]=Tvec3<T>( m1,0);
		(*this)[2]=Tvec3<T>(0,0,1);
	}

	INLINE Tmat3(const Tvec3<T> &m0,const Tvec3<T> &m1,const Tvec3<T> &m2)
	{
		(*this)[0]=m0;
		(*this)[1]=m1;
		(*this)[2]=m2;
	}

	INLINE Tmat3(const T m00, const T m01,
				 const T m10, const T m11)
	{
		m[0][0]=m00; m[0][1]=m01; m[0][2]=0;
		m[1][0]=m10; m[1][1]=m11; m[1][2]=0;
		m[2][0]=  0; m[2][1]=  0; m[2][2]=1;
	}

	INLINE Tmat3(const T m00, const T m01, const T m02,
				 const T m10, const T m11, const T m12,
				 const T m20, const T m21, const T m22)
	{
		m[0][0]=m00; m[0][1]=m01; m[0][2]=m02;
		m[1][0]=m10; m[1][1]=m11; m[1][2]=m12;
		m[2][0]=m20; m[2][1]=m21; m[2][2]=m22;
	}

	INLINE const Tvec3<T> operator*(const Tvec3<T> &v) const
	{
#ifndef D3D_MAT_STYLE
		return Tvec3<T>(v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[2][0],
						v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[2][1],
						v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m[2][2]);
#else
		return Tvec3<T>(v[0] * m[0][0] + v[1] * m[0][1] + v[2] * m[0][2],
						v[0] * m[1][0] + v[1] * m[1][1] + v[2] * m[1][2],
						v[0] * m[2][0] + v[1] * m[2][1] + v[2] * m[2][2]);
#endif
	}

	INLINE const Tmat3 operator*(const Tmat3 &M) const
	{
		Tmat3 r;
		int j;
		T c0,c1,c2;

#ifndef D3D_MAT_STYLE
#define column(i)					\
		c0=M.e[i*3+0];				\
		c1=M.e[i*3+1];				\
		c2=M.e[i*3+2];				\
									\
		for (j=0;j<3;j++)			\
			r.e[i*3+j]=e[0*3+j]*c0	\
					  +e[1*3+j]*c1	\
					  +e[2*3+j]*c2;
#else
#define column(i)					\
		c0=e[i*3+0];				\
		c1=e[i*3+1];				\
		c2=e[i*3+2];				\
									\
		for (j=0;j<3;j++)			\
			r.e[i*3+j]=M.e[0*3+j]*c0\
					  +M.e[1*3+j]*c1\
					  +M.e[2*3+j]*c2;
#endif

		column(0) column(1) column(2)

#undef column
		return r;
	}
};

template <class T>
INLINE const Tvec3<T> operator*(const Tvec3<T> &v,const Tmat3<T> &M)
{
#ifndef D3D_MAT_STYLE
	return Tvec3<T>(v[0] * M[0][0] + v[1] * M[0][1] + v[2] * M[0][2],
					v[0] * M[1][0] + v[1] * M[1][1] + v[2] * M[1][2],
					v[0] * M[2][0] + v[1] * M[2][1] + v[2] * M[2][2]);
#else
	return Tvec3<T>(v[0] * M[0][0] + v[1] * M[1][0] + v[2] * M[2][0],
					v[0] * M[0][1] + v[1] * M[1][1] + v[2] * M[2][1],
					v[0] * M[0][2] + v[1] * M[1][2] + v[2] * M[2][2]);
#endif
}

operators_xxx(3)

template <class T>
INLINE Tmat2<T>::Tmat2(const Tmat3<T> &M)
{
	(*this)[0]=Tvec2<T>(M[0]);
	(*this)[1]=Tvec2<T>(M[1]);
}

////////////////////////////////////////////////////////////////////////////
//////////////////////// %   %    %    %%%%%     ## ////////////////////////
//////////////////////// %% %%   % %     %      # # ////////////////////////
//////////////////////// %% %%  %   %    %     #  # ////////////////////////
//////////////////////// % % %  %%%%%    %    ##### ////////////////////////
//////////////////////// %   %  %   %    %        # ////////////////////////
////////////////////////////////////////////////////////////////////////////

template <class T> class Tmat4
{
public:

	operators4(4)

#undef operator_x
#undef operator_xx
#undef operators4
#undef operators

/*	union
	{
		struct {Tvec4<T> m[4];};
		struct {T e[16];};
	};

	INLINE Tmat4() {}*/

	typedef T TYPE;//for external use only

	//copy constructor
	INLINE Tmat4(const Tmat4 &M)
	{
#pragma ivdep
		for (int i=0;i<16;i++) e[i]=M.e[i];
	}

	INLINE explicit Tmat4(const T a)
	{
		m[0][0]=a; m[0][1]=0; m[0][2]=0; m[0][3]=0;
		m[1][0]=0; m[1][1]=a; m[1][2]=0; m[1][3]=0;
		m[2][0]=0; m[2][1]=0; m[2][2]=a; m[2][3]=0;
		m[3][0]=0; m[3][1]=0; m[3][2]=0; m[3][3]=a;
	}

	INLINE explicit Tmat4(const Tquat<T> &q,HMbool quatIsUnit=HMfalse) {*this=Tmat4(Tmat3<T>(q,quatIsUnit));}

	INLINE explicit Tmat4(const Tmat2<T> &M)
	{
		(*this)[0]=Tvec4<T>(M[0],0,0);
		(*this)[1]=Tvec4<T>(M[1],0,0);
		(*this)[2]=Tvec4<T>( 0,0,1,0);
		(*this)[3]=Tvec4<T>( 0,0,0,1);
	}

	INLINE explicit Tmat4(const Tmat3<T> &M)
	{
		(*this)[0]=Tvec4<T>( M[0],0);
		(*this)[1]=Tvec4<T>( M[1],0);
		(*this)[2]=Tvec4<T>( M[2],0);
		(*this)[3]=Tvec4<T>(0,0,0,1);
	}

	INLINE Tmat4(const Tvec2<T> &m0,const Tvec2<T> &m1)
	{
		(*this)[0]=Tvec4<T>( m0,0,0);
		(*this)[1]=Tvec4<T>( m1,0,0);
		(*this)[2]=Tvec4<T>(0,0,1,0);
		(*this)[3]=Tvec4<T>(0,0,0,1);
	}

	INLINE Tmat4(const Tvec3<T> &m0,const Tvec3<T> &m1,const Tvec3<T> &m2)
	{
		(*this)[0]=Tvec4<T>(   m0,0);
		(*this)[1]=Tvec4<T>(   m1,0);
		(*this)[2]=Tvec4<T>(   m2,0);
		(*this)[3]=Tvec4<T>(0,0,0,1);
	}

	INLINE Tmat4(const Tvec4<T> &m0,const Tvec4<T> &m1,const Tvec4<T> &m2,const Tvec4<T> &m3)
	{
		(*this)[0]=m0;
		(*this)[1]=m1;
		(*this)[2]=m2;
		(*this)[3]=m3;
	}

	INLINE Tmat4(const T m00, const T m01,
				 const T m10, const T m11)
	{
		m[0][0]=m00; m[0][1]=m01; m[0][2]=0; m[0][3]=0;
		m[1][0]=m10; m[1][1]=m11; m[1][2]=0; m[1][3]=0;
		m[2][0]=  0; m[2][1]=  0; m[2][2]=1; m[2][3]=0;
		m[3][0]=  0; m[3][1]=  0; m[3][2]=0; m[3][3]=1;
	}

	INLINE Tmat4(const T m00, const T m01, const T m02,
				 const T m10, const T m11, const T m12,
				 const T m20, const T m21, const T m22)
	{
		m[0][0]=m00; m[0][1]=m01; m[0][2]=m02; m[0][3]=0;
		m[1][0]=m10; m[1][1]=m11; m[1][2]=m12; m[1][3]=0;
		m[2][0]=m20; m[2][1]=m21; m[2][2]=m22; m[2][3]=0;
		m[3][0]=  0; m[3][1]=  0; m[3][2]=  0; m[3][3]=1;
	}

	INLINE Tmat4(const T m00, const T m01, const T m02, const T m03,
				 const T m10, const T m11, const T m12, const T m13,
				 const T m20, const T m21, const T m22, const T m23,
				 const T m30, const T m31, const T m32, const T m33)
	{
		m[0][0]=m00; m[0][1]=m01; m[0][2]=m02; m[0][3]=m03;
		m[1][0]=m10; m[1][1]=m11; m[1][2]=m12; m[1][3]=m13;
		m[2][0]=m20; m[2][1]=m21; m[2][2]=m22; m[2][3]=m23;
		m[3][0]=m30; m[3][1]=m31; m[3][2]=m32; m[3][3]=m33;
	}


/*	INLINE Tvec4<T> &operator[](const int i) {return m[i];}
	INLINE const Tvec4<T> &operator[](const int i) const {return m[i];}
	INLINE operator T* () {return &m[0].x;}
	INLINE operator const T* () const {return &m[0].x;}

	INLINE HMbool operator==(const Tmat4 &M) const
	{
		for (int i=0;i<16;i++)
			if (notEqual(e[i],M.e[i])) return HMfalse;

		return HMtrue;
	}
	INLINE HMbool operator!=(const Tmat4 &M) const
	{
		for (int i=0;i<16;i++)
			if (notEqual(e[i],M.e[i])) return HMtrue;

		return HMfalse;
	}*/

	//matrix=matrix
	INLINE const Tmat4 &operator=(const Tmat4 &M)
	{
#pragma ivdep
		for (int i=0;i<16;i++) e[i]=M.e[i];
		return *this;
	}

	INLINE const Tvec4<T> operator*(const Tvec4<T> &v) const
	{
//	StartRecordTime
#ifndef D3D_MAT_STYLE
		Tvec4<T> r;
		//T c0,c1,c2,c3;

//		c0=v.x;
//		c1=v.y;
//		c2=v.z;
//		c3=v.w;

#pragma ivdep
#pragma vector always
		for (int i=0;i<4;i++)
			r[i]=e[0*4+i]*v.x
				+e[1*4+i]*v.y
				+e[2*4+i]*v.z
				+e[3*4+i]*v.w;

		return r;
#else
/*		for (int i=0;i<4;i++)
			r[i]=e[i*4+0]*v.x
				+e[i*4+1]*v.y
				+e[i*4+2]*v.z
				+e[i*4+3]*v.w;*/
		return Tvec4<T>(v[0] * m[0][0] + v[1] * m[0][1] + v[2] * m[0][2] + v[3] * m[0][3],
						v[0] * m[1][0] + v[1] * m[1][1] + v[2] * m[1][2] + v[3] * m[1][3],
						v[0] * m[2][0] + v[1] * m[2][1] + v[2] * m[2][2] + v[3] * m[2][3],
						v[0] * m[3][0] + v[1] * m[3][1] + v[2] * m[3][2] + v[3] * m[3][3]);
#endif

//	StopRecordTime
	}

	INLINE const Tmat4 operator*(const Tmat4 &M) const
	{
//	StartRecordTime
		Tmat4 r;
		int j;
		T c0,c1,c2,c3;

#ifndef D3D_MAT_STYLE
#define column(i)					\
		c0=M.e[i*4+0];				\
		c1=M.e[i*4+1];				\
		c2=M.e[i*4+2];				\
		c3=M.e[i*4+3];				\
									\
		for (j=0;j<4;j++)			\
			r.e[i*4+j]=e[0*4+j]*c0	\
					  +e[1*4+j]*c1	\
					  +e[2*4+j]*c2	\
					  +e[3*4+j]*c3;
#else
#define column(i)					\
		c0=e[i*4+0];				\
		c1=e[i*4+1];				\
		c2=e[i*4+2];				\
		c3=e[i*4+3];				\
									\
		for (j=0;j<4;j++)			\
			r.e[i*4+j]=M.e[0*4+j]*c0\
					  +M.e[1*4+j]*c1\
					  +M.e[2*4+j]*c2\
					  +M.e[3*4+j]*c3;
#endif

#pragma ivdep
#pragma vector always
		column(0)
#pragma ivdep
#pragma vector always
		column(1)
#pragma ivdep
#pragma vector always
		column(2)
#pragma ivdep
#pragma vector always
		column(3)

#undef column
//	StopRecordTime
		return r;
	}

/*	INLINE Tmat4 &operator+=(const T a) {for (int i=0;i<16;i++) e[i]+=a; return *this;}
	INLINE Tmat4 &operator-=(const T a) {for (int i=0;i<16;i++) e[i]-=a; return *this;}
	INLINE Tmat4 &operator*=(const T a) {for (int i=0;i<16;i++) e[i]*=a; return *this;}
	INLINE Tmat4 &operator/=(const T a) {for (int i=0;i<16;i++) e[i]/=a; return *this;}
	INLINE Tmat4 &operator+=(const Tmat4 &M) {for (int i=0;i<16;i++) e[i]+=M.e[i]; return *this;}
	INLINE Tmat4 &operator-=(const Tmat4 &M) {for (int i=0;i<16;i++) e[i]-=M.e[i]; return *this;}
	INLINE Tmat4 &operator*=(const Tmat4 &M) {return *this=(*this)*M;}
	INLINE Tmat4 &operator/=(const Tmat4 &M) {for (int i=0;i<16;i++) e[i]/=M.e[i]; return *this;}

	INLINE const Tmat4 operator-() const {Tmat4 res; for (int i=0;i<16;i++) res.e[i]=-e[i]; return res;}//{return Tmat4(-m[0], -m[1], -m[2], -m[3]);}
	INLINE const Tmat4 operator++() {for (int i=0;i<16;i++) ++e[i]; return *this;}
	INLINE const Tmat4 operator--() {for (int i=0;i<16;i++) --e[i]; return *this;}
	INLINE const Tmat4 operator++(int notused) {Tmat4 t(*this); for (int i=0;i<16;i++) ++e[i]; return t;}//{return Tmat4(m[0]++, m[1]++, m[2]++, m[3]++);}
	INLINE const Tmat4 operator--(int notused) {Tmat4 t(*this); for (int i=0;i<16;i++) --e[i]; return t;}//{return Tmat4(m[0]--, m[1]--, m[2]--, m[3]--);}
*/
};

template <class T>
INLINE const Tvec4<T> operator*(const Tvec4<T> &v,const Tmat4<T> &M)
{
#ifndef D3D_MAT_STYLE
	return Tvec4<T>(v[0] * M[0][0] + v[1] * M[0][1] + v[2] * M[0][2] + v[3] * M[0][3],
					v[0] * M[1][0] + v[1] * M[1][1] + v[2] * M[1][2] + v[3] * M[1][3],
					v[0] * M[2][0] + v[1] * M[2][1] + v[2] * M[2][2] + v[3] * M[2][3],
					v[0] * M[3][0] + v[1] * M[3][1] + v[2] * M[3][2] + v[3] * M[3][3]);
#else
	Tvec4<T> r;
#pragma ivdep
#pragma vector always
	for (int i=0;i<4;i++)
		r[i]=M.e[0*4+i]*v.x
			+M.e[1*4+i]*v.y
			+M.e[2*4+i]*v.z
			+M.e[3*4+i]*v.w;
	return r;
/*	return Tvec4<T>(v[0] * M[0][0] + v[1] * M[1][0] + v[2] * M[2][0] + v[3] * M[3][0],
					v[0] * M[0][1] + v[1] * M[1][1] + v[2] * M[2][1] + v[3] * M[3][1],
					v[0] * M[0][2] + v[1] * M[1][2] + v[2] * M[2][2] + v[3] * M[3][2],
					v[0] * M[0][3] + v[1] * M[1][3] + v[2] * M[2][3] + v[3] * M[3][3]);*/
#endif
}

operators_xxx(4)

#undef operator_xxx
#undef operators_xxx

template <class T>
INLINE Tmat2<T>::Tmat2(const Tmat4<T> &M)
{
	(*this)[0]=Tvec2<T>(M[0]);
	(*this)[1]=Tvec2<T>(M[1]);
}

template <class T>
INLINE Tmat3<T>::Tmat3(const Tmat4<T> &M)
{
	(*this)[0]=Tvec3<T>(M[0]);
	(*this)[1]=Tvec3<T>(M[1]);
	(*this)[2]=Tvec3<T>(M[2]);
}


#define typedef_matrices(dimension)							\
typedef Tmat##dimension<HMfloat>		mat##dimension;		\
typedef Tmat##dimension<HMint>			imat##dimension;	\
typedef Tmat##dimension<HMdouble>		dmat##dimension;	\
typedef Tmat##dimension<HMlong_double>	ldmat##dimension;

typedef_matrices(2)
typedef_matrices(3)
typedef_matrices(4)

#undef typedef_matrices

#endif //HM_MAT_H
