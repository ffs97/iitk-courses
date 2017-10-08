/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  expcomp.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef EXCOMP_H
#define EXCOMP_H

#include<LEDA/internal/std/math.h>

#include <LEDA/numbers/fp.h>

LEDA_BEGIN_NAMESPACE



#define EXCOMP_NO_IDEA 2


#if !defined(__SUNPRO_CC)

const double EXCOMP_eps     = fp::power_two((long)-53);
const double EXCOMP_eps1    = fp::power_two((long)-52);
const double EXCOMP_spe1    = fp::power_two((long)52);
const double EXCOMP_sqrtMul = fp::power_two((long)26);
const double EXCOMP_MinDbl  = fp::power_two((long)-1022);
const double EXCOMP_SupOfs  = fp::power_two((long)-970);
const double EXCOMP_correction = 1 + 2*EXCOMP_eps1;

#else

#define EXCOMP_eps     fp::power_two((long)-53)
#define EXCOMP_eps1    fp::power_two((long)-52)
#define EXCOMP_spe1    fp::power_two((long)52);
#define EXCOMP_sqrtMul fp::power_two((long)26)
#define EXCOMP_MinDbl  fp::power_two((long)-1022)
#define EXCOMP_SupOfs  fp::power_two((long)-970)
#define EXCOMP_correction  (1 + 2*EXCOMP_eps1)

#endif

inline int EXCOMP_sign(const double &x) { return fp::sign(x); } 
inline double EXCOMP_abs(const double &x) { return fp::abs(x); }
inline double EXCOMP_max(const double &x, const double& y) { return x>y ? x : y; }

#define EXCOMP_TRUE  1
#define EXCOMP_FALSE 0


LEDA_END_NAMESPACE

#endif
