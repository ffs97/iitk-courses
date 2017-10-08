#ifndef LEDA_D3_GL_COL_H
#define LEDA_D3_GL_COL_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600147
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/graphics/color.h>
#include <LEDA/graphics/gl_window.h>


LEDA_BEGIN_NAMESPACE


/*{\Manpage {d3_gl_col} {T} {D3-GL-COL} {C}}*/

/*{\Mdefinition
An instance |\Mvar| of the data type |\Mname| represents an OpenGL-color consisting
of a quadruplet $(r,g,b,a)$ with $0\le r,g,b,a \le 1$, the so called rgba-color-value.
Corresponding to the OpenGL color commands glColor3f(), glColor4f(), glColor3d() and
glColor4d() you can use the data types GLdouble, GLfloat or double or float for $T$.
The use of other types for T, especially non-mathematical types, can cause abnormal
programm termination in worst case. $d3\_gl\_colF$ and $d3\_gl\_colD$ are predefinded
data types for $d3\_gl\_col<GLfloat>$ respectively $d3\_gl\_col<GLdouble>$.
}*/


template <class T>
class __exportC d3_gl_col
{	
	
protected :
	T rv,gv,bv,av;

	static void init(d3_gl_col<T>& c) { c.rv = c.gv = c.bv = c.av = 0; }

	static bool test(const T& r, const T& g, const T& b, const T& a)
	{
		if(r < 0 || r > 1) return false;
		if(g < 0 || g > 1) return false;
	    if(b < 0 || b > 1) return false;
		if(a < 0 || a > 1) return false;
		return true;
	}

public :

	/*{\Mtypes 3}*/

	typedef T rgba_type;
	/*{\Mtypemember the type of the r,g,b and a values.}*/

	/*{\Mcreation C}*/ 

	d3_gl_col() { init(*this); }
	/*{\Mcreate creates an instance |\Mvar| of type |\Mname|. All color-components
	are initialized to their default value.}*/

	d3_gl_col(const T& r, const T& g, const T& b, const T& a = 1)
	{		
		if(test(r,g,b,a)){
			rv = r;
			gv = g;
			bv = b;
			av = a;	
		}
		else init(*this);
	}
	/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes
	it with the value $(r,g,b,a)$ or to its default value if the precondition
	is not fulfilled. \precond $0 <= r,g,b,a <= 1$.}*/

	d3_gl_col(T array[4])
	{
		if(test(array[0],array[1],array[2],array[3])){
			rv = array[0];
			gv = array[1];
			bv = array[2];
			av = array[3];
		}
		else init(*this);
	}
	/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes
	it with the values in $array$ or to its default values if the precondition
	is not fulfilled. \precond $0 <= array[0],...,array[3] <= 1$.}*/

	d3_gl_col(const color& c, const T& a = 1)
	{
		if(a >= 0 && a <= 1){
			int r,g,b;
			c.get_rgb(r,g,b);
			rv = ((T)r) / ((T)255);
			gv = ((T)g) / ((T)255);
			bv = ((T)b) / ((T)255);	
			av = a;
		}
		else init(*this);
	}
	/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes
	it with the values of c and a or to its default values if the precondition
	is not fulfilled. \precond $0 <= a <= 1$.}*/

	d3_gl_col(const d3_gl_col<T>& c)
	{
		rv = c.rv;
		gv = c.gv;
		bv = c.bv;
		av = c.av;
	}

	/*{\Moperations 2 4}*/

	T r() const { return rv; }	
	/*{\Mop      returns the $r$-component of |\Mvar|.}*/

	T g() const { return gv; }	
	/*{\Mop      returns the $g$-component of |\Mvar|.}*/
		  
	T b() const { return bv; }
	/*{\Mop      returns the $b$-component of |\Mvar|.}*/

	T a() const { return av; }	
	/*{\Mop      returns the $a$-component of |\Mvar|.}*/

	void set_r(const T& r)
	{ if(r >= 0 && r <= 1)  rv = r; }
	/*{\Mop    sets the  r-value of |\Mvar| to $r$.
	\precond $0 <= r <= 1$.}*/

	void set_g(const T& g)
	{ if(g >= 0 && g <= 1)	gv = g; }
	/*{\Mop    sets the  g-value of |\Mvar| to $g$.
	\precond $0 <= g <= 1$.}*/

	void set_b(const T& b)
	{ if(b >= 0 && b <= 1)	bv = b; }
	/*{\Mop    sets the  b-value of |\Mvar| to $b$.
	\precond $0 <= b <= 1$.}*/

	void set_a(const T& a)
	{ if(a >= 0 && a <= 1)	av = a; }
	/*{\Mop    sets the  a-value of |\Mvar| to $a$.
	\precond $0 <= a <= 1$.}*/
	
	void get_rgba(T& r, T& g, T& b, T& a) const
	{
		r = rv;
		g = gv;
		b = bv;
		a = av;
	}
	/*{\Mop    assigns the rgba-value of |\Mvar| to $(r,g,b,a)$. }*/

	void get_rgba(T array[4]) const
	{
		array[0] = rv;
		array[1] = gv;
		array[2] = bv;
		array[3] = av;
	}
	/*{\Mop    assigns the rgba-value of |\Mvar| to $array$. }*/

	void set_rgba(const T& r, const T& g, const T& b, const T& a)
	{
		if(test(r,g,b,a)){
			rv = r;
			gv = g;
			bv = b;
			av = a;
		}	
	}
	/*{\Mop    sets the rgba-value of |\Mvar| to $(r,g,b,a)$.
	\precond $0 <= r,g,b,a <= 1$.}*/

	void set_rgba(T array[4])
	{
		if(test(array[0],array[1],array[2],array[3])){
			rv = array[0];
			gv = array[1];
			bv = array[2];
			av = array[3];
		}	
	}
	/*{\Mop    sets the rgba-value of |\Mvar| to $array$.
	\precond $0 <= array[0],...,array[3] <= 1$.}*/	
		

	void  set_color(const color& c, const T& a = 1)
	{
		if(a >= 0 && a <= 1){
			int r,g,b;
			c.get_rgb(r,g,b);
			rv = ((T)r) / ((T)255);
			gv = ((T)g) / ((T)255);
			bv = ((T)b) / ((T)255);	
			av = a;
		}
	}
	/*{\Mop    sets the rgba-value of |\Mvar| to $c$ and $a$.
	\precond $0 <= a <= 1$.}*/
	
	
	operator color() const                                                        
	{ return color( (int)(((T)255)*rv), (int)(((T)255)*gv), (int)(((T)255)*bv) ); }
	/*{\Mconversion    converts |\Mvar| to the data type color and returns it.}*/
	
	
	void operator()() const
	{ glColor4f(rv,gv,bv,av); }
	/*{\Mfunop  makes |\Mvar| to the current color (of the active LEDA $gl\_window$),
	            i.e. calls the OpenGL-method glColor4*(r,g,b,a).}*/ 
	            

	d3_gl_col<T>& operator=(const d3_gl_col<T>& c)
	{
		rv = c.rv;
		gv = c.gv;
		bv = c.bv;
		av = c.av;
		return *this;
	}


	template<class T2> friend inline istream& operator>>(istream& I , d3_gl_col<T2>& c);
	/*{\Mbinopfunc  reads |\Mvar| componentwise from the input stream $I$.
	if I gives invalid input, the input is ignored and |\Mvar| doesn't change.}*/

	template<class T2> friend inline ostream& operator<<(ostream& O, const d3_gl_col<T2>& c);
	/*{\Mbinopfunc  writes |\Mvar| componentwise to the output stream $O$.}*/

	template<class T2> friend inline bool operator==(const d3_gl_col<T2>& c1,
													 const d3_gl_col<T2>& c2);
	/*{\Mbinopfunc     Test for equality.}*/

	template<class T2> friend inline bool operator!=(const d3_gl_col<T2>& c1,
		                                             const d3_gl_col<T2>& c2);
	/*{\Mbinopfunc     Test for inequality.}*/

	template<class T2> 
        friend inline int d3_gl_col_cmp(const d3_gl_col<T2>& c1,
		                        const d3_gl_col<T2>& c2);
	/*{\Mfunc lexicographic ordering for |d3_gl_col|.}*/
};


template<>
inline
void d3_gl_col<GLdouble>::operator()() const
{	glColor4d(rv,gv,bv,av);	  }

template<>
inline
void d3_gl_col<GLfloat>::operator()() const
{	glColor4f(rv,gv,bv,av);	  }


template <class T>
inline bool operator==(const d3_gl_col<T>& c1, const d3_gl_col<T>& c2)
{	return (c1.rv==c2.rv && c1.gv==c2.gv && c1.bv==c2.bv && c1.av==c2.av);   }

template <class T>
inline bool operator!=(const d3_gl_col<T>& c1, const d3_gl_col<T>& c2)
{	return !(c1.rv==c2.rv && c1.gv==c2.gv && c1.bv==c2.bv && c1.av==c2.av);	 }


template <class T>
inline ostream& operator<<(ostream& O, const d3_gl_col<T>& c)
{	
	O << c.rv << " ";
	O << c.gv << " ";
	O << c.bv << " ";
	O << c.av;
	return O;
}

template <class T>
inline istream& operator>>(istream& I, d3_gl_col<T>& c)
{
	T r,g,b,a;
	I >> r;
	I >> g;
	I >> b;
	I >> a;

	if(c.test(r,g,b,a)){
		c.rv = r;
		c.gv = g;
		c.bv = b;
		c.av = a;
	}
	
	return I;
}


template <class T>
inline int d3_gl_col_cmp(const d3_gl_col<T>& c1, const d3_gl_col<T>& c2)
{
	if(c1.rv < c2.rv) return -1;
	if(c1.rv > c2.rv) return  1;
	if(c1.gv < c2.gv) return -1;
	if(c1.gv > c2.gv) return  1;
	if(c1.bv < c2.bv) return -1;
	if(c1.bv > c2.bv) return  1;
	if(c1.av < c2.av) return -1;
	if(c1.av > c2.av) return  1;
	return 0;
} 


typedef d3_gl_col<GLfloat>  d3_gl_colF;
typedef d3_gl_col<GLdouble> d3_gl_colD;

inline int compare(const d3_gl_colF& c1, const d3_gl_colF& c2)
{ return d3_gl_col_cmp(c1,c2); } 

inline int compare(const d3_gl_colD& c1, const d3_gl_colD& c2)
{ return d3_gl_col_cmp(c1,c2); } 


LEDA_END_NAMESPACE


#if LEDA_ROOT_INCL_ID == 600147
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

#endif
