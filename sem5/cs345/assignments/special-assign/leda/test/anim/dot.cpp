/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  dot.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/anim/gw_view.h>
#include <LEDA/anim/cv_view.h>

using namespace leda;


static bool quick = false;


void swap(int* A,int i, int j, cv_dispatcher& cvdp)
{ leda::swap(A[i],A[j]);
  cvdp.scene_begin(j-i);
  cvdp.swap(i,j);
  cvdp.scene_end();
  cvdp.scene_play();
}

void set_color(cv_dispatcher& cvdp, int i, color col)
{ cvdp.scene_begin(1);
  cvdp.set_value_color(i,col);
  cvdp.scene_end();
  cvdp.scene_play();
}


template<class T>
void quicksort(T* A, int l, int r, int depth, cv_dispatcher& cvdp)
{ 
  int i = l;
  int j = r+1;

  T s = A[l];


  for(;;)
  { while (A[++i] < s);
    while (s < A[--j]);
    if (i<j) swap(A,i,j,cvdp);
    else break;
   }

  swap(A,l,j,cvdp); 
  set_color(cvdp,j,grey2);

  if (l <= j-1) quicksort(A,l,j-1,depth+1,cvdp);
  if (r >= j+1) quicksort(A,j+1,r,depth+1,cvdp);
}

template<class T>
void bubblesort(T* A, int n, cv_dispatcher& cvdp)
{ 
  while (n > 0)
  { for(int i = 0; i<n; i++) 
      if (A[i] > A[i+1]) swap(A,i,i+1,cvdp);
    n--;
    set_color(cvdp,n,grey2);
   }
}
    

int main(int argc, char** argv)
{
  int i = 0;
  int n = 64;

  if (argc > 1) 
  { 
    if (string(argv[1]) == "-q") quick = true;
   }


  cv_dispatcher cvdp;

  cv_view<gw_view,gw_dot_element>    sv1(500,500,"Dot View");
  sv1.win_init(-10,10*n,-10);

if (quick)
  sv1.display(512,100);
else
  sv1.display(0,100);

  cvdp.attach(sv1);


  int* A = new int[n+1];

  for(i=0;i<n;i++) 
  { A[i] = 1;
    cvdp.new_element(i,A[i],10*i,0);
   }

  A[n] = MAXINT;

  list<int> L;
  for(i=0;i<n;i++)  L.append(8*(i+1));

  cvdp.draw();

//do {

  L.permute();

  cvdp.scene_begin(32);
  i = 0;
  int r;
  forall(r, L)
  { A[i] = r;
    cvdp.assign(i,double(r));
    cvdp.set_value_color(i,yellow);
    i++;
   }
  cvdp.scene_end();
  cvdp.scene_play();

/*
  sv1.message("click done to start animation");
  if (!sv1.wait()) break;
  sv1.message("");
*/

  if (!cvdp.wait()) /*break */;

  if (quick)
    quicksort(A,0,n-1,0,cvdp);
  else
    bubblesort(A,n,cvdp);

//} while(cvdp.wait());

  cvdp.wait();

  delete[] A;
 
  return 0;
}
