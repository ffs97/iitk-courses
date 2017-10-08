/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  TRANSFORM.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


LEDA_BEGIN_NAMESPACE

/*{\Manpage {TRANSFORM} {} {Transformation} {T} }*/

class __exportC TRANSFORM
{ 
/*{\Mdefinition

There are three instantiations of |TRANSFORM|: 
|transform| (floating point kernel),
|rat_transform| (rational kernel) and
|real_transform| (real kernel).
The respective header file name corresponds to the type name (with ``.h''
appended).

An instance |\Mvar| of type |TRANSFORM| is an affine transformation of 
two-dimensional space. It is given by a $3 \times 3$ matrix $T$ 
with $T_{2,0} = T_{2,1} = 0$ and $T_{2,2} \not= 0$ and 
maps the point $p$ with homogeneous coordinate vector 
$(p_x, p_y, p_w)$ to the point $T \cdot p$.

A matrix of the form
\[ \left( \begin{array}{ccc}
      w & 0 & x \\
      0 & w & y \\
      0 & 0 &  w 
    \end{array} \right) \]
realizes an translation by the vector $(x/w,y/w)$ and a matrix of the
form 
\[ \left( \begin{array}{ccc}
      a & -b &  0 \\
      b & a &  0 \\
      0 & 0 & w 
    \end{array} \right) \]
where $a^2 + b^2 = w^2$ realizes a rotation by the angle $\alpha$ about the
origin, where $\cos \alpha = a/w$ and $\sin \alpha = b/w$. 
Rotations are in counter-clockwise direction. 
}*/

INT_MATRIX M;

public:

/*{\Mcreation }*/

TRANSFORM();
/*{\Mcreate creates a variable introduces a variable |\Mvar| of type |\Mname|. 
|\Mvar| is initialized with the identity transformation. }*/

TRANSFORM(const INT_MATRIX t);
/*{\Mcreate introduces a variable |\Mvar| of type |\Mname|. 
|\Mvar| is initialized with the matrix $t$.\\
\precond $t$ is a $3 \times 3$ matrix with $t_{2,0} = t{2,1} = 0$
and $t_{2,2} \not= 0$.}*/


/*{\Moperations}*/

INT_MATRIX T_matrix() const;
/*{\Mop returns the transformation matrix }*/

void simplify();
/*{\Mop  
The operation has no effect for |transform|. For |rat_transform| let
$g$ be the ggT of all matrix entries. Cancels out $g$.}*/

RAT_TYPE norm() const;
/*{\Mop  returns the norm of the transformation }*/

TRANSFORM operator()(const TRANSFORM& T1) const;
/*{\Mfunop returns the transformation $T \circ |T1|$.}*/

POINT operator()(const POINT& p) const;
/*{\Mfunop returns $T(p)$.}*/

VECTOR operator()(const VECTOR& v) const;
/*{\Mfunop returns $T(v)$.}*/

SEGMENT operator()(const SEGMENT& s) const;
/*{\Mfunop returns $T(s)$.}*/

LINE operator()(const LINE& l) const;
/*{\Mfunop returns $T(l)$.}*/

RAY operator()(const RAY& r) const;
/*{\Mfunop returns $T(r)$.}*/

CIRCLE operator()(const CIRCLE& C) const;
/*{\Mfunop returns $T(C)$.}*/

POLYGON operator()(const POLYGON& P) const;
/*{\Mfunop returns $T(P)$.}*/

GEN_POLYGON operator()(const GEN_POLYGON& P) const;
/*{\Mfunop returns $T(P)$.}*/

};

/*{\Mtext

{\bf Non-member Functions }

In any of the function below a point can be specified to the origin by
replacing it by an anonymous object of type POINT, e.g.,
|rotation90(POINT())|
will generate a rotation about the origin. 

\medskip

}*/

extern __exportF TRANSFORM translation(const INT_TYPE& dx, const INT_TYPE& dy, const INT_TYPE& dw);
/*{\Mfunc returns the translation by the vector $(|dx|/|dw|,|dy|/|dw|)$. }*/

extern __exportF TRANSFORM translation(const RAT_TYPE& dx, const RAT_TYPE& dy);
/*{\Mfunc returns the translation by the vector $(|dx|,|dy|)$. }*/

extern __exportF TRANSFORM translation(const VECTOR& v);
/*{\Mfunc returns the translation by the vector $v$. }*/

extern __exportF TRANSFORM rotation(const POINT& q, double alpha, double eps);
/*{\Mfunc returns the  rotation about |q| by an angle $|alpha| \pm |eps|$. }*/

extern __exportF TRANSFORM rotation90(const POINT & q);
/*{\Mfunc   returns the rotation about $q$ by an angle of $90$ degrees.}*/

extern __exportF TRANSFORM reflection(const POINT& q, const POINT& r);
/*{\Mfunc     returns the reflection across the straight line passing
            through $q$ and $r$.}*/

extern __exportF TRANSFORM reflection(const POINT& q);
/*{\Mfunc     returns the reflection across point $q$. }*/


LEDA_END_NAMESPACE


