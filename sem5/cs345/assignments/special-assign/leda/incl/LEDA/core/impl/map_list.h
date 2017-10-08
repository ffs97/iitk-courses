/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  map_list.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/




LEDA_BEGIN_NAMESPACE

template<class T, class link_map>
class map_list  {

/*
link_map& _succ;
link_map& _pred;
*/

link_map _succ;
link_map _pred;

T  _head;
T  _tail;

public:

typedef T item;

/*
map_list(link_map& s, link_map& p) : _succ(s), _pred(p) 
{ _head = 0; _tail = 0; }
*/

map_list() : _succ(0), _pred(0), _head(0), _tail(0) {}

void clear() { _head = 0; _tail = 0; }

T head()  const { return _head; }
T first() const { return _head; }

T tail() const { return _tail; }
T last() const { return _tail; }

T succ(const T& x) const { return _succ[x]; }
T pred(const T& x) const { return _pred[x]; }

T cylic_succ(const T& x) const
{ return (x == _tail) ? _head : _succ[x]; }

T cylic_pred(const T& x) const
{ return (x == _head) ? _tail : _pred[x]; }

bool empty()  const { return _head == 0; }

void append(const T& e)
{ _succ[e] = 0;
  _pred[e] = _tail;
  if (_tail) 
     _succ[_tail] = e; 
  else 
     _head = e;
  _tail = e;
}

void push(const T& e)
{ _pred[e] = 0;
  _succ[e] = _head;
  if (_head) 
     _pred[_head] = e; 
  else 
     _tail = e;
  _head = e;
}

void insert(const T& e, const T& x)
{ if (x == nil)
  { push(e);
    return;
   }
  T s = _succ[x];
  _succ[e] = s;
  _pred[e] = x;
  _succ[x] = e;
  if (s) _pred[s] = e; else _tail = e;
}



T pop()
{ T e = _head;
  if (e)
  { T s = _succ[e];
    _head = s;
    if (s) _pred[s] = 0; else _tail = 0;
    _succ[e] = 0;
   }
  return e;
}

T pop_last()
{ T e = _tail;
  if (e)
  { T p = _pred[e];
    _tail = p;
    if (p) _succ[p] = 0; else _head = 0;
    _succ[e] = 0;
   }
  return e;
}


void del(const T& e1, const T& e2)
{ T s = _succ[e2];
  T p = _pred[e1];
  if (s)
    _pred[s] = p;
  else
    _tail = p;
  if (p)
    _succ[p] = s;
  else
    _head = s;
}


void del(const T& e) 
{ del(e,e);
  _succ[e] = 0;
}


bool member(const T& e) const { return e == _tail || _succ[e] != 0; }
bool operator()(T e)    const { return e == _tail || _succ[e] != 0; }


// iteration

 T first_item()    const { return first(); }
 T last_item()     const { return last(); }
 T next_item(T x)  const { return x ? succ(x) : 0; }
 T pred_item(T x)  const { return x ? pred(x) : 0; }
 T inf(T x)        const { return x; }

};

LEDA_END_NAMESPACE

