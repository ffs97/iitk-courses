/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  POLYGON.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


LEDA_BEGIN_NAMESPACE

class __exportC polygon;
class __exportC rat_polygon;
class __exportC real_polygon;

class __exportC gen_polygon;
class __exportC rat_gen_polygon;
class __exportC real_gen_polygon;

class __exportC POLYGON_REP : public handle_rep {

friend class POLYGON;
friend class GEN_POLYGON;

  list<POINT>   pt_list;
  list<SEGMENT> seg_list;
  POINT xmin, ymin, xmax, ymax; // bounding box
  int orient;

public:

  POLYGON_REP() {}
  POLYGON_REP(const list<SEGMENT>&, int orientation = 0);
 ~POLYGON_REP() {}
   
};


/*{\Manpage {POLYGON} {} {Polygons} {P}}*/

/*{\Msubst list<POLYGON> list<POLYGON> }*/


class __exportC POLYGON   : public HANDLE_BASE(POLYGON_REP) 
{
/*{\Mdefinition
There are three instantiations of |POLYGON|: 
|polygon| (floating point kernel),
|rat_polygon| (rational kernel) and
|real_polygon| (real kernel).
The respective header file name corresponds to the type name (with ``.h''
appended).

An instance $P$ of the data type |POLYGON| is a cyclic list of points 
(equivalently segments) in the plane. A polygon is called \emph{simple}
if all nodes of the graph induced by its segments have degree two and
it is called weakly simple, if its segments are disjoint except for common
endpoints and if the chain does not cross itself. See the LEDA book for 
more details.

A weakly simple polygon splits the plane into an unbounded region and
one or more bounded regions. For a simple polygon there is just one
bounded region. When a weakly simple
polygon $P$ is traversed either the bounded region is consistently to
the left of $P$ or the unbounded region is consistently to the left of $P$.
We say that $P$ is positively oriented in the former case and negatively
oriented in the latter case. We use $P$ to also denote the region to the left 
of $P$ and call this region the positive side of $P$.

The number of vertices is called the size
of $P$. A polygon with empty vertex sequence is called empty.

Only the types |rat_polygon| and |real_polygon| guarantee correct results. 
Almost all operations listed below are available for all the three 
instantiations of |POLYGON|. 
There is a small number of operations that are only available for |polygon|,
they are indicated as such.
}*/

friend class GEN_POLYGON;

POLYGON_REP* ptr() const { return (POLYGON_REP*)PTR; }

public:

/*{\Mtypes 5}*/ 

typedef COORD coord_type;
/*{\Mtypemember the coordinate type (e.g. |rational|).}*/

typedef POINT point_type;
/*{\Mtypemember the point type (e.g. |rat_point|).}*/

typedef SEGMENT segment_type;
/*{\Mtypemember the segment type (e.g. |rat_segment|).}*/

typedef polygon float_type;
/*{\Mtypemember the corresponding floating-point type (|polygon|).}*/



enum CHECK_TYPE { NO_CHECK = 0, SIMPLE = 1, WEAKLY_SIMPLE = 2, NOT_WEAKLY_SIMPLE = 3 };

enum RESPECT_TYPE { DISREGARD_ORIENTATION = 0, RESPECT_ORIENTATION = 1 };


static  CHECK_TYPE input_check_type;  // used in input operator

/*{\Mcreation}*/

POLYGON() { PTR = new POLYGON_REP; }
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the
empty polygon.}*/

explicit

POLYGON(const list<POINT>& pl, CHECK_TYPE check= POLYGON::WEAKLY_SIMPLE, 
        RESPECT_TYPE respect_orientation = POLYGON::RESPECT_ORIENTATION);
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the
polygon with vertex sequence |pl|. If |respect_orientation| is 
DISREGARD\_ORIENTATION, 
the positive orientation is chosen.\\
\precond If |check| is SIMPLE, |pl| must define a simple polygon, and 
if |check| is WEAKLY\_SIMPLE, |pl| must define a weakly simple polygon. 
If no test is to performed, the second argument has to be set to 
NO\_CHECK. The constants NO\_CHECK, SIMPLE, and WEAKLY\_SIMPLE 
are part of a local enumeration type
CHECK\_TYPE.  }*/

#if ( KERNEL == RAT_KERNEL )

POLYGON(const polygon& Q, int prec = rat_point::default_precision);
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to 
a rational approximation of the (floating point) polygon $Q$ of coordinates 
with denominator at most |prec|. If |prec| is zero, the implementation chooses |prec| large enough such that there is no loss of precision in the conversion.
}*/
#endif

#if ( KERNEL == REAL_KERNEL )
POLYGON(const polygon& Q, int prec = 0);
#endif

#if ( KERNEL == FLOAT_KERNEL )
POLYGON(const polygon& Q, int prec);
#endif

explicit

POLYGON(const list<SEGMENT>& sl,int orientation = 0);
/*{\Xcreate creates a polygon with segment list |sl|.\\
\precond |sl| is a weakly simple polygon; if |orientation| is non-zero,
then it is the orientation of |sl|.
}*/


 POLYGON(const POLYGON& P) : HANDLE_BASE(POLYGON_REP)(P) {}


~POLYGON() {}

 POLYGON& operator=(const POLYGON& P) 
 { HANDLE_BASE(POLYGON_REP)::operator=(P); return *this;}


/*{\Moperations 3.5 4.6}*/

polygon     to_polygon() const;

polygon     to_float() const;
/*{\Mop     returns a floating point approximation of |\Mvar|. }*/

void normalize() const;
/*{\Mop    simplifies the homogenous representation by calling
           |p.normalize()| for every vertex $p$ of |\Mvar|. }*/


bool        is_simple() const;
/*{\Mop   tests whether |\Mvar| is simple or not. }*/

bool        is_weakly_simple() const;
/*{\Mop   tests whether |\Mvar| is weakly simple or not. }*/

bool        is_weakly_simple(list<POINT>& L) const;
/*{\Mop   as above, returns all proper points of intersection in L. }*/

POLYGON::CHECK_TYPE check_simplicity() const;
/*{\Mop   returns the |CHECK_TYPE| of |\Mvar|. The result can 
          be SIMPLE, WEAKLY\_SIMPLE or NOT\_WEAKLY\_SIMPLE. }*/

bool        is_convex() const;
/*{\Mop     returns true if |\Mvar| is convex, false otherwise.}*/

const list<POINT>&   vertices() const  { return ptr()->pt_list; } 
/*{\Mop     returns the sequence of vertices of |\Mvar| in 
            counterclockwise ordering.  }*/

/*{\Moptions nextwarning=no}*/
const list<SEGMENT>& segments() const { return ptr()->seg_list; }
const list<SEGMENT>& edges() const { return ptr()->seg_list; }
/*{\Mop     returns the sequence of bounding segments 
            of |\Mvar| in counterclockwise ordering. }*/

list<POINT> intersection(const SEGMENT& s) const;
/*{\Mopl    returns the proper crossings between $P$ and $s$ 
as a list of points.}*/

list<POINT> intersection(const LINE& l) const;
/*{\Mopl    returns the proper crossings between $P$ and $l$ 
as a list of points.}*/

POLYGON intersect_halfplane(const LINE& l) const;
/*{\Mopl    returns the intersection of $P$ with the halfspace on
the positive side of $l$. }*/

int         size()   const  { return ptr()->seg_list.size(); }
/*{\Mop     returns the size of |\Mvar|.}*/

bool        empty()  const  { return ptr()->seg_list.empty(); }
/*{\Mop     returns true if |\Mvar| is empty, false otherwise.}*/

POLYGON translate(RAT_TYPE dx, RAT_TYPE dy) const;
/*{\Mopl    returns |\Mvar| translated by vector $(dx,dy)$.}*/

POLYGON translate(INT_TYPE dx, INT_TYPE dy, INT_TYPE dw) const;
/*{\Mopl    returns |\Mvar| translated by vector $(dx/dw,dy/dw)$.}*/

POLYGON translate(const VECTOR& v) const;
/*{\Mop     returns |\Mvar| translated by vector $v$.}*/

POLYGON operator+(const VECTOR& v) const { return translate(v); }
/*{\Mbinop  returns |\Mvar| translated by vector $v$.}*/

POLYGON operator-(const VECTOR& v) const { return translate(-v); }
/*{\Mbinop  returns |\Mvar| translated by vector $-v$.}*/

POLYGON rotate90(const POINT& q, int i=1) const;
/*{\Mopl    returns |\Mvar| rotated about $q$ by an angle of $i\times 90$ 
            degrees. If $i > 0$ the rotation is counter-clockwise otherwise
            it is clockwise. }*/


POLYGON reflect(const POINT& p, const POINT& q) const;
/*{\Mop     returns |\Mvar| reflected  across the straight line passing
            through $p$ and $q$.}*/

POLYGON reflect(const POINT& p) const;
/*{\Mop     returns |\Mvar| reflected  across point $p$.}*/

RAT_TYPE    sqr_dist(const POINT& p) const;
/*{\Mop     returns the square of the minimal Euclidean distance 
between a segment in |\Mvar| and $p$. Returns zero if |\Mvar| is empty.}*/

POLYGON complement() const;
/*{\Mopl    returns the complement of $P$.}*/

POLYGON eliminate_colinear_vertices() const;
/*{\Mopl    returns a copy of $P$ without colinear vertices.}*/

list<POLYGON> simple_parts() const;
/*{\Mopl    returns the simple parts of $P$ as a list of simple polygons.}*/

list<POLYGON> split_into_weakly_simple_parts(bool strict = false) const;
/*{\Mopl    splits $P$ into a set of weakly simple polygons whose union coincides 
            with the inner points of $P$. 
	    If |strict| is true a point is considered an inner point if it is 
	    left of all surrounding segments, otherwise it is considered as an
	    inner point if it is locally to the left of some surrounding edge.
	    (This function is experimental.)}*/

GEN_POLYGON make_weakly_simple(bool with_neg_parts = true, 
							   bool strict = false) const;
/*{\Mopl creates a weakly simple generalized polygon |Q| from a possibly
         non-simple polygon |\Mvar| such that |Q| and |\Mvar| have the same
         inner points. 
	 The flag |with_neg_parts| determines whether inner points in 
	 negatively oriented parts are taken into account, too.
	 The meaning of the flag |strict| is the same as in the method above.
	 (This function is experimental.)}*/

GEN_POLYGON buffer(RAT_TYPE d, int p) const;
/*{\Mopl adds an exterior buffer zone to $P$ ($d > 0$), 
         or removes an interior buffer zone from $P$ ($d < 0$).
         More precisely, for $d \geq 0$ define the buffer tube $T$ as 
		 the set of all points in the complement of $P$ whose distance to $P$ 
		 is at most $d$. Then the function returns $P \cup T$.
		 For $d < 0$ let $T$ denote the set of all points in $P$ whose 
		 distance to the complement is less than $\Labs{d}$. Then the result 
		 is $P \setminus T$. $p$ specifies the number of points used to represent
		 convex corners. At the moment, only $p = 1$ and $p = 3$ are supported.
		 (This function is experimental.)}*/


#if ( KERNEL == FLOAT_KERNEL )

/*{\Mtext The functions in the following group 
are only available for |polygons|. They
have no counterpart for |rat_polygons|.

\medskip}*/

polygon  translate_by_angle(double alpha, double d) const;
/*{\Mop     returns |\Mvar| translated in direction |alpha| by distance $d$.}*/

polygon rotate(const point& p, double alpha) const;
/*{\Mop returns |\Mvar| rotated by $\alpha$ degrees about $p$.}*/

polygon rotate(double alpha) const;
/*{\Mop returns |\Mvar| rotated by $\alpha$ degrees about the origin.}*/

double    distance(const point& p) const;
/*{\Mop     returns the Euclidean distance between |\Mvar|
            and $p$.}*/

rat_polygon to_rational(int prec = -1) const;
/*{\Mop     returns a representation of |\Mvar| with rational coordinates
            with precision |prec| (cf. Section \ref{Rational Points}).}*/

#endif

#if ( KERNEL == REAL_KERNEL )
real distance(const real_point& p) const;
rat_polygon to_rational(int prec = -1) const;
#endif


/*{\Mtext 

\medskip
{\bf All functions below assume that $P$ is weakly simple.}
\medskip
}*/

int         side_of(const POINT& p) const;
/*{\Mop     returns $+1$ if $p$ lies to the left of |\Mvar|, $0$ if 
            $p$ lies on |\Mvar|, and $-1$ if $p$ lies to the 
            right of |\Mvar|.}*/ 

region_kind region_of(const POINT& p) const;
/*{\Mop     returns BOUNDED\_REGION if $p$ lies in the 
            bounded region of |\Mvar|, returns ON\_REGION if $p$ lies on 
            |\Mvar|, and returns UNBOUNDED\_REGION if $p$ lies in the 
            unbounded region. }*/

bool        inside(const POINT& p) const  
            { return side_of(p) == +1; }
/*{\Mop     returns true if $p$ lies to the left of |\Mvar|, i.e.,
            |side_of(p) == +1|. }*/

bool        on_boundary(const POINT& p) const  
            { return side_of(p) == 0; }
/*{\Mop     returns true if $p$ lies on |\Mvar|, i.e., 
            |side_of(p) == 0|. }*/

bool        outside(const POINT& p) const 
            { return side_of(p) == -1; }
/*{\Mop     returns true if $p$ lies to the right 
            of |\Mvar|, i.e., |side_of(p) == -1|.}*/

bool        contains(const POINT& p) const
            { return !outside(p); }
/*{\Mop     returns true if $p$ lies to the left of or on |\Mvar|.}*/


RAT_TYPE    area0() const;
RAT_TYPE    area1() const;

RAT_TYPE    area() const;
/*{\Mop     returns the signed area of the bounded region of $P$. The sign of
the area is positive if the bounded region is the positive side of $P$. }*/

int          orientation() const { return ptr()->orient; }
/*{\Mop     returns the orientation of $P$. }*/


void bounding_box(POINT& xmin, POINT& ymin, POINT& xmax, POINT& ymax) const
{ xmin = ptr()->xmin;
  ymin = ptr()->ymin;
  xmax = ptr()->xmax;
  ymax = ptr()->ymax;
}
/*{\Mop     returns the coordinates of a rectangular bounding box of $P$. }*/


RAT_TYPE xmin() const { return ptr()->xmin.xcoord(); }
RAT_TYPE ymin() const { return ptr()->ymin.ycoord(); }
RAT_TYPE xmax() const { return ptr()->xmax.xcoord(); }
RAT_TYPE ymax() const { return ptr()->ymax.ycoord(); }


bool    operator==(const POLYGON& P1) const;

friend __exportF ostream& operator<<(ostream& out, const POLYGON& p);
friend __exportF istream& operator>>(istream& in,  POLYGON& p);


};

/*{\Mtext
\bigskip
{\bf Iterations Macros}

{\bf forall\_vertices}($v,P$)       
$\{$ ``the vertices of $P$ are successively assigned to rat\_point $v$'' $\}$

{\bf forall\_segments}($s,P$)       
$\{$ ``the edges of $P$ are successively assigned to rat\_segment $s$'' $\}$ 
}*/

inline list_item
LedaForallVerticesFirstItem(const POLYGON& P)
{ return P.vertices().first_item(); }

inline void
LedaForallVerticesNextItem(const POLYGON& P, list_item& loop_var)
{ loop_var = P.vertices().next_item(loop_var); }

inline list_item
LedaForallVerticesAssign(const POLYGON& P, list_item loop_var, POINT& v)
{ if (loop_var) v = P.vertices().inf(loop_var); return loop_var; }

#if !defined(forall_vertices)
#define forall_vertices(v,P) \
LEDA_FORALL_PREAMBLE \
for(list_item vert_loop_var = LEDA_SCOPE LedaForallVerticesFirstItem(P);\
LEDA_SCOPE LedaForallVerticesAssign((P),vert_loop_var, v);\
LEDA_SCOPE LedaForallVerticesNextItem((P),vert_loop_var))
#endif

#if !defined(forall_segments)
#define forall_segments(s,P)  forall(s,(P).segments())
#endif

/*{\Mtext 

\medskip

{\bf Non-Member Functions}

\medskip

}*/

extern __exportF POLYGON   reg_n_gon(int n, CIRCLE C, double epsilon);
/*{\Mfunc generates a (nearly) regular $n$-gon whose vertices lie on the 
circle $C$. The $i$-th point is generated by 
$|C.point_of_circle|(2 \pi i/n, |epsilon|)$. With the rational kernel 
the vertices of the n-gon are guaranteed to lie on the circle, with the 
floating point kernel they are only guaranteed to lie near $C$. }*/

extern __exportF POLYGON   n_gon(int n, CIRCLE C, double epsilon);
/*{\Mfunc generates a (nearly) regular $n$-gon whose vertices lie near the 
circle $C$. For the flaoting point kernel the function is equivalent to 
the function above. For the rational kernel the function first generates
a n-gon with floating point arithmetic and then converts the resulting 
|polygon| to a |rat_polygon|. }*/

extern __exportF POLYGON hilbert(int n, RAT_TYPE x1, RAT_TYPE y1, RAT_TYPE x2, RAT_TYPE y2);
/*{\Mfunc generates the Hilbert polygon of order $n$ within the rectangle 
with boundary $(|x1|,|y1|)$ and $(|x2|,|y2|)$.\\
\precond $|x1| < |x2|$ and $|y1| < |y2|$. }*/


LEDA_END_NAMESPACE
