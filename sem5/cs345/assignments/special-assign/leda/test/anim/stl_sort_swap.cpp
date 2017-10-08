/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  stl_sort_swap.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <algorithm>



#include <LEDA/anim/view.h>
#include <LEDA/anim/stick.h>
#include <LEDA/anim/e_value.h>

#include <LEDA/core/tuple.h>
#include <LEDA/core/queue.h>


typedef leda::e_value<int> Int;

static void dummy_sort(Int* A, int n) { std::sort(A,A+n); }
static void dummy_shuffle(Int* A, int n) { std::random_shuffle(A,A+n); }


#ifdef LEDA_NAMESPACE
using namespace leda;
#endif

struct swap_pattern
{
  Int* tmp;
  Int* i;
  Int* j;

  int step;

  queue< three_tuple<event*,Int*,Int*> > EQ;

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
  { attach(Int::construct_event, *this, &swap_pattern::construct);
    attach(Int::copy_event,      *this, &swap_pattern::copy);
    attach(Int::assign_event,    *this, &swap_pattern::assign);
    attach(Int::assign_event,    *this, &swap_pattern::assign_value);
    attach(Int::destruct_event,  *this, &swap_pattern::destruct);
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

    insert(copy_event,tmp,i);
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
  static event copy_event;
  static event assign_event;
  static event destruct_event;

  static event swap_event;

};

event swap_pattern::construct_event;
event swap_pattern::copy_event;
event swap_pattern::assign_event;
event swap_pattern::destruct_event;

event swap_pattern::swap_event;


static view V(500,450,"Sort Algorithms");
static map<const Int*,stick> M;

static int id1 = 0;
static int id2 = 0;
static int steps = 200;


void value_assign(Int& i, const int& x)
{
  V.scene_begin(steps);
  M[&i].assign(x);
  V.scene_end();
  V.scene_play();
}

void cell_assign(Int& i, const Int& j)
{
  V.scene_begin(steps);
  M[&i].assign(M[&j]);
  V.scene_end();
  V.scene_play();
}

void construct_cell(Int& i, const int& x)
{
  M[&i] = stick(x, 10, point(150 + 35 * id1++, 200));
  V.add_element(M[&i]);
}

void copy_cell(Int& i, const Int& j)
{
  point p = M[&j].get_position();
  M[&i] = stick(0, 10, point(p.xcoord() + 35 * id2++, 100));
  V.add_element(M[&i]);
  cell_assign(i,j);
}

void destruct_cell(Int& i)
{
  V.del_element(M[&i]);
  if (id2 > 0) id2--;
}


void cell_swap(const Int& i, const Int& j)
{
  V.scene_begin(steps);

// entweder circular_motion
//
//  M[&i].swap(M[&j],circular_motion(LEDA_PI / 2));
//
// oder eine Bezier Kurve; sieht etwas weicher aus
//
  bezier_motion Path;
  Path.append(rel_coord(0.0), abs_coord(10));
  Path.append(rel_coord(0.5), abs_coord(40));
  Path.append(rel_coord(1.0), abs_coord(10));

  M[&i].swap(M[&j],Path);


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


template <class Iter>
void bubblesort(Iter first, Iter stop)
{
  for (Iter x = stop; x != first; x--)
    for (Iter y = first + 1; y != x; y++)
       if (*(y-1) > *y) swap(*(y-1),*y);
}


int main(int argc, char** argv)
{
  swap_pattern P;

  attach(swap_pattern::construct_event, construct_cell);
  attach(swap_pattern::copy_event,      copy_cell);
  attach(swap_pattern::assign_event,    value_assign);
  attach(swap_pattern::assign_event,    cell_assign);
  attach(swap_pattern::destruct_event,  destruct_cell);

  attach(swap_pattern::swap_event,      cell_swap);

  V.display();
  V.set_delay(4);

  Int A[7];

  do
  { V.wait(2000);

    for (int i = 0; i < 7; i++) A[i] = i + 1;

    V.wait(1000);

    //std::random_shuffle(A,A+7);
    dummy_shuffle(A,7);

    V.wait(1000);

    if (argc > 1 && strcmp(argv[1],"-qsort") == 0)  quicksort(A,A+6);
    else if (argc > 1 && strcmp(argv[1],"-bubble") == 0)
bubblesort(A,A+7);
         else //std::sort(A,A+7);
              dummy_sort(A,7);

  } while (V.wait(2000));

  return 0;
}


