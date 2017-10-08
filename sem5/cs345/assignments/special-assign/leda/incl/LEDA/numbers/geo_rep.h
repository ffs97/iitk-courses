/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geo_rep.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_GEO_REP_H
#define LEDA_GEO_REP_H 

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600253
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/numbers/integer.h>
#include <LEDA/numbers/integer_vector.h> 

LEDA_BEGIN_NAMESPACE

class __exportC geo_rep : public handle_rep {

friend class __exportC rat_vector;

 int      dim;
 integer* v;


public:

 geo_rep();
 geo_rep(int d);
 geo_rep(integer,integer,integer);
 geo_rep(integer,integer,integer,integer);
 geo_rep(const integer_vector&);
 geo_rep(const integer_vector&,integer);
~geo_rep();


static integer* allocate(int);
static void     deallocate(integer*, int);
static int cmp_rat_coords(geo_rep*,geo_rep*);
static int cmp_hom_coords(geo_rep*,geo_rep*);

integer_vector vec() const;

void init3(integer,integer,integer);
void init4(integer,integer,integer,integer);
void copy(geo_rep*);
void negate(int);

friend __exportF ostream& operator<<(ostream&out,geo_rep*p);
friend __exportF istream& operator>>(istream&in,geo_rep*p);
friend __exportF void c_add(geo_rep*res,geo_rep*a,geo_rep*b);
friend __exportF void c_sub(geo_rep*res,geo_rep*a,geo_rep*b);

LEDA_MEMORY(geo_rep)

};


#if LEDA_ROOT_INCL_ID == 600253
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif



