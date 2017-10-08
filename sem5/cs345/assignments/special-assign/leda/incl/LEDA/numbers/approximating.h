/*******************************************************************************
+
+  LEDA 6.3
+
+
+  approximating.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/

// Author(s): Susanne Schmitt

#ifndef _LEDA_APPROXIMATING_H
#define _LEDA_APPROXIMATING_H

#include <LEDA/numbers/polynomial.h>
#include <LEDA/numbers/bigfloat.h>

LEDA_BEGIN_NAMESPACE


bigfloat find_minimum(const polynomial<bigfloat>& P, 
                      const polynomial<bigfloat>& Ps, bigfloat l, bigfloat r);
bigfloat find_maximum(const polynomial<bigfloat>& P, 
                      const polynomial<bigfloat>& Ps, bigfloat l, bigfloat r);
                                           
bigfloat newton(const polynomial<bigfloat>& P, bigfloat l, bigfloat r, sz_t prec);

class __exportC approximating {

   polynomial<bigfloat> P_;
   bigfloat app;

public:
   approximating(const polynomial<bigfloat>& P, bigfloat l, bigfloat r, sz_t prec)
   : P_(P), app(newton(P,l,r,prec)) {}

   inline bigfloat approximation() const { return app; }
};


LEDA_END_NAMESPACE

#endif // _LEDA_APPROXIMATING_H


