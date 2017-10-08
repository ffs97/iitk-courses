/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  r_circle_segment.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_R_CIRCLE_SEGMENT_H
#define LEDA_R_CIRCLE_SEGMENT_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600099
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/geo/r_circle_point.h>
#include <LEDA/graph/graph.h> 

LEDA_BEGIN_NAMESPACE

//------------------------------------------------------------------------------
// r_circle_segment: segment of rational circle
// SWEEP_SEGMENTS(const list<r_circle_segment>& S, ...)
//------------------------------------------------------------------------------

/// r_circle_segment_rep /////////////////////////////////////////////////////////////////////

class r_circle_segment_rep : public handle_rep {
public:
	r_circle_segment_rep(const r_circle_point& s, const r_circle_point& t, const rat_circle& c)
	 : Source(s), Target(t), Circle(c)
	{}

	friend __exportC class r_circle_segment;

private:
	r_circle_point Source, Target;
	rat_circle     Circle; 
	// the orientation of the Circle determines which of the two arcs is represented
	// by this object

public:
	LEDA_MEMORY(r_circle_segment_rep);
};

/// r_circle_segment /////////////////////////////////////////////////////////////////////////

/*{\Mtext \clearpage}*/
/*{\Manpage {r_circle_segment} {} {Segment of Rational Circle} {cs}}*/

class __exportC r_circle_segment : public HANDLE_BASE(r_circle_segment_rep) {
/*{\Mdefinition
An instance |\Mvar| of type |\Mtype| is a segment of a rational circle 
(see Section~\ref{rat_circle}), i.e.~a circular arc.
A segment is called \emph{trivial} if it consists of a single point. 
A non-trivial instance |\Mvar| is defined by two points |s| and |t| (of type
|r_circle_point|) and an oriented circle |c| (of type |rat_circle|) such that 
|c| contains both |s| and |t|. 
We call |s| and |t| the \emph{source} and the \emph{target} of |\Mvar|, and 
|c| is called its \emph{supporting circle}. 
We want to point out that the circle may be a line, which means that |\Mvar|
is a straight line segment. An instance |\Mvar| is called \emph{degenerate}, 
if it is trivial or a straight line segment.
}*/
public:
	typedef r_circle_segment_rep rep;
	typedef HANDLE_BASE(rep)     base;

private:
	rep* ptr() const { return (rep*) PTR; }

public:
/*{\Mcreation}*/

	r_circle_segment()
	{ init_as_trivial(r_circle_point()); }
	/*{\Mcreate creates a trivial instance |\Mvar| with source and target 
	equal to the point $(0,0)$.}*/

	r_circle_segment(const r_circle_point& src, const r_circle_point& tgt, const rat_circle& c);
	/*{\Mcreate creates an instance |\Mvar| with source |src|, target |tgt| 
	and supporting circle |c|.\\
	\precond $|src| \neq |tgt|$, |c| is not trivial and contains |src| and 
	|tgt|.}*/

	r_circle_segment(const r_circle_point& src, const r_circle_point& tgt, const rat_line& l);
	/*{\Mcreate creates an instance |\Mvar| with source |src|, target |tgt| 
	and supporting line |l|.\\
	\precond $|src| \neq |tgt|$, |l| contains |src| and |tgt|.}*/

	r_circle_segment(const rat_point& src, const rat_point& middle, const rat_point& tgt);
	/*{\Mcreate creates an instance |\Mvar| with source |src| and target |tgt| 
	which passes through |middle|.\\
	\precond the three points are distinct.}*/

	r_circle_segment(const r_circle_point& p)
	{ init_as_trivial(p); }
	/*{\Mcreate creates a trivial instance |\Mvar| with source and target 
	equal to |p|.}*/

	r_circle_segment(const rat_point& rat_pnt)
	{ init_as_trivial( r_circle_point(rat_pnt) ); }
	/*{\Mcreate creates a trivial instance |\Mvar| with source and target 
	equal to |rat_pnt|.}*/

	r_circle_segment(const rat_circle& c);
	/*{\Mcreate creates an instance |\Mvar| which is equal to the full 
	circle |c|.\\
	\precond |c| is not degenerate.}*/

	r_circle_segment(const rat_point& src, const rat_point& tgt)
	{ init_as_segment(src, tgt); }
	/*{\Mcreate creates an instance |\Mvar| which is equal to the straight 
	line segment from |src| to |tgt|.}*/

	r_circle_segment(const rat_segment& s)
	{ init_as_segment(s.source(), s.target()); }
	/*{\Mcreate creates an instance |\Mvar| which is equal to the straight 
	line segment |s|.}*/

	r_circle_segment(const r_circle_point& src, const r_circle_point& tgt);
	/*{\Mcreate creates an instance |\Mvar| which is equal to the straight 
	line segment from |src| to |tgt|.\\
	\precond Both |src| and |tgt| are |rat_points|.}*/

	r_circle_segment(const r_circle_segment& cs) : base(cs) {}

	const r_circle_segment& operator=(const r_circle_segment& cs)
	{ base::operator=(cs); return *this; }

/*{\Moperations}*/
	void normalize();
	/*{\Mop simplifies the internal representation of |\Mvar|.}*/

	r_circle_segment clone() const
	{ return r_circle_segment(circle(), source(), target()); }
	// same segment but different ID_Number!

	bool verify(bool report_errors = true) const;

	const r_circle_point& source() const { return ptr()->Source; }
	/*{\Mop returns the source of |\Mvar|.}*/

	const r_circle_point& target() const { return ptr()->Target; }
	/*{\Mop returns the target of |\Mvar|.}*/

	const rat_circle& circle() const { return ptr()->Circle; }
	/*{\Mop returns the supporting circle of |\Mvar|.}*/

	rat_line supporting_line() const 
	{ return rat_line(circle().point1(), circle().point3()); }
	/*{\Mop returns a line containing |\Mvar|.\\
	\precond |\Mvar| is a straight line segment.}*/

	rat_point center() const { return circle().center(); }
	/*{\Mop returns the center of the supporting circle of |\Mvar|.}*/

	int orientation() const { return circle().orientation(); }
	/*{\Mop returns the orientation (of the supporting circle) of |\Mvar|.}*/

	real_point real_middle() const;
	/*{\Mop returns the middle point of |\Mvar|, i.e. the intersection of |\Mvar| 
	and the bisector of its source and target.}*/

	r_circle_point middle() const;
	/*{\Mop returns a point on the circle of |\Mvar|, which is close to 
	|real_middle()|.}*/

	bool is_trivial() const { return circle().is_trivial(); }
	/*{\Mop returns true iff |\Mvar| is trivial.}*/

	bool is_degenerate() const { return circle().is_degenerate(); }
	/*{\Mop returns true iff |\Mvar| is degenerate.}*/

	bool is_full_circle() const { return !is_trivial() && identical(source(),target()); }
	/*{\Mop returns true iff |\Mvar| is a full circle.}*/

	bool is_proper_arc() const { return !is_degenerate() && !identical(source(),target()); }
	/*{\Mop returns true iff |\Mvar| is a proper arc, i.e.~neither degenerate 
	        nor a full circle.}*/

	bool is_straight_segment() const { return circle().is_line(); }
	/*{\Mop returns true iff |\Mvar| is a straight line segment.}*/

	bool is_vertical_segment() const 
	{ return circle().is_line() && circle().point1().xcoord() == circle().point3().xcoord(); }
	/*{\Mop returns true iff |\Mvar| is a vertical straight line segment.}*/

	bool is_rat_segment() const 
	{ return is_degenerate() && source().is_rat_point() && target().is_rat_point(); }
	/*{\Mop returns true, if |\Mvar| can be converted to |rat_segment|.
	(The value false means ``do not know''.)}*/

	rat_segment to_rat_segment() const 
	{ return rat_segment(source().to_rat_point(), target().to_rat_point()); }
	/*{\Mop converts |\Mvar| to a |rat_segment|.\\
	\precond |is_rat_segment| returns true.}*/

	bool contains(const r_circle_point& p) const;
	/*{\Mop returns true iff |\Mvar| contains |p|.}*/

	bool overlaps(const r_circle_segment& cs2) const;
	/*{\Mop returns true iff |\Mvar| (properly) overlaps |cs2|.}*/

	bool overlaps(const r_circle_segment& cs2, 
		          const r_circle_point& common1, const r_circle_point& common2) const;
	// precond: cs and cs2 both contain the distinct points common1 and common2

	bool wedge_contains(const real_point& p) const;
	/*{\Mop returns true iff the (closed) wedge induced by |\Mvar| contains
	|p|. This wedge is spanned by the rays which start at the center and pass
	through source and target. (Note that |p| belongs to |\Mvar| iff |p| is on
	the supporting circle and the wedge contains |p|.)}*/

	r_circle_segment reverse() const
	{ return r_circle_segment(target(), source(), circle().reverse()); }
	/*{\Mop returns the reversal of [[\Mvar]], i.e.~source and target are 
	swapped and the supporting circle is reversed.}*/

	r_circle_segment round(int prec = 0) const;
	/*{\Mop returns a rounded representation of |\Mvar|. (experimental)}*/


	r_circle_segment translate(rational dx, rational dy) const;
	/*{\Mopl    returns |\Mvar| translated by vector $(dx,dy)$.}*/

	r_circle_segment translate(const rat_vector& v) const { return translate(v[0], v[1]); }
	/*{\Mop     returns |\Mvar| translated by vector $v$.}*/

	r_circle_segment operator+(const rat_vector& v) const { return translate(v); }
	/*{\Mbinop  returns |\Mvar| translated by vector $v$.}*/

	r_circle_segment operator-(const rat_vector& v) const { return translate(-v); }
	/*{\Mbinop  returns |\Mvar| translated by vector $-v$.}*/

	r_circle_segment rotate90(const rat_point& q, int i=1) const;
	/*{\Mopl    returns |\Mvar| rotated about $q$ by an angle of $i\times 90$ 
				degrees. If $i > 0$ the rotation is counter-clockwise otherwise
				it is clockwise. }*/

	r_circle_segment reflect(const rat_point& p, const rat_point& q) const;
	/*{\Mop     returns |\Mvar| reflected  across the straight line passing
				through $p$ and $q$.}*/

	r_circle_segment reflect(const rat_point& p) const;
	/*{\Mop     returns |\Mvar| reflected  across point $p$.}*/


	list<r_circle_point> intersection(const rat_line& l) const;
	/*{\Mop computes $|\Mvar| \cap l$ (ordered along $l$).}*/

	list<real_point> intersection(const real_line& l) const;
	/*{\Mop as above.}*/

	list<r_circle_point> intersection(const rat_circle& c) const
	{ return intersection(r_circle_segment(c)); }
	/*{\Mop computes $|\Mvar| \cap c$ (ordered lexicographically).}*/

	list<r_circle_point> intersection(const r_circle_segment& cs2) const;
	/*{\Mop computes $|\Mvar| \cap |cs2|$ (ordered lexicographically).}*/

	real sqr_dist(const real_point& p) const;
	/*{\Mop computes the squared Euclidean distance between |\Mvar| and $p$.}*/

	real dist(const real_point& p) const { return sqrt(sqr_dist(p)); }
	/*{\Mop computes the euclidean distance between |\Mvar| and $p$.}*/

	real_line tangent_at(const r_circle_point& p) const;
	/*{\Mop computes the tanget to |\Mvar| at $p$.\\
	\precond |\Mvar| is not trivial.}*/

	real_line tangent_at_src() const { return tangent_at(source()); }
	// This function must guarantee tangent.point1() == source()

	int orientation_of_p_wrt_supporting_line(const r_circle_point& p) const;

	r_circle_segment trim(const r_circle_point& new_src, const r_circle_point& new_tgt) const;

	r_circle_segment trim(const r_circle_point& new_src, const r_circle_point& new_tgt, 
		                  bool& same_direction) const;

	double approximate_area() const;
	/*{\Mop computes the (oriented) area enclosed by the convex hull of |\Mvar|.}*/

	void compute_bounding_box(real& xmin, real& ymin, real& xmax, real& ymax) const;
	/*{\Mop computes a tight bounding box for |\Mvar|.}*/

	r_circle_point lex_smallest_point() const;

	int compare_y(const r_circle_point& p) const;
	// precond: vertical line through p intersects *this in a point q=(qx,qy)
	//          and *this is x-monotonous (or vertical) and source() <_lex target()
	// returns compare(p.ycoord(), qy)

	rational sqr_radius() const { return circle().sqr_radius(); }
	real radius() const { return _real_circle().radius(); }
	real_circle _real_circle() const { return real_circle(circle()); }
	real_point real_center() const { return real_point(center()); }

	list<point> approximate(double dist) const;
	/*{\Mop approximates |\Mvar| by a sequence of points. Connecting the points
	with straight line segments yields a chain with the following property:
	The maximum distance from a point on |\Mvar| to the chain is bounded by 
	|dist|.}*/

	list<rat_point> approximate_by_rat_points(double dist) const;
	/*{\Mop as above, returns |rat_points| instead of |points|.}*/

	list<rat_segment> approximate_by_rat_segments(double dist) const;
	/*{\Mop approximates |\Mvar| by a chain of |rat_segments|. The maximum 
	distance from a point on |\Mvar| to the chain is bounded by |dist|.}*/

	list<r_circle_point> compute_split_points(double dist) const;

	r_circle_segment trim_approximate(const rat_point& new_src, const rat_point& new_tgt) const;

	friend unsigned long ID_Number(const r_circle_segment& cs) 
	{ //return *((unsigned long*) &cs.PTR); 
          union { GenPtr ptr; unsigned long i; } u = {cs.PTR};
          return u.i;
         }

public:
	static list<r_circle_segment> split_circle(const rat_circle& c);

private:
	r_circle_segment(const rat_circle& c, const r_circle_point& src, const r_circle_point& tgt)
	{ PTR = new rep(src, tgt, c); } 
		// special constructor for movement ops that avoids checks

	void init_as_trivial(const r_circle_point& p) { PTR = new rep(p, p, rat_circle()); }
	void init_as_segment(const rat_point& src, const rat_point& tgt);
	
public:
	LEDA_MEMORY(r_circle_segment);
};

inline bool operator==(const r_circle_segment& c1, const r_circle_segment& c2)
{ 
	return identical(c1,c2) 
		|| (c1.source()==c2.source() && c1.target()==c1.target() && c1.circle()==c2.circle());
}

inline bool operator!=(const r_circle_segment& c1, const r_circle_segment& c2)
{ return !operator==(c1,c2); }

__exportF bool equal_as_sets(const r_circle_segment& cs1, const r_circle_segment& cs2);
/*{\Mfuncl returns whether |cs1| and |cs2| describe the same set of points.}*/

__exportF int compare_tangent_slopes(const r_circle_segment& cs1, const r_circle_segment& cs2, 
                                     const r_circle_point& p);
/*{\Mfuncl compares the slopes of the tangents to |cs1| and |cs2| in the point
|p|.\\ \precond |cs1| and |cs2| contain |p|.}*/

__exportF ostream& operator<<(ostream& os, const r_circle_segment& cs);
__exportF istream& operator>>(istream& is, r_circle_segment& cs);

/*{\Mtext 
We provide the operator [[<<]] to display an instance |\Mvar| of type |\Mtype|
in a |window| and the operator [[>>]] for reading |\Mvar| from a |window| 
(see |real_window.h|).}*/

inline const char* leda_tname(const r_circle_segment*) { return "r_circle_segment"; }

/// sweep routines ///////////////////////////////////////////////////////////////////////////

__exportF void SWEEP_SEGMENTS(const list<r_circle_segment>& L, 
							  GRAPH<r_circle_point,r_circle_segment>& G, 
							  bool embed = true);
/*{\Mfuncl 
takes as input a list $L$ of |r_circle_segments| and computes 
the planar graph $G$ induced by the segments in $L$. 
The nodes of $G$ are all endpoints and all proper intersection points of 
segments in $L$. The edges of $G$ are the maximal relatively open
subsegments of segments in $L$ that contain no node of $G$. The edges are
directed as the corresponding segments, if |embed| is false.  
Otherwise, the corresponding planar map is computed. Note that for each edge
$e$ $G[e]$ is the input segment containing $e$.\\
The algorithm (a variant of \cite{BO79}) runs in time $O((n+s)\log n) + m)$, 
where $n$ is the number of segments, $s$ is the number of vertices of the 
graph $G$, and $m$ is the number of edges of $G$. If $L$ contains no 
overlapping segments then $m = O(n + s)$.}*/

__exportF void SWEEP_SEGMENTS(const list<r_circle_segment>& L, 
							  GRAPH<r_circle_point,r_circle_segment>& G, 
							  edge_array<edge>& corresp_edge, 
							  bool embed = true);

__exportF void SWEEP_SEGMENTS(const list<r_circle_segment>& S, 
							  const map<r_circle_segment, unsigned long>& colour_map, 
							  GRAPH<r_circle_point,r_circle_segment>& G, 
							  bool embed = true);

__exportF void SWEEP_SEGMENTS_MC(const list<r_circle_segment>& S, 
							     const map<r_circle_segment, unsigned long>& colour_map, 
							     GRAPH<r_circle_point,r_circle_segment>& G, 
							     edge_array<edge>& corresp_edge, 
							     bool embed = true);
	// multi-colour version

__exportF void SWEEP_SEGMENTS_SC(const list<r_circle_segment>& S, 
							     const map<r_circle_segment, int>& colour_map, 
							     GRAPH<r_circle_point,r_circle_segment>& G, 
							     edge_array<edge>& corresp_edge, 
							     bool embed = true);
	// single-colour version

__exportF void INTERSECT_SEGMENTS_BRUTE_FORCE(const list<r_circle_segment>& S, 
											  GRAPH<r_circle_point,r_circle_segment>& G);

#if LEDA_ROOT_INCL_ID == 600099
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
