/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  rat_window.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_RAT_WINDOW_H
#define LEDA_RAT_WINDOW_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600133
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/graphics/window.h>
#include <LEDA/geo/rat_kernel.h>

LEDA_BEGIN_NAMESPACE

extern __exportF window& operator>>(window& W, rat_point& p);
/*{\Mbinopfunc   reads a point $p$: clicking the left button 
             assigns the current cursor position to $p$.}*/

extern __exportF window& operator<<(window& W, const rat_point& p);
/*{\Mbinopfunc   diplays point $p$ at window $W$.}*/



extern __exportF window& operator>>(window& W, rat_segment& s);
/*{\Mbinopfunc   reads a segment $s$: use the left button to input 
                 the start and end point of $s$.}*/

extern __exportF window& operator<<(window& W, const rat_segment& s);
/*{\Mbinopfunc   diplays segment $s$ at window $W$.}*/



extern __exportF window& operator>>(window& W, rat_ray& r);
/*{\Mbinopfunc   reads a ray $r$: use the left button to input 
                 the start point and a second point on $r$.}*/

extern __exportF window& operator<<(window& W, const rat_ray& r);
/*{\Mbinopfunc   diplays ray $r$ at window $W$.}*/


extern __exportF window& operator>>(window& W, rat_line& l);
/*{\Mbinopfunc   reads a line $l$: use the left button to input 
                 two different points on $l$.}*/

extern __exportF window& operator<<(window& W, const rat_line& l);
/*{\Mbinopfunc   diplays line $l$ at window $W$.}*/

extern __exportF window& operator>>(window& W, rat_circle& C);
/*{\Mbinopfunc   reads a circle $C$: use the left button to input 
                 the center of $C$ and a point on $C$.}*/

extern __exportF window& operator<<(window& W, const rat_circle& c);
/*{\Mbinopfunc   diplays circle $c$ at window $W$.}*/


extern __exportF window& operator>>(window& W, rat_polygon& P);
/*{\Mbinopfunc   reads a polygon $P$: use the left button to input 
                 the sequence of vertices of $P$, end the sequence 
                 by clicking the right button.}*/


extern __exportF window& operator<<(window& W, const rat_polygon& p);
/*{\Mbinopfunc   diplays polygon $p$ at window $W$.}*/


extern __exportF window& operator>>(window& W, rat_gen_polygon& P);
/*{\Mbinopfunc   reads a generalized polygon $P$: 
                 use the left button to input 
                 the sequence of vertices of $P$, end the sequence 
                 by clicking the right button.}*/

extern __exportF window& operator<<(window& W, const rat_gen_polygon& p);
/*{\Mbinopfunc   diplays generalized polygon $p$ at window $W$.}*/


extern __exportF window& operator>>(window& W, rat_rectangle& r);
/*{\Mbinopfunc   reads a rectangle $r$: use the left button to input 
                 the lower left and upper right corner.}*/

extern __exportF window& operator<<(window& W, const rat_rectangle& r);
/*{\Mbinopfunc   diplays rectangle $r$ at window $W$.}*/


extern __exportF window& operator>>(window& W, rat_triangle& t);
/*{\Mbinopfunc   reads a triangle $t$: use the left button to input 
                 the corners.}*/

extern __exportF window& operator<<(window& W, const rat_triangle & t);
/*{\Mbinopfunc   diplays triangle $t$ at window $W$.}*/


#if LEDA_ROOT_INCL_ID == 600133
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
