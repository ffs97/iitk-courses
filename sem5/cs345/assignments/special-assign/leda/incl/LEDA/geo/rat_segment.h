/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  rat_segment.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_RAT_SEGMENT_H
#define LEDA_RAT_SEGMENT_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600105
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/geo/rat_point.h>
#include <LEDA/geo/segment.h>

LEDA_BEGIN_NAMESPACE

//------------------------------------------------------------------------------
// rat_segments
//------------------------------------------------------------------------------


class __exportC rat_segment_rep : public handle_rep {

friend class __exportC rat_segment;
friend class __exportC rat_line;

static atomic_counter id_counter;

   rat_point start;
   rat_point end;

   //integer dx;
   //integer dy;

   double dxd;
   double dyd;

   unsigned long id;

public:
   
   rat_segment_rep(const rat_point&, const rat_point&);
   rat_segment_rep();  
  ~rat_segment_rep() {}

   friend inline unsigned long ID_Number(const rat_segment& s);
   
};

/*{\Manpage {rat_segment} {} {Rational Segments} }*/


class __exportC rat_segment  : public HANDLE_BASE(rat_segment_rep) {

/*{\Mdefinition
An instance $s$ of the data type |\Mname| is a directed straight line
segment in the two-dimensional plane, i.e.,
a line segment $[p,q]$ connecting two rational points $p$ and $q$ (cf. 
\ref{Rational Points}). $p$ is called the {\em source} or start point and 
$q$ is called the {\em target} or end point of $s$. A segment is called \emph{trivial} if its source is equal to its target.
}*/
 
  friend class __exportC rat_line;

rat_segment_rep* ptr() const { return (rat_segment_rep*)PTR; }

public:

static int use_filter;

/*{\Mtypes 5}*/ 

typedef rational  coord_type;
/*{\Mtypemember the coordinate type (|rational|).}*/

typedef rat_point point_type;
/*{\Mtypemember the point type (|rat_point|).}*/

typedef segment   float_type;
/*{\Mtypemember the corresponding floatin-point type (|segment|).}*/



/*{\Mcreation s }*/

rat_segment() { PTR = new rat_segment_rep; }
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to the empty segment. }*/

rat_segment(const rat_point& p, const rat_point& q) 
  { PTR = new rat_segment_rep(p,q); }
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to the segment $[p,q]$. }*/

rat_segment(const rat_point& p, const rat_vector& v); 
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to the segment $[p,p+v]$.\\ \precond $v.dim() = 2$. }*/


rat_segment(const rational& x1, const rational& y1,
            const rational& x2, const rational& y2)
{ PTR = new rat_segment_rep(rat_point(x1,y1), rat_point(x2,y2)); }
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to the segment $[(x1,y1),(x2,y2)]$. }*/

/*{\Moptions nextwarning=no }*/
rat_segment(int x1, int y1, int w1, int x2, int y2, int w2) 
{ PTR = new rat_segment_rep(rat_point(x1,y1,w1), rat_point(x2,y2,w2)); }

rat_segment(const integer& x1, const integer& y1, const integer& w1,
            const integer& x2, const integer& y2,const integer& w2) 
{ PTR = new rat_segment_rep(rat_point(x1,y1,w1), rat_point(x2,y2,w2)); }
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to the segment $[(x1,y1,w1),(x2,y2,w2)]$. }*/

/*{\Moptions nextwarning=no }*/
rat_segment(int x1, int y1, int x2, int y2) 
{ PTR = new rat_segment_rep(rat_point(x1,y1), rat_point(x2,y2)); }

rat_segment(const integer& x1, const integer& y1, 
            const integer& x2, const integer& y2) 
{ PTR = new rat_segment_rep(rat_point(x1,y1), rat_point(x2,y2)); }
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to the segment $[(x1,y1),(x2,y2)]$. }*/

rat_segment(const segment& s1, int prec = rat_point::default_precision)
{ PTR = new rat_segment_rep(rat_point(s1.source(),prec), 
                            rat_point(s1.target(),prec)); }
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized to the segment obtained by approximating the two
defining points of $s_1$. }*/
   

rat_segment(const rat_segment& s) : HANDLE_BASE(rat_segment_rep)(s) {}     
 ~rat_segment()                {}

  rat_segment& operator=(const rat_segment& s) 
  { HANDLE_BASE(rat_segment_rep)::operator=(s); return *this;}


/*{\Moperations 2 3.5 }*/


segment to_segment() const;

segment to_float() const { return to_segment(); }
/*{\Mop       returns a floating point approximation of |\Mvar|. }*/

void normalize() const { source().normalize(); target().normalize(); }
/*{\Mop simplifies the homogenous representation by calling
        |source().normalize()| and |target().normlize()|. }*/

/*{\Moptions nextwarning=no }*/
rat_point start()  const { return ptr()->start; }
rat_point source() const { return ptr()->start; }
/*{\Mop       returns the source point of |\Mvar|.}*/

/*{\Moptions nextwarning=no }*/

rat_point end()    const { return ptr()->end; }
rat_point target() const { return ptr()->end; }
/*{\Mop       returns the target point of |\Mvar|.}*/

rat_segment reversal() const;
/*{\Mop       returns the segment |(target(),source())|. }*/


rational xcoord1() const { return ptr()->start.xcoord(); }
/*{\Mop       returns the $x$-coordinate of the source point of |\Mvar|.}*/

rational xcoord2() const { return ptr()->end.xcoord();   }
/*{\Mop       returns the $x$-coordinate of the target point of |\Mvar|.}*/

rational ycoord1() const { return ptr()->start.ycoord(); }
/*{\Mop       returns the $y$-coordinate of the source point of |\Mvar|.}*/

rational ycoord2() const { return ptr()->end.ycoord();   }
/*{\Mop       returns the $y$-coordinate of the target point of |\Mvar|.}*/


double xcoord1D() const { return ptr()->start.xcoordD(); }
/*{\Mop       returns a double precision approximation of |\Mvar.xcoord1()|.}*/

double xcoord2D() const { return ptr()->end.xcoordD();   }
/*{\Mop       returns a double precision approximation of |\Mvar.xcoord2()|.}*/

double ycoord1D() const { return ptr()->start.ycoordD(); }
/*{\Mop       returns a double precision approximation of |\Mvar.ycoord1()|.}*/

double ycoord2D() const { return ptr()->end.ycoordD();   }
/*{\Mop       returns a double precision approximation of |\Mvar.ycoord2()|.}*/

integer X1() const { return ptr()->start.X(); }
/*{\Mop       returns the first homogeneous coordinate of the source point 
              of |\Mvar|.}*/

integer X2() const { return ptr()->end.X();   }
/*{\Mop       returns the first homogeneous coordinate of the target point 
              of |\Mvar|.}*/

integer Y1() const { return ptr()->start.Y(); }
/*{\Mop       returns the second homogeneous coordinate of the source point 
              of |\Mvar|.}*/

integer Y2() const { return ptr()->end.Y();   }
/*{\Mop       returns the second homogeneous coordinate of the target point 
              of |\Mvar|.}*/

integer W1() const { return ptr()->start.W(); }
/*{\Mop       returns the third homogeneous coordinate of the source point 
              of |\Mvar|.}*/

integer W2() const { return ptr()->end.W();   }
/*{\Mop       returns the third homogeneous coordinate of the target point 
              of |\Mvar|.}*/

double XD1() const { return ptr()->start.XD(); }
/*{\Mop       returns a floating point approximation of |\Mvar.X1()|.}*/

double XD2() const { return ptr()->end.XD();   }
/*{\Mop       returns a floating point approximation of |\Mvar.X2()|.}*/

double YD1() const { return ptr()->start.YD(); }
/*{\Mop       returns a floating point approximation of |\Mvar.Y1()|.}*/

double YD2() const { return ptr()->end.YD();   }
/*{\Mop       returns a floating point approximation of |\Mvar.Y2()|.}*/

double WD1() const { return ptr()->start.WD(); }
/*{\Mop       returns a floating point approximation of |\Mvar.W1()|.}*/

double WD2() const { return ptr()->end.WD();   }
/*{\Mop       returns a floating point approximation of |\Mvar.W2()|.}*/


//integer dx() const { return ptr()->dx; }
//integer dy() const { return ptr()->dy; }

integer dx() const { return ptr()->end.X() * ptr()->start.W()
                          - ptr()->start.X() * ptr()->end.W(); }
/*{\Mop       returns the normalized $x$-difference $X2\cdot W1 - X1\cdot W2$
              of |\Mvar|. }*/

integer dy() const { return ptr()->end.Y() * ptr()->start.W()
                          - ptr()->start.Y() * ptr()->end.W(); }
/*{\Mop       returns the normalized $y$-difference $Y2\cdot W1 - Y1\cdot W2$
              of |\Mvar|. }*/

double dxD() const { return ptr()->dxd; }
/*{\Mop       returns a floating point approximation of |\Mvar.dx()|.}*/

double dyD() const { return ptr()->dyd; }
/*{\Mop       returns a floating point approximation of |\Mvar.dy()|.}*/



bool is_trivial() const { return start() == end(); }
/*{\Mopl    returns true if |\Mvar| is trivial. }*/


bool is_vertical()   const { return rat_point::cmp_x(start(),end()) == 0; }
/*{\Mopl    returns true if |\Mvar| is vertical. \\
\precond |\Mvar| is non-trivial.}*/

bool is_horizontal() const { return rat_point::cmp_y(start(),end()) == 0; }
/*{\Mopl    returns true if |\Mvar| is horizontal.\\
\precond |\Mvar| is non-trivial. }*/

rational slope() const { return rational(dy(), dx()); }
/*{\Mop     returns the slope of $s$.\\
\precond |\Mvar| is not vertical.}*/

int cmp_slope(const rat_segment& s1) const; 
/*{\Mopl    compares the slopes of |\Mvar| and $s_1$. \\
\precond |\Mvar| and $s_1$ are non-trivial.}*/

int orientation(const rat_point& p) const;
/*{\Mopl    computes orientation($a$, $b$, $p$) (see below), where $a \not= b$
            and $a$ and $b$ appear in this order on segment $s$. }*/


rational  x_proj(rational y) const;
/*{\Mop     returns $p$.xcoord(), where $p \in line(|\Mvar|)$ with 
            $p$.ycoord() = $y$.\\
\precond |\Mvar| is not horizontal.}*/

rational  y_proj(rational x) const;
/*{\Mop     returns $p$.ycoord(), where $p \in line(|\Mvar|)$ with 
            $p$.xcoord() = $x$.\\
\precond |\Mvar| is not vertical.}*/

rational  y_abs() const;
/*{\Mop     returns the y-abscissa of $line(\Mvar)$, i.e., |\Mvar|.y\_proj(0).\\
\precond |\Mvar|  is not vertical.}*/


bool   contains(const rat_point& p) const;
/*{\Mopl    decides whether |\Mvar| contains $p$. }*/

bool intersection(const rat_segment& t) const;
/*{\Mopl    decides whether |\Mvar| and $t$ intersect. }*/

bool intersection(const rat_segment& t, rat_point& p) const;
/*{\Mopl    decides whether |\Mvar| and $t$ intersect. If so, some point of 
intersection is assigned to $p$.
 }*/

bool intersection(const rat_segment& t, rat_segment& inter) const;
/*{\Mopl    decides whether |\Mvar| and $t$ intersect. If so, the segment formed by the points of intersection is assigned to |inter|.
 }*/


bool intersection_of_lines(const rat_segment& t, rat_point& p) const;
/*{\Mopl    decides if the lines supporting |\Mvar| and $t$ intersect in a
            single point. If so, the point of intersection is assigned to |p|.\\
            \precond |\Mvar| and |t| are nontrivial. }*/

bool overlaps(const rat_segment& t) const
{ rat_segment inter; return intersection(t, inter) && !inter.is_trivial(); }
/*{\Mopl    decides whether |\Mvar| and $t$ overlap, i.e. they have a non-trivial intersection.
 }*/


rat_segment translate(const rational& dx, const rational& dy) const;
/*{\Mopl    returns |\Mvar| translated by vector $(dx,dy)$.}*/


rat_segment translate(const integer& dx, const integer& dy, const integer& dw) const;
/*{\Mopl    returns |\Mvar| translated by vector $(dx/dw,dy/dw)$.}*/


rat_segment translate(const rat_vector& v) const;
/*{\Mopl    returns $s+v$, i.e.,  |\Mvar| translated by vector $v$.\\
            \precond $v$.dim() = 2.}*/ 

rat_segment operator+(const rat_vector& v) const { return translate(v); }
/*{\Mbinop  returns |\Mvar| translated by vector $v$.}*/

rat_segment operator-(const rat_vector& v) const { return translate(-v); }
/*{\Mbinop  returns |\Mvar| translated by vector $-v$.}*/


rat_segment rotate90(const rat_point& q, int i=1) const;
/*{\Mopl    returns |\Mvar| rotated about $q$ by an angle of $i\times 90$ 
            degrees. If $i > 0$ the rotation is counter-clockwise otherwise
            it is clockwise. }*/
 
rat_segment rotate90(int i=1) const;
/*{\Mopl    returns |\Mvar| rotated about the origin by an angle of 
            $i\times 90$ degrees. }*/


rat_segment reflect(const rat_point& p, const rat_point& q) const;
/*{\Mop     returns |\Mvar| reflected  across the straight line passing
            through $p$ and $q$.}*/

rat_segment reflect(const rat_point& p) const;
/*{\Mop     returns |\Mvar| reflected  across point $p$. }*/


rat_segment reverse() const { return rat_segment(target(),source()); }
/*{\Mop     returns |\Mvar| reversed.}*/


rat_segment perpendicular(const rat_point& p) const;
/*{\Mop    returns the segment perpendicular to |\Mvar| with source $p$
           and target on $|line|(\Mvar)$.\\
\precond |\Mvar| is nontrivial.}*/

rational   sqr_length() const;
/*{\Mop    returns the square of the length of |\Mvar|. }*/

rational  sqr_dist(const rat_point& p) const;
/*{\Mopl   returns the squared Euclidean distance between $p$ and $|\Mvar|$.}*/

rational  sqr_dist() const { return sqr_dist(rat_point(0,0,1)); }
/*{\Mopl    returns the squared distance between |\Mvar| and the origin. }*/

rat_vector  to_vector() const { return target() - source(); }
/*{\Mop returns the vector |\Mvar.target() - \Mvar.source()|. }*/


bool operator==(const rat_segment& t) const
{ return (ptr()->start == t.ptr()->start && ptr()->end == t.ptr()->end); }
/*{\Mbinop returns true if |\Mvar| and |t| are equal as oriented segments}*/

bool operator!=(const rat_segment& t) const { return !operator==(t);}

friend inline
int equal_as_sets(const rat_segment& s, const rat_segment& t)
{return ( s == t || s == t.reversal() ); }
/*{\Mfunc returns true if |s| and |t| are equal as unoriented segments}*/



friend __exportF ostream& operator<<(ostream& O, const rat_segment& s);
friend __exportF istream& operator>>(istream& I, rat_segment& s);


/*{\Mtext
{\bf Non-Member Functions}
\smallskip
}*/

friend __exportF int cmp_slopes(const rat_segment& s1, const rat_segment& s2);
/*{\Mfuncl      returns compare(slope($s_1$), slope($s_2$)).}*/

friend __exportF int cmp_segments_at_xcoord(const rat_segment& s1,
                                            const rat_segment& s2,
                                            const rat_point& p);
/*{\Mfuncl      compares points $l_1 \cap v$ and $l_2 \cap v$ where
                $l_i$ is the line underlying segment $s_i$ and $v$ is
                the vertical straight line passing through point $p$. }*/

friend unsigned long ID_Number(const rat_segment& s) { return s.ptr()->id; }

};


inline int orientation(const rat_segment& s, const rat_point& p)
{ return s.orientation(p); }
/*{\Mfuncl   computes orientation($a$, $b$, $p$), where $a \not= b$
             and $a$ and $b$ appear in this order on segment $s$. }*/



inline int parallel(const rat_segment& s1, const rat_segment& s2)
{ return cmp_slopes(s1,s2) == 0; }


inline const char* leda_tname(const rat_segment*) { return "rat_segment"; }

#if LEDA_ROOT_INCL_ID == 600105
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

