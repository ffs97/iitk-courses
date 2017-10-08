/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  qs.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/anim/gw_view.h>
#include <LEDA/anim/cv_view.h>
#include <LEDA/system/assert.h>

using namespace leda;


static int color_table[] = 
{ blue2, yellow, orange, brown, /* red, */ green, green2,cyan, blue, violet };


template <class T>
class event_array : public cv_dispatcher {

T*     A;
size_t sz;

color default_clr;
color focus_clr;

int   depth;

public:

 event_array() : A(0), sz(0), default_clr(grey2), focus_clr(red), depth(0) {}

~event_array() { if (A) delete[] A;     }

 void init(size_t s) 
 { if (A) delete[] A;
   sz = s;
   A = new T[s+1]; 
   A[s] = MAXINT;
   for(size_t i=0; i<s; i++) 
   { A[i] = 1;
     new_element(i,A[i],10*i,0);
     set_value_color(i,ivory);
    }
   draw();
  }


 void set(int i, const T& val) 
 { A[i] = val; 
   cv_dispatcher::scene_begin(10);
   cv_dispatcher::assign(i,double(val));
   cv_dispatcher::scene_end();
   cv_dispatcher::scene_play();
  }

 void set(const list<int>& L) 
 { cv_dispatcher::scene_begin(32);
   size_t i = 0;
   int x;
   forall(x,L)
   { if (i >= sz) break;
     A[i] = x;
     cv_dispatcher::assign(i,double(x));
     i++;
    }
   cv_dispatcher::scene_end();
   cv_dispatcher::scene_play();
  }
   


 void swap(int i, int j) 
 { T tmp = A[i];
   A[i] = A[j];
   A[j] = tmp;
   cv_dispatcher::scene_begin(10*(j-i));
   cv_dispatcher::swap(i,j);
   cv_dispatcher::scene_end();
   cv_dispatcher::scene_play();
  }
     
 const T& operator[](int i) { return A[i]; }

 void focus(int i)
 { cv_dispatcher::set_value_color(i,red);
   cv_dispatcher::focus(i);
   cv_dispatcher::draw();
  }

 void unfocus(int i)
 { cv_dispatcher::set_value_color(i,grey2);
   cv_dispatcher::unfocus();
   cv_dispatcher::draw();
  }

 void begin_recursion(int l, int r)
 { 
   int level = depth++;

   int i;
   for(i=l; i<=r; i++) 
      cv_dispatcher::set_cell_color(i,color_table[level]);
 
   cv_dispatcher::draw();
 
   cv_dispatcher::scene_begin(16);
   for(i=l; i<=r; i++) cv_dispatcher::translate(i,0,4);
   cv_dispatcher::scene_end();
 
   cv_dispatcher::scene_play();
 }
 
 void end_recursion(int l, int r)
 { 
   int level = depth--;
   cv_dispatcher::scene_begin(16);
   for(int i=l; i<=r; i++) cv_dispatcher::translate(i,0,-4);
   cv_dispatcher::scene_end();
   cv_dispatcher::scene_play();
 
   if (level > 0)
   { for(int i=l; i<=r; i++)  
       cv_dispatcher::set_cell_color(i,color_table[level-1]);
     cv_dispatcher::draw();
    }
  }

};



static bool step = true;


template<class T>
void quicksort(event_array<T>& A, int l, int r)
{ 
  A.begin_recursion(l,r);

  int i = l;
  int j = r+1;

  if (l < r) A.focus(l);

  T s = A[l];

  for(;;)
  { while (A[++i] < s);
    while (s < A[--j]);
    if (i<j) A.swap(i,j);
    else break;
   }

  A.swap(l,j); 
  if (step) step = A.wait();
  A.unfocus(j);

  if (l <= j-1) quicksort(A,l,j-1);
  if (r >= j+1) quicksort(A,j+1,r);

  A.end_recursion(l,r);
}



void stick_focus_draw(window* wp, double x, double y, double rx, double ry) 
{ wp->draw_hline(y + ry); }

void dot_focus_draw(window* wp, double x, double y, double rx, double ry) 
{ wp->draw_hline(y); }
    
void stick_bg_draw(window* wp, double, double, double, double)
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

  event_array<int> A;

  cv_view<gw_view,gw_box_element>    sv1(900,200,"Array View");
  sv1.win_init(-10,10*n,-15);
  sv1.display(50,70);

  cv_view<gw_view,gw_stick_element>  sv2(443,450,"Stick View");
  sv2.win_init(-10,10*n,-10);
  sv2.set_focus_draw(stick_focus_draw);
  //sv2.set_bg_redraw(stick_bg_draw);
  sv2.display(50,300);

  cv_view<gw_view,gw_dot_element>    sv3(443,450,"Dot View");
  sv3.win_init(-10,10*n,-10);
  sv3.set_focus_draw(dot_focus_draw);
  sv3.display(507,300);

  A.attach(sv1);
  A.attach(sv2);
  A.attach(sv3);

  A.init(n);


  list<int> L;
  for(int i=0;i<n;i++)  L.append(5*(i+1));


  do { L.permute();
       A.set(L);
       if (!A.wait()) break;
       quicksort(A,0,n-1);
     } while(A.wait());

  return 0;
}






