/*******************************************************************************
+
+  LEDA 6.3
+
+
+  polynomial.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/

// Author(s): Arno Eigenwillig, Susanne Schmitt, Michael Seel

#ifndef _LEDA_POLYNOMIAL_H
#define _LEDA_POLYNOMIAL_H    

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600245
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/core/array.h>
#include <LEDA/core/growing_array.h>
#include <LEDA/system/assert.h>

#undef LEDA_VECTOR
#define LEDA_VECTOR growing_array

#define PACKAGE_SCOPE LEDA_SCOPE
#define leda_array LEDA_SCOPE array

LEDA_BEGIN_NAMESPACE



/*{\Manpage {polynomial} {NT} {Polynomials} {p}}*/

/*{\Mdefinition
The class |\Mname| represents polynomial over the number type |NT|.
A polynomial |\Mvar| is defined by a coefficient vector $c=[c_k, \dots, c_0]$,
$k \leq 0$: $|\Mvar|(X) = \sum_{i=0}^{k} c_i \cdot X^i$.
We may assume that $c_d \neq 0$ or $d = 0$, then $d$ is called the 
\emph{degree} of $|\Mvar|$.
}*/

template <class T>
class polynomial {

  LEDA_VECTOR<T> coeff;

  protected:

    // sets the polynomial to the polynomial of degree d with coefficients 0
    void set_to_zero(int d)
    {
       coeff.resize(d+1);
       for (int i = 0; i <= d; i++) coeff[i] = T();
  /*  the code above should be more efficient
       // vector of 0 coefficients
       LEDA_VECTOR<T> coeff_d(d+1);
       for (int i = 0; i <= d; i++) coeff_d[i] = T();

       // initialize the polynomial
       coeff = coeff_d;
  */     
    }

    // deletes leading zeroes
    void reduce()
    {
          while ( coeff.size()>1 && coeff.back()==T()) coeff.pop_back(); 
    }

    // computes this - b*x^k*p
    // degree() >= p.degree() + k!
     void minus_offsetmult(const polynomial<T>& p, const T& b, int k)
      {
  #if defined(CHECK_REALS) && !defined(LEDA_CHECKING_OFF)
          assert(degree() >= p.degree() + k);
  #endif
          int pd = p.degree();
          for (int i = 0; i <= pd; i++) coeff[i+k] -= b*p[i];

          reduce();
      }


public:

/*{\Mcreation}*/
  
    polynomial() : coeff(1) {}
    /*{\Mcreate creates the zero-polynomial.}*/

    polynomial(const T& c0) : coeff(1)
    {
       coeff[0] = c0;
  //   reduce();
    }
    /*{\Mcreate creates the polynomial which is identical to the constant $c_0$.}*/

    polynomial(const T& c1, const T& c0) : coeff(2)
    {
       coeff[1] = c1; coeff[0] = c0;
       reduce();
    }
    /*{\Mcreate creates the polynomial $P(X) = c_1 \cdot X + c_0$.}*/

    polynomial(const array<T>& coeffs) : coeff(coeffs)
    {
       reduce();
    }
    /*{\Mcreate creates the polynomial with the coefficient vector |coeffs|.}*/

    polynomial(const LEDA_VECTOR<T>& coeffs) : coeff(coeffs)
    {
       reduce();
    }

    polynomial(const polynomial<T>& P) : coeff(P.coeff)
    {}

    ~polynomial()
    {
    }

    const polynomial& operator=(const polynomial& P)
    {
       if(this == &P) return (*this);

       coeff = P.coeff; 
       return (*this);
    }

    const polynomial& operator=(const LEDA_VECTOR<T>& coeffs)
    {
       coeff = coeffs;
       reduce(); 
       return (*this);
    }
/*{\Moperations}*/
  
    const T& operator[](unsigned int i) const {
  #if defined(CHECK_REALS) && !defined(LEDA_CHECKING_OFF)
          assert( i < unsigned(coeff.size()) );
  #endif
          return coeff[i];
    }
    /*{\Marrop return the coefficient $c_i$ of |\Mvar|.}*/

    const T& lcoeff() const 
    { return coeff.back(); }
    /*{\Mop returns the leading coefficient $c_d$ of |\Mvar|.}*/

    const LEDA_VECTOR<T>& get_coefficient_vector() const
    { return coeff; }

    const array<T> get_coefficients() const
    {
      array<T> c(coeff.size());
      for (int i = 0; i < coeff.size(); i++) c[i] = coeff[i];
      return c;
    }
    /*{\Mop returns the coefficient vector of |\Mvar|.}*/

    int degree() const { return (int) coeff.size()-1; }
    /*{\Mop returns the degree of |\Mvar|.}*/

    bool is_zero() const
    { return degree()==0 && coeff[0]==T(); }
    /*{\Mop returns whether |\Mvar| is identical to zero.}*/


    T evaluate(const T& x) const 
    {
          int d = degree();
          T y(coeff[d]);
          while (--d >= 0) y = y*x + coeff[d];
          return y;
    }
    /*{\Mop evaluates |\Mvar| at |x|.}*/


    void diff() 
    {
       if (is_zero()) return;
       if (degree() == 0) { coeff[0] = T(); return; }
       coeff[0] = coeff[1]; // avoid redundant multiplication by T(1)
       for (int i = 2; i <= degree(); i++) coeff[i-1] = coeff[i]*T(i);
       coeff.pop_back();
    }
    /*{\Mop replaces |\Mvar| by its first derivative.}*/


    // compute x^d P(1/x)
    void reversal() {
      T t;
      for(int l=0, r=degree(); l<r; l++,r--)
      {
        t = coeff[l];
        coeff[l] = coeff[r];
        coeff[r] = t;
      }
      reduce();
    }
    /*{\Mop replaces |\Mvar| by $X^d \cdot |\Mvar|(1/X)$.}*/

    //compute P(x+1)
    void translate_by_one()
    {   // implemented using Ruffini-Horner, see [Akritas, 1989]
        const int n = degree();
        for (int j = n-1; j >= 0; j--) 
        {
          for (int i = j; i < n; i++) coeff[i] += coeff[i+1]; 
        }
    }
    /*{\Mop replaces |\Mvar| by $|\Mvar|(X+1)$.}*/

    //compute b^d P(x+a/b)
    void translate(const T& a, const T& b) 
    {   // implemented using Mehlhorn's variant of Ruffini-Horner
        const int n = degree();
        T b_to_n_minus_j = b;
        int j;
        for (j = n-1; j >= 0; j--) {
            coeff[j] *= b_to_n_minus_j;
            b_to_n_minus_j *= b;
        }
        for (j = n-1; j >= 0; j--) {
            coeff[j] += a*coeff[j+1];
            for (int i = j+1; i < n; i++) coeff[i] = b*coeff[i] + a*coeff[i+1]; 
            coeff[n] *= b;
        }
    }
    /*{\Mop replaces |\Mvar| by $b^d \cdot |\Mvar|(X+a/b)$.}*/


    //compute P(x+a)
    void translate(const T& a) 
    {   // implemented using Mehlhorn's variant of Ruffini-Horner
        const int n = degree();
        for (int j = n-1; j >= 0; j--) {
            coeff[j] += a*coeff[j+1];
            for (int i = j+1; i < n; i++) coeff[i] += a*coeff[i+1]; 
        }
    }
    /*{\Mop replaces |\Mvar| by $|\Mvar|(X+a)$.}*/

    // compute P(a*x)
    void scale_up(const T& a) {
        if (degree() == 0) return;
        T a_to_j = a;
        for (int j = 1; j <= degree(); j++) {
            coeff[j] *= a_to_j; 
            a_to_j *= a;
        }
    }
    /*{\Mop replaces |\Mvar| by $|\Mvar|(a \cdot X)$.}*/

    // compute b^d P(x/b)
    void scale_down(const T& b)
    {
        if (degree() == 0) return;
        T b_to_n_minus_j = b;
        for (int j = degree() - 1; j >= 0; j--) {
            coeff[j] *= b_to_n_minus_j; 
            b_to_n_minus_j *= b;
        }
    }
    /*{\Mop replaces |\Mvar| by $b^d \cdot |\Mvar|(X/b)$.}*/

  
    int sign() const { return PACKAGE_SCOPE sign(lcoeff()); }
    /*{\Mop returns the sign of the leading coefficient.}*/

    int compare(const polynomial& p2) const 
    {
          int d1 = degree();
          int d2 = p2.degree();
          if (d1 > d2) return PACKAGE_SCOPE sign(coeff[d1]);
          else if (d1 < d2) return -PACKAGE_SCOPE sign(p2[d2]);

          for (int i = d1; i >= 0; i--) {
            if(coeff[i] != p2[i]) return PACKAGE_SCOPE sign(coeff[i]-p2[i]);
          }
          return 0;
    }

    int compare(const T& p2) const 
    {
          if (degree() > 0) return PACKAGE_SCOPE sign(lcoeff());
          else return PACKAGE_SCOPE sign(coeff[0]- p2);
    }

  
    polynomial<T>& operator - ()
    {
      if (is_zero()) return *this;

      for (int i = 0; i <= degree(); i++) coeff[i] = -coeff[i];
      return *this;
    }

    polynomial<T>& operator /= (const T& num)
    {
      if (is_zero()) return *this;

      for (int i = 0; i <= degree(); i++) coeff[i] = coeff[i]/num;
      return *this;
    }

    void euclidean_division(const polynomial<T>& g,
                            polynomial<T>& q, polynomial<T>& r)
    {
        int fd = degree(), gd = g.degree();
        if ( fd < gd ) {
            q.set_to_zero(0); r = *this; return;
        }
        // now we know fd >= gd 
        int qd = fd-gd, delta = qd+1, rd = fd;
        q.set_to_zero(delta);
        // r = f
        r = *this;
        while ( qd >= 0 ) {
            //T Q = idiv(r[rd], g[gd]); Is it ok to use normal division here?
            T Q = r[rd] / g[gd];
            q.coeff[qd] += Q;
            r.minus_offsetmult(g,Q,qd);
            if (r.is_zero()) break;
            rd = r.degree();
            qd = rd - gd;
        }
        q.reduce();
    }
    /*{\Mop computes polynomials $q$ and $r$ s.th.~$|\Mvar| = q \cdot g + r$
            and the degree of $r$ is less than the degree of $g$.}*/
};

/*{\Mtext The comparison operators [[==]], [[!=]], [[<]], [[<=]], [[>]], [[>=]]
          are provided as well as all arithmetic operators [[+]], [[-]], [[*]]
                  and [[/]].}*/


template<class T>
class sylvester_matrix{

  LEDA_VECTOR<LEDA_VECTOR<T> > MAT;

public:

  sylvester_matrix(){};

  sylvester_matrix(polynomial<T>& P, polynomial<T>& Q)
  : MAT(P.degree()+Q.degree())
  {
     int n=P.degree();
     int m=Q.degree();

     int i; 
     for(i=0; i<m; i++)
     {  // i-te Zeile = (i-1)*0, n+1 Polynomcoeff, (m-i)*0
        MAT[i].reserve(n+m);
        int j;
        for(j=0; j<= i-1; j++) (MAT[i]).push_back(T());
        for(j=n; j>=0; j--)    (MAT[i]).push_back(P[j]);
        for(j=i+n; j<m+n; j++) (MAT[i]).push_back(T()); 
     }
     for(i=0; i<n; i++)
     {  // (m+i)-te Zeile = (i-1)*0, m+1 Polynomcoeff, (n-i)*0
        MAT[i].reserve(n+m);
        int j;
        for(j=0; j<= i-1; j++) (MAT[m+i]).push_back(T());
        for(j=m; j>=0; j--)    (MAT[m+i]).push_back(Q[j]);
        for(j=i+m; j<m+n; j++) (MAT[m+i]).push_back(T()); 
     }

/*
     for(i=0; i<m+n; i++)
     {
        for(int j=0; j<m+n; j++) cout << (MAT[i])[j] << "  ";
        cout << endl;
     }
     cout << endl;
     */
  }

  // swap column i and column j
  void swap(int i, int j)
  {
     int n = MAT.size();

     for(int k=0; k<n; k++)
     {
        T h = MAT[k][i];
        MAT[k][i] = MAT[k][j];
        MAT[k][j] = h;
     }
  }

  T determinant()
  {
     T det(1);
     int n = MAT.size();
     for(int i=0; i<n; i++)
     {
        // Consider the (i+1)-th row. It should look like i*0, elements.
        // Find an element in this row which is not 0.
        T a;
        int j;
        for(j=i; j<n; j++)
        {
           a = MAT[i][j];
           if(a!= T()) break;
        }

        // If no such element is found return 0
        if(j==n) return T();

        // Swap the columns if necessary.
        if(j!=i) 
        {
                swap(i,j);
                // don't needed in our case - we consider only the absolute value
                //det = -det;
        }

        // Multiply det by the nonzero element on place i,i
        det *= a;

        // delete the (i+1)-th colum below the (i+1)-th row
        for(int k=i+1; k<n; k++)
        {
           T b = MAT[k][i];
           if(b!=T()) 
           {
              for(j=i+1; j<n; j++) MAT[k][j] = MAT[k][j] - b*MAT[i][j]/a;
              MAT[k][i] = T();
           }
        }
     }
     return det;
  }

};


template <class T> inline
polynomial<T> diff(const polynomial<T>& p)
{ polynomial<T> q(p); q.diff(); return q; }


template<class T> inline
polynomial<T> reversal(const polynomial<T>& p)
{ polynomial<T> q(p); q.reversal(); return q; }

template<class T> inline
polynomial<T> translate_by_one(const polynomial<T>& p)
{ polynomial<T> q(p); q.translate_by_one(); return q; }

template<class T> inline
polynomial<T> translate(const polynomial<T>& p, const T& a, const T& b)
{ polynomial<T> q(p); q.translate(a, b); return q; }

template<class T> inline
polynomial<T> translate(const polynomial<T>& p, const T& a)
{ polynomial<T> q(p); q.translate(a); return q; }

template<class T> inline
polynomial<T> scale_up(const polynomial<T>& p, const T& a)
{ polynomial<T> q(p); q.scale_up(a); return q; }

template<class T> inline
polynomial<T> scale_down(const polynomial<T>& p, const T& b)
{ polynomial<T> q(p); q.scale_down(b); return q; }


// returns the polynomial $(1 + x)^n P(1/(1 + x))$.
template<class T> inline
polynomial<T> variation_transformation(const polynomial<T>& P) 
{  
        polynomial<T> R = reversal(P);  
        return translate_by_one(R);  
}



// This function returns 0,1 if there are 0,1 sign variations and 2 if there
// are 2 or more sign variations.
template <class T>
int sign_variations(const polynomial<T>& P) { 
    const int n = P.degree();
    int variations = 0;
    int old_sign = PACKAGE_SCOPE sign(P[n]); // never zero unless P is zero
    for (int i = n-1; i >= 0; i--) {
        int s = PACKAGE_SCOPE sign(P[i]);
        if (s == 0) continue;
        if (old_sign != s) {
            old_sign = s;
            variations++;
            if(variations==2) return variations;
        }
    }
    return variations;
}

// This function returns $-1$, if the sign variation could not be determined.
// It returns 0,1 if there are 0,1 sign variations and 
// 2 if there  are 2 or more sign variations.
// It returns 3,4 if there is one unknown sign at the beginning or at the end.
// The sign variation or the remaining sequence is the return value - 3.
template <class T>
int sign_variations_ip(const polynomial<T>& P) { 

    const int n = P.degree();
    int variations = 0;
    bool attention_first = false;
    bool variation_not_possible = false;
    bool this_not_possible = false;
    bool attention = false; // needed to check cases like +?-

    int old_sign = 0;

    // If the sign of the first coefficient is not known we have the following
    // possibilities:
    // - The sign of the next coefficient (!=0) is known. Then we might
    //   be able to say something if the variation of the remaining sequence
    //   is 0 or 1. To check this case we set |attention_first = true|.
    //   But we can say something only if not both the first and the last
    //   coefficient signs are not known. Therefore we test at the end if
    //   |attention && attention_first|.
    // - If the sign of the next coefficient is not known, we are in trouble.
    //   Therefore we set |attention = true| to be careful with the sign of the
    //   next coefficient.
    // |this_not_possible=true| so that we do not compare the (not known) sign
    // of the first coefficient to the next one.

    if(!PACKAGE_SCOPE sign_is_known(P[n])) 
    {
       attention_first = true;
       attention = true;
       this_not_possible = true;
    }
    else old_sign = PACKAGE_SCOPE sign(P[n]); // never zero unless P is zero

    for (int i = n-1; i >= 0; i--) {

        // If the sign of a coefficient is not clear, we have the
        // following cases.
        // - The first two coefficient signs are not known (maybe there are some 0
        //   in between). This is checked because in that case |attention=true|
        //   and we see that something is wrong.
        // - The sign of the coefficient before is not known. This is the
        //   same as before.
        // - The sign before is known. Then |attention=false| (at least I hope so)
        //   and we set |attention=true| to see that we have to be careful.
        //   We then search for the next sign.

        if(!PACKAGE_SCOPE sign_is_known(P[i])) 
        {
           if(attention)  // case ***??***
           {
              // In case there is a problem we set |variation_not_possible=true|.
              // This ensures that we do not return 0 or 1 sign variations, because
              // there might be some more. We can, however, return 2 if there are
              // 2 known sign variations. In that case the Uspensky algorithm
              // has to bisect anyway and it does not matter how many sign 
              // changes there are exactly.
              // We also set |this_not_possible=true| so ensure that we do not 
              // count a sign change of the form +??- for example.

              variation_not_possible = true;
              this_not_possible = true;
           }
           attention = true;
           continue; // find the next coefficient !=0
        }
        int s = PACKAGE_SCOPE sign(P[i]);
        if (s == 0) continue;

        // We don't want to count sign changes as +??-. But we want to go further.
        // We hope that we can do sign computation from now on.

        if(this_not_possible)
        {
            old_sign = s;
            attention = false;
            this_not_possible = false;
        }

        // In this case we increase the variation. If we have found more than
        // 2 sign changes we return 2 (causing bisection in Uspensky). 
        // If we needed to be careful we can set |attention=false|.

        else if (old_sign != s) 
        {
            old_sign = s;
            variations++;
            if(variations==2) return variations;
            if(attention) attention = false;  // +?- or -?+ -> ok.
        }

        // If there is no sign change between the actual coefficent and
        // the last coefficient with known sign there might be sign changes 
        // in between (if |attention=true|). In this case there is something
        // wrong, hence we set |variation_not_possible = true| in order to not
        // return $0$ or $1$. Further we hope to compute the sign from now
        // on, therefore we set |attention = false|.

        else if(attention) // +?+ or -?- or ***?..?***
        {
           variation_not_possible = true;
           attention = false;
        }
    }

    if(attention && attention_first) 
    {
      return -1;  // case ?****?
    }
    if(attention || attention_first) 
    {
       return (variations +3); // cases ?*** or ***?
    }
    if(variation_not_possible) 
    {
       return -1; // the sign variation of the remaining
                                          // sequence might be wrong
    }
    return variations;
}


template <class T> inline
bool operator == (const polynomial<T>& p1, const polynomial<T>& p2) 
{
    if (p1.degree() != p2.degree()) return false;
    for (int i = p1.degree(); i >= 0; i--) if (p1[i] != p2[i]) return false;
    return true;
}
template <class T> inline
bool operator != (const polynomial<T>& p1, const polynomial<T>& p2)
{ return !(p1 == p2); }    
template <class T> inline
bool operator < (const polynomial<T>& p1, const polynomial<T>& p2)
{ return ( p1.compare(p2) < 0 ); }    
template <class T> inline
bool operator <= (const polynomial<T>& p1, const polynomial<T>& p2)
{ return ( p1.compare(p2) <= 0 ); }    
template <class T> inline
bool operator > (const polynomial<T>& p1, const polynomial<T>& p2)
{ return ( p1.compare(p2) > 0 ); }    
template <class T> inline
bool operator >= (const polynomial<T>& p1, const polynomial<T>& p2)
{ return ( p1.compare(p2) >= 0 ); }    

// mixed-mode, lefthand side
template <class T> inline
bool operator == (const T& num, const polynomial<T>& p)  
{
    return p.degree() == 0 && p[0] == num;
}
template <class T> inline
bool operator != (const T& num, const polynomial<T>& p) 
{ return !(num == p);}
template <class T> inline
bool operator < (const T& num, const polynomial<T>& p) 
{ return ( p.compare(num) > 0 );}
template <class T> inline
bool operator <=  (const T& num, const polynomial<T>& p) 
{ return ( p.compare(num) >= 0 );}
template <class T> inline
bool operator > (const T& num, const polynomial<T>& p) 
{ return ( p.compare(num) < 0 );}
template <class T> inline
bool operator >= (const T& num, const polynomial<T>& p) 
{ return ( p.compare(num) <= 0 );}

// mixed-mode, righthand side
template <class T> inline
bool operator == (const polynomial<T>& p, const T& num) 
{ return num == p; }
template <class T> inline
bool operator != (const polynomial<T>& p, const T& num) 
{ return !(num == p); }
template <class T> inline
bool operator < (const polynomial<T>& p, const T& num) 
{ return ( p.compare(num) < 0 );}
template <class T> inline
bool operator <= (const polynomial<T>& p, const T& num) 
{ return ( p.compare(num) <= 0 );}
template <class T> inline
bool operator > (const polynomial<T>& p, const T& num) 
{ return ( p.compare(num) > 0 );}
template <class T> inline
bool operator >= (const polynomial<T>& p, const T& num) 
{ return ( p.compare(num) >= 0 );}


  

template<class T> 
ostream& operator<<(ostream& os, const polynomial<T>& P)
{
 if (P.is_zero()) { os << "P[0 (0," << T() << ")]"; return os; }

 os << "P[" << P.degree();
 for (int i = 0; i <= P.degree(); i++) {
    if (P[i] != T()) os << "(" << i << ","
                        << P[i] << ")";
 }
 os << "]";
 return os;
}

template<class T> 
istream& operator>>(istream& is, polynomial<T>&) { return is; }

template <class T> inline
polynomial<T> operator - (const polynomial<T>& p1)
{ 
    int d = p1.degree();
    LEDA_VECTOR<T> cp(d + 1);
    for (int i = 0; i <= d; ++i ) cp[i]=-p1[i];
    polynomial<T> p(cp);
    return p;
}


template<class T> inline
polynomial<T> operator + (const polynomial<T>& p1, const polynomial<T>& p2)
{ 
    bool p1d_smaller_p2d = p1.degree() < p2.degree();
    int min,max,i;
    if (p1d_smaller_p2d) { min = p1.degree(); max = p2.degree(); }
    else                 { max = p1.degree(); min = p2.degree(); }

    LEDA_VECTOR<T> cp(max + 1);
    for (i = 0; i <= min; ++i ) cp[i] = p1[i]+p2[i];
    if (p1d_smaller_p2d)  for (; i <= max; ++i ) cp[i]=p2[i];
    else /* p1d >= p2d */ for (; i <= max; ++i ) cp[i]=p1[i];

    polynomial<T> p(cp);
    return p;
}

template <class T> inline
polynomial<T> operator - (const polynomial<T>& p1, const polynomial<T>& p2)
{ 
    bool p1d_smaller_p2d = p1.degree() < p2.degree();
    int min,max,i;
    if (p1d_smaller_p2d) { min = p1.degree(); max = p2.degree(); }
    else                 { max = p1.degree(); min = p2.degree(); }
    
    LEDA_VECTOR<T> cp(max + 1);
    for (i = 0; i <= min; ++i ) cp[i]=p1[i]-p2[i];
    if (p1d_smaller_p2d)  for (; i <= max; ++i ) cp[i]= -p2[i];
    else /* p1d >= p2d */ for (; i <= max; ++i ) cp[i]=  p1[i];

    polynomial<T> p(cp);
    return p;
}

template <class T> inline
polynomial<T> operator * (const polynomial<T>& p1, const polynomial<T>& p2)
{
    LEDA_VECTOR<T> cp(p1.degree()+p2.degree() + 1);
    // initialize with zeros
    int i;
    for(i=0; i<=p1.degree()+p2.degree(); i++) cp[i] =T();
    for(i=0; i <= p1.degree(); ++i)
        for (int j=0; j <= p2.degree(); ++j)
            cp[i+j] += (p1[i]*p2[j]); 

    polynomial<T> p(cp);
    return p;
}

template<class T>
polynomial<T> operator / (polynomial<T> p1, const T& num)
{ p1 /= num; return p1; }

// p2 | p1
template<class T>
polynomial<T> operator / (polynomial<T> p1, const polynomial<T>& p2)
{ 
  polynomial<T> q,r;
  p1.euclidean_division(p2,q,r);
  return q;
}


template <class T> inline
polynomial<T> operator += (polynomial<T>& p1, const polynomial<T>& p2)
{ p1 = p1 + p2; return p1; }

template <class T> inline
polynomial<T> operator -= (polynomial<T>& p1, const polynomial<T>& p2)
{ p1 = p1 - p2; return p1; }

template <class T> inline
polynomial<T> operator *= (polynomial<T>& p1, const polynomial<T>& p2)
{ p1 = p1 * p2; return p1; }

template <class T> inline
polynomial<T> operator /= (polynomial<T>& p1, const polynomial<T>& p2)
{ p1 = p1 / p2; return p1; }
template<class T>
polynomial<T> poly_gcd(polynomial<T> p1, polynomial<T> p2)
{
    // handle trivial cases
    if (p1.is_zero())
    {
        if (p2.is_zero()) return polynomial<T>(T(1));
        else return p2 / p2.lcoeff();   
    }
    if (p2.is_zero()) return p1 / p1.lcoeff();
    if (p2.degree() > p1.degree()) 
    {
        polynomial<T> p3 = p1; p1 = p2; p2 = p3;
    }


    polynomial<T> q, r;
    while (!p2.is_zero()) { 
        p1.euclidean_division(p2,q,r);
        p1 = p2; p2 = r;
    }
    p1 /= p1.lcoeff();   

    return p1;
}


template<class T>
T resultant(polynomial<T>& P, polynomial<T>& Q)
{
   sylvester_matrix<T> M(P,Q);
   return M.determinant();
}

template<class T>
bool possible_common_roots(polynomial<T>& P, polynomial<T>& Q)
{
   T r = resultant(P,Q);
   return (r==0);
}

#undef leda_array
#undef PACKAGE_SCOPE
#undef LEDA_VECTOR

#if LEDA_ROOT_INCL_ID == 600245
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

LEDA_END_NAMESPACE

#endif // _LEDA_POLYNOMIAL_H


