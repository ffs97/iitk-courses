/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  stl_sort.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#define LEDA_NO_MIN_MAX_TEMPL

#include <LEDA/anim/view.h>
#include <LEDA/anim/stick.h>
#include <LEDA/anim/dot.h>
#include <LEDA/anim/e_value.h>

#include <algorithm>


#ifdef LEDA_NAMESPACE
using namespace leda;
#endif


typedef e_value<int> Int;

static view V(500,550,"STL Sort");
static map<const Int*,dot>   Dots;  
static map<const Int*,stick> Sticks;  

static int id1 = 0;
static int id2 = 0;
static int steps = 200;

void value_assign(Int& i, const int& x) 
{ 
  V.scene_begin(steps); 
  Dots[&i].assign(x); 
  Sticks[&i].assign(x); 
  V.scene_end();
  V.scene_play(); 
}

void cell_assign(Int& i, const Int& j) 
{ 
  V.scene_begin(steps);  
  Dots[&i].assign(Dots[&j]);   
  Sticks[&i].assign(Sticks[&j]);   
  V.scene_end();  
  V.scene_play(); 
}

void construct_cell(Int& i, const int& x)
{
  Dots[&i]   =   dot(x, 8, point(150 + 35 * id1,   120));
  Sticks[&i] = stick(x, 8, point(150 + 35 * id1++, 350));

  V.add_element(Dots[&i]);  
  V.add_element(Sticks[&i]);  
}

void copy_cell(Int& i, const Int& j)
{ 
  point p_dot   = Dots[&j].get_position();
  point p_stick = Sticks[&j].get_position();

  
  
  Dots[&i]   =   dot(0, 8, point(p_dot.xcoord()   + 35 * id2,    20));
  Sticks[&i] = stick(0, 8, point(p_stick.xcoord() + 35 * id2++, 250));

  V.add_element(Dots[&i]);  
  V.add_element(Sticks[&i]);  

  cell_assign(i,j);
}

void destruct(Int& i)
{ 
  V.del_element(Dots[&i]); 
  V.del_element(Sticks[&i]);   
  if (id2 > 0) id2--;
}

int main()
{ 
  
  attach(Int::copy_event,     copy_cell);
  attach(Int::construct_event,construct_cell);
  attach(Int::assign_event,   value_assign);
  attach(Int::assign_event,   cell_assign);
  attach(Int::destruct_event, destruct);
  
  V.display();  
  V.set_delay(8);
   
  Int A[7];   
    
  do
  { V.wait(2000);

    Int x = 1;
    while (x < 8)
    { A[x] = x++;    
      //x++;
     }


    V.wait();    
    //V.wait(1000);    
    
    std::random_shuffle(A,A+7);    
    
    V.wait(1000);    
   
    std::sort(A,A+7);         
    
  } while (V.wait(3000));

  return 0;
}

