/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  stl_sort_large.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#define LEDA_NO_MIN_MAX_TEMPL

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


static gw_view V(850,700,"STL Sort");
static map<const Int*,gw_stick_element*> M(0);  
static map<const Iterator*,gw_pointer_element*> P(0);  
static map<gw_pointer_element*,list<point> > p_stack;  
static map<gw_pointer_element*,list<point> > e_stack;  

static bool Wait = false;
static int id = 0;
static int id1 = 0;
static int id2 = 0;

static double cell_x =  80;
static double cell_y = 200;


//const int steps = 150;
//const int steps = 64;
const int steps = 16;


void assign_value(Int& i, const int& x) 
{ V.scene_begin(steps);
  M[&i]->assign(x); 
  V.scene_end();
  V.scene_play(); 
}

void assign_cell(Int& i, const Int& j) 
{ gw_stick_element* s1 = M[&i];
  gw_stick_element* s2 = M[&j];

  assert(s1);
  assert(s2);

  point pi = M[&i]->get_position();
  point pj = M[&j]->get_position();


  double angle = LEDA_PI/2; 
  if (pi.xcoord() < pj.xcoord()) angle = -angle;

  circular_motion cm(angle);   
  
  V.scene_begin(steps);
  s1->assign(*s2,&cm);   
  V.scene_end();  
  V.scene_play(); 
}

void construct_cell(Int& i, const int& x)
{ 
cout << "construct_cell  x = " << x << endl;
  gw_stick_element* st = new gw_stick_element; 
  st->set_value(x);
  st->set_position(cell_x+ 25 * id++, cell_y);
  V.add_element(*st);  
  V.draw();
  M[&i] = st;
}


void copy_cell(Int& i, const Int& j)
{ gw_stick_element* st = new gw_stick_element; 
  st->set_value(0);
  st->set_position(cell_x + 25 * id1++, cell_y+150);
  V.add_element(*st);  
  M[&i] = st;
  assign_cell(i,j);
}


void delete_cell(Int& i)
{ //if (Wait) V.wait();
  V.del_element(*M[&i]); id1--; 
}






void assign_pointer(Iterator& i, const Int* p) 
{
  cout << "assign pointer" << endl;

  V.scene_begin(steps);
  P[&i]->assign(*M[p]); 
  V.scene_end();
  V.scene_play(); 
}

void assign_iterator(Iterator& i, const Iterator& j) 
{
  cout << "assign iterator" << endl;
  gw_pointer_element* p1 = P[&i];
  gw_stick_element* c2 = M[&(*j)];

  assert(p1);
  assert(c2);

  V.scene_begin(steps);
  p1->assign(*c2);   
  V.scene_end();  
  V.scene_play(); 
}

void construct_iterator(Iterator& i, Int* p)
{ 
cout << "construct_iterator" << endl;
  gw_pointer_element* pt = new gw_pointer_element(*M[p]); 
  point pos(cell_x, cell_y-(20*++id2));
  pt->set_position(pos);
  V.add_element(*pt);  
  V.draw();
  P[&i] = pt;
}


void copy_iterator(Iterator& i, const Iterator& j)
{
cout << "copy_iterator" << endl;
  gw_pointer_element* pt = P[&j];
  P[&i] = pt;
  p_stack[pt].push(pt->get_position());
  e_stack[pt].push(pt->get_value_position());

  V.draw();
  leda::wait(0.25);
}


void delete_iterator(Iterator& i)
{ //if (Wait) V.wait();
cout << "delete_iterator" << endl;
  gw_pointer_element* pt = P[&i];
  if (p_stack[pt].empty())
    {  V.del_element(*pt); 
       id2--; 
     }
  else {
    point p = p_stack[pt].pop();
    point q = e_stack[pt].pop();
    pt->set_position(p);
    pt->set_value_position(q);
  }
  V.draw();
  leda::wait(0.25);
}

void increment_iterator(Iterator& i)
{ 
cout << "increment_iterator" << endl;
  gw_pointer_element* pt = P[&i];
  V.scene_begin(steps);
  pt->assign(*M[&(*i)]);
  V.scene_end();  
  V.scene_play(); 
}

void increment_iterator1(Iterator& i, Int*) { increment_iterator(i); }



void decrement_iterator(Iterator& i)
{ 
cout << "decrement_iterator" << endl;
  gw_pointer_element* pt = P[&i];
  V.scene_begin(steps);
  pt->assign(*M[&(*i)]);
  V.scene_end();  
  V.scene_play(); 
}

void decrement_iterator1(Iterator& i, Int*) { decrement_iterator(i); }


template<class Iter>
void bubblesort(const Iter& first, const Iter& stop)
{ for(Iter x=stop; x!=first; x--) 
    for(Iter y=first+1; y!=x; y++) 
    { Iter z = y-1;
      if (*z > *y) std::swap(*z,*y);
     }
}
    


template<class Iter>
void merge(Iter first, Iter middle, const Iter& stop)
{
  typedef typename Iter::value_type E;
  int len = stop-first;
  E* tmp = new E[len+1];

  Iter p = tmp;
  Iter q = tmp + len;

  for(Iter p = tmp; p != q; ++p)
  {  if (middle == stop || *first < *middle)
       *p = *first++;
     else
       *p = *middle++;
   }
}
  
  
  



int main(int argc, char** argv)
{ 

  cout << argc << endl;

  if (argc > 1) { srandom(atoi(argv[1])); srand(atoi(argv[1])); }

  attach(Int::construct_event, construct_cell);
  attach(Int::copy_event, copy_cell);
  attach(Int::assign_event,  assign_value);
  attach(Int::assign_event,  assign_cell);
  attach(Int::destruct_event,  delete_cell);


  attach(Iterator::construct_event, construct_iterator);
  attach(Iterator::copy_event, copy_iterator);

  attach(Iterator::increment_event,   increment_iterator);
  attach(Iterator::increment_event,   increment_iterator1);

  attach(Iterator::decrement_event,   decrement_iterator);
  attach(Iterator::decrement_event,   decrement_iterator1);

  attach(Iterator::assign_event,  assign_iterator);
  attach(Iterator::assign_event,  assign_pointer);

  attach(Iterator::destruct_event,  delete_iterator);



  
  V.display(window::center,window::center);  

  //V.set_delay(4);

  int B[21];   
  int* i_first  = B;
  int* i_stop   = B+20;
   
  Int A[21];   
  Iterator first  = A;
  Iterator stop   = A+20;

  do
  { 

    int* ip = i_stop;
    do *--ip = 10*(ip - i_first + 1);    
    while (ip!=i_first);
    V.wait();    

    std::random_shuffle(i_first,i_stop);    
    V.wait();    

{
    Iterator i = stop;
    ip = i_stop;
    do *--i = *--ip;    
    while (i!=first);
}
    V.wait();    

/*
{
    Iterator i = stop;
    do *--i = 10*(i - first + 1);    
    while (i!=first);
}

    std::random_shuffle(first,stop);    
    V.wait();    
*/


    std::sort(first,stop); 
    V.wait();    

  } while (V.wait());

  return 0;
}

