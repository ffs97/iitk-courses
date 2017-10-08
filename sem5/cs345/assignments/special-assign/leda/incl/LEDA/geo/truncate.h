/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  truncate.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_TRUNCATE_H
#define LEDA_TRUNCATE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600078
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/geo/rat_point.h>
#include <LEDA/geo/point.h>
#include <LEDA/core/list.h>

LEDA_BEGIN_NAMESPACE

extern __exportF list<point> truncate(const list<point>& L0, int prec);
/*{\Mfunc If all points in |L0| are equal to the origin, |L0| is returned.
Otherwise, let $M$ be the smallest power of two larger than the absolute
value of all coordinates of all points in |L0|, and let $P = 2^{\mathit prec}$.
For each point $p = (x,y)$
the point 
$(\lfloor (x/M)\cdot P \rfloor \cdot M, \lfloor (y/M)\cdot P \rfloor \cdot M)$
is added to the result list.}*/

extern __exportF list<rat_point> truncate(const list<rat_point>& L0, int prec);
/*{\Mfunc Converts every point $p$ in |L0| to a point by |p.to_point()|.
Calls the function above for the resulting list of points. Converts every 
|point| $q$ in the result list to a point by calling the 
constructor |rat_point(q)|. }*/

#if LEDA_ROOT_INCL_ID == 600078
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

