/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  filtered_integer.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_FILTERED_INTEGER_H
#define LEDA_FILTERED_INTEGER_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600257
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/system/basic.h>
#include <LEDA/numbers/integer.h>
#include <LEDA/numbers/fp.h>


LEDA_BEGIN_NAMESPACE

const double filtered_integer_eps0 = ldexp(1.0,-53);


/*{\Manpage {filtered_integer} {} {Filtered Integers} {x}}*/


class filtered_integer_rep;



class filtered_integer : public HANDLE_BASE(filtered_integer_rep) {

friend class filtered_integer_rep;

filtered_integer_rep* ptr() const { return (filtered_integer_rep*)PTR; }

inline filtered_integer(filtered_integer_rep* r);


public:

  static int exact_value_count;


/*{\Mcreation 5.2 }*/

filtered_integer();
/*{\Mcreate   introduces a variable |\Mvar| of type |\Mname| and initializes
              it with zero. }*/

filtered_integer(int i);
/*{\Mcreate   introduces a variable |\Mvar| of type |\Mname| and initializes
              it with $i$. }*/

filtered_integer(double d);
/*{\Mcreate   introduces a variable |\Mvar| of type |\Mname| and initializes
              it with $d$. \precond $d$ must represent an integer. }*/


inline filtered_integer(const filtered_integer& x);
inline filtered_integer& operator=(const filtered_integer& x);


integer exact_value() const;

bool    is_leaf() const;

integer to_integer() const;
double  to_float() const;
double  to_double() const;

static int cmp(const filtered_integer&, const filtered_integer&);



/*{\Moperations 1.5 5.0 }*/

friend int sign(const filtered_integer& f);
/*{\Mfunc  returns the sign of |f|.}*/


friend inline filtered_integer operator+(const filtered_integer& a, 
                                         const filtered_integer& b);
/*{\Mbinopfunc  }*/

friend inline filtered_integer operator-(const filtered_integer& a, 
                                         const filtered_integer& b);
/*{\Mbinopfunc  }*/

friend inline filtered_integer operator*(const filtered_integer& a, 
                                         const filtered_integer& b);
/*{\Mbinopfunc  }*/


filtered_integer& operator+=(const filtered_integer& b);
/*{\Mbinop   }*/

filtered_integer& operator-=(const filtered_integer& b);
/*{\Mbinop   }*/

filtered_integer& operator*=(const filtered_integer& b);
/*{\Mbinop   }*/


filtered_integer operator-() const;
/*{\Munop    }*/

/*
filtered_integer operator+() const { return *this; }
*/


};


class filtered_integer_rep : public handle_rep {

friend class filtered_integer;

friend inline filtered_integer operator+(const filtered_integer& a, 
                                         const filtered_integer& b);

friend inline filtered_integer operator-(const filtered_integer& a, 
                                         const filtered_integer& b);

friend inline filtered_integer operator*(const filtered_integer& a, 
                                         const filtered_integer& b);

friend int sign(const filtered_integer& f);


  double num;
  double mes;
  float  ind;
  char   op;

  filtered_integer left_child;
  filtered_integer right_child;

  integer* exact_val_ptr;

public:

  filtered_integer_rep(double d, double m, float i, char c, 
                                                    const filtered_integer& a,
                                                    const filtered_integer& b)
  : num(d), mes(m), ind(i), op(c), left_child(a), right_child(b) 
  { exact_val_ptr = 0; }

  filtered_integer_rep(double d, double m, float i)
  : num(d), mes(m), ind(i), op(0), left_child((filtered_integer_rep*)0), 
                                   right_child((filtered_integer_rep*)0)
  { exact_val_ptr = 0; }


~filtered_integer_rep() 
 { if (exact_val_ptr) delete exact_val_ptr; }

};



inline filtered_integer::filtered_integer(filtered_integer_rep* r) 
{ PTR = r; }


inline filtered_integer::filtered_integer()  
{ PTR = new filtered_integer_rep(0,1,0.5); }

inline filtered_integer::filtered_integer(int i)  
{ PTR = new filtered_integer_rep(i,2*::abs(i),0.5); }

// fabs ?

inline filtered_integer::filtered_integer(double d)  
{ PTR = new filtered_integer_rep(d,2*::fabs(d),0.5); }


inline integer filtered_integer::to_integer() const 
{ if (ptr()->exact_val_ptr) 
    return *(ptr()->exact_val_ptr);
  else
  { integer val = exact_value(); 
    ptr()->exact_val_ptr = new integer(val);
    exact_value_count++;
    return val;
   }
}

inline double filtered_integer::to_float()  const { return ptr()->num; }
inline double filtered_integer::to_double() const { return ptr()->num; }
inline bool   filtered_integer::is_leaf()   const { return ptr()->op == 0; }


inline filtered_integer filtered_integer::operator-() const
{
  if (is_leaf()) 
    return new filtered_integer_rep(-ptr()->num,ptr()->mes,ptr()->ind); 
  else
    return new filtered_integer_rep(-ptr()->num,ptr()->mes,ptr()->ind,
                                                           'u',*this,*this);
}





inline filtered_integer operator+(const filtered_integer& a, 
                                  const filtered_integer& b)
{ 
  filtered_integer_rep* a_rep = a.ptr();
  filtered_integer_rep* b_rep = b.ptr();

  double a_num = a_rep->num;
  double b_num = b_rep->num;
  double   sum = a_num + b_num;

  if (a_rep->op == 0 && b_rep->op == 0 && sum - a_num == b_num 
                                       && sum - b_num == a_num)
     return sum;

  return new filtered_integer_rep(sum, 2*(a_rep->mes + b_rep->mes),
                                  (a_rep->ind + b_rep->ind + 1)/2, '+', a, b);
 }


inline filtered_integer operator-(const filtered_integer& a, 
                                  const filtered_integer& b)
{ 
  filtered_integer_rep* a_rep = a.ptr();
  filtered_integer_rep* b_rep = b.ptr();

  double a_num = a_rep->num;
  double b_num = b_rep->num;
  double  diff = a_num - b_num;

  if (a_rep->op == 0 && b_rep->op == 0 && a_num - diff == b_num 
                                       && b_num + diff == a_num)
     return diff;

  return new filtered_integer_rep(diff, 2*(a_rep->mes + b_rep->mes),
                                 (a_rep->ind + b_rep->ind + 1)/2, '-', a, b);
 }



inline filtered_integer operator*(const filtered_integer& a, 
                                  const filtered_integer& b)
{ 
  filtered_integer_rep* a_rep = a.ptr();
  filtered_integer_rep* b_rep = b.ptr();

  double a_num = a_rep->num;
  double b_num = b_rep->num;
  double  prod = a_num * b_num;

  if (a_rep->op == 0 && b_rep->op == 0)
  { if (fp::is_single_precision(a_num) && fp::is_single_precision(b_num))
    /*if (fp::is_finite(prod) && fp::is_not_underflowed(prod))*/ return prod;
   }
  return new filtered_integer_rep(prod, a_rep->mes*b_rep->mes,
                                  float(a_rep->ind+b_rep->ind+0.5),'*',a, b);
 }

inline filtered_integer& filtered_integer::operator+=(const filtered_integer& b)
{ *this = *this + b; return *this; } 

inline filtered_integer& filtered_integer::operator-=(const filtered_integer& b)
{ *this = *this - b; return *this; } 

inline filtered_integer& filtered_integer::operator*=(const filtered_integer& b)
{ *this = *this * b; return *this; } 


inline int sign(const filtered_integer& f)
{ double d = f.ptr()->num;
  if (f.is_leaf()) return fp::sign(d);
  double eps = f.ptr()->ind * f.ptr()->mes * filtered_integer_eps0;
  if (d >  eps) return  1;
  if (d < -eps) return -1;
  if (eps < 1)  return  0;
  return sign(f.to_integer());
 }


inline int filtered_integer::cmp(const filtered_integer& x, 
                                  const filtered_integer& y) 
{ //return sign(x-y); 
  filtered_integer_rep* x_rep = x.ptr();
  filtered_integer_rep* y_rep = y.ptr();
  double diff = x_rep->num - y_rep->num;
  double mes = 2*(x_rep->mes + y_rep->mes);
  double ind = (x_rep->ind + y_rep->ind + 1)/2;
  double eps = ind * mes * filtered_integer_eps0;
  if (diff >  eps) return  1;
  if (diff < -eps) return -1;
  if (eps < 1)  return  0;
  return compare(x.to_integer(),y.to_integer());
}

 
COMPARE_DECL_PREFIX
inline int DEFAULT_COMPARE(const filtered_integer& x, const filtered_integer& y) 
{ if (x.is_leaf() && y.is_leaf()) 
     return compare(x.to_float(),y.to_float());
  else
     return filtered_integer::cmp(x,y);
 }


inline bool operator<(const filtered_integer& x, const filtered_integer& y)
{ if (x.is_leaf() && y.is_leaf()) 
     return x.to_float() < y.to_float();
  else
     return filtered_integer::cmp(x,y) < 0; 
}

inline bool operator<=(const filtered_integer& x, const filtered_integer& y)
{ if (x.is_leaf() && y.is_leaf()) 
     return x.to_float() <= y.to_float();
  else
     return filtered_integer::cmp(x,y) <= 0; 
}

inline bool operator>(const filtered_integer& x, const filtered_integer& y)
{ if (x.is_leaf() && y.is_leaf()) 
     return x.to_float() > y.to_float();
  else
     return filtered_integer::cmp(x,y) > 0; 
}

inline bool operator>=(const filtered_integer& x, const filtered_integer& y)
{ if (x.is_leaf() && y.is_leaf()) 
     return x.to_float() >= y.to_float();
  else
     return filtered_integer::cmp(x,y) >= 0; 
}

inline bool operator==(const filtered_integer& x, const filtered_integer& y)
{ if (x.is_leaf() && y.is_leaf()) 
     return x.to_float() == y.to_float();
  else
     return filtered_integer::cmp(x,y) == 0; 
}

inline bool operator!=(const filtered_integer& x, const filtered_integer& y)
{ if (x.is_leaf() && y.is_leaf()) 
     return x.to_float() != y.to_float();
  else
     return filtered_integer::cmp(x,y) != 0; 
}




inline ostream& operator<<(ostream& out, const filtered_integer& x)
{ return out << x.to_integer(); }

inline istream& operator>>(istream& in, filtered_integer& x)
{ int i;
  in >> i; 
  x = i;
  return in;
}



inline filtered_integer::filtered_integer(const filtered_integer& x) :
HANDLE_BASE(filtered_integer_rep)(x) {}

inline filtered_integer& filtered_integer::operator=(const filtered_integer& x)
{ HANDLE_BASE(filtered_integer_rep)::operator=(x); 
  return *this; 
 }



#if LEDA_ROOT_INCL_ID == 600257
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

