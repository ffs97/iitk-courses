/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  persist.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/window.h>
#include <LEDA/core/p_queue.h>
#include <LEDA/core/sortseq.h>

LEDA_BEGIN_NAMESPACE

double X_POS;  // current position of sweep line

#if defined(__aCC__)
template<>
#endif
int compare(const segment& s1, const segment& s2)
{ line l1(s1);
  line l2(s2);
  double y1 = l1.y_proj(X_POS);
  double y2 = l2.y_proj(X_POS);
  return compare(y1,y2);
}

LEDA_END_NAMESPACE

#include <LEDA/core/p_dictionary.h>

using namespace leda;



typedef p_queue<point,segment>    X_structure;
typedef p_dictionary<segment,int> Y_structure;

sortseq<double,Y_structure>  HISTORY;

void SWEEP(const list<segment>& L)
{
  // Precondition: L is a list of non-intersecting
  // from left to right directed line segments

  X_structure    X;
  Y_structure    Y;           
  segment        s;
  
  forall(s,L) // initialize the X_structure
  { X.insert(s.start(),s);
    X.insert(s.end(),s);
   }

  HISTORY.insert(-MAXDOUBLE,Y); // insert empty Y_structure at -infinity

  while( ! X.empty() )
  { 
    pq_item it = X.find_min();
    point    p = X.prio(it);
    segment  s = X.inf(it);

    X.del_item(it);

    X_POS = p.xcoord();
  
    if (s.start()==p)             
       Y = Y.insert(s,0);      // p is left end of s 
    else
       Y = Y.del(s);           // p is right end of s
  
    HISTORY.insert(X_POS,Y);   // insert Y into history sequence 
  }
  
  HISTORY.insert(MAXDOUBLE,Y); // insert empty Y_structure at +infinity
}



segment LOCATE(const point& p)
{
  segment seg(p,p.translate(1,0));   // small horizontal segment

  X_POS = p.xcoord();

  Y_structure Y = HISTORY.inf(HISTORY.locate_pred(X_POS));

  p_dic_item pit = Y.locate_succ(seg);

  if (pit)
     return Y.key(pit);
  else
     return segment(p,p);
}




int main()
{
  window W;

  W.display();

  list<segment> L;

  segment s;
  while (W >> s) 
  { // direct all segments from left to right (bottom to top)
    point p = s.source();
    point q = s.target();
    if (compare(p,q) > 0) s = segment(q,p);
    L.append(s);
    W.draw_segment(s);
   }

  SWEEP(L);

  segment x;

  point p;
  while (W >> p) 
  { W.draw_segment(x,white);
    W.set_line_width(1);
    W.draw_segment(x,black);
    x = LOCATE(p);
    W.set_line_width(2);
    W.draw_segment(x,red);
   }

  return 0;
}

