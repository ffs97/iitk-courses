/*******************************************************************************
+
+  LEDA 6.3
+
+
+  isolating.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/

// Author(s): Susanne Schmitt  <sschmitt@mpi-sb.mpg.de>

#ifndef _LEDA_ISOLATING_H
#define _LEDA_ISOLATING_H

#include <LEDA/numbers/polynomial.h>
#include <LEDA/numbers/real.h>

#undef LEDA_VECTOR
#define LEDA_VECTOR growing_array

#define PACKAGE_SCOPE

LEDA_BEGIN_NAMESPACE


template <class T>
class isolating;


template<class T>
class Sturm_chain 
{
    friend class isolating<T>;

protected:

    LEDA_VECTOR<polynomial<T> > V;
public:

    Sturm_chain(){};
    Sturm_chain(polynomial<T> P);

    int sign_variation(rational a);
};

template <class T>
class isolating {
    
    polynomial<T> P_;
    int number_of_real_roots_;
    rational* left;   
    rational* right; 
    bool* is_exact;
    integer real_root_bound;
    bool one_root;
    int root_number;
    bool given_interval;
    bool interval_coefficients;
    bool can_be_isolated_;

private:
 
 integer weak_upper_root_bound();

 void Uspensky();
 void Uspensky_with_gcd();
 void Uspensky(rational low, rational upp);
 void Uspensky_with_gcd(rational low, rational upp);
 void zero_one_uspensky(const polynomial<T>& P, integer i, integer D);
 void strong_zero_one_uspensky(const polynomial<T>& P, integer i, integer D);

 void Sturm();
 void Sturm(rational low, rational upp);
 void Sturm_algorithm(Sturm_chain<T> sturm, rational a, rational b);
 void strong_Sturm_algorithm(Sturm_chain<T> sturm, rational a, rational b);
 int  Sturm_sign_variation(rational a);

     
public:
     
     isolating(){}


     isolating(const polynomial<T>& P, algorithm_type algorithm=STURM, bool ic=false)
         : P_(P) {

         int d = P.degree();
         
         left     = new rational[d]; 
         right    = new rational[d];
         is_exact = new bool[d]; 
         number_of_real_roots_ = 0;
         interval_coefficients = ic;
         can_be_isolated_=true;
         if(d!=0) real_root_bound = weak_upper_root_bound();


         one_root = false;
         given_interval = false;

         
         if(P.degree()==0) 
         {
            if(P[0] == T(0)) number_of_real_roots_ = -1;
            return;
         }

         if(!can_be_isolated_) return;
         if(real_root_bound== integer(0)) 
         {
            if(P[0] == T(0))
            {
                 left[0] = rational(0,1);
                 right[0] = rational(0,1);
                 is_exact[0] = true;
                 number_of_real_roots_ = 1;
            }
            return;
         }

         
         switch(algorithm)
         {
            case USPENSKY: 
                    Uspensky();
                    break;
            case USPENSKY_WITH_GCD:
                    Uspensky_with_gcd();
                    break;
            case STURM:
            default: // Sturm
                    Sturm();
                    break;
         }
         #ifdef CHECK_REALS
         
         if(!ic && algorithm == USPENSKY)
         {
            if(one_root)
            {
               int i = root_number;
               if(is_exact_root(i)) assert( left_boundary(i) == right_boundary(i)
                                    && P.evaluate(left_boundary(i)) == T(0) );
               else assert(P.evaluate(left_boundary(i))*P.evaluate(right_boundary(i))<T(0));
            }
            else
            {
               for(int i=0; i<number_of_real_roots(); i++)
               {
                  if(is_exact_root(i)) assert( left_boundary(i) == right_boundary(i)
                                       && P.evaluate(left_boundary(i)) == T(0) );
                  else assert(P.evaluate(left_boundary(i))*P.evaluate(right_boundary(i))<T(0));
               }
            }
         }

         #endif


     }


     isolating(const polynomial<T>& P, int i, algorithm_type algorithm=STURM, bool ic=false)
         : P_(P) {

         int d = i+1; 
         
         left     = new rational[d]; 
         right    = new rational[d];
         is_exact = new bool[d]; 
         number_of_real_roots_ = 0;
         interval_coefficients = ic;
         can_be_isolated_=true;
         if(d!=0) real_root_bound = weak_upper_root_bound();


         one_root = true;
         root_number = i;
         given_interval = false;

         
         if(P.degree()==0) 
         {
            if(P[0] == T(0)) number_of_real_roots_ = -1;
            return;
         }

         if(!can_be_isolated_) return;
         if(real_root_bound== integer(0)) 
         {
            if(P[0] == T(0))
            {
                 left[0] = rational(0,1);
                 right[0] = rational(0,1);
                 is_exact[0] = true;
                 number_of_real_roots_ = 1;
            }
            return;
         }

         
         switch(algorithm)
         {
            case USPENSKY: 
                    Uspensky();
                    break;
            case USPENSKY_WITH_GCD:
                    Uspensky_with_gcd();
                    break;
            case STURM:
            default: // Sturm
                    Sturm();
                    break;
         }
         #ifdef CHECK_REALS
         
         if(!ic && algorithm == USPENSKY)
         {
            if(one_root)
            {
               int i = root_number;
               if(is_exact_root(i)) assert( left_boundary(i) == right_boundary(i)
                                    && P.evaluate(left_boundary(i)) == T(0) );
               else assert(P.evaluate(left_boundary(i))*P.evaluate(right_boundary(i))<T(0));
            }
            else
            {
               for(int i=0; i<number_of_real_roots(); i++)
               {
                  if(is_exact_root(i)) assert( left_boundary(i) == right_boundary(i)
                                       && P.evaluate(left_boundary(i)) == T(0) );
                  else assert(P.evaluate(left_boundary(i))*P.evaluate(right_boundary(i))<T(0));
               }
            }
         }

         #endif


     }


     // [low, upp] is isolating interval for a root of P. We use algorithm to
     // make the interval small.
     isolating(const polynomial<T>& P, rational low, rational upp, algorithm_type algorithm=STURM, bool ic=false)
         : P_(P) {

         int d = 1; 
         
         left     = new rational[d]; 
         right    = new rational[d];
         is_exact = new bool[d]; 
         number_of_real_roots_ = 0;
         interval_coefficients = ic;
         can_be_isolated_=true;
         if(d!=0) real_root_bound = weak_upper_root_bound();


         one_root = true;
         root_number = 0;
         given_interval = true;

         switch(algorithm)
         {
            case USPENSKY: 
                    Uspensky(low, upp);
                    break;
            case USPENSKY_WITH_GCD:
                    Uspensky_with_gcd(low, upp);
                    break;
            case STURM:
            default: // Sturm
                    Sturm(low, upp);
                    break;
         }
 #ifdef CHECK_REALS
         
         if(!ic && algorithm == USPENSKY)
         {
            if(one_root)
            {
               int i = root_number;
               if(is_exact_root(i)) assert( left_boundary(i) == right_boundary(i)
                                    && P.evaluate(left_boundary(i)) == T(0) );
               else assert(P.evaluate(left_boundary(i))*P.evaluate(right_boundary(i))<T(0));
            }
            else
            {
               for(int i=0; i<number_of_real_roots(); i++)
               {
                  if(is_exact_root(i)) assert( left_boundary(i) == right_boundary(i)
                                       && P.evaluate(left_boundary(i)) == T(0) );
                  else assert(P.evaluate(left_boundary(i))*P.evaluate(right_boundary(i))<T(0));
               }
            }
         }

 #endif


     }

     ~isolating()
     { delete[] left; delete[] right; delete[] is_exact; }
    

 
     int number_of_real_roots()     const { return number_of_real_roots_; }
     bool is_exact_root(int i)      const { return is_exact[i]; }
     bool is_exact_root()           const { return is_exact[0]; }
     const rational& left_boundary(int i)  const { return left[i]; }
     const rational& left_boundary()       const { return left[0]; }
     const rational& right_boundary(int i) const { return right[i]; }
     const rational& right_boundary()      const { return right[0]; }
     bool can_be_isolated()         const { return can_be_isolated_; }
    
};



// Returns an upper bound on the absolute value of all roots of $P$.
// The upper bound is a power of two. Only works for univariate polynomials.
template <class T>
integer isolating<T>::weak_upper_root_bound() 
{ 
  
        const int n = P_.degree();
        integer r(1);  // return value
        T x(1);  // needed to "evaluate" the polynomial
        T val;
        int depth = 0;
        for (;;) {
            val = -abs(P_[n]);
            for (int i = n-1; i >= 0; i--) {
                val = val*x + abs(P_[i]);
            }
            if(interval_coefficients)
            {
               if (!PACKAGE_SCOPE sign_is_known(val)) 
               {
                   if(depth == 10)
                   {
                      can_be_isolated_ = false;
                      return integer(0);
                   }
                   else depth++;
               }
               else if (PACKAGE_SCOPE sign(val) == -1) return r;
            }
            else if (val < T(0)) return r;
            r *= integer(2); 
            x = T(r);
        }
}



template <class T>
void isolating<T>::Uspensky()
{
   
   polynomial<T> R = scale_up(P_,T(4*real_root_bound)); 
   polynomial<T> Q = translate(R,T(-1),T(2));       
   zero_one_uspensky(Q,0,1);

}


template<class T>
void isolating<T>::Uspensky_with_gcd()
{
   
   polynomial<T> g = poly_gcd(P_, diff(P_));
   polynomial<T> p,r;
   P_.euclidean_division(g,p,r);
   P_ = p;

   
   polynomial<T> R = scale_up(P_,T(4*real_root_bound)); 
   polynomial<T> Q = translate(R,T(-1),T(2));       
   zero_one_uspensky(Q,0,1);

}


template <class T>
void isolating<T>::Uspensky(rational low, rational upp)
{
    
    if(P_.evaluate(T(low)) == 0)
    {
        left[number_of_real_roots_] = low;
        right[number_of_real_roots_] = low;
        is_exact[number_of_real_roots_] = true;
        number_of_real_roots_=1;
        return;
    }
    if(P_.evaluate(T(upp)) == 0)
    {
        left[number_of_real_roots_] = upp;
        right[number_of_real_roots_] = upp;
        is_exact[number_of_real_roots_] = true;
        number_of_real_roots_=1;
        return;
    }

    
    // P_ has one root in (low, upp)
    // Q has one root in (0,1)
    // Q(x) = P_((upp - low)*x + low)
    polynomial<T> Q = scale_up(translate(P_,T(low)),T(upp-low));
        
    zero_one_uspensky(Q,0,1);

    // in this case we get a root of Q(x) and have to transform it back
    left[0] = low + (upp-low)*left[0];
    right[0] = low + (upp-low)*right[0];

}

template <class T>
void isolating<T>::Uspensky_with_gcd(rational low, rational upp)
{
    
    if(P_.evaluate(T(low)) == 0)
    {
        left[number_of_real_roots_] = low;
        right[number_of_real_roots_] = low;
        is_exact[number_of_real_roots_] = true;
        number_of_real_roots_=1;
        return;
    }
    if(P_.evaluate(T(upp)) == 0)
    {
        left[number_of_real_roots_] = upp;
        right[number_of_real_roots_] = upp;
        is_exact[number_of_real_roots_] = true;
        number_of_real_roots_=1;
        return;
    }

    
    polynomial<T> g = poly_gcd(P_, diff(P_));
    polynomial<T> p,r;
    P_.euclidean_division(g,p,r);
    P_ = p;

    
    // P_ has one root in (low, upp)
    // Q has one root in (0,1)
    // Q(x) = P_((upp - low)*x + low)
    polynomial<T> Q = scale_up(translate(P_,T(low)),T(upp-low));
        
    zero_one_uspensky(Q,0,1);

    // in this case we get a root of Q(x) and have to transform it back
    left[0] = low + (upp-low)*left[0];
    right[0] = low + (upp-low)*right[0];

}


template <class T>
void isolating<T>::zero_one_uspensky(const polynomial<T>& P, integer i, integer D) 
{ 
     if(!can_be_isolated_) return;
     if(one_root && number_of_real_roots_> root_number) return;

     polynomial<T> R = variation_transformation(P);
     int descartes;
     if(interval_coefficients) 
     {
        descartes = sign_variations_ip(R);

        
        if(descartes == -1)
        {
           can_be_isolated_ = false;
           return;
        }
        else if(descartes == 3)
        {
           if(!PACKAGE_SCOPE sign_is_known(P[0]) || !PACKAGE_SCOPE sign_is_known(P.evaluate(T(1))))
           {
              can_be_isolated_ = false;
              return;
           }
           if(PACKAGE_SCOPE sign(P[0]) == PACKAGE_SCOPE sign(P.evaluate(T(1)))) descartes = 0;
           else descartes = 1;
        }
        else if(descartes == 4)
        {
           if(!PACKAGE_SCOPE sign_is_known(P[0]) || !PACKAGE_SCOPE sign_is_known(P.evaluate(T(1))))
           {
              can_be_isolated_ = false;
              return;
           }
           if(PACKAGE_SCOPE sign(P[0]) == PACKAGE_SCOPE sign(P.evaluate(T(1)))) 
           {
              can_be_isolated_ = false;
              return;
           }
           else descartes = 1;
        }

     }
     else descartes = sign_variations(R);       

     
        // no root
        if ( descartes == 0 ) return;

        // exactly one root
        if ( descartes == 1 && P[0] != T(0) && P.evaluate(T(1)) != T(0) ) 
        { 
            if(!one_root || number_of_real_roots_ == root_number)
                   strong_zero_one_uspensky(P,i,D);
            number_of_real_roots_++;
            return;
        }


     i = 2*i; D = 2*D;

     // First half
     polynomial<T> Q = scale_down(P,T(2)); 
     zero_one_uspensky(Q,i,D);

     if(one_root && number_of_real_roots_> root_number) return;
     
     // midpoint and second half 
     polynomial<T>  S = translate_by_one(Q);    
     if ( S[0] == T(0) ) 
     { 
        if(!one_root || number_of_real_roots_ == root_number)
        {
           
              integer num;
              if(given_interval) num = i+1;
              else
              {
                  integer vb = 4*real_root_bound;
                  num = vb *(i+1) - 2*real_root_bound*D;
              }
              left[number_of_real_roots_] = rational(num,D);
              right[number_of_real_roots_] = rational(num,D);
              is_exact[number_of_real_roots_] = true;

        }
        number_of_real_roots_++;
     }
     zero_one_uspensky(S,i+1,D); 
}



template <class T>
void isolating<T>::strong_zero_one_uspensky(const polynomial<T>& P, integer i, integer D) 
{ 

     if(!can_be_isolated_) return;
     if(D > 8*real_root_bound) // interval is small enough
     {
       integer vb, num;
       if(given_interval) { vb = 1; num = i; }
       else
       {
          vb = 4*real_root_bound;
          num = vb *i - 2*real_root_bound*D;
       }
       left[number_of_real_roots_] = rational(num,D);
       num = num + vb;
       right[number_of_real_roots_] = rational(num,D);
       is_exact[number_of_real_roots_] = false;
       return;
     }

     // Refine the interval.
     i = 2*i; D = 2*D;

     // First half
     polynomial<T> Q = scale_down(P,T(2)); 

     // Test if the polynomial is zero at the midpoint of the interval 
     polynomial<T>  S = translate_by_one(Q);    
     if ( S[0] == T(0) ) 
     { 
        
           integer num;
           if(given_interval) num = i+1;
           else
           {
               integer vb = 4*real_root_bound;
               num = vb *(i+1) - 2*real_root_bound*D;
           }
           left[number_of_real_roots_] = rational(num,D);
           right[number_of_real_roots_] = rational(num,D);
           is_exact[number_of_real_roots_] = true;

        return;
     }
         
     if(interval_coefficients)
     {
        int descartes = sign_variations_ip(variation_transformation(Q));

        
        if(descartes == -1)
        {
           can_be_isolated_ = false;
           return;
        }
        else if(descartes == 3)
        {
           if(!PACKAGE_SCOPE sign_is_known(P[0]) || !PACKAGE_SCOPE sign_is_known(P.evaluate(T(1))))
           {
              can_be_isolated_ = false;
              return;
           }
           if(PACKAGE_SCOPE sign(P[0]) == PACKAGE_SCOPE sign(P.evaluate(T(1)))) descartes = 0;
           else descartes = 1;
        }
        else if(descartes == 4)
        {
           if(!PACKAGE_SCOPE sign_is_known(P[0]) || !PACKAGE_SCOPE sign_is_known(P.evaluate(T(1))))
           {
              can_be_isolated_ = false;
              return;
           }
           if(PACKAGE_SCOPE sign(P[0]) == PACKAGE_SCOPE sign(P.evaluate(T(1)))) 
           {
              can_be_isolated_ = false;
              return;
           }
           else descartes = 1;
        }


        if(descartes ==  1) 
        {
            strong_zero_one_uspensky(Q,i,D);
            return;
        }
     }
     else
     {
        if(sign_variations(variation_transformation(Q)) == 1) {
            strong_zero_one_uspensky(Q,i,D);
            return;
        }
     }
     
     // Consider the second half of the interval. 
     strong_zero_one_uspensky(S,i+1,D); 
     return;
}





template<class T>
Sturm_chain<T>::Sturm_chain(polynomial<T> P)
{
      V.push_back(P);

      polynomial<T>     P0,P1,Q,R;
      R = diff(P);
      P0=P;
      P1=R;
      while(!R.is_zero())
      {
        V.push_back(R);
        P0.euclidean_division(P1,Q,R);
        R = -R;
        P0 = P1;
        P1 = R;
      }
}

// this function returns 0,1 if there are 0 or 1 sign variation and 2 if there
// are 2 or more sign variations.
template<class T>
int Sturm_chain<T>::sign_variation(rational a)
{
    int var = 0;


    int k = V.size();

    int s, s_old;
    s_old = PACKAGE_SCOPE sign(V[0].evaluate(T(a)));
    //s_old = PACKAGE_SCOPE sign(V[0].evaluate(T(a.numerator())/T(a.denominator())));
    for(int i=1; i<k; i++)
    {
       s = PACKAGE_SCOPE sign(V[i].evaluate(T(a)));
       //s = PACKAGE_SCOPE sign(V[i].evaluate(T(a.numerator())/T(a.denominator())));
       if(s!=0 && s!=s_old)
       {
          s_old = s;
          var++;
       }
    }
    return var;
}


template <class T>
  void isolating<T>::Sturm()
  {
      Sturm_chain<T> sturm(P_);
      Sturm_algorithm(sturm, -real_root_bound, real_root_bound);
  }

// [low, upp] is isolating interval for P_.
template <class T>
  void isolating<T>::Sturm(rational low, rational upp)
  {
      
      if(P_.evaluate(T(low)) == 0)
      {
          left[number_of_real_roots_] = low;
          right[number_of_real_roots_] = low;
          is_exact[number_of_real_roots_] = true;
          number_of_real_roots_=1;
          return;
      }
      if(P_.evaluate(T(upp)) == 0)
      {
          left[number_of_real_roots_] = upp;
          right[number_of_real_roots_] = upp;
          is_exact[number_of_real_roots_] = true;
          number_of_real_roots_=1;
          return;
      }


      Sturm_chain<T> sturm(P_);
      strong_Sturm_algorithm(sturm, low, upp);
      number_of_real_roots_ = 1;
  }


template <class T>
  void isolating<T>::Sturm_algorithm(Sturm_chain<T> sturm, rational a, rational b)
  {
      if(one_root && number_of_real_roots_> root_number) return;

      int vab = sturm.sign_variation(a) - sturm.sign_variation(b);

      
      if(vab==0) return;
      if(vab==1)
      {
          if(!one_root || number_of_real_roots_ == root_number)
              strong_Sturm_algorithm(sturm,a,b);
          number_of_real_roots_++;
          return;
      }

      
           // 'midpoint'
           rational m = (a+b)/rational(2,1);
           int i=3;
           while(P_.evaluate(T(m.numerator())/T(m.denominator())) == 0)
           {
               m = (a+rational(i,1)*b)/rational(i+1,1); // m = (a+(2^j-1)b)/2^j
               i = 2*i + 1;                             // i = 2^j-1
           }


      // lower half of the interval
      Sturm_algorithm(sturm, a,m);

      // upper half of the interval
      Sturm_algorithm(sturm, m,b);

      return;
  }


template <class T>
  void isolating<T>::strong_Sturm_algorithm(Sturm_chain<T> sturm, rational a, rational b)
  {
     if(4*(b-a) < 1) // interval small
     {
       left[number_of_real_roots_] = a;
       right[number_of_real_roots_] = b;
       is_exact[number_of_real_roots_] = false;
       return;
     }
    
          // 'midpoint'
          rational m = (a+b)/rational(2,1);
          int i=3;
          while(P_.evaluate(T(m.numerator())/T(m.denominator())) == 0)
          {
              m = (a+rational(i,1)*b)/rational(i+1,1); // m = (a+(2^j-1)b)/2^j
              i = 2*i + 1;                             // i = 2^j-1
          }


     // lower or upper half of the interval
     int vam = sturm.sign_variation(a) - sturm.sign_variation(m);
     if(vam == 1) strong_Sturm_algorithm(sturm, a,m);
     else strong_Sturm_algorithm(sturm,m,b);
     return;
  }


LEDA_END_NAMESPACE

#undef PACKAGE_SCOPE
#undef LEDA_VECTOR

#endif // _LEDA_ISOLATING_H


