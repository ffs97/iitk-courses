/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  swap.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/anim/view.h>
#include <LEDA/anim/stick.h>
#include <LEDA/anim/cell.h>

#include <LEDA/tuple.h>
#include <LEDA/queue.h>

#include <algorithm>

#ifdef LEDA_NAMESPACE
using namespace leda;
#endif

typedef cell<int> Int;

static view V(500,450,"STL Sort");
static map<const Int*,stick> M;  
static int id = 0;


struct swap_pattern
{
  Int* tmp;
  Int* i;
  Int* j;

  int step;

  queue<three_tuple<event*,Int*,Int*> > EQ;

  void init() { tmp = i = j = 0; step = 0; }
  
  void insert(event& e, Int* i, Int* j)
  { EQ.append(three_tuple<event*,Int*,Int*>(&e,i,j)); }
  
    
  void clear()
  { 
    while (!EQ.empty())
    { three_tuple<event*,Int*,Int*> t = EQ.pop();
      occur<Int&,const Int&>(*t.first(),*t.second(),*t.third());      
    }
  }
  
  
  swap_pattern() : tmp(0), i(0), j(0), step(0)
  { attach(Int::construct_event,      *this, &swap_pattern::construct);
    attach(Int::copy_construct_event, *this, &swap_pattern::copy);
    attach(Int::assign_event,         *this, &swap_pattern::assign);
    attach(Int::assign_event,         *this, &swap_pattern::assign_value);
    attach(Int::destruct_event,       *this, &swap_pattern::destruct);
  }
  

  void construct(Int& x, const int& v)
  { init();

    if (!EQ.empty()) clear();    
    occur<Int&,const int&>(construct_event,x,v);    
  }


  void copy(Int& x, const Int& y)
  {   
    if (!EQ.empty())
    { init();    
      clear();
    }          
   
    step = 1;   

    tmp = &x;
    i = &const_cast<Int&>(y);

    insert(copy_construct_event,tmp,i);
  }

  
  void assign_value(Int& x, const int& v)
  { init();
    clear();    
    occur<Int&,const int&>(assign_event,x,v);      
  }

  void assign(Int& x, const Int& y)
  { 
    switch (step)
    {
      case 0: 
      { step = 1;

        tmp = &x;
        i = &const_cast<Int&>(y);

        insert(assign_event,tmp,i);

        break;      
      }
      
      case 1:
      { 
        if (&x != i) 
        {
          init(); 
          clear();
          occur<Int&,const Int&>(assign_event,x,y);      
        
          break; 
        }      
        
        step = 2;
        j = &const_cast<Int&>(y);

        insert(assign_event,i,j);
        break;
      }
    
      case 2:
      { if (&x == j && &const_cast<Int&>(y) == tmp) 
        { occur<const Int&,const Int&>(swap_event,*i,*j);                
          EQ.clear();
        }
        else
        { init();        
          clear();           
          occur<Int&,const Int&>(assign_event,x,y); 
        }
                
        break;
      }    
    };
  }

  void destruct(Int& x)
  { clear();            
    if (&x != tmp) occur<Int&>(destruct_event,x);
    init();   
  }

  static event construct_event;
  static event copy_construct_event;
  static event assign_event;
  static event destruct_event;

  static event swap_event;

};

event swap_pattern::construct_event;
event swap_pattern::copy_construct_event;
event swap_pattern::assign_event;
event swap_pattern::destruct_event;

event swap_pattern::swap_event;



void construct(Int& i, const int& x)
{ M[&i] = stick(x, 10, point(130 + 35 * id++, 200));
  V.add_element(M[&i]);  
}

void value_assign(Int& i, const int& x) 
{ V.scene_begin();
  M[&i].assign(x); 
  V.scene_end();
  V.scene_play(); 
}

void cell_assign(Int& i, const Int& j) 
{ V.scene_begin();  
  M[&i].assign(M[&j]);   
  V.scene_end();  
  V.scene_play(); 
}

void copy(Int& i, const Int& j)
{ M[&i] = stick(0, 10, point(140 + 35 * id++, 200));
  V.add_element(M[&i]);  
  cell_assign(i,j);
}

void destruct(Int& i)
{ V.wait(200);
  V.del_element(M[&i]); 
  id--; 
}


void cell_swap(const Int& i, const Int& j)
{ V.scene_begin();
  M[&i].swap(M[&j]);
  V.scene_end();  
  V.scene_play(); 
}



template <class Iter>
void quicksort(Iter l, Iter r)
{
  Iter i = l;
  Iter j = r + 1;
  
  for(;;)
  { 
    while (*(++i) < *l);
    while (*l < *(--j));
    if (i < j) swap(*i,*j);
    else break;
  }
 
  swap(*l,*j);
 
  if (l <= j - 1) quicksort(l,j-1);
  if (r >= j + 1) quicksort(j+1,r);
}



int main()
{ 
  swap_pattern P;
  
  attach(swap_pattern::construct_event,      construct);
  attach(swap_pattern::copy_construct_event, copy);
  attach(swap_pattern::assign_event,         value_assign);  
  attach(swap_pattern::assign_event,         cell_assign);
  attach(swap_pattern::destruct_event,       destruct);

  attach(swap_pattern::swap_event,           cell_swap);
  
  V.display();  
   
  Int A[7];   
    
  do
  { V.wait();

    for (int i = 0; i < 7; i++) A[i] = i + 1;    
   
    std::random_shuffle(A,A+7);       
    std::cout << "std::random_shuffle finished!\n";
    
    V.wait();    
   
//    quicksort(A,A+6);         
//    std::sort(A,A+7);
    std::stable_sort(A,A+7);
    
  } while (V.wait());

  return 0;
}

