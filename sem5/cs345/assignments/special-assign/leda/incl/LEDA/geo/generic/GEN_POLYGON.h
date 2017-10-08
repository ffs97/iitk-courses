/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  GEN_POLYGON.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/core/map.h>

LEDA_BEGIN_NAMESPACE

class __exportC GEN_POLYGON;

class __exportC GEN_POLYGON_REP : public handle_rep {

friend class __exportC GEN_POLYGON;

public: 

  enum KIND { EMPTY, FULL, NON_TRIVIAL };

private:

  KIND k;
  list<POLYGON> pol_list;

  
public:


  GEN_POLYGON_REP(const POLYGON& P)
  { if ( P.size() == 0 ) { k = EMPTY; }
    else {k = NON_TRIVIAL; pol_list.append(P); }
  }

  GEN_POLYGON_REP(KIND _k)
  { if ( _k == NON_TRIVIAL ) LEDA_EXCEPTION(1,"k must be empty or full");
    k = _k;
  }

  GEN_POLYGON_REP(const list<POLYGON>& PL)
  { pol_list = PL; 
    k = (PL.empty()? EMPTY : NON_TRIVIAL);
  }

  
 ~GEN_POLYGON_REP() {}
   
};

class __exportC rat_gen_polygon;

/*{\Manpage {GEN_POLYGON} {} {Generalized Polygons} }*/

/*{\Msubst list_POLYGON_ list<POLYGON> }*/


class __exportC GEN_POLYGON   : public HANDLE_BASE(GEN_POLYGON_REP) 
{
/*{\Mdefinition
There are three instantiations of |POLYGON|: 
|gen_polygon| (floating point kernel),
|rat_gen_polygon| (rational kernel) and
|real_gen_polygon| (real kernel).
The respective header file name corresponds to the type name (with ``.h''
appended).

An instance $P$ of the data type |GEN_POLYGON| is a regular 
polygonal region in the plane. A regular region is an open set that is 
equal to the interior of its closure. A region is polygonal if its boundary
consists of a finite number of line segments.

The boundary of a |GEN_POLYGON| consists of zero or more weakly 
simple closed polygonal chains. There are two regions whose boundary is 
empty, namely the empty region and the full region. The full 
region encompasses the 
entire plane. We call a region non-trivial if its boundary is non-empty.
The boundary cycles $P_1$, $P_2$, \ldots, $P_k$ of a |GEN_POLYGON| are ordered
such that no $P_i$ is nested in a $P_j$ with $i < j$. 

Only the types |rat_polygon| and |real_polygon| guarantee correct results. 
Almost all operations listed below are available for all the three 
instantiations of |POLYGON|. 
There is a small number of operations that are only available for |polygon|,
they are indicated as such.

A detailed discussion of polygons and generalized polygons can be found
in the LEDA book.

The local enumeration type KIND consists of elements EMPTY, FULL, 
and NON\_TRIVIAL. 
}*/

GEN_POLYGON_REP* ptr() const { return (GEN_POLYGON_REP*)PTR; }

//GEN_POLYGON(const list<rat_segment>& sl);

public:

/*{\Mtypes 5}*/ 

typedef COORD coord_type;
/*{\Mtypemember the coordinate type (e.g. |rational|).}*/

typedef POINT point_type;
/*{\Mtypemember the point type (e.g. |rat_point|).}*/

typedef SEGMENT segment_type;
/*{\Mtypemember the segment type (e.g. |rat_segment|).}*/

typedef POLYGON polygon_type;
/*{\Mtypemember the polygon type (e.g. |rat_polygon|).}*/

typedef gen_polygon float_type;
/*{\Mtypemember the corresponding floating-point type (|gen_polygon|).}*/


typedef GEN_POLYGON_REP::KIND KIND;

enum CHECK_TYPE { NO_CHECK = 0, SIMPLE = 1, WEAKLY_SIMPLE = 2, CHECK_REP = 3};
enum RESPECT_TYPE { DISREGARD_ORIENTATION = 0, RESPECT_ORIENTATION = 1 };


static CHECK_TYPE input_check_type;  // used in input operator

/*{\Mcreation P }*/

GEN_POLYGON(KIND k = GEN_POLYGON_REP::EMPTY) { PTR = new GEN_POLYGON_REP(k); }
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the
empty polygon if $k$ is EMPTY and to the full polygon if $k$ is FULL.}*/


GEN_POLYGON(const POLYGON& p, CHECK_TYPE check = WEAKLY_SIMPLE, 
            RESPECT_TYPE respect_orientation = RESPECT_ORIENTATION);
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the
polygonal region with boundary $p$. If |respect_orientation| is 
DISREGARD\_ORIENTATION, 
the orientation is chosen such |\Mvar| is bounded.\\
\precond |p| must be a weakly simple polygon. 
If |check| is set appropriately this is checked.  }*/

explicit

GEN_POLYGON(const list<POINT>& pl, CHECK_TYPE check= GEN_POLYGON::WEAKLY_SIMPLE, 
            RESPECT_TYPE respect_orientation = RESPECT_ORIENTATION);
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the
polygon with vertex sequence |pl|. If |respect_orientation| is 
DISREGARD\_ORIENTATION, 
the orientation is chosen such that |\Mvar| is bounded.\\
\precond If |check| is SIMPLE, |pl| must define a simple polygon, 
and if |check| is WEAKLY\_SIMPLE, |pl| must define a weakly simple polygon. If no test is to performed, the second argument has to be set to 
NO\_CHECK. The three constants NO\_CHECK, SIMPLE, and WEAKLY\_SIMPLE are 
part of a local enumeration type CHECK\_TYPE.  }*/

explicit

GEN_POLYGON(const list<POLYGON> & PL, CHECK_TYPE check = CHECK_REP);
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the
polygon with boundary representation |PL|.\\
\precond |PL| must be a boundary representation. This conditions is 
checked if |check| is set to CHECK\_REP.}*/

static
GEN_POLYGON unite(const list<GEN_POLYGON> & PL)
{ return unite_parallel(PL); }

static
GEN_POLYGON unite_with_bool_ops(const list<GEN_POLYGON> & PL, map<SEGMENT, GenPtr>* ptr_corresp = 0);

static
GEN_POLYGON unite_parallel(const list<GEN_POLYGON> & PL, int step_size = 128, map<SEGMENT, GenPtr>* ptr_corresp = 0);

explicit

GEN_POLYGON(const list<GEN_POLYGON> & PL);
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to the
union of all generalized polygons in |PL|.}*/


#if ( KERNEL == RAT_KERNEL )
GEN_POLYGON(const gen_polygon& Q, int prec = rat_point::default_precision);
/*{\Mcreate 
introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is initialized to 
a rational approximation of the (floating point) polygon $Q$ of coordinates 
with denominator at most |prec|. If |prec| is zero, the implementation chooses |prec| large enough such that there is no loss of precision in the conversion}*/
#endif

#if ( KERNEL == REAL_KERNEL )
GEN_POLYGON(const gen_polygon& Q, int prec = 0);
#endif

#if ( KERNEL == FLOAT_KERNEL )
GEN_POLYGON(const gen_polygon& Q, int prec);
// cannot allow default argument because of clash with copy
#endif

GEN_POLYGON(const GEN_POLYGON& P) : HANDLE_BASE(GEN_POLYGON_REP)(P) {}


~GEN_POLYGON() {}

GEN_POLYGON& operator=(const GEN_POLYGON& P) 
{ HANDLE_BASE(GEN_POLYGON_REP)::operator=(P); return *this;}

/*{\Moperations 3.5 4.6}*/

bool        empty()  const  { return ptr()->k == GEN_POLYGON_REP::EMPTY; }
/*{\Mop     returns true if |\Mvar| is empty, false otherwise.}*/

bool        full()  const  { return ptr()->k == GEN_POLYGON_REP::FULL; }
/*{\Mop     returns true if |\Mvar| is the entire plane, false otherwise.}*/

bool        trivial()  const  { return empty() || full(); }
/*{\Mop     returns true if |\Mvar| is either empty or full,
  false otherwise.}*/
  
bool        is_convex() const;
/*{\Mop     returns true if |\Mvar| is convex, false otherwise.}*/

KIND        kind()  const  { return ptr()->k; }
/*{\Mop     returns the kind of |\Mvar|.}*/

gen_polygon to_gen_polygon() const;

gen_polygon to_float() const;
/*{\Mop     returns a floating point approximation of |\Mvar|. }*/


void normalize() const;
/*{\Mop    simplifies the homogenous representation by calling
           |p.normalize()| for every vertex $p$ of |\Mvar|. }*/


bool        is_simple() const;
/*{\Mop   returns true if the polygonal region is simple, i.e., if the 
          graph defined by the segments in the boundary of |\Mvar| has only vertices of degree two.}*/


static bool check_representation(const list<POLYGON>& PL);
/*{\Mstatic   checks whether |PL| is a boundary representation.}*/

bool        check_representation() const;
/*{\Mop   tests whether the representation of |\Mvar| is OK. This test is partial. }*/

void       canonical_rep() ;
/*{\Mop    NOT IMPLEMENTED YET.}*/

list<POINT>   vertices() const;
/*{\Mop     returns the concatenated vertex lists of all polygons in the boundary representation of |\Mvar|. }*/

/*{\Moptions nextwarning=no}*/
list<SEGMENT> segments() const;
list<SEGMENT> edges() const { return segments() ; }
/*{\Mop     returns the concatenated edge lists of all polygons in the boundary representation of |\Mvar|. \\
            Please note that it is not save to use this function in a forall-loop. Instead of writing 
            forall(SEGMENT s, edges()){..} please write list<SEGMENT> L = edges(); forall(SEGMENT s, L){...}.}*/

const list<POLYGON>&   polygons() const { return ptr()->pol_list; }
/*{\Mop     returns the lists of all polygons in the boundary representation of |\Mvar|. }*/

list<POINT> intersection(const SEGMENT& s) const;
/*{\Mopl    returns the list of all proper intersections between $s$ and the
boundary of |\Mvar|.}*/

list<POINT> intersection(const LINE& l) const;
/*{\Mopl    returns the list of all proper intersections between $l$ and the
boundary of |\Mvar|.}*/

int         size()   const  { return segments().size(); }
/*{\Mop     returns the number of segments in the boundary of |\Mvar|.}*/

GEN_POLYGON translate(RAT_TYPE dx, RAT_TYPE dy) const;
/*{\Mopl    returns |\Mvar| translated by vector $(dx,dy)$.}*/

GEN_POLYGON translate(INT_TYPE dx, INT_TYPE dy, INT_TYPE dw) const;
/*{\Mopl    returns |\Mvar| translated by vector $(dx/dw,dy/dw)$.}*/

GEN_POLYGON translate(const VECTOR& v) const;
/*{\Mop     returns |\Mvar| translated by vector $v$.}*/

GEN_POLYGON operator+(const VECTOR& v) const { return translate(v); }
/*{\Mbinop  returns |\Mvar| translated by vector $v$.}*/

GEN_POLYGON operator-(const VECTOR& v) const { return translate(-v); }
/*{\Mbinop  returns |\Mvar| translated by vector $-v$.}*/


GEN_POLYGON rotate90(const POINT& q, int i=1) const;
/*{\Mopl    returns |\Mvar| rotated about $q$ by an angle of $i\times 90$ 
            degrees. If $i > 0$ the rotation is counter-clockwise otherwise
            it is clockwise. }*/

GEN_POLYGON reflect(const POINT& p, const POINT& q) const;
/*{\Mop     returns |\Mvar| reflected  across the straight line passing
            through $p$ and $q$.}*/

GEN_POLYGON reflect(const POINT& p) const;
/*{\Mop     returns |\Mvar| reflected  across point $p$.}*/

RAT_TYPE    sqr_dist(const POINT& p) const;
/*{\Mop     returns the square of the minimal Euclidean distance 
between a segment in the boundary of |\Mvar| and $p$. Returns zero is |\Mvar|
is trivial. }*/

//list_rat_gen_polygon_ simple_parts() const;
/*{\Xopl    returns the simple parts of $P$ as a list of simple polygons. }*/

GEN_POLYGON make_weakly_simple(bool with_neg_parts = true, 
							   bool strict = false) const;
/*{\Mopl creates a weakly simple generalized polygon |Q| from a possibly
         non-simple polygon |\Mvar| such that |Q| and |\Mvar| have the same
         inner points. 
		 The flag |with_neg_parts| determines whether inner points in 
		 negatively oriented parts are taken into account, too.
 		 If |strict| is true a point is considered an inner point if it is 
		 left of all surrounding segments, otherwise it is considered as an
		 inner point if it is locally to the left of some surrounding edge.
		 (This function is experimental.)}*/

static GEN_POLYGON make_weakly_simple(const POLYGON& Q, 
									  bool with_neg_parts = true, 
									  bool strict = false);
/*{\Mstatic same as above but the input is a polygon |Q|. 
            (This function is experimental.)}*/

GEN_POLYGON complement() const;
/*{\Mopl    returns the complement of $P$.}*/

GEN_POLYGON eliminate_colinear_vertices() const;
/*{\Mopl    returns a copy of $P$ without colinear vertices.}*/

int         side_of(const POINT& p) const;
/*{\Mop     returns $+1$ if $p$ lies to the left of |\Mvar|, $0$ if 
            $p$ lies on |\Mvar|, and $-1$ if $p$ lies to the 
            right of |\Mvar|.}*/ 

region_kind region_of(const POINT& p) const;
/*{\Mop     returns BOUNDED\_REGION if $p$ lies in the 
            bounded region of |\Mvar|, returns ON\_REGION if $p$ lies on 
            |\Mvar|, and returns UNBOUNDED\_REGION if $p$ lies in the 
            unbounded region. The bounded region of the full polygon is the entire plane. }*/


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
/*{\Mop     returns true if $p$ lies to the right of |\Mvar|, i.e., 
            |side_of(p) == -1|.}*/

bool        contains(const POINT& p) const
            { return !outside(p); }
/*{\Mop     returns true if $p$ lies to the left of or on |\Mvar|.}*/



RAT_TYPE    area() const;
/*{\Mop     returns the signed area of the bounded region of $P$. The sign of
the area is positive if the bounded region is the positive side of $P$.
\precond $P$ is not the full polygon. }*/

int         orientation() const 
{ return trivial() ? 0 : ptr()->pol_list.head().orientation(); }
/*{\Mop     returns the orientation of $P$. }*/

list<GEN_POLYGON> regional_decomposition() const;
/*{\Mop     computes a decomposition of the bounded region of $P$ into 
            simple connected components $P1, \ldots, P_n$. 
			If $P$ is trivial the decomposition is $P$ itself. 
			Otherwise, the boundary of every $P_i$ consists of an exterior 
			polygon and zero or more holes nested inside. But the holes do 
			not contain any nested polygons. (Note that $P$ may have holes
			containing nested polygons; they appear as seperate components
			in the decomposition.)
			Every $P_i$ has the same orientation as $P$. If it is positive 
			then $P$ is the union of $P_1, \ldots, P_n$, otherwise $P$ is 
			the intersection of $P_1,\ldots, P_n$.}*/

GEN_POLYGON buffer(RAT_TYPE d, int p = 3) const;
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



/*{\Mtext \newcommand{\reg}{\mathop {\rm reg}} 

All binary boolean operations are regularized, i.e., the result 
$R$ of the standard boolean operation is replaced by the interior 
of the closure of $R$. We use $\reg X$ to denote the regularization 
of a set $X$.

}*/

/*{\Moptions nextwarning=no}*/
GEN_POLYGON unite(const GEN_POLYGON& Q, map<SEGMENT, GenPtr>* ptr_corresp) const;
GEN_POLYGON intersection(const GEN_POLYGON& Q, map<SEGMENT, GenPtr>* ptr_corresp) const;
GEN_POLYGON diff(const GEN_POLYGON& Q, map<SEGMENT, GenPtr>* ptr_corresp) const;
GEN_POLYGON sym_diff(const GEN_POLYGON& Q, map<SEGMENT, GenPtr>* ptr_corresp) const;

/*{\Moptions nextwarning=no}*/
GEN_POLYGON unite(const GEN_POLYGON& Q) const { return unite(Q, 0); }
/*{\Mopl    returns $\reg (P \cup Q)$. }*/


/*{\Moptions nextwarning=no}*/
GEN_POLYGON intersection(const GEN_POLYGON& Q) const { return intersection(Q, 0); }
/*{\Mopl    returns $\reg(P \cap Q)$.}*/

/*{\Moptions nextwarning=no}*/
GEN_POLYGON diff(const GEN_POLYGON& Q) const { return diff(Q, 0); }
/*{\Mopl    returns $\reg(P \setminus Q)$.}*/

/*{\Moptions nextwarning=no}*/
GEN_POLYGON sym_diff(const GEN_POLYGON& Q) const { return sym_diff(Q, 0); }
/*{\Mopl    returns $\reg( (P\cup Q) - (P\cap Q))$.}*/


bool operator==(const GEN_POLYGON& P1) const;


bool operator!=(const GEN_POLYGON& P1) const { return !operator==(P1); }


friend __exportF ostream& operator<<(ostream& out, const GEN_POLYGON& p);
friend __exportF istream& operator>>(istream& in,  GEN_POLYGON& p);

#if ( KERNEL == FLOAT_KERNEL )
/*{\Mtext The following functions are only available for |gen_polygons|. They
have no counterpart for |rat_gen_polygons| or |real_gen_polygons|.

\medskip}*/

gen_polygon  translate_by_angle(double alpha, double d) const;
/*{\Mop     returns |\Mvar| translated in direction |alpha| by distance $d$.}*/

gen_polygon rotate(const point& p, double alpha) const;
/*{\Mop returns |\Mvar| rotated by $\alpha$ degrees about $p$.}*/

gen_polygon rotate(double alpha) const;
/*{\Mop returns |\Mvar| rotated by $\alpha$ degrees about the origin.}*/

double    distance(const point& p) const;
/*{\Mop     returns the Euclidean distance between |\Mvar|
            and $p$.}*/

rat_gen_polygon to_rational(int prec = -1) const;
/*{\Mop     returns a representation of |\Mvar| with rational coordinates
            with precision |prec| (cf. Section \ref{Rational Points}).}*/

#endif

#if ( KERNEL == REAL_KERNEL )
real distance(const real_point& p) const;
rat_gen_polygon to_rational(int prec = -1) const;
#endif

private:

static
GEN_POLYGON unite_parallel_step(const list<GEN_POLYGON> & PL, map<SEGMENT, GenPtr>* ptr_corresp);
};




/*{\Mtext
\bigskip
{\bf Iterations Macros}

{\bf forall\_polygons}($p,P$)       
$\{$ ``the boundary polygons of $P$ are successively assigned to 
POLYGON $p$'' $\}$ 
}*/


#if !defined(forall_polygons)
#define forall_polygons(v,P)  forall(v,(P).polygons())
#endif

LEDA_END_NAMESPACE
