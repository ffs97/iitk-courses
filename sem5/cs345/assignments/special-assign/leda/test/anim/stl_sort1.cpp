/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  stl_sort1.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/anim/gw_view.h>
#include <LEDA/anim/cell.h>
#include <LEDA/anim/smart_pointer.h>
#include <assert.h>

#include <algorithm>

using namespace leda;

using std::cout;
using std::endl;

typedef cell<int> Int;

typedef smart_pointer<Int> Iterator;


static gw_view V(500,450,"STL Sort");
static map<const Int*,gw_stick_element> M;  
static map<const Iterator*,gw_pointer_element> P;  
static bool Wait = false;
static int id = 0;
static int id1 = 0;
static int id2 = 0;


const int steps = 150;


void assign_value(Int& i, const int& x) 
{ V.scene_begin(steps);
  M[&i].assign(x); 
  V.scene_end();
  V.scene_play(); 
}

void assign_cell(Int& i, const Int& j) 
{ gw_stick_element s1 = M[&i];
  gw_stick_element s2 = M[&j];

  point pi = M[&i].get_position();
  point pj = M[&j].get_position();

  double angle = LEDA_PI/2; 
  if (pi.xcoord() < pj.xcoord()) angle = -angle;

  circular_motion cm(angle);   
  
  V.scene_begin(steps);
  s1.assign(s2,&cm);   
  V.scene_end();  
  V.scene_play(); 
}

void construct_cell(Int& i, const int& x)
{ 
cout << "construct_cell x = "  << x << endl;
  gw_stick_element st(x,10,point(130 + 35 * id++, 300));
  V.add_element(st);  
  V.draw();
  M[&i] = st;
}


void copy_cell(Int& i, const Int& j)
{ M[&i] = gw_stick_element(0,10,point(50 + 35 * id1++, 350));
  V.add_element(M[&i]);  
  assign_cell(i,j);
}


void delete_cell(Int& i)
{ //if (Wait) V.wait();
  V.del_element(M[&i]); id1--; 
}






void assign_pointer(Iterator& i, const Int* p) 
{
  cout << "assign pointer" << endl;

  V.scene_begin(steps);
  P[&i].assign(M[p]); 
  V.scene_end();
  V.scene_play(); 
}

void assign_iterator(Iterator& i, const Iterator& j) 
{
  cout << "assign iterator" << endl;
  gw_pointer_element p1 = P[&i];
  gw_stick_element c2 = M[&(*j)];

  V.scene_begin(steps);
  p1.assign(c2);   
  V.scene_end();  
  V.scene_play(); 
}

void construct_iterator(Iterator& i, Int* p)
{ 
cout << "construct_iterator" << endl;
  gw_pointer_element pt(M[p]); 
  pt.set_position(130 + 25*id2, 200-10*id2);
  id2++;
  V.add_element(pt);  
  V.draw();
  P[&i] = pt;
}


void copy_iterator(Iterator& i, const Iterator& j)
{
cout << "copy_iterator" << endl;
  gw_pointer_element pt(M[&(*i)]);
  P[&i] = pt;
  pt.set_position(130 + 25*id2, 200-10*id2);
  id2++;
  V.add_element(pt);  
  V.draw();
  leda::wait(0.5);
}


void delete_iterator(Iterator& i)
{ //if (Wait) V.wait();
cout << "delete_iterator" << endl;
  V.del_element(P[&i]); id2--; 
  V.draw();
}

void increment_iterator(Iterator& i)
{ 
cout << "increment_iterator" << endl;
  gw_pointer_element pt = P[&i];
  V.scene_begin(steps);
  pt.assign(M[&(*i)]);
  V.scene_end();  
  V.scene_play(); 
}

void increment_iterator1(Iterator& i, Int*) { increment_iterator(i); }



void decrement_iterator(Iterator& i)
{ 
cout << "decrement_iterator" << endl;
  //gw_pointer_element pt = P[&i];
  V.scene_begin(steps);
  P[&i].assign(M[&(*i)]);
  V.scene_end();  
  V.scene_play(); 
}

void decrement_iterator1(Iterator& i, Int*) { decrement_iterator(i); }


template<class Iter>
void bubblesort(const Iter& first, const Iter& stop)
{ for(Iter x=stop; x!=first; x--) 
    for(Iter y=first+1; y!=x; y++) 
       if (*(y-1) > *y) leda::swap(*(y-1),*y);
}
    




int main()
{ 

  attach(Int::construct_event, construct_cell);
  attach(Int::copy_event,      copy_cell);
  attach(Int::assign_event,    assign_value);
  attach(Int::assign_event,    assign_cell);
  attach(Int::destruct_event,  delete_cell);


  attach(Iterator::construct_event, construct_iterator);
  attach(Iterator::copy_event,      copy_iterator);

  attach(Iterator::increment_event, increment_iterator);
  attach(Iterator::increment_event, increment_iterator1);

  attach(Iterator::decrement_event, decrement_iterator);
  attach(Iterator::decrement_event, decrement_iterator1);

  attach(Iterator::assign_event,    assign_iterator);
  attach(Iterator::assign_event,    assign_pointer);

  attach(Iterator::destruct_event,  delete_iterator);



  
  V.display(window::center,window::center);  
   
  Int A[8];   

  Iterator first = A;
  Iterator stop  = A+7;
    
  do
  { V.wait();

    Iterator i = stop;
    do *--i = 10*(i - first + 1);    
    while (i!=first);

    V.wait();

    std::random_shuffle(first,stop);    
    V.wait();    

    bubblesort(first,stop);
    V.wait();    
   
    Wait = true;
    std::sort(first,stop); 
    Wait = false;
    
  } while (V.wait());

  return 0;
}

