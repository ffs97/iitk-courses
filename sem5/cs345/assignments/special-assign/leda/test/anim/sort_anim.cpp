/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  sort_anim.c
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

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif


static int color_table[] = 
{ blue2, yellow, orange, brown, /* red, */ green, green2,cyan, blue, violet };


static bool anim = true;
static bool bubble = false;



void swap_noanim(int* A,int i, int j, cv_dispatcher& cvdp)
{ leda::swap(A[i],A[j]);
  cvdp.swap(i,j);
}

void swap(int* A,int i, int j, cv_dispatcher& cvdp)
{ leda::swap(A[i],A[j]);
  cvdp.scene_begin(10 + 5*(j-i));
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
void quicksort_noanim(T* A, int l, int r, int depth, cv_dispatcher& cvdp)
{ 
  int i = l;
  int j = r+1;

  T s = A[l];

  for(;;)
  { while (A[++i] < s);
    while (s < A[--j]);
    if (i<j) swap_noanim(A,i,j,cvdp);
    else break;
   }

  swap_noanim(A,l,j,cvdp); 

  if (l <= j-1) quicksort_noanim(A,l,j-1,depth+1,cvdp);
  if (r >= j+1) quicksort_noanim(A,j+1,r,depth+1,cvdp);
}



template<class T>
void quicksort(T* A, int l, int r, int depth, cv_dispatcher& cvdp)
{ 
  recursion_start(l,r,depth,cvdp);

  int i = l;
  int j = r+1;

  cvdp.scene_begin(1);
  cvdp.p_assign(1,l);
  cvdp.p_assign(2,r);
  cvdp.scene_end();
  cvdp.scene_play();

  if (l < r) 
  { cvdp.scene_begin(1);
    cvdp.set_value_color(l,red);
    cvdp.focus(l);
    cvdp.scene_end();
    cvdp.scene_play();
   }

  T s = A[l];

  for(;;)
  {
    do { if (++i > r) break;
         cvdp.scene_begin(16);
         cvdp.p_assign(1,i);
         cvdp.scene_end();
         cvdp.scene_play();
        } while (A[i] < s);

    do { j--;
         cvdp.scene_begin(16);
         cvdp.p_assign(2,j);
         cvdp.scene_end();
         cvdp.scene_play();
        } while (s < A[j]);

    if (i >= j) break;

    swap(A,i,j,cvdp);
   }

  swap(A,l,j,cvdp); 

  cvdp.wait();

  cvdp.scene_begin(1);
  cvdp.set_value_color(j,grey2);
  cvdp.unfocus();
  cvdp.scene_end();
  cvdp.scene_play();

  if (l <= j-1) quicksort(A,l,j-1,depth+1,cvdp);
  if (r >= j+1) quicksort(A,j+1,r,depth+1,cvdp);

  recursion_finish(l,r,depth,cvdp);
}

template<class T>
void bubblesort(T* A, int n, cv_dispatcher& cvdp)
{ 
  while (n > 0)
  { for(int i = 0; i<n; i++) 
    { cvdp.scene_begin(1);
      if (i > 0) cvdp.set_value_color(i-1,ivory);
      cvdp.set_value_color(i,orange);
      cvdp.scene_end();
      cvdp.scene_play();
      if (A[i] > A[i+1]) swap(A,i,i+1,cvdp);
      else leda::wait(0.4);
     }
    n--;
    cvdp.scene_begin(1);
    cvdp.set_value_color(n,grey2);
    cvdp.scene_end();
    cvdp.scene_play();
    //if (!cvdp.wait()) exit(0);
   }
}
    


void bg_draw0(window* wp, double, double, double, double)
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




void stick_focus_draw(window* wp, double x, double y, double rx, double ry) 
{ wp->draw_hline(y + ry); }

void dot_focus_draw(window* wp, double x, double y, double rx, double ry) 
{ wp->draw_hline(y); }



int main(int argc, char** argv)
{
  int  n = 16;

  if (argc > 1) 
  { if (string(argv[1]) == "-n") anim = false;
    if (string(argv[1]) == "-b") bubble = true;
   }


  poly_motion poly_swap_motion;
  poly_swap_motion.append(rel_coord(0),abs_coord(10));
  poly_swap_motion.append(rel_coord(1),abs_coord(10));

  circular_motion circ_swap_motion(LEDA_PI/3);


 cv_dispatcher cvdp;
 cvdp.set_speed(0.25);

 cv_view<gw_view,gw_box_element,gw_pointer_element> sv1(900,250,"Array View");
 sv1.set_swap_motions(&circ_swap_motion,&circ_swap_motion);
 sv1.win_init(-10,10*n,-25);
 sv1.display(50,70);

 cv_view<gw_view,gw_stick_element,gw_pointer_element> sv2(443,450,"Stick View");
 sv2.win_init(-10,10*n,-25);
 sv2.set_focus_draw(stick_focus_draw);
 sv2.display(50,350);
 if (!anim) sv2.iconify();

 cv_view<gw_view,gw_dot_element,gw_pointer_element>    sv3(443,450,"Dot View");
 sv3.win_init(-10,10*n,-25);
 sv3.set_focus_draw(dot_focus_draw);
 sv3.display(507,350);
 if (!anim) sv3.iconify();


 cvdp.attach(sv1);
 cvdp.attach(sv2);
 cvdp.attach(sv3);


  int* A = new int[n+1];

  int i;
  for(i=0;i<n;i++) 
  { A[i] = 1;
    cvdp.new_element(i,A[i],10*i,0);
   }

  cvdp.new_pointer_element(1, 0,          0, -14);
  cvdp.set_pointer_color(1,green2);

  cvdp.new_pointer_element(2, n-1, 10*(n-1), -12);
  cvdp.set_pointer_color(2,brown);

  A[n] = MAXINT;

  list<int> L;
  for(i=0;i<n;i++)  L.append(5*(i+1));

  cvdp.draw();

do {

  L.permute();

  cvdp.scene_begin(32);

  int i = 0;
  int r;
  forall(r, L)
  { A[i] = r;

    //cvdp.scene_begin(16);
    //cvdp.p_assign(1,i);
    //cvdp.scene_end();
    //cvdp.scene_play();

    //cvdp.scene_begin(32);
    cvdp.assign(i,double(r));
    cvdp.set_value_color(i,ivory);
    //cvdp.scene_end();
    //cvdp.scene_play();
    i++;
   }

  cvdp.scene_end();
  cvdp.scene_play();

/*
  sv1.message("click done to start animation");
  if (!sv1.wait()) break;
  sv1.message("");
*/

  if (!cvdp.wait()) break;

  if (anim)
      if (bubble)
        bubblesort(A,n,cvdp);
      else
        quicksort(A,0,n-1,0,cvdp);
  else
   { cvdp.scene_begin(64);
     quicksort_noanim(A,0,n-1,0,cvdp);
     cvdp.scene_end();
     cvdp.scene_play();
    }
} while(cvdp.wait());

  delete[] A;
 
  return 0;
}






