/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  RANDOM_POINT.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/core/list.h>

LEDA_BEGIN_NAMESPACE

/*{\Manpage {point generators} {} {Point Generators} }*/

/*{\Mtext
All generators are available for |point|, |rat_point|, |real_point|, 
|d3_point|, and |d3_rat_point|. We use |POINT| to stand for any of these
classes. The corresponding header files are called random\_point.h, 
random\_rat\_point.h, random\_real\_point.h, random\_d3\_point.h, and 
random\_d3\_rat\_point.h, respectively. 
These header files are included in the corresponding kernel header files, 
e.g., random\_rat\_point.h is part of rat\_kernel.h.

We use the following naming conventions: square, circle, segment, and disk
refer to two-dimensional objects (even in 3d) and cube, ball, and sphere refer
to full-dimensional objects, i.e, in 2d cube and square, ball and disk, and 
circle and sphere are synonymous. 
}*/

extern __exportF void random_point_in_square(POINT& p, int maxc );
/*{\Mfunc returns a point whose $x$ and $y$-coordinates are random integers in 
$\range{-|maxc|}{|maxc|}$. The $z$-coordinate is zero. }*/

extern __exportF void random_points_in_square(int n, int maxc, list<POINT>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/

extern __exportF void random_point_in_unit_square(POINT& p, int D = (1<<30) - 1 );
/*{\Mfunc returns a point whose coordinates are random rationals of the form
$i/D$ where $i$ is a random integer in the range $\range{0}{D}$. The default 
value of $D$ is $2^{30} - 1$.}*/

extern __exportF void random_points_in_unit_square(int n, int D, list<POINT>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/


extern __exportF void random_points_in_unit_square(int n, list<POINT>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . The default value of $D$
is used. }*/

extern __exportF void random_point_in_cube(POINT& p, int maxc);
/*{\Mfunc returns a point whose coordinates are random integers in 
$\range{-|maxc|}{|maxc|}$. In 2d this function is equivalent to 
|random_point_in_square|. }*/

extern __exportF void random_points_in_cube(int n, int maxc, list<POINT>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/


extern __exportF void random_point_in_unit_cube(POINT& p, int D = (1<<30) - 1);
/*{\Mfunc returns a point whose coordinates are random rationals of the form
$i/D$ where $i$ is a random integer in the range $\range{0}{D}$. The default 
value of $D$ is $2^{30} - 1$.}*/

extern __exportF void random_points_in_unit_cube(int n, int D, list<POINT>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/

extern __exportF void random_points_in_unit_cube(int n, list<POINT>& L);
/*{\Mfunc as above, but the default value of $D$ is used. }*/

extern __exportF void random_point_in_disc(POINT& p, int R);
/*{\Mfuncl returns a random point with integer $x$ and $y$-coordinates 
in the disc with radius $R$ centered at the origin. The $z$-coordinate is 
zero. \\
\precond $R \le 2^{30}$.}*/

extern __exportF void random_points_in_disc(int n, int R, list<POINT>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/

extern __exportF void random_point_in_unit_disc(POINT& p, int D = (1<<30) - 1);
/*{\Mfuncl returns a point in the unit disc 
whose coordinates are quotients with denominator $D$. The default 
value of $D$ is $2^{30} - 1$.}*/ 

extern __exportF void random_points_in_unit_disc(int n, int D, list<POINT>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/

extern __exportF void random_points_in_unit_disc(int n, list<POINT>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ .  The default
value of $D$ is used. }*/



extern __exportF void random_point_in_ball(POINT& p, int R);
/*{\Mfuncl returns a random point with integer coordinates 
in the ball with radius $R$ centered at the origin. In 2d this function is 
equivalent to |random_point_in_disc|.\\
\precond $R \le 2^{30}$.}*/

extern __exportF void random_points_in_ball(int n, int R, list<POINT>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/

extern __exportF void random_point_in_unit_ball(POINT& p, int D = (1<<30) - 1);
/*{\Mfuncl returns a point in the unit ball 
whose coordinates are quotients with denominator $D$. The default 
value of $D$ is $2^{30} - 1$.}*/ 

extern __exportF void random_points_in_unit_ball(int n, int D, list<POINT>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/

extern __exportF void random_points_in_unit_ball(int n, list<POINT>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ .  The default
value of $D$ is used. }*/





extern __exportF void random_point_near_circle(POINT& p, int R);
/*{\Mfuncl  returns a random point with integer coordinates that lies
close to the circle with radius $R$ centered at the origin.}*/

extern __exportF void random_points_near_circle(int n, int R, list<POINT>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/

extern __exportF void random_point_near_unit_circle(POINT& p, int D = (1<<30) - 1);
/*{\Mfuncl  returns a point close to the unit circle  
whose coordinates are quotients with denominator $D$.
The default 
value of $D$ is $2^{30} - 1$.}*/ 

extern __exportF void random_points_near_unit_circle(int n, int D, list<POINT>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/

extern __exportF void random_points_near_unit_circle(int n, list<POINT>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ .  The default
value of $D$ is used. }*/

extern __exportF void random_point_near_sphere(POINT& p, int R);
/*{\Mfuncl returns a point with integer coordinates close to the sphere with
radius $R$ centered at the origin.}*/

extern __exportF void random_points_near_sphere(int n, int R, list<POINT>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/

extern __exportF void random_point_near_unit_sphere(POINT& p, int D = (1<<30) - 1);
/*{\Mfuncl  returns a point close to the unit sphere  
whose coordinates are quotients with denominator $D$.  
In 2d this function is equivalent to 
|point_near_unit_circle|.}*/ 

extern __exportF void random_points_near_unit_sphere(int n, int D, list<POINT>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/

extern __exportF void random_points_near_unit_sphere(int n, list<POINT>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ .  The default
value of $D$ is used. }*/

/*{\Mtext 
Wit the rational kernel the 
functions |_on_circle| are guaranteed to produce points that lie 
precisely on the specified circle. With the floating point kernel 
the functions are equivalent to the |_near_circle| functions. 
}*/

extern __exportF void random_point_on_circle(POINT& p, int R, int C = 1000000);
/*{\Mfuncl  returns a random point with integer coordinates that lies
on the circle with radius $R$ centered at the origin. The point is 
chosen from a set of at least $C$ candidates.}*/

extern __exportF void random_points_on_circle(int n, int R, list<POINT>& L, 
int C = 1000000);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/

extern __exportF void random_point_on_unit_circle(POINT& p, int C = 1000000);
/*{\Mfuncl  returns a point on the unit circle. The point is chosen from 
a set of at least $C$ candidates.  }*/ 

extern __exportF void random_points_on_unit_circle(int n, list<POINT>& L,
int C = 1000000);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/

extern __exportF void random_point_on_sphere(POINT& p, int R);
/*{\Mfuncl same as |random_point_near_sphere|.}*/

extern __exportF void random_points_on_sphere(int n, int R, list<POINT>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/

extern __exportF void random_point_on_unit_sphere(POINT& p, int D = (1<<30) - 1);
/*{\Mfuncl  same as |random_point_near_unit_sphere|.}*/ 

extern __exportF void random_points_on_unit_sphere(int n, int D, list<POINT>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ . }*/

extern __exportF void random_points_on_unit_sphere(int n, list<POINT>& L);
/*{\Mfuncl returns a list $L$ of $n$ points \ldots\ .  The default
value of $D$ is used. }*/

extern __exportF void random_point_on_segment(POINT& p, SEGMENT s);
/*{\Mfunc generates a random point on $s$. }*/

extern __exportF void random_points_on_segment(SEGMENT s, int n, list<POINT>& L);
/*{\Mfunc generates a list $L$ of $n$ points \ldots\ . }*/

extern __exportF void points_on_segment(SEGMENT s, int n, list<POINT>& L);
/*{\Mfunc generates a list $L$ of $n$ equally spaced points on $s$. }*/


extern __exportF void random_point_on_paraboloid(POINT& p, int maxc);
/*{\Mfuncl returns a point $(x,y,z)$  with $x$ and $y$ random integers
in the range $\range{-|maxc|}{|maxc|}$, and 
$z = 0.004*(x*x + y*y) - 1.25*|maxc|$. The function does not make sense in
2d.}*/

extern __exportF void random_points_on_paraboloid(int n, int maxc, list<POINT>& L);
/*{\Mfuncl returns a list |L| of $n$ points \ldots\ .}*/

extern __exportF void lattice_points(int n, int maxc, list<POINT>& L);
/*{\Mfuncl returns a list $L$ of approximately $n$ points. The points
have integer coordinates $id/|maxc|$ for an appropriately chosen $d$
and $-|maxc|/d \le i \le |maxc|/d$. }*/

extern __exportF void random_points_on_diagonal(int n, int maxc, list<POINT>& L);
/*{\Mfunc generates $n$ points on the diagonal whose coordinates are random
integer in the range from $-|maxc|$ to |maxc|. }*/

LEDA_END_NAMESPACE
