/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  random_variate.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_RANDOM_VARIATE_H
#define LEDA_RANDOM_VARIATE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600013
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/system/basic.h>
#include <LEDA/core/random.h>
#include <LEDA/core/stack.h>
#include <LEDA/core/array.h>

LEDA_BEGIN_NAMESPACE

/*{\Manpage {random_variate} {} {Random Variates} {R}}*/ 

class random_variate{

/*{\Mdefinition
An instance $R$ of the data type |\Mname| is
a non-uniform random number generator. The generation process is
governed by an |array<int>| |w|. Let $\range{l}{r}$ be the index range of
|w| and let $W = \sum_i |w|[i]$ be the total weight. Then any integer
$i \in \range{l}{h}$ is generated 
with probability $|w|[i]/W$. The weight function |w| must
be non-negative and |W| must be non-zero.  
}*/


private:
  array<int> T, L, U;
  int l, h, n, K;  

public:

/*{\Mcreation}*/
  
  random_variate(const array<int>& w)
/*{\Mcreate creates an instance |\Mvar| of type |\Mname|. }*/

  { 
    l =  w.low(); h = w.high();
    int W = 0;
    array<int> u(l,h+1);
    n = 0;  // number of non-zero weights
    int i;
    for (i = l; i <= h; i++) 
    { W += u[i] = w[i];
      if ( u[i] < 0 ) 
         LEDA_EXCEPTION(1,"random variate: negative weight");
      if ( u[i] > 0 ) n++;
    }
    if ( n == 0 ) 
           LEDA_EXCEPTION(1,"random_variate: no nonzero weight");

    K = W/n + (W % n == 0? 0 : 1);

    u[h + 1] = K*(n+1) - W; n++;


    stack<int> Small,Large;
        
    for (i = l; i <= h + 1; i++)
    { if ( u[i] == 0 ) continue; 
      if ( u[i] <= K )  Small.push(i);
      else              Large.push(i); 
    }
        
    U = T = L = array<int>(n);

    for (int c = 0; c < n; c++)
    { int i = Small.pop();
      T[c] = u[i];
      L[c] = i; 
      if ( u[i] < K )
      { int j = Large.pop();
        U[c] = j;
        u[j] -= (K-u[i]);
        if ( u[j] <= K ) Small.push(j); else Large.push(j);
      }
    }
 }

/*{\Moperations 2 4}*/



  int generate()
/*{\Mop generates $i \in \range{l}{h}$ with probability $|w|[i]/W$.}*/
  { 
    int r; 
    do { int row = rand_int(1,K);
         int column = rand_int(0,n-1);
         r = (row <= T[column] ? L[column] : U[column]);
       } 
    while (r == h + 1);
    return r;
 }
};


#include <LEDA/core/random.h>
#include <LEDA/core/stack.h>
#include <LEDA/core/array.h>


/*{\Manpage {dynamic_random_variate} {} {Dynamic Random Variates} {R}}*/ 

class dynamic_random_variate{

/*{\Mdefinition

An instance $R$ of the data type |\Mname| is
a non-uniform random number generator. The generation process is
governed by an |array<int>| |w|. Let $\range{l}{r}$ be the index range of
|w| and let $W = \sum_i |w|[i]$ be the total weight. Then any integer
$i \in \range{l}{h}$ is generated 
with probability $|w|[i]/W$. The weight function |w| must
be non-negative and |W| must be non-zero. The weight function can be changed
dynamically. 
}*/


private:
  array<int> u;
  int n, h, l;  

public:

/*{\Mcreation}*/
  
  dynamic_random_variate(const array<int>& w)
/*{\Mcreate creates an instance |\Mvar| of type |\Mname|. }*/
  { 
    l = w.low(); h = w.high(); n = h - l + 1;
    u = array<int>(1,2*n - 1);
    int i;
    for (i = 0; i < n ; i++) 
    { u[n + i] = w[l + i];
      if ( u[n + i] < 0 )
        LEDA_EXCEPTION(1,"dynamic variate: negative weight");
    }
    for (i = n - 1; i > 0; i--)
      u[i] = u[2*i] + u[2*i + 1];

    if (u[1] == 0 ) 
        LEDA_EXCEPTION(1,"dynamic variate: no non-zero weight");
 }

/*{\Moperations 2 4}*/

  int generate()
/*{\Mop generates $i \in \range{l}{h}$ with probability $|w|[i]/W$.}*/
  { 
    int s = rand_int(0,u[1] - 1);

    int i = 1;
    while ( i < n )
    { int j = 2*i;
      if ( s < u[j] ) 
        i = j;
      else 
        { i = j + 1;
          s -= u[j];
        }
    }
    return l + i - n;
 }

  int set_weight(int i, int g)
/*{\Mop sets $|w|[i]$ to $g$ and returns the old value of |w[i]|.\\
\precond $i \in \range{l}{h}$.}*/
  { 
    i = n + (i - l);

    int ui = u[i];
    int delta = g - u[i];
    if ( g < 0 ) LEDA_EXCEPTION(1,"dynamic variate: negative weight");
    while (i > 1)
    { u[i] += delta;
      i = i/2;
    } 
    u[1] += delta;
    if ( u[1] == 0 ) 
      LEDA_EXCEPTION(1,"dynamic variate: no positive weight");
    return ui;
 }


};

#if LEDA_ROOT_INCL_ID == 600013
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

LEDA_END_NAMESPACE

#endif

