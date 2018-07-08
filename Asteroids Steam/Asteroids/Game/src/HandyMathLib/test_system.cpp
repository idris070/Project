//#define D3D_MAT_STYLE
//#define QUAT_MUL_REVERSE_ORDER
//#define DISABLE_VECTORIZATION
//#define MAKE_TESTS

#include "hm.h"

#include <iostream>
#include <iomanip>

#ifdef MAKE_TESTS

#define TEST(A,B) do A; while(false)

#define MAKE(A) cout<<"TEST("<<#A<<','<<(A)<<");\n"

#include <string>

std::string typeLetter(HMint) {return "i";}
std::string typeLetter(HMfloat) {return "";}
std::string typeLetter(HMdouble) {return "d";}
std::string typeLetter(HMlong_double) {return "ld";}
int typePrecision(HMint) {return 0;}
int typePrecision(HMfloat) {return 6;}
int typePrecision(HMdouble) {return 9;}
int typePrecision(HMlong_double) {return 9;}

#define operator_shift_ostream_vec(d)											\
template <class T> std::ostream &operator<<(std::ostream &s,const Tvec##d<T> &v)\
{																				\
	s<<typeLetter(v.x)<<"vec"#d"(";												\
	s.precision(typePrecision(v.x));											\
	for (int i=0;i<d;i++)														\
		s<<v[i]<<(i<(d-1) ? ',' : ')');											\
	return s;																	\
}
operator_shift_ostream_vec(2)
operator_shift_ostream_vec(3)
operator_shift_ostream_vec(4)
#undef operator_shift_ostream_vec

#define operator_shift_ostream_mat(d)											\
template <class T> std::ostream &operator<<(std::ostream &s,const Tmat##d<T> &M)\
{																				\
	s<<typeLetter(M[0][0])<<"mat"#d"(";											\
	s.precision(typePrecision(M[0][0]));										\
	for (int i=0;i<d*d;i++)														\
		s<<M.e[i]<<(i<(d*d-1) ? ',' : ')');										\
	return s;																	\
}
operator_shift_ostream_mat(2)
operator_shift_ostream_mat(3)
operator_shift_ostream_mat(4)
#undef operator_shift_ostream_mat

template <class T> std::ostream &operator<<(std::ostream &s,const Tquat<T> &q)
{
	s<<typeLetter(q.x)<<"quat(";
	s.precision(typePrecision(q.x));
	for (int i=0;i<4;i++)
		s<<(&q.x)[i]<<(i<3 ? ',' : ')');
	return s;
}

template <class T> std::ostream &operator<<(std::ostream &s,const Taabb<T> &b)
{
	return s<<typeLetter(b.min.x)<<(dimension(b.min)==3 ? "aabb(" : "rect(")<<b.min<<','<<b.max<<')';
}

#else

#define TEST(A,B) do {											\
totalTests++;													\
cout<<left<<setw(75)<<#A;										\
if ((A)==(B)) cout<<"- ok\n";									\
else {totalBugs++; cout<<"- BUG!!! (line: "<<__LINE__<<")\n";}	\
} while (false)

#define MAKE(A) do A; while(false)

#endif

int main()
{
	using std::cout;
	using std::endl;
	using std::setw;
	using std::left;
	unsigned totalBugs=0,totalTests=0;

	cout<<  "=================Testing \"vec.h\"==================\n\n";

	bvec2 bv2;
	TEST(bvec2(bvec2(HMtrue,HMfalse)),bvec2(HMtrue,HMfalse));
	TEST(bvec2(HMfalse),bvec2(HMfalse,HMfalse));
	TEST(bvec2(HMfalse,HMtrue)[1],HMtrue);
	TEST(bvec2(HMfalse,HMtrue)!=bvec2(HMtrue,HMfalse),HMtrue);
	TEST(bv2=bvec2(HMtrue,HMtrue),bvec2(HMtrue,HMtrue));
	vec2 v2;
	TEST(vec2(vec2(-2,1)),vec2(-2,1));
	TEST(vec2(12),vec2(12,12));
	TEST(vec2(3,-1)[1],-1);
	TEST(vec2(2,0)!=vec2(0,2),HMtrue);
	TEST(v2=vec2(5,8),vec2(5,8));
	TEST(ldvec2(10,-20)+2.0l,ldvec2(12,-18));
	TEST(  vec2(10,-20)-2.0f,vec2(8,-22));
	TEST( dvec2(10,-20)*-2.0,dvec2(-20,40));
	TEST( ivec2(10,-20)/2   ,ivec2(5,-10));
	TEST(ldvec2(1,-2)+ldvec2(-2,1),ldvec2(-1,-1));
	TEST(  vec2(1,-2)-  vec2(-2,1),vec2(3,-3));
	TEST( dvec2(1,-2)* dvec2(-2,1),dvec2(-2,-2));
	TEST( ivec2(1,-2)/ ivec2(-2,1),ivec2(0,-2));
	TEST(ldvec2(10,-20)+=2.0l,ldvec2(12,-18));
	TEST(  vec2(10,-20)-=2.0f,vec2(8,-22));
	TEST( dvec2(10,-20)*=-2.0,dvec2(-20,40));
	TEST( ivec2(10,-20)/=2   ,ivec2(5,-10));
	TEST(ldvec2(1,-2)+=ldvec2(-2,1),ldvec2(-1,-1));
	TEST(  vec2(1,-2)-=  vec2(-2,1),vec2(3,-3));
	TEST( dvec2(1,-2)*= dvec2(-2,1),dvec2(-2,-2));
	TEST( ivec2(1,-2)/= ivec2(-2,1),ivec2(0,-2));
	TEST(2.0l+ldvec2(10,-20),ldvec2(12,-18));
	TEST(2.0f-  vec2(10,-20),vec2(-8,22));
	TEST(-2.0* dvec2(10,-20),dvec2(-20,40));
	TEST(200/  ivec2(10,-20),ivec2(20,-10));
	TEST(-dvec2(1,-2),dvec2(-1,2));
	TEST(++ldvec2(1,-2),ldvec2(2,-1));
	TEST(--  vec2(1,-2),  vec2(0,-3));
	v2=vec2(5,8);
	TEST((v2++)==vec2(5,8) && v2==vec2(6,9),true);
	v2=vec2(5,8);
	TEST((v2--)==vec2(5,8) && v2==vec2(4,7),true);

	bvec3 bv3;
	TEST(bvec3(bvec3(HMtrue,HMfalse,HMfalse)),bvec3(HMtrue,HMfalse,HMfalse));
	TEST(bvec3(HMtrue),bvec3(HMtrue,HMtrue,HMtrue));
	TEST(bvec3(HMtrue,HMfalse,HMtrue)[2],HMtrue);
	TEST(bvec3(HMtrue,HMfalse,HMtrue)!=bvec3(HMtrue,HMfalse,HMtrue),HMfalse);
	TEST(bv3=bvec3(HMtrue,HMtrue,HMfalse),bvec3(HMtrue,HMtrue,HMfalse));
	TEST(bvec2(bvec3(HMfalse,HMtrue,HMfalse)),bvec2(HMfalse,HMtrue));
	vec3 v3;
	TEST(vec3(vec3(-2,1,4)),vec3(-2,1,4));
	TEST(vec3(12),vec3(12,12,12));
	TEST((v3=vec3(3,-1,4))[2],4);
	TEST(v3.st(),vec2(3,-1));
	TEST(v3.gb(),vec2(-1,4));
	TEST(vec3(2,0,-1)!=vec3(2,0,0),HMtrue);
	TEST(v3=vec3(5,8,6),vec3(5,8,6));
	TEST(ldvec3(10,-20,4)+2.0l,ldvec3(12,-18,6));
	TEST(  vec3(10,-20,4)-2.0f,vec3(8,-22,2));
	TEST( dvec3(10,-20,4)*-2.0,dvec3(-20,40,-8));
	TEST( ivec3(10,-20,4)/2   ,ivec3(5,-10,2));
	TEST(ldvec3(1,-2,3)+ldvec3(-2,1,0),ldvec3(-1,-1,3));
	TEST(  vec3(1,-2,3)-  vec3(-2,1,0),vec3(3,-3,3));
	TEST( dvec3(1,-2,3)* dvec3(-2,1,0),dvec3(-2,-2,0));
	TEST( ivec3(1,-2,0)/ ivec3(-2,1,3),ivec3(0,-2,0));
	TEST(ldvec3(10,-20,4)+=2.0l,ldvec3(12,-18,6));
	TEST(  vec3(10,-20,4)-=2.0f,vec3(8,-22,2));
	TEST( dvec3(10,-20,4)*=-2.0,dvec3(-20,40,-8));
	TEST( ivec3(10,-20,4)/=2   ,ivec3(5,-10,2));
	TEST(ldvec3(1,-2,3)+=ldvec3(-2,1,0),ldvec3(-1,-1,3));
	TEST(  vec3(1,-2,3)-=  vec3(-2,1,0),vec3(3,-3,3));
	TEST( dvec3(1,-2,3)*= dvec3(-2,1,0),dvec3(-2,-2,0));
	TEST( ivec3(1,-2,0)/= ivec3(-2,1,3),ivec3(0,-2,0));
	TEST(2.0l+ldvec3(10,-20,4),ldvec3(12,-18,6));
	TEST(2.0f-  vec3(10,-20,4),vec3(-8,22,-2));
	TEST(-2.0* dvec3(10,-20,4),dvec3(-20,40,-8));
	TEST(200/  ivec3(10,-20,4),ivec3(20,-10,50));
	TEST(-dvec3(1,-2,-3),dvec3(-1,2,3));
	TEST(++ldvec3(1,-2,-3),ldvec3(2,-1,-2));
	TEST(--  vec3(1,-2,-3),  vec3(0,-3,-4));
	v3=vec3(5,8,6);
	TEST((v3++)==vec3(5,8,6) && v3==vec3(6,9,7),true);
	v3=vec3(5,8,6);
	TEST((v3--)==vec3(5,8,6) && v3==vec3(4,7,5),true);
	TEST(vec2(vec3(1,2,3)),vec2(1,2));

	bvec4 bv4;
	TEST(bvec4(bvec4(HMtrue,HMfalse,HMfalse,HMtrue)),bvec4(HMtrue,HMfalse,HMfalse,HMtrue));
	TEST(bvec4(HMtrue),bvec4(HMtrue,HMtrue,HMtrue,HMtrue));
	TEST(bvec4(HMtrue,HMfalse,HMtrue,HMfalse)[2],HMtrue);
	TEST(bvec4(HMtrue,HMfalse,HMtrue,HMtrue)!=bvec4(HMtrue,HMfalse,HMtrue,HMfalse),HMtrue);
	TEST(bv4=bvec4(HMtrue,HMtrue,HMfalse,HMfalse),bvec4(HMtrue,HMtrue,HMfalse,HMfalse));
	TEST(bvec2(bvec4(HMfalse,HMtrue,HMfalse,HMtrue)),bvec2(HMfalse,HMtrue));
	TEST(bvec3(bvec4(HMfalse,HMtrue,HMfalse,HMtrue)),bvec3(HMfalse,HMtrue,HMfalse));
	vec4 v4;
	TEST(vec4(vec4(-2,1,4,3)),vec4(-2,1,4,3));
	TEST(vec4(12),vec4(12,12,12,12));
	TEST((v4=vec4(3,-1,4,5))[2],4);
	TEST(vec4(2,0,-1,-2)!=vec4(0,2,-1,-2),HMtrue);
	TEST(v4=vec4(5,8,6,-7),vec4(5,8,6,-7));
	TEST(v4.rgb(),vec3(5,8,6));
	TEST(v4.yzw(),vec3(8,6,-7));
	TEST(v4.xy(),vec2(5,8));
	TEST(v4.tp(),vec2(8,6));
	TEST(v4.ba(),vec2(6,-7));
	TEST(ldvec4(10,-20,4,8)+2.0l,ldvec4(12,-18,6,10));
	TEST(  vec4(10,-20,4,8)-2.0f,vec4(8,-22,2,6));
	TEST( dvec4(10,-20,4,8)*-2.0,dvec4(-20,40,-8,-16));
	TEST( ivec4(10,-20,4,8)/2   ,ivec4(5,-10,2,4));
	TEST(ldvec4(1,-2,3,4)+ldvec4(-2,1,0,-1),ldvec4(-1,-1,3,3));
	TEST(  vec4(1,-2,3,4)-  vec4(-2,1,0,-1),vec4(3,-3,3,5));
	TEST( dvec4(1,-2,3,4)* dvec4(-2,1,0,-1),dvec4(-2,-2,0,-4));
	TEST( ivec4(1,-2,0,4)/ ivec4(-2,1,3,-1),ivec4(0,-2,0,-4));
	TEST(ldvec4(10,-20,4,8)+=2.0l,ldvec4(12,-18,6,10));
	TEST(  vec4(10,-20,4,8)-=2.0f,vec4(8,-22,2,6));
	TEST( dvec4(10,-20,4,8)*=-2.0,dvec4(-20,40,-8,-16));
	TEST( ivec4(10,-20,4,8)/=2   ,ivec4(5,-10,2,4));
	TEST(ldvec4(1,-2,3,4)+=ldvec4(-2,1,0,-1),ldvec4(-1,-1,3,3));
	TEST(  vec4(1,-2,3,4)-=  vec4(-2,1,0,-1),vec4(3,-3,3,5));
	TEST( dvec4(1,-2,3,4)*= dvec4(-2,1,0,-1),dvec4(-2,-2,0,-4));
	TEST( ivec4(1,-2,0,4)/= ivec4(-2,1,3,-1),ivec4(0,-2,0,-4));
	TEST(2.0l+ldvec4(10,-20,4,8),ldvec4(12,-18,6,10));
	TEST(2.0f-  vec4(10,-20,4,8),vec4(-8,22,-2,-6));
	TEST(-2.0* dvec4(10,-20,4,8),dvec4(-20,40,-8,-16));
	TEST(200/  ivec4(10,-20,4,8),ivec4(20,-10,50,25));
	TEST(-dvec4(1,-2,-3,4),dvec4(-1,2,3,-4));
	TEST(++ldvec4(1,-2,-3,4),ldvec4(2,-1,-2,5));
	TEST(--  vec4(1,-2,-3,4),  vec4(0,-3,-4,3));
	v4=vec4(5,8,6,-7);
	TEST((v4++)==vec4(5,8,6,-7) && v4==vec4(6,9,7,-6),true);
	v4=vec4(5,8,6,-7);
	TEST((v4--)==vec4(5,8,6,-7) && v4==vec4(4,7,5,-8),true);
	TEST(vec2(vec4(1,2,3,4)),vec2(1,2));
	TEST(vec3(vec4(1,2,3,4)),vec3(1,2,3));

	cout<<"\n=================Testing \"mat.h\"==================\n\n";

	ldmat2 ldm2(1.1,-.2,
				-.05,.95);
	dmat2 dm2(1.1,-.2,
			-.05,.95);
	mat2 m2(1.1,-.2,
			-.05,.95);
	imat2 im2(110,-20,
			  -5,95);
	ldmat2 ldM2(1.05,-.22,
				.03,.97);
	dmat2 dM2(1.05,-.22,
			.03,.97);
	mat2 M2(1.05,-.22,
			.03,.97);
	imat2 iM2(55,-2,
			  3,47);
	TEST(imat2(imat2(110,-20,-5,95)),im2);
	TEST(mat2(12),mat2(12,0,0,12));
	TEST(equal(m2[0][1],-.2f),HMtrue);
	TEST(mat2(110,-20,-5,95)!=mat2(111,-20,-5,95),HMtrue);
	TEST(m2=mat2(1.1,-0.2,-0.05,0.95),mat2(1.1,-0.2,-0.05,0.95));
	TEST(ldm2+2.0l,ldmat2(3.1,1.8,1.95,2.95));
	TEST(m2-2.0f,mat2(-0.9,-2.2,-2.05,-1.05));
	TEST(dm2*2.0,dmat2(2.2,-0.4,-0.1,1.9));
	TEST(im2/2,imat2(55,-10,-2,47));
	TEST(ldm2+ldM2,ldmat2(2.15,-0.42,-0.02,1.92));
	TEST(m2- M2,mat2(0.05,0.02,-0.08,-0.02));
#ifndef D3D_MAT_STYLE
	TEST(dm2* dM2,dmat2(1.166,-0.419,-0.0155,0.9155));
#else
	TEST(dM2* dm2,dmat2(1.166,-0.419,-0.0155,0.9155));
#endif
	TEST(im2/ iM2,imat2(2,10,-1,2));
#ifndef D3D_MAT_STYLE
	TEST(m2*vec2(1,2),vec2(1,1.7));
#else
	TEST(vec2(1,2)*m2,vec2(1,1.7));
#endif
#ifndef D3D_MAT_STYLE
	TEST(vec2(2,1)*m2,vec2(2,0.85));
#else
	TEST(m2*vec2(2,1),vec2(2,0.85));
#endif
	TEST(2.0l+ldm2,ldmat2(3.1,1.8,1.95,2.95));
	TEST(2.0f- m2,mat2(0.9,2.2,2.05,1.05));
	TEST(2.0* dm2,dmat2(2.2,-0.4,-0.1,1.9));
	TEST(300/ im2,imat2(2,-15,-60,3));
	TEST(ldm2+=.01l,ldmat2(1.11,-0.19,-0.04,0.96));
	TEST(m2-=.01f,mat2(1.09,-0.21,-0.06,0.94));
	TEST(dm2*=1.01,dmat2(1.111,-0.202,-0.0505,0.9595));
	TEST(im2/=2,imat2(55,-10,-2,47));
	TEST(ldm2+=ldM2,ldmat2(2.16,-0.41,-0.01,1.93));
	TEST(m2-= M2,mat2(0.04,0.01,-0.09,-0.03));
#ifndef D3D_MAT_STYLE
	TEST(dm2*= dM2,dmat2(1.17766,-0.42319,-0.015655,0.924655));
#else
	TEST(dm2=dM2*dm2,dmat2(1.17766,-0.42319,-0.015655,0.924655));
#endif
	TEST(im2/= iM2,imat2(1,5,0,1));
	TEST(-dm2,dmat2(-1.17766,0.42319,0.015655,-0.924655));
	TEST(++ldm2,ldmat2(3.16,0.59,0.99,2.93));
	TEST(-- m2,mat2(-0.96,-0.99,-1.09,-1.03));
	TEST(dm2++,dmat2(1.17766,-0.42319,-0.015655,0.924655));
	TEST(dm2,dmat2(2.17766,0.57681,0.984345,1.924655));
	TEST(im2--,imat2(1,5,0,1));
	TEST(im2,imat2(0,4,-1,0));

	ldmat3 ldm3(1.1,-.2,.1,
				-.05,.95,.07,
				-.17,.18,1.2);
	dmat3 dm3(1.1,-.2,.1,
			-.05,.95,.07,
			-.17,.18,1.2);
	mat3 m3(1.1,-.2,.1,
			-.05,.95,.07,
			-.17,.18,1.2);
	imat3 im3(110,-20,10,
			  -5,95,7,
			  -17,18,120);
	ldmat3 ldM3(1.05,-.22,-.11,
				.03,.97,-.04,
				.14,.19,1.12);
	dmat3 dM3(1.05,-.22,-.11,
			.03,.97,-.04,
			.14,.19,1.12);
	mat3 M3(1.05,-.22,-.11,
			.03,.97,-.04,
			.14,.19,1.12);
	imat3 iM3(55,-2,-1,
			  3,47,-4,
			  4,9,66);
	TEST(imat3(imat3(110,-20,10,-5,95,7,-17,18,120)),im3);
	TEST(mat3(12),mat3(12,0,0,0,12,0,0,0,12));
	TEST(equal(m3[1][2],.07f),HMtrue);
	TEST(mat3(110,-20,10,-5,95,7,-17,18,120)!=mat3(110,-20,10,-5,95,7,-17,18,120),HMfalse);
	TEST(m3=mat3(1.1,-0.2,0.1,-0.05,0.95,0.07,-0.17,0.18,1.2),mat3(1.1,-0.2,0.1,-0.05,0.95,0.07,-0.17,0.18,1.2));
	TEST(ldm3+2.0l,ldmat3(3.1,1.8,2.1,1.95,2.95,2.07,1.83,2.18,3.2));
	TEST(m3-2.0f,mat3(-0.9,-2.2,-1.9,-2.05,-1.05,-1.93,-2.17,-1.82,-0.8));
	TEST(dm3*2.0,dmat3(2.2,-0.4,0.2,-0.1,1.9,0.14,-0.34,0.36,2.4));
	TEST(im3/2,imat3(55,-10,5,-2,47,3,-8,9,60));
	TEST(ldm3+ldM3,ldmat3(2.15,-0.42,-0.01,-0.02,1.92,0.03,-0.03,0.37,2.32));
	TEST(m3- M3,mat3(0.05,0.02,0.21,-0.08,-0.02,0.11,-0.31,-0.01,0.08));
#ifndef D3D_MAT_STYLE
	TEST(dm3* dM3,dmat3(1.1847,-0.4388,-0.0424,-0.0087,0.9083,0.0229,-0.0459,0.3541,1.3713));
#else
	TEST(dM3* dm3,dmat3(1.1847,-0.4388,-0.0424,-0.0087,0.9083,0.0229,-0.0459,0.3541,1.3713));
#endif
	TEST(im3/ iM3,imat3(2,10,-10,-1,2,-1,-4,2,1));
#ifndef D3D_MAT_STYLE
	TEST(m3*vec3(1,2,3),vec3(0.49,2.24,3.84));
#else
	TEST(vec3(1,2,3)*m3,vec3(0.49,2.24,3.84));
#endif
#ifndef D3D_MAT_STYLE
	TEST(vec3(4,3,2)*m3,vec3(4,2.79,2.26));
#else
	TEST(m3*vec3(4,3,2),vec3(4,2.79,2.26));
#endif
	TEST(2.0l+ldm3,ldmat3(3.1,1.8,2.1,1.95,2.95,2.07,1.83,2.18,3.2));
	TEST(2.0f- m3,mat3(0.9,2.2,1.9,2.05,1.05,1.93,2.17,1.82,0.8));
	TEST(2.0* dm3,dmat3(2.2,-0.4,0.2,-0.1,1.9,0.14,-0.34,0.36,2.4));
	TEST(300/ im3,imat3(2,-15,30,-60,3,42,-17,16,2));
	TEST(ldm3+=.01l,ldmat3(1.11,-0.19,0.11,-0.04,0.96,0.08,-0.16,0.19,1.21));
	TEST(m3-=.01f,mat3(1.09,-0.21,0.09,-0.06,0.94,0.06,-0.18,0.17,1.19));
	TEST(dm3*=1.01,dmat3(1.111,-0.202,0.101,-0.0505,0.9595,0.0707,-0.1717,0.1818,1.212));
	TEST(im3/=2,imat3(55,-10,5,-2,47,3,-8,9,60));
	TEST(ldm3+=ldM3,ldmat3(2.16,-0.41,0,-0.01,1.93,0.04,-0.02,0.38,2.33));
	TEST(m3-= M3,mat3(0.04,0.01,0.2,-0.09,-0.03,0.1,-0.32,-0.02,0.07));
#ifndef D3D_MAT_STYLE
	TEST(dm3*= dM3,dmat3(1.196547,-0.443188,-0.042824,-0.008787,0.917383,0.023129,-0.046359,0.357641,1.385013));
#else
	TEST(dm3=dM3*dm3,dmat3(1.196547,-0.443188,-0.042824,-0.008787,0.917383,0.023129,-0.046359,0.357641,1.385013));
#endif
	TEST(im3/= iM3,imat3(1,5,-5,0,1,0,-2,1,0));
	TEST(-dm3,dmat3(-1.196547,0.443188,0.042824,0.008787,-0.917383,-0.023129,0.046359,-0.357641,-1.385013));
	TEST(++ldm3,ldmat3(3.16,0.59,1,0.99,2.93,1.04,0.98,1.38,3.33));
	TEST(-- m3,mat3(-0.96,-0.99,-0.8,-1.09,-1.03,-0.9,-1.32,-1.02,-0.93));
	TEST(dm3++,dmat3(1.196547,-0.443188,-0.042824,-0.008787,0.917383,0.023129,-0.046359,0.357641,1.385013));
	TEST(dm3,dmat3(2.196547,0.556812,0.957176,0.991213,1.917383,1.023129,0.953641,1.357641,2.385013));
	TEST(im3--,imat3(1,5,-5,0,1,0,-2,1,0));
	TEST(im3,imat3(0,4,-6,-1,0,-1,-3,0,-1));
//	TEST(mat2(m3),mat2(1.1,-0.2,-0.05,0.95));
//	TEST(mat3(m2),mat3(1.1,-0.2,0,-0.05,0.95,0,0,0,1));

	ldmat4 ldm4(1.1,-.2,.1,.15,
				-.05,.95,.07,.2,
				-.17,.18,1.2,-.1,
				.06,-.12,-.13,.9);
	dmat4 dm4(1.1,-.2,.1,.15,
			-.05,.95,.07,.2,
			-.17,.18,1.2,-.1,
			.06,-.12,-.13,.9);
	mat4 m4(1.1,-.2,.1,.15,
			-.05,.95,.07,.2,
			-.17,.18,1.2,-.1,
			.06,-.12,-.13,.9);
	imat4 im4(110,-20,10,15,
			  -5,95,7,20,
			  -17,18,120,-10,
			  6,-12,-13,90);
	ldmat4 ldM4(1.05,-.22,-.11,-.25,
				.03,.97,-.04,.02,
				.14,.19,1.12,-.21,
				-.16,.1,-.01,.94);
	dmat4 dM4(1.05,-.22,-.11,-.25,
			.03,.97,-.04,.02,
			.14,.19,1.12,-.21,
			-.16,.1,-.01,.94);
	mat4 M4(1.05,-.22,-.11,-.25,
			.03,.97,-.04,.02,
			.14,.19,1.12,-.21,
			-.16,.1,-.01,.94);
	imat4 iM4(55,-2,-1,-5,
			  3,47,-4,2,
			  4,9,66,-2,
			  -6,1,-1,48);
	TEST(imat4(imat4(110,-20,10,15,-5,95,7,20,-17,18,120,-10,6,-12,-13,90)),im4);
	TEST(mat4(12),mat4(12,0,0,0,0,12,0,0,0,0,12,0,0,0,0,12));
	TEST(m4[3][2],-.13f);
	TEST(mat4(110,-20,10,15,-5,95,7,20,-17,18,120,-10,6,-12,-13,90)!=mat4(110,-20,10,15,-5,95,7,20,-17,18,120,-10,7,-12,-13,90),HMtrue);
	TEST(im4=imat4(110,-20,10,15,-5,95,7,20,-17,18,120,-10,6,-12,-13,90),imat4(110,-20,10,15,-5,95,7,20,-17,18,120,-10,6,-12,-13,90));
	TEST(ldm4+2.0l,ldmat4(3.1,1.8,2.1,2.15,1.95,2.95,2.07,2.2,1.83,2.18,3.2,1.9,2.06,1.88,1.87,2.9));
	TEST(m4-2.0f,mat4(-0.9,-2.2,-1.9,-1.85,-2.05,-1.05,-1.93,-1.8,-2.17,-1.82,-0.8,-2.1,-1.94,-2.12,-2.13,-1.1));
	TEST(dm4*2.0,dmat4(2.2,-0.4,0.2,0.3,-0.1,1.9,0.14,0.4,-0.34,0.36,2.4,-0.2,0.12,-0.24,-0.26,1.8));
	TEST(im4/2,imat4(55,-10,5,7,-2,47,3,10,-8,9,60,-5,3,-6,-6,45));
	TEST(ldm4+ldM4,ldmat4(2.15,-0.42,-0.01,-0.1,-0.02,1.92,0.03,0.22,-0.03,0.37,2.32,-0.31,-0.1,-0.02,-0.14,1.84));
	TEST(m4- M4,mat4(0.05,0.02,0.21,0.4,-0.08,-0.02,0.11,0.18,-0.31,-0.01,0.08,0.11,0.22,-0.22,-0.12,-0.04));
#ifndef D3D_MAT_STYLE
	TEST(dm4* dM4,dmat4(1.1697,-0.4088,-0.0099,-0.1005,-0.0075,0.9059,0.0203,0.2205,-0.0585,0.3793,1.3986,-0.242,-0.1229,0.0124,-0.1432,0.843));
#else
	TEST(dM4* dm4,dmat4(1.1697,-0.4088,-0.0099,-0.1005,-0.0075,0.9059,0.0203,0.2205,-0.0585,0.3793,1.3986,-0.242,-0.1229,0.0124,-0.1432,0.843));
#endif
	TEST(im4/ iM4,imat4(2,10,-10,-3,-1,2,-1,10,-4,2,1,5,-1,-12,13,1));
#ifndef D3D_MAT_STYLE
	TEST(m4*vec4(1,2,3,4),vec4(0.73,1.76,3.32,3.85));
#else
	TEST(vec4(1,2,3,4)*m4,vec4(0.73,1.76,3.32,3.85));
#endif
#ifndef D3D_MAT_STYLE
	TEST(vec4(4,3,2,1)*m4,vec4(4.15,2.99,2.16,0.52));
#else
	TEST(m4*vec4(4,3,2,1),vec4(4.15,2.99,2.16,0.52));
#endif
	TEST(2.0l+ldm4,ldmat4(3.1,1.8,2.1,2.15,1.95,2.95,2.07,2.2,1.83,2.18,3.2,1.9,2.06,1.88,1.87,2.9));
	TEST(2.0f- m4,mat4(0.9,2.2,1.9,1.85,2.05,1.05,1.93,1.8,2.17,1.82,0.8,2.1,1.94,2.12,2.13,1.1));
	TEST(2.0* dm4,dmat4(2.2,-0.4,0.2,0.3,-0.1,1.9,0.14,0.4,-0.34,0.36,2.4,-0.2,0.12,-0.24,-0.26,1.8));
	TEST(300/ im4,imat4(2,-15,30,20,-60,3,42,15,-17,16,2,-30,50,-25,-23,3));
	TEST(ldm4+=.01l,ldmat4(1.11,-0.19,0.11,0.16,-0.04,0.96,0.08,0.21,-0.16,0.19,1.21,-0.09,0.07,-0.11,-0.12,0.91));
	TEST(m4-=.01f,mat4(1.09,-0.21,0.09,0.14,-0.06,0.94,0.06,0.19,-0.18,0.17,1.19,-0.11,0.05,-0.13,-0.14,0.89));
	TEST(dm4*=1.01,dmat4(1.111,-0.202,0.101,0.1515,-0.0505,0.9595,0.0707,0.202,-0.1717,0.1818,1.212,-0.101,0.0606,-0.1212,-0.1313,0.909));
	TEST(im4/=2,imat4(55,-10,5,7,-2,47,3,10,-8,9,60,-5,3,-6,-6,45));
	TEST(ldm4+=ldM4,ldmat4(2.16,-0.41,0,-0.09,-0.01,1.93,0.04,0.23,-0.02,0.38,2.33,-0.3,-0.09,-0.01,-0.13,1.85));
	TEST(m4-= M4,mat4(0.04,0.01,0.2,0.39,-0.09,-0.03,0.1,0.17,-0.32,-0.02,0.07,0.1,0.21,-0.23,-0.13,-0.05));
#ifndef D3D_MAT_STYLE
	TEST(dm4*= dM4,dmat4(1.181397,-0.412888,-0.009999,-0.101505,-0.007575,0.914959,0.020503,0.222705,-0.059085,0.383093,1.412586,-0.24442,-0.124129,0.012524,-0.144632,0.85143));
#else
	TEST(dm4=dM4*dm4,dmat4(1.181397,-0.412888,-0.009999,-0.101505,-0.007575,0.914959,0.020503,0.222705,-0.059085,0.383093,1.412586,-0.24442,-0.124129,0.012524,-0.144632,0.85143));
#endif
	TEST(im4/= iM4,imat4(1,5,-5,-1,0,1,0,5,-2,1,0,2,0,-6,6,0));
	TEST(-dm4,dmat4(-1.181397,0.412888,0.009999,0.101505,0.007575,-0.914959,-0.020503,-0.222705,0.059085,-0.383093,-1.412586,0.24442,0.124129,-0.012524,0.144632,-0.85143));
	TEST(++ldm4,ldmat4(3.16,0.59,1,0.91,0.99,2.93,1.04,1.23,0.98,1.38,3.33,0.7,0.91,0.99,0.87,2.85));
	TEST(-- m4,mat4(-0.96,-0.99,-0.8,-0.61,-1.09,-1.03,-0.9,-0.83,-1.32,-1.02,-0.93,-0.9,-0.79,-1.23,-1.13,-1.05));
	TEST(dm4++,dmat4(1.181397,-0.412888,-0.009999,-0.101505,-0.007575,0.914959,0.020503,0.222705,-0.059085,0.383093,1.412586,-0.24442,-0.124129,0.012524,-0.144632,0.85143));
	TEST(dm4,dmat4(2.181397,0.587112,0.990001,0.898495,0.992425,1.914959,1.020503,1.222705,0.940915,1.383093,2.412586,0.75558,0.875871,1.012524,0.855368,1.85143));
	TEST(im4--,imat4(1,5,-5,-1,0,1,0,5,-2,1,0,2,0,-6,6,0));
	TEST(im4,imat4(0,4,-6,-2,-1,0,-1,4,-3,0,-1,1,-1,-7,5,-1));
//	TEST(dmat4(dm2),dmat4(1.1,-0.2,0,0,-0.05,0.95,0,0,0,0,1,0,0,0,0,1));
//	TEST(imat4(im3),imat4(110,-20,10,0,-5,95,7,0,-17,18,120,0,0,0,0,1));
//	TEST(imat3(im4),imat3(110,-20,10,-5,95,7,-17,18,120));
//	TEST(imat2(im4),imat2(110,-20,-5,95));

	cout<<"\n=================Testing \"quat.h\"=================\n\n";

	quat q=quat(vec3(1,1,1),radians(75.0f));
	TEST(q==quat(0.35147,0.35147,0.35147,0.79335),HMtrue);
	TEST(q!=quat(0.35247,0.35147,0.35147,0.79335),HMtrue);
	TEST(q=quat(0.35147,0.35147,0.35147,0.79335),quat(0.35147,0.35147,0.35147,0.79335));
	TEST(quat(m3=mat3(q),HMtrue),q);
	TEST(quat(m4=mat4(q),HMtrue),q);
	TEST(normalize(quat(mat3(q),HMfalse)),q);
	TEST(normalize(quat(mat4(q),HMfalse)),q);
	TEST(quat(mat3(q,HMtrue),HMtrue),q);
	TEST(quat(mat4(q,HMtrue),HMtrue),q);
	TEST(q*2.0f,quat(0.70294,0.70294,0.70294,1.5867));
	TEST(q/2.0f,quat(0.17573,0.17573,0.17573,0.39668));
	TEST(2.0f*q,quat(0.70294,0.70294,0.70294,1.5867));
	quat Q=quat(vec3(1,2,3),radians(145.0f));
	TEST(q+Q,quat(0.60636,0.86125,1.1161,1.0941));
	TEST(q-Q,quat(0.096577,-0.15831,-0.41321,0.49265));
#ifndef QUAT_MUL_REVERSE_ORDER
	TEST(q*Q,quat(0.39749,0.33095,0.80193,-0.29895));
#else
	TEST(Q*q,quat(0.39749,0.33095,0.80193,-0.29895));
#endif
	TEST(q*=1.5f,quat(0.5272,0.5272,0.5272,1.19));
	TEST(Q/=1.5f,quat(0.16993,0.33986,0.50978,0.20047));
	v3=vec3(3,2,1);
#ifndef QUAT_MUL_REVERSE_ORDER
	TEST(q*v3,vec3(1.7011,3.1154,1.1835));
#else
	TEST(v3*q,vec3(1.7011,3.1154,1.1835));
#endif
	TEST(q+=Q,quat(0.69713,0.86706,1.037,1.3905));
	TEST(Q-=q,quat(-0.5272,-0.5272,-0.5272,-1.19));
#ifndef QUAT_MUL_REVERSE_ORDER
	TEST(q*=Q,quat(-1.4731,-1.9441,-1.8775,-0.28339));
#else
	TEST(q=Q*q,quat(-1.4731,-1.9441,-1.8775,-0.28339));
#endif
	TEST(-q,quat(1.4731,1.9441,1.8775,0.28339));

	cout<<"\n=================Testing \"aabb.h\"=================\n\n";

	aabb b;
	vec3 vert[3]={vec3(2,1,-1),vec3(4,2,3),vec3(3,0,-2)};
	TEST(aabb(2.5),aabb(vec3(-2.5,-2.5,-2.5),vec3(2.5,2.5,2.5)));
	TEST(aabb(-.5,3),aabb(vec3(-0.5,-0.5,-0.5),vec3(3,3,3)));
	TEST(b=aabb(3,vert),aabb(vec3(2,0,-2),vec3(4,2,3)));
	TEST(b+vec3(3,3,3),aabb(vec3(2,0,-2),vec3(4,3,3)));
	TEST(vec3(3,3,3)+b,aabb(vec3(2,0,-2),vec3(4,3,3)));
	TEST(aabb(0)+vec3(3,3,3),aabb(vec3(3,3,3),vec3(3,3,3)));
	TEST(vec3(3,3,3)+aabb(0),aabb(vec3(3,3,3),vec3(3,3,3)));
	TEST(aabb(0)+b,aabb(vec3(2,0,-2),vec3(4,2,3)));
	TEST(b+aabb(1),aabb(vec3(-1,-1,-2),vec3(4,2,3)));
	irect ir=irect(ivec2(-3,-2),ivec2(-1,-1));
	TEST(ir*irect(0),irect(ivec2(2147483647,2147483647),ivec2(-2147483647,-2147483647)));
	TEST(ir*irect(1),irect(ivec2(2147483647,2147483647),ivec2(-2147483647,-2147483647)));
	TEST(ir*irect(2),irect(ivec2(-2,-2),ivec2(-1,-1)));
	TEST(ir*=irect(2),irect(ivec2(-2,-2),ivec2(-1,-1)));
	TEST(ir*=irect(1),irect(ivec2(2147483647,2147483647),ivec2(-2147483647,-2147483647)));
	TEST(ir+=ivec2(2,3),irect(ivec2(2,3),ivec2(2,3)));
	TEST(ir+=irect(0),irect(ivec2(2,3),ivec2(2,3)));
	TEST(ir+=irect(1),irect(ivec2(-1,-1),ivec2(2,3)));

	cout<<"\n=================Testing \"func.h\"=================\n\n";

	v2=vec2(1,2);
	TEST(vec_cast<ivec2>(v2),ivec2(1,2));
	ivec3 iv3=ivec3(1,2,3);
	TEST(vec_cast<ldvec3>(iv3),ldvec3(1,2,3));
	dvec4 dv4=dvec4(1,2,3,4);
	TEST(vec_cast<vec4>(dv4),vec4(1,2,3,4));
	bv2=bvec2(HMfalse,HMtrue);
	bv3=bvec3(HMfalse,HMtrue,HMfalse);
	bv4=bvec4(HMfalse,HMtrue,HMfalse,HMtrue);
	TEST(vec_cast<ldvec2>(bv2),ldvec2(0,1));
	TEST(vec_cast<vec3>(bv3),vec3(0,1,0));
	TEST(vec_cast<ivec4>(bv4),ivec4(0,1,0,1));
	TEST(mat_cast<ldmat2>(mat2(1.5)),ldmat2(1.5));
	TEST(mat_cast<imat3>(dmat3(2.7)),imat3(2));
	TEST(mat_cast<mat4>(imat4(-7)),mat4(-7));
	TEST(quat_cast<dquat>(quat(4,-2,1,3)),dquat(4,-2,1,3));
	TEST(equal(radians(180.0f),3.14159f),HMtrue);
	TEST(equal(degrees(1.57),89.95437383553),HMtrue);
	TEST(inversesqrt(4.0),0.5);
	rseed(123);
	TEST(random(20)!=random(20) && random(20)!=random(20),true);
	TEST(random(2,20)!=random(2,20) && random(2,20)!=random(2,20),true);
	TEST(random(10.0)!=random(10.0) && random(10.0)!=random(10.0),true);
	TEST(random(5.0,10.0)!=random(5.0,10.0) && random(5.0,10.0)!=random(5.0,10.0),true);
	TEST(square(-23),529);
	TEST(cube(-12.0l),-1728.0l);
	TEST(equal(sign(-.8l),-1.l),HMtrue);
	TEST(equal(sign(0.01),1.),HMtrue);
	TEST(equal(sign(0.f),0.f),HMtrue);
	TEST(equal(fract(-.9f),0.1f),HMtrue);
	TEST(mod(400.0,360.0),40);
	TEST(wrap(-40.0f,360.0f),320);
	TEST(wrap(220,100,200),120);
	TEST(min(3.0,-2.0),-2);
	TEST(max(8,10),10);
	TEST(min(vec3(1,2,3)),1);
	TEST(max(ivec3(1,2,3)),3);
	TEST(min(dvec3(1,2,3),dvec3(3,2,1)),dvec3(1,2,1));
	TEST(max(ldvec3(1,2,3),ldvec3(3,2,1)),ldvec3(3,2,3));
	TEST(clamp(-1,4,6),4);
	TEST(clamp(5.0,4.0,6.0),5);
	TEST(clamp(7.0f,4.0f,6.0f),6);
	TEST(mix(-1,7,.25),1);
	TEST(lerp(vec2(1,3),vec2(3,0),.5),vec2(2,1.5));
	TEST(lerp(vec2(1,3),vec2(3,0),.75),vec2(2.5,0.75));
	TEST(cerp(vec2(1,3),vec2(3,0),.75),vec2(2.70711,0.43934));
	TEST(herp3(vec2(1,3),vec2(3,0),.75),vec2(2.6875,0.46875));
	TEST(herp5(vec2(1,3),vec2(3,0),.75),vec2(2.79297,0.310547));
	TEST(step(1,2),1);
	TEST(step(3,3),1);
	TEST(step(4,0),0);
	TEST(equal(smoothstep(10.0,20.0,12.0),0.104),HMtrue);
	TEST(equal(smoothstep(10.0,20.0,2.0),0.0),HMtrue);
	TEST(equal(smoothstep(10.0,20.0,22.0),1.0),HMtrue);
	TEST(equal(dot(vec2(.5f,3.f),vec2(2.f,.5f)),2.5f),HMtrue);
	TEST(equal(sqlen(dvec3(1,2,3)),14.0),HMtrue);
	TEST(equal(length(ldvec2(3,4)),5.0l),HMtrue);
	TEST(equal(distance(vec4(1,2,3,4),vec4(4,3,2,1)),4.47214f),HMtrue);
	TEST(cross(ivec3(1,0,4),ivec3(-1,3,2)),ivec3(-12,-6,3));
	TEST(normalize(vec4(2,2,0,1)),vec4(0.66667,0.66667,0,0.33333));
	TEST(isZero(dot(perp(vec2(1,0)),vec2(1,0))),HMtrue);
	TEST(isZero(dot(perp(vec2(0,-2)),vec2(0,-2))),HMtrue);
	TEST(isZero(dot(perp(vec2(1.5,3)),vec2(1.5,3))),HMtrue);
	TEST(isZero(dot(perp(vec3(-1,0,0)),vec3(-1,0,0))),HMtrue);
	TEST(isZero(dot(perp(vec3(0,1,0)),vec3(0,1,0))),HMtrue);
	TEST(isZero(dot(perp(vec3(0,0,1)),vec3(0,0,1))),HMtrue);
	TEST(isZero(dot(perp(vec3(0,0,-2)),vec3(0,0,-2))),HMtrue);
	TEST(isZero(dot(perp(vec3(-1,1,2)),vec3(-1,1,2))),HMtrue);
	TEST(isZero(dot(perp(vec3(2,1,-3)),vec3(2,1,-3))),HMtrue);
	TEST(slerp(vec3(0,1,1),vec3(2,0,0),.5f),vec3(-1.4142,-4.3711e-008,-4.3711e-008));
	TEST(faceforward(vec3(0,0,1),vec3(-1,-1,-1),vec3(1,2,3)),vec3(0,0,1));
	TEST(reflect(vec2(2,2),vec2(1,0)),vec2(-2,2));
	TEST(reflect(vec3(2,2,2),vec3(0,1,0)),vec3(2,-2,2));
	TEST(reflect(vec3(1,.5,3),unitPlane(vec3(1,1,-1),vec3(-2,.7,1))),vec3(0.46667,-0.033333,3.5333));
	TEST(reflect(vec4(2,1,6,2),unitPlane(vec3(1,1,-1),vec3(-2,.7,1))),vec4(0.93333,-0.066667,7.0667,2));
	TEST(refract(normalize(vec3(1,.5,3)),normalize(vec3(1,1,-1)),1.5f),vec3(0,0,0));
	TEST(matrixCompMult(imat2(2,3,-2,1),imat2(4,-1,-3,2)),imat2(8,-3,6,2));
	m2=mat2(1)+.1f; m2[0][1]-=.2f;
	TEST(inverse(m2)*m2,mat2(1));
	TEST(inverse(inverse(m2)),m2);
	TEST(transpose(m2),mat2(1.1,0.1,-0.1,1.1));
	TEST(transpose(transpose(m2)),m2);
	m3=mat3(1)+.1f; m3[0][1]-=.2f;
	TEST(inverse(m3)*m3,mat3(1));
	TEST(inverse(inverse(m3)),m3);
	TEST(transpose(m3),mat3(1.1,0.1,0.1,-0.1,1.1,0.1,0.1,0.1,1.1));
	TEST(transpose(transpose(m3)),m3);
	m4=mat4(1)+.1f; m4[2][1]-=.2f;
	TEST(inverse(m4)*m4,mat4(1));
	TEST(inverse(inverse(m4)),m4);
	TEST(transpose(m4),mat4(1.1,0.1,0.1,0.1,0.1,1.1,-0.1,0.1,0.1,0.1,1.1,0.1,0.1,0.1,0.1,1.1));
	TEST(transpose(transpose(m4)),m4);
	TEST(orthonormalize(m3),mat3(0.99184,-0.090167,0.090167,0.099015,0.99015,-0.099015,-0.080351,0.10713,0.99099));
	v3=vec3(1,-2,4);
	TEST(scalingMat4(vec3(7,8,9))*vec4(v3,1.0f),vec4(vec3(7,8,9)*v3,1.0f));
	TEST(scalingMat3(vec3(7,8,9))*v3,vec3(7,8,9)*v3);
#ifndef D3D_MAT_STYLE
	TEST(translationMat4(vec3(7,8,9))*vec4(v3,1.0f),vec4(vec3(7,8,9)+v3,1.0f));
	TEST(crossMat3(vec3(7,8,9))*v3,cross(vec3(7,8,9),v3));
	TEST(mat3(crossMat4(vec3(7,8,9)))*v3,cross(vec3(7,8,9),v3));
	v2=vec2(v3);
	TEST(scalingMat3(vec2(7,8))*vec3(v2,1.0f),vec3(vec2(7,8)*v2,1.0f));
	TEST(scalingMat2(vec2(7,8))*v2,vec2(7,8)*v2);
	TEST(translationMat3(vec2(7,8))*vec3(v2,1.0f),vec3(vec2(7,8)+v2,1.0f));
	TEST(rotationXMat(.345f)*v3,rotateX(.345f,v3));
	TEST(rotationYMat(.34f)*v3,rotateY(.34f,v3));
	TEST(rotationZMat(.3f)*v3,rotateZ(.3f,v3));
#else
	TEST(vec4(v3,1.0f)*translationMat4(vec3(7,8,9)),vec4(vec3(7,8,9)+v3,1.0f));
	TEST(v3*crossMat3(vec3(7,8,9)),cross(vec3(7,8,9),v3));
	TEST(v3*mat3(crossMat4(vec3(7,8,9))),cross(vec3(7,8,9),v3));
	v2=vec2(v3);
	TEST(vec3(v2,1.0f)*scalingMat3(vec2(7,8)),vec3(vec2(7,8)*v2,1.0f));
	TEST(v2*scalingMat2(vec2(7,8)),vec2(7,8)*v2);
	TEST(vec3(v2,1.0f)*translationMat3(vec2(7,8)),vec3(vec2(7,8)+v2,1.0f));
	TEST(v3*rotationXMat(.345f),rotateX(.345f,v3));
	TEST(v3*rotationYMat(.34f),rotateY(.34f,v3));
	TEST(v3*rotationZMat(.3f),rotateZ(.3f,v3));
#endif
	TEST(rotationXMat4(.345f),rotationMat4(.345f,vec3(1,0,0)));
	TEST(rotationYMat3(.34f),rotationMat3(.34f,vec3(0,1,0)));
	TEST(rotationZMat(.3f),rotationMat(.3f,vec3(0,0,1)));
	vec4 pl=unitPlane(vec3(1,2,3),vec3(3,2,1)); v4=vec4(10,20,30,40);
#ifndef D3D_MAT_STYLE
	TEST(reflect(v4,pl),reflectionMat(pl)*v4);
	TEST(project(v4,pl),projectionMat(pl)*v4);
	v3=vec3(v4);
	TEST(vec4(reflect(v3,pl),1),reflectionMat(pl)*vec4(v3,1));
	TEST(vec4(project(v3,pl),1),projectionMat(pl)*vec4(v3,1));
#else
	TEST(reflect(v4,pl),v4*reflectionMat(pl));
	TEST(project(v4,pl),v4*projectionMat(pl));
	v3=vec3(v4);
	TEST(vec4(reflect(v3,pl),1),vec4(v3,1)*reflectionMat(pl));
	TEST(vec4(project(v3,pl),1),vec4(v3,1)*projectionMat(pl));
#endif
	TEST(any(bvec2(HMtrue,HMfalse)),HMtrue);
	TEST(any(bvec3(HMfalse,HMfalse,HMfalse)),HMfalse);
	TEST(any(bvec4(HMfalse,HMfalse,HMfalse,HMtrue)),HMtrue);
	TEST(all(bvec2(HMtrue,HMfalse)),HMfalse);
	TEST(all(bvec3(HMtrue,HMtrue,HMtrue)),HMtrue);
	TEST(all(bvec4(HMtrue,HMtrue,HMfalse,HMtrue)),HMfalse);
	TEST(not(bvec3(HMtrue,HMtrue,HMfalse)),bvec3(HMfalse,HMfalse,HMtrue));
	TEST(planeNormal(vec3(1,2,3),vec3(-3,4,.5),vec3(-2,.7,1)),vec3(-7.25,-0.5,11.2));
	TEST(v4=plane(vec3(1,2,3),vec3(-3,4,.5),vec3(-2,.7,1)),vec4(-7.25,-0.5,11.2,-25.35));
	TEST(plane(planeNormal(vec3(1,2,3),vec3(-3,4,.5),vec3(-2,.7,1)),vec3(1,2,3)),vec4(-7.25,-0.5,11.2,-25.35));
	TEST(unitPlane(vec3(1,2,3),vec3(-3,4,.5),vec3(-2,.7,1)),vec4(-0.54303,-0.03745,0.83888,-1.8987));
	TEST(unitPlane(planeNormal(vec3(1,2,3),vec3(-3,4,.5),vec3(-2,.7,1)),vec3(1,2,3)),vec4(-0.54303,-0.03745,0.83888,-1.8987));
	TEST(normalizePlane(v4),vec4(-0.54303,-0.03745,0.83888,-1.8987));
	TEST(translatePlane(v4,vec3(1,-1,2)),vec4(-7.25,-0.5,11.2,-41));
	TEST(planePoint(v4),vec3(-1.0311,-0.071107,1.5928));
	TEST(unitPlanePoint(normalizePlane(v4)),vec3(-1.0311,-0.071107,1.5928));
	TEST(reflectPlane(vec4(2,1,6,2),normalizePlane(v4)),vec4(6.24622,1.29284,-0.559683,16.8471));
	TEST(equal(planeDist(v4,vec3(6,8,10)),2.9323f),HMtrue);
	TEST(equal(planeDist(v4,vec4(3,4,5,.5)),2.9323f),HMtrue);
	TEST(isZero(planeDist(v4,planePoint(v4))),HMtrue);
	TEST(notZero(planeDist(v4,unitPlanePoint(normalizePlane(v4)))),HMfalse);
	TEST(equal(unitPlaneDist(normalizePlane(v4),vec3(6,8,10)),2.9323f),HMtrue);
	TEST(equal(unitPlaneDist(normalizePlane(v4),vec4(3,4,5,.5)),2.9323f),HMtrue);
	TEST(onPlane(v4,vec3(-3,4,.5)),HMtrue);
	TEST(rayPlaneIntn(v3,vec3(0,1,1),vec3(-1,1,2),v4)==HMtrue && v3==vec3(-0.50257,1.5026,2.0051),true);
	TEST(segPlaneIntn(v3,vec3(0,1,1),vec3(-1,2,3),v4)==HMtrue && v3==vec3(-0.50257,1.5026,2.0051),true);
	TEST(lookAtDirnMat(vec3(10,-20,40),v3,vec3(0,0,1)),mat4(0.94836,0.24888,0.19666,0,0.3172,-0.7441,-0.58796,0,0,0.61998,-0.78462,0,-3.1395,-42.17,17.659,1));
	TEST(shortestArcUnitQuat(vec3(1,2,3),vec3(-3,4,.5)),quat(-0.35662061,-0.30799049,0.32420054,0.82027900));
	TEST(normalize(shortestArcQuat(vec3(1,2,3),vec3(-3,4,.5))),quat(-0.35662061,-0.30799049,0.32420054,0.82027900));
	q=quat(vec3(1,1,1),radians(75.0f));
	vec3 r=rotate(radians(75.0f),q.xyz(),v3=vec3(2,-.4,.9));
#ifndef D3D_MAT_STYLE
	TEST(rotationMat(radians(75.0f),q.xyz())*v3,r);
#else
	TEST(v3*rotationMat(radians(75.0f),q.xyz()),r);
#endif
#ifndef QUAT_MUL_REVERSE_ORDER
	TEST((q*2)*v3,r);
	TEST((q*quat(v3)*conjugate(q)).xyz(),r);
#else
	TEST(v3*(q*2),r);
	TEST((conjugate(q)*(quat(v3)*q)).xyz(),r);
#endif
	v4=vec4(3,2,1,.5f);
	TEST(equal(quatToAxisAngle(v3,quat(v4.xyz(),v4.w)),v4.w) && v3==normalize(v4.xyz()),HMtrue);
	TEST(normalize(unitQuatToAxisAngle(quat(vec3(v4),v4.w))),normalize(vec3(v4)));
	TEST(equal(quatAngle(quat(v4.xyz(),v4.w)),v4.w),HMtrue);
	TEST(slerp(quat(vec3(1,-2,3),radians(65.)),quat(vec3(-2,.5,1),radians(190.)),.5f),quat(0.69144261,-0.34436423,-0.0027140975,0.63506925));

	cout<<"\n============Testing Noise Functions============\n\n";

	extern HMint testNoiseRect(HMint minX,HMint maxX,HMint minY,HMint maxY,HMint step);
	TEST(testNoiseRect(-30,-4,10,120,1),12);
	TEST(testNoiseRect(10,94,-40,150,2),10);
	TEST(testNoiseRect(0,1024,0,2048,16),2);
	TEST(noise2(vec4(2.1,7.8,-4.3,1.4)),vec2(0.443738,-0.152736));
	int i,j;
	HMfloat avg=0;
#define NOISE_SIZE 512
	for (i=0;i<NOISE_SIZE;i++)
		for (j=0;j<NOISE_SIZE;j++)
			avg+=noise1(vec2(i*NOISE_SIZE/(j+1),j));
	TEST(equal(avg/(NOISE_SIZE*NOISE_SIZE),-0.000898521f),HMtrue);
	avg=0;
	for (i=0;i<6;i++)
		for (j=0;j<5;j++)
			for (int k=0;k<7;k++)
				for (int l=0;l<4;l++)
					avg+=noise1(vec4(i,j,k,l));
	TEST(equal(avg/(6*5*7*4),0.00614244f),HMtrue);

	extern void tga_write(const char *fname,HMint width,HMint height,HMuint c,HMubyte *pixels);

	static ubvec3 pixelsRGB[NOISE_SIZE][NOISE_SIZE];
	static HMfloat perlinNoise[NOISE_SIZE][NOISE_SIZE];
	static HMubyte pixelsA[NOISE_SIZE][NOISE_SIZE];
	ivec3 avgubv=ivec3(0);
	//HMuint avgub=0;
	for (i=0;i<NOISE_SIZE;i++)
		for (j=0;j<NOISE_SIZE;j++)
		{
			//pixelsRGB[i][j]=vec_cast<ubvec3>((noise3(vec2(i/100000.0+.65,j/20000.0+.37))+1.f)*.5f*255.f);
//			pixelsRGB[i][j]=vec_cast<ubvec3>((noise3(vec2(i/(float)NOISE_SIZE*4.-2.,j/(float)NOISE_SIZE*4.-2.),lerp)+1.f)*.5f*255.f);
			pixelsRGB[i][j]=vec_cast<ubvec3>(noise3(ivec2(i-NOISE_SIZE/2,j-NOISE_SIZE/2)));
			avgubv+=vec_cast<ivec3>(pixelsRGB[i][j]);
			//pixelsA[i][j]=(noise1(vec4(i/10000.0+.95,.2,j/300000.0+.13,.77))+1.f)*.5f*255.f;
			//pixelsA[i][j]=(noise1(dvec4(i/(float)NOISE_SIZE*40.,.2,j/(float)NOISE_SIZE*40.,.77))+1.f)*.5f*255.f;
			//avgub+=pixelsA[i][j];
		}

	TEST(sqlen(avgubv/sizeof(pixelsA)-ivec3(127)),0);
	//TEST(avgub/sizeof(pixelsA),127);
	tga_write("noiseRGB.tga",NOISE_SIZE,NOISE_SIZE,3,pixelsRGB[0][0]);
	cout<<"RGB Noise was written into a file \"noiseRGB.tga\"\n";

	HMfloat k,l;
	for (i=0,k=NOISE_SIZE;i<NOISE_SIZE;i++,k+=4.f/NOISE_SIZE)
		for (j=0,l=0;j<NOISE_SIZE;j++,l+=4.f/NOISE_SIZE)
			perlinNoise[i][j]=noise1(vec2(k,l),cerp);
	for (i=0/*,k=0*/;i<NOISE_SIZE;i++,k+=16.f/NOISE_SIZE)
		for (j=0,l=0;j<NOISE_SIZE;j++,l+=16.f/NOISE_SIZE)
			perlinNoise[i][j]+=.5f*noise1(vec2(k,l),herp3);
	for (i=0/*,k=0*/;i<NOISE_SIZE;i++,k+=64.f/NOISE_SIZE)
		for (j=0,l=0;j<NOISE_SIZE;j++,l+=64.f/NOISE_SIZE)
			perlinNoise[i][j]+=.25f*noise1(vec2(k,l),lerp);
	for (i=0;i<NOISE_SIZE;i++)
		for (j=0;j<NOISE_SIZE;j++)
			perlinNoise[i][j]+=.125f*((HMuint)noise1(ivec2(i,j))*(2.f/limits<HMuint>::max())-1.f);

	for (i=0;i<NOISE_SIZE;i++)
		for (j=0;j<NOISE_SIZE;j++)
			pixelsA[i][j]=(perlinNoise[i][j]/3.75f+.5f)*255.f;

	tga_write("noiseA.tga",NOISE_SIZE,NOISE_SIZE,1,pixelsA[0]);
	cout<<"Perlin Noise was written into a file \"noiseA.tga\"\n";


	cout<<"\nTotal tests: "<<totalTests;

	if (totalBugs) cout<<"\nTotal bugs: "<<totalBugs<<endl;
	else           cout<<"\nStrange as it may seem, but it's working! :-)\n";

	return 0;
}

typedef unsigned char  BYTE;
typedef unsigned short WORD;

#pragma pack(1)
struct TGA_Header
{
	BYTE IDLength;
	BYTE ColorMapType;
	BYTE ImageType;
	WORD ColorMapStart;
	WORD ColorMapLength;
	BYTE ColorMapDepth;
	WORD XOffset;
	WORD YOffset;
	WORD Width;
	WORD Height;
	BYTE Depth;
	BYTE ImageDescriptor;
};

#define TARGA_EMPTY_IMAGE			0
#define TARGA_COLORMAP				1
#define TARGA_TRUECOLOR_IMAGE		2
#define TARGA_BW_IMAGE				3
#define TARGA_COLORMAP_RLE_IMAGE	9
#define TARGA_TRUECOLOR_RLE_IMAGE	10
#define TARGA_BW_RLE_IMAGE			11

void tga_write(const char *fname,HMint width,HMint height,HMuint c,HMubyte *pixels)
{
	TGA_Header header={0};
	header.ImageType=(c==1) ? TARGA_BW_IMAGE : TARGA_TRUECOLOR_IMAGE;
	header.Width=width;
	header.Height=height;
	header.Depth=8*c;

	FILE *f=fopen(fname,"wb");
	fwrite(&header,sizeof(TGA_Header),1,f);
	fwrite(pixels,width*c,height,f);
	fclose(f);
}

HMint testNoiseRect(HMint minX,HMint maxX,HMint minY,HMint maxY,HMint step)
{
	ivec4 sum=ivec4(0);
	for (HMint x=minX;x<maxX;x+=step)
		for (HMint y=minY;y<maxY;y+=step)
			sum+=vec_cast<ivec4>(vec_cast<Tvec4<HMuint> >(noise4(ivec2(x,y)))/16777216U);
	return sqlen((sum*step*step)/((maxX-minX)*(maxY-minY))-ivec4(127));
}
