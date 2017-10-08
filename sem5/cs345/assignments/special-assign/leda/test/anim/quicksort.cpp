/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  quicksort.c
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


static int color_table[] = 
{ blue2, yellow, orange, brown, red, green, green2,cyan, blue, violet };

void swap(int* A,int i, int j, cv_dispatcher& cvdp)
{ leda::swap(A[i],A[j]);
  cvdp.scene_begin(10*(j-i));
  cvdp.swap(i,j);
  cvdp.scene_end();
  cvdp.scene_play();
}

void recursion_start(int l, int r, int level, cv_dispatcher& cvdp)
{
  int i;
  for(i=l; i<=r; i++) 
     cvdp.set_cell_color(i,color_table[level]);

  cvdp.draw();

  cvdp.scene_begin(16);
  for(i=l; i<=r; i++) cvdp.translate(i,0,4);
  cvdp.scene_end();

  cvdp.scene_play();
}

void recursion_finish(int l, int r, int level, cv_dispatcher& cvdp)
{ 
  cvdp.scene_begin(16);
  for(int i=l; i<=r; i++) cvdp.translate(i,0,-4);
  cvdp.scene_end();
  cvdp.scene_play();

  if (level > 0)
  { for(int i=l; i<=r; i++)  
      cvdp.set_cell_color(i,color_table[level-1]);
    cvdp.draw();
   }
}


template<class T>
void quicksort(T* A, int l, int r, int depth, cv_dispatcher& cvdp)
{ 
  recursion_start(l,r,depth,cvdp);

  int i = l;
  int j = r+1;

  cvdp.set_value_color(l,grey2);

  T s = A[l];

  for(;;)
  { while (A[++i] < s);
    while (s < A[--j]);
    if (i<j) swap(A,i,j,cvdp);
    else break;
   }

  swap(A,l,j,cvdp); 

  if (l <= j-1) quicksort(A,l,j-1,depth+1,cvdp);
  if (r >= j+1) quicksort(A,j+1,r,depth+1,cvdp);

  recursion_finish(l,r,depth,cvdp);
}


void bg_draw(window* wp, double, double, double, double)
{ int i;
  for(i=-8; i<=10; i++) 
  { if (i>0 || i > 10)
      wp->draw_hline(4*i,color_table[i-1]);
    else
      wp->draw_hline(4*i,grey1);
  }

  wp->draw_hline(0,grey3);

  for(i=-8; i<32; i++) 
  { point p(10*i,0);
    point q = p.translate(0,40);
    wp->draw_ray(q,p,grey1);
  }
}



int main()
{
  int  n = 16;

  cv_dispatcher cvdp;

  cv_view<gw_view,gw_stick_element>  sv1(500,450,"Stick View");
  sv1.win_init(-10,10*n,-50);
  sv1.set_bg_redraw(bg_draw);
  sv1.display(100,250);

  cv_view<gw_view,gw_box_element>    sv2(500,200,"Box View");
  sv2.win_init(-10,10*n,-20);
  sv2.display(640,250);

  cv_view<gw_view,gw_circle_element> sv3(500,200,"Circle View");
  sv3.win_init(-10,10*n,-20);
  sv3.display(640,500);

  cvdp.attach(sv1);
  cvdp.attach(sv2);
  cvdp.attach(sv3);


  int* A = new int[n+1];

  for(int i=0;i<n;i++) 
  { A[i] = 1;
    cvdp.new_element(i,A[i],10*i,0);
   }

  A[n] = MAXINT;

  cvdp.draw();

  for(;;)
  { 
    cvdp.scene_begin(32);
    for(int i=0;i<n;i++) 
    { int r = rand_int(-40,40);
      A[i] = r;
      cvdp.assign(i,double(r));
      cvdp.set_value_color(i,ivory);
     }
    cvdp.scene_end();
    cvdp.scene_play();

    sv1.message("click done to start animation");
    if (!sv1.wait()) break;
    sv1.message("");

    quicksort(A,0,n-1,0,cvdp);
   }
  
    delete[] A;
  
    return 0;
}






