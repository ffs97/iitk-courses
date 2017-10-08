/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  std_copy.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/anim/view.h>
#include <LEDA/anim/stick.h>
#include <LEDA/anim/cv_view.h>

#include <algorithm>

#include <LEDA/new/anim/e_value.h>
#include <LEDA/new/anim/e_iterator.h>

using namespace leda;

typedef e_value<int>    Int;
typedef e_iterator<Int> Iterator;


struct observer
{
  typedef observer  self;

  cv_view<view,stick> V;
  map<const Int*,unsigned> M;  

  int id;
  Int* index;
  double x;
  double y;

  observer() : V(500,450,"Standard Copy"), M(0), id(0), index(0), x(100), y(50)
  { 
    attach(Iterator::construct_event,      *this, &self::construct_pointer);  
    attach(Iterator::copy_event,           *this, &self::copy_pointer);     
    attach(Iterator::access_event,         *this, &self::access_pointer);  
    attach(Iterator::destruct_event,       *this, &self::destruct_pointer);  
    attach(Iterator::increment_event,      *this, &self::increment_pointer);  

    attach(Int::construct_event, *this, &self::construct);
    attach(Int::assign_event,    *this, &self::value_assign);
    attach(Int::assign_event,    *this, &self::cell_assign);
    attach(Int::destruct_event,  *this, &self::destruct);  
    
    V.display();
    V.set_delay(1);
  }

  void construct(Int& i, const int& val)
  { M[&i] = id;
    V.new_element(M[&i], 6 * val, x + 32 * id++, y);
    V.draw();
  }

  void value_assign(Int& i, const int& x) 
  { V.scene_begin();
    V.assign(M[&i],double(6 * x)); 
    V.scene_end();
    V.scene_play();
  }

  void cell_assign(Int& i, const Int& j) 
  { V.scene_begin();
    V.assign(M[&i],M[&j]); 
    V.scene_end();
    V.scene_play(); 
  }

  void destruct(Int& i)
  { V.del_element(M[&i]);
    V.draw();
    id--;
  }

  void construct_pointer(e_iterator<Int>&, Int* i)
  { x = 100 - 32 * id; y += 200; 
    V.set_cell_color(M[i],green);
    index = i;
  }

  void copy_pointer(Iterator& i, const Iterator& j)
  { V.set_cell_color(M[index],grey1); 
    V.set_cell_color(M[j.data()],green); 
  }

  void access_pointer(Iterator& p, int x)
  { V.set_cell_color(M[index],grey1);    
    V.set_cell_color(M[p.data() + x],green); 
    index = p.data() + x;    
  }
  
  void increment_pointer(Iterator& p)
  { V.set_cell_color(M[p.data()-1],grey1);
    V.set_cell_color(M[p.data()],green);
  }

  void destruct_pointer(Iterator& p)
  { V.set_cell_color(M[p.data()],grey1); }  
  
  bool wait() { return V.wait(); }
};


int main()
{     
  observer OB;
  
  Iterator A = new Int[10];  
  Iterator B = new Int[10];  
  
  OB.wait();

  do 
  { for (int i = 0; i < 10; i++) 
      B[i] = rand_int(1,20);
    
    OB.wait();
  
    std::copy(B,B+10,A);    
        
  } while (OB.wait());
  
  delete [] A;
  delete [] B;
  
  return 0;
}
