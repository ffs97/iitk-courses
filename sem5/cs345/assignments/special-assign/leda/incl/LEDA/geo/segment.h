/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  segment.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_SEGMENT_H
#define LEDA_SEGMENT_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600106
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/geo/point.h>

LEDA_BEGIN_NAMESPACE

//------------------------------------------------------------------------------
// segments
//------------------------------------------------------------------------------


class __exportC segment_rep : public handle_rep {

friend class __exportC segment;
friend class __exportC line;
friend class __exportC circle;

static atomic_counter id_counter;

   unsigned long id;

   point start;
   point end;

   double dx;
   double dy;

public:
   
   segment_rep(const point&, const point&);
  ~segment_rep() {}

   friend inline unsigned long ID_Number(const segment&);

};

/*{\Manpage {segment} {} {Segments}}*/

class __exportC segment  : public HANDLE_BASE(segment_rep)
{
/*{\Mdefinition
    An instance $s$ of the data type |\Mname| is a directed straight line
    segment in the two-dimensional plane, i.e., a straight line segment $[p,q]$
    connecting two points $p,q \in \real ^2$. $p$ is called the {\em source} or 
    start point and $q$ is called the {\em target} or end point of $s$. The 
    length of $s$ is the Euclidean distance between $p$ and $q$. If $p = q$
    $s$ is called empty. We use |line(s)| to denote a straight line
    containing $s$. The angle between a right oriented horizontal ray and 
    $s$ is called the direction of $s$.}*/

friend class __exportC line;
friend class __exportC circle;

segment_rep* ptr() const { return (segment_rep*)PTR; }

public:

/*{\Mtypes 5}*/ 

typedef double  coord_type;
/*{\Mtypemember the coordinate type (|double|).}*/

typedef point   point_type;
/*{\Mtypemember the point type (|point|).}*/

typedef segment float_type;



/*{\Mcreation s}*/

segment(const point& p, const point& q); 
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to the segment $[p,q]$. }*/

segment(const point& p, const vector& v); 
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to the segment $[p,p+v]$.\\
	    \precond $v.dim() = 2$. }*/

segment(double x1, double y1, double x2, double y2) ;
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to the segment $[(x_1,y_1),(x_2,y_2)]$.}*/ 

segment(const point& p, double alpha, double length);
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to the segment with start point $p$, 
            direction $alpha$, 
            and length $length$.}*/

segment();                 
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to the empty segment.}*/

segment(const segment& s1, int) : HANDLE_BASE(segment_rep)(s1) {} 
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to a copy of $s_1$. }*/

 segment(const segment& s) : HANDLE_BASE(segment_rep)(s) {}     
~segment() {}
 segment& operator=(const segment& s) 
 { HANDLE_BASE(segment_rep)::operator=(s); return *this;}


/*{\Moperations 2 3.5}*/

segment  to_segment() const { return *this; }

segment  to_float() const { return *this; }
/*{\Xop  for compatibility with rat_segment. }*/

void    normalize() const {}
/*{\Xop  for compatibility with |rat_segment|. }*/


/*{\Moptions nextwarning=no}*/
point start()  const      { return ptr()->start; }
point source() const      { return ptr()->start; }
/*{\Mop       returns the source point of segment |\Mvar|.}*/

/*{\Moptions nextwarning=no}*/
point end()    const      { return ptr()->end; }
point target() const      { return ptr()->end; }
/*{\Mop       returns the target point of segment |\Mvar|.}*/

double xcoord1() const    { return ptr()->start.ptr()->x; }
/*{\Mop       returns the x-coordinate of |\Mvar|.source().}*/

double xcoord2() const    { return ptr()->end.ptr()->x;   }
/*{\Mop       returns the x-coordinate of |\Mvar|.target().}*/

double ycoord1() const    { return ptr()->start.ptr()->y; }
/*{\Mop       returns the y-coordinate of |\Mvar|.source().}*/

double ycoord2() const    { return ptr()->end.ptr()->y;   }
/*{\Mop       returns the y-coordinate of |\Mvar|.target().}*/


operator vector()  
{ return vector(xcoord2()-xcoord1(), ycoord2()-ycoord1()); }


double dx() const    { return ptr()->dx;   }
/*{\Mop       returns the $xcoord2 - xcoord1$.}*/

double dy() const    { return ptr()->dy;   }
/*{\Mop       returns the $ycoord2 - ycoord1$.}*/


double slope() const;
/*{\Mop       returns the slope of $s$.\\
	      \precond  |\Mvar|  is not vertical.}*/


double sqr_length() const;
/*{\Mop       returns the square of the length of |\Mvar|.}*/

double length() const;
/*{\Mop       returns the length of |\Mvar|.}*/

vector  to_vector() const { return target() - source(); }
/*{\Mop returns the vector |\Mvar.target() - \Mvar.source()|. }*/

double direction() const { return angle(); }
/*{\Mop       returns the direction of |\Mvar| as an angle in 
	      the intervall $[0,2\pi)$.}*/

double angle()     const;
/*{\Mop       returns |\Mvar|.direction().}*/

double  angle(const segment& t) const;
/*{\Mop       returns the angle between |\Mvar| and $t$, i.e., 
              $t$.direction() - |\Mvar|.direction().}*/


bool is_trivial() const { return ( ptr()->dx == 0 && ptr()->dy == 0); }
/*{\Mopl    returns true if |\Mvar| is trivial. }*/


bool is_vertical()   const { return xcoord1() == xcoord2(); }
/*{\Mop       returns true iff |\Mvar| is vertical.}*/

bool is_horizontal() const { return ycoord1() == ycoord2(); }
/*{\Mop       returns true iff |\Mvar| is horizontal.}*/

int orientation(const point& p) const
{ return compare(dy()*(xcoord1()-p.xcoord()), dx()*(ycoord1()-p.ycoord())); }
/*{\Mop   computes orientation($\Mvar.source()$, $\Mvar.target()$, $p$) (see below).}*/

double  x_proj(double y) const;
/*{\Mop    returns $p$.xcoord(), where $p \in line(|\Mvar|)$ with 
           $p$.ycoord() = $y$.\\
	   \precond |\Mvar| is not horizontal.}*/

double  y_proj(double x) const;
/*{\Mop    returns $p$.ycoord(), where $p \in line(|\Mvar|)$ with 
           $p$.xcoord() = $x$.\\
           \precond |\Mvar| is not vertical.}*/

double  y_abs() const;
/*{\Mop    returns the y-abscissa of $line(\Mvar)$, i.e., |\Mvar|.y\_proj(0).\\
	   \precond  |\Mvar|  is not vertical.}*/


bool   contains(const point& p) const;
/*{\Mopl    decides whether |\Mvar| contains $p$. }*/


bool intersection(const segment& t) const;
/*{\Mopl    decides whether |\Mvar| and $t$ intersect in one point. }*/


bool intersection(const segment& t, point& p) const;
/*{\Mopl    if |\Mvar| and $t$ intersect in a single point this point
	    is assigned to $p$ and the result is true, otherwise the 
             result is false.}*/

bool intersection_of_lines(const segment& t, point& p) const;
/*{\Mopl    if $line(\Mvar)$ and $line(t)$ intersect in a single point
            this point is assigned to $p$ and the result is true, otherwise
            the result is false.}*/


segment translate_by_angle(double alpha, double d) const;
/*{\Mopl    returns |\Mvar| translated in direction $alpha$ by distance $d$.}*/

segment translate(double dx, double dy) const;
/*{\Mopl    returns |\Mvar| translated by vector $(dx,dy)$.}*/

segment translate(const vector& v) const;
/*{\Mop     returns $s+v$, i.e.,  |\Mvar| translated by vector $v$.\\
	    \precond $v$.dim() = 2.}*/ 

segment operator+(const vector& v) const { return translate(v); }
/*{\Mbinop  returns |\Mvar| translated by vector $v$.}*/

segment operator-(const vector& v) const { return translate(-v); }
/*{\Mbinop  returns |\Mvar| translated by vector $-v$.}*/


segment perpendicular(const point& p) const;
/*{\Mop    returns the segment perpendicular to |\Mvar| with source $p$
           and target on $line(\Mvar)$.}*/

double  distance(const point& p) const;
/*{\Mopl   returns the Euclidean distance between $p$ and $|\Mvar|$.}*/

double  sqr_dist(const point& p) const;
/*{\Mopl   returns the squared Euclidean distance between $p$ and $|\Mvar|$.}*/

double  distance() const { return distance(point(0,0)); }
/*{\Mopl   returns the Euclidean distance between $(0,0)$ 
           and $|\Mvar|$.}*/

segment rotate(const point& q, double a) const;
/*{\Mopl    returns |\Mvar| rotated about point $q$ by angle $a$.}*/

segment rotate(double alpha) const;
/*{\Mop     returns $s$.rotate($s$.source(), $alpha$).}*/

segment rotate90(const point& q, int i=1) const;
/*{\Mopl    returns |\Mvar| rotated about $q$ by an angle of $i\times 90$ 
            degrees. If $i > 0$ the rotation is counter-clockwise otherwise
            it is clockwise. }*/

segment rotate90(int i=1) const;
/*{\Mop     returns $s$.rotate90($s$.source(),i).}*/


segment reflect(const point& p, const point& q) const;
/*{\Mop     returns |\Mvar| reflected  across the straight line passing
            through $p$ and $q$.}*/

segment reflect(const point& p) const;
/*{\Mop     returns |\Mvar| reflected  across point $p$.}*/


segment reverse() const { return segment(target(),source()); }
/*{\Mop     returns |\Mvar| reversed.}*/


bool operator==(const segment& t) const
{ return (ptr()->start == t.ptr()->start && ptr()->end == t.ptr()->end); }

bool operator!=(const segment& t) const { return !operator==(t);}


friend __exportF istream& operator>>(istream& I, segment& s);

friend inline unsigned long ID_Number(const segment&);

};



inline ostream& operator<<(ostream& out, const segment& s) 
{ return out << "[" << s.start() << "===" << s.end() << "]"; } 



/*{\Mtext
{\bf Non-Member Functions}
\smallskip
}*/

inline unsigned long ID_Number(const segment& s) { return s.ptr()->id; }


inline int orientation(const segment& s, const point& p)
{ return s.orientation(p); }
/*{\Mfuncl   computes orientation($s.source()$, $s.target()$, $p$).}*/


inline int cmp_slopes(const segment& s1, const segment& s2)
/*{\Mfuncl   returns compare(slope($s_1$), slope($s_2$)).}*/
{ return compare(s1.slope(), s2.slope()); }

extern __exportF int cmp_segments_at_xcoord(const segment& s1,
                                            const segment& s2,
                                            const point& p);
/*{\Mfuncl      compares points $l_1 \cap v$ and $l_2 \cap v$ where
                $l_i$ is the line underlying segment $s_i$ and $v$ is
                the vertical straight line passing through point $p$. }*/


inline bool parallel(const segment& s1, const segment& s2)
/*{\Mfuncl   returns true if $s_1$ and $s_2$ are parallel and false otherwise.}*/
{ return cmp_slopes(s1,s2) == 0; }


inline const char* leda_tname(const segment*) { return "segment"; }

#if LEDA_ROOT_INCL_ID == 600106
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

