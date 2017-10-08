/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  event.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_EVENT_H
#define LEDA_EVENT_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600229
#include <LEDA/internal/PREAMBLE.h>
#endif

#if defined(__HP_aCC)
#include <LEDA/system/event_old/event.h>
#else

#include <LEDA/core/list.h>
#include <LEDA/core/map.h>

LEDA_BEGIN_NAMESPACE

// ---------------------------------------------------------------------------
// event_item : BASE_RECEIVER*;
// ---------------------------------------------------------------------------

struct BASE_RECEIVER;
typedef BASE_RECEIVER* event_item;

struct BASE_EVENT
{ virtual ~BASE_EVENT() {}
  virtual void detach(event_item) = 0;
};

struct BASE_RECEIVER
{ BASE_EVENT* e;
  GenPtr data;
  bool enabled;

  BASE_RECEIVER(BASE_EVENT* x) : e(x), data(0), enabled(true) {}
  virtual ~BASE_RECEIVER() {}
  
  void enable()  { enabled = true; }
  void disable() { enabled = false; }
  bool is_enabled() const { return enabled; }
};


template <class PT>
struct base_receiver : public BASE_RECEIVER
{ base_receiver(BASE_EVENT* e) : BASE_RECEIVER(e) {}
  virtual void notify(const PT&) const = 0;
  virtual ~base_receiver() {}
};

// ---------------------------------------------------------------------------
// parameter structs
// ---------------------------------------------------------------------------

struct param0
{ enum { number = 0 }; 
  explicit param0() {}
};

template <class T> 
struct param1
{ enum { number = 1 }; 
  T val1;    
  explicit param1(T x1) : val1(x1) {}
};

template <class T1, class T2> 
struct param2 : public param1<T1>
{ enum { number = 2 };
  T2 val2;
  explicit param2(T1 x1, T2 x2) : param1<T1>(x1), val2(x2) {}
};

template <class T1, class T2, class T3> 
struct param3 : public param2<T1,T2>
{ enum { number = 3 };
  T3 val3;
  explicit param3(T1 x1, T2 x2, T3 x3) : param2<T1,T2>(x1,x2), val3(x3) {}
};

template <class T1, class T2, class T3, class T4> 
struct param4 : public param3<T1,T2,T3>
{ enum { number = 4 };
  T4 val4;
  explicit param4(T1 x1, T2 x2, T3 x3, T4 x4) 
    : param3<T1,T2,T3>(x1,x2,x3), val4(x4) {}
};

template <class T1, class T2, class T3, class T4, class T5> 
struct param5 : public param4<T1,T2,T3,T4>
{ enum { number = 5 };
  T5 val5;
  explicit param5(T1 x1, T2 x2, T3 x3, T4 x4, T5 x5) 
    : param4<T1,T2,T3,T4>(x1,x2,x3,x4), val5(x5) {}
};

template <class T1, class T2, class T3, class T4, class T5, class T6> 
struct param6 : public param5<T1,T2,T3,T4,T5>
{ enum { number = 6 };
  T6 val6;
  explicit param6(T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6) 
    : param5<T1,T2,T3,T4,T5>(x1,x2,x3,x4,x5), val6(x6) {}
};

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7> 
struct param7 : public param6<T1,T2,T3,T4,T5,T6>
{ enum { number = 7 };
  T7 val7;
  explicit param7(T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6, T7 x7) 
    : param6<T1,T2,T3,T4,T5,T6>(x1,x2,x3,x4,x5,x6), val7(x7) {}
};

// ---------------------------------------------------------------------------
// caller classes
// ---------------------------------------------------------------------------

template <unsigned dummy>
struct caller
{ template <class P, class R, class F>
  caller(const P& p, R r, F f);

  template <class P, class F>
  caller(const P& p, F f);
};  

template <>
struct caller<0>
{ template <class P, class R, class F>
  caller(const P& p, R r, F f) { (r->*f)(); }

  template <class P, class F>
  caller(const P& p, F f) { f(); } 
};

template <>  
struct caller<1> 
{ template <class P, class R, class F>
  caller(const P& p, R r, F f) { (r->*f)(p.val1); } 

  template <class P, class F>
  caller(const P& p, F f) { f(p.val1); } 
};

template <>
struct caller<2> 
{ template <class P, class R, class F>
  caller(const P& p, R r, F f) { (r->*f)(p.val1,p.val2); } 
  
  template <class P, class F>
  caller(const P& p, F f) { f(p.val1,p.val2); } 
};

template <>
struct caller<3> 
{ template <class P, class R, class F>
  caller(const P& p, R r, F f) { (r->*f)(p.val1,p.val2,p.val3); } 

  template <class P, class F>
  caller(const P& p, F f) { f(p.val1,p.val2,p.val3); } 
};

template <>
struct caller<4> 
{ template <class P, class R, class F>
  caller(const P& p, R r, F f) 
  { (r->*f)(p.val1,p.val2,p.val3,p.val4); } 

  template <class P, class F>
  caller(const P& p, F f) { f(p.val1,p.val2,p.val3,p.val4); } 
};

template <>
struct caller<5> 
{ template <class P, class R, class F>
  caller(const P& p, R r, F f) 
  { (r->*f)(p.val1,p.val2,p.val3,p.val4,p.val5); } 

  template <class P, class F>
  caller(const P& p, F f) { f(p.val1,p.val2,p.val3,p.val4,p.val5); } 
};

template <>
struct caller<6> 
{ template <class P, class R, class F>
  caller(const P& p, R r, F f) 
  { (r->*f)(p.val1,p.val2,p.val3,p.val4,p.val5,p.val6); } 

  template <class P, class F>
  caller(const P& p, F f) 
  { f(p.val1,p.val2,p.val3,p.val4,p.val5,p.val6); } 
};

template <>
struct caller<7> 
{ template <class P, class R, class F>
  caller(const P& p, R r, F f) 
  { (r->*f)(p.val1,p.val2,p.val3,p.val4,p.val5,p.val6,p.val7); } 

  template <class P, class F>
  caller(const P& p, F f) 
  { f(p.val1,p.val2,p.val3,p.val4,p.val5,p.val6,p.val7); } 
};

// ---------------------------------------------------------------------------
// receiver class
// ---------------------------------------------------------------------------

struct _NoClassReceiver {};

template <class PT, class F, class R = _NoClassReceiver>
struct receiver : public base_receiver<PT>
{
  enum { num = PT::number };
  
  typedef PT param_type;

  typedef base_receiver<PT> base;
  typedef receiver<PT,F,R>  self;
  
  R* r; // receiver object
  F  f; // handler function    

  void (self::*call)(const PT& p);

  void c_call(const PT& p) { caller<num>(p,r,f); }
  void f_call(const PT& p) { caller<num>(p,f); }

  receiver(BASE_EVENT& e, const self& x) : base(&e), r(x.r), f(x.f), call(x.call) {}

  receiver(R& _r, F _f) : base(0), r(&_r), f(_f), call(&self::c_call) {}
  receiver(F _f) : base(0), f(_f), call(&self::f_call) {}  

  void notify(const PT& p) const 
  { if (!this->is_enabled()) return;
    (const_cast<self*>(this)->*call)(p); 
  } 
};


class base_event : public BASE_EVENT
{ list<event_item> L;  
  bool enabled;
  
  public:

  base_event() : enabled(true) {}

  template <class RT>
  event_item attach(const RT& r) 
  { RT * x = new RT(*this,r);    
    x->data = L.append(x);
    return x; 
  }

  void detach(event_item x) 
  { delete L.del_item((list_item)x->data); }

  template <class PT>
  void occur(const PT& p) const
  { if (!enabled) return;
  
    event_item x;
    forall(x,L)
      ((base_receiver<PT>*)x)->notify(p);  
  }
  
  void enable()  { enabled = true; }   
  void disable() { enabled = false; }    
  bool is_enabled() const { return enabled; } 

 ~base_event() 
  { while (!L.empty()) delete L.pop(); }
};


// ---------------------------------------------------------------------------
// event
// ---------------------------------------------------------------------------

class event;

namespace detail {

class base_singleton;

struct data_struct
{ base_singleton* instance;
  list_item       global;
  list_item       local;  
  data_struct(base_singleton* x) : instance(x) {}
};
  
class base_singleton
{ protected:
  list< list<event_item> > L;
      
  public:
      
  template <class RT>
  event_item attach(event& e, event_item& x, const RT& r) 
  { data_struct& T = *(new data_struct(this));
    
    RT* R = new RT(e,r); R->data = &T;    
    if (x != 0)
    { data_struct& S = *(data_struct*) x->data;    
      T.global = S.global;        
    } 
    else
    { T.global = L.append(list<event_item>());        
      x = R;
    } 
    
    T.local = L[T.global].append(R);    
    return R;
  }  
  
  template <class PT>     
  void occur(event_item x, const PT& p) const
  { data_struct& D = *(data_struct*) x->data;    
    event_item it;
    forall(it,L[D.global])
     ((base_receiver<PT>*)it)->notify(p);  
  }

  virtual event_item detach(event_item x) = 0;
  virtual void clear(event_item x) = 0;
  virtual ~base_singleton() {}
};


template <class T>
class singleton : public base_singleton
{ static GenPtr instance;
  singleton() {}    
  
  public:

  event_item detach(event_item x) 
  { data_struct& D = *(data_struct*) x->data;    
    list_item item = D.global;    
    
    delete L[item].del_item(D.local); 
    delete &D;    
    
    if (!L[item].empty()) return L[item].front();
    
    L.del_item(item);      
    if (L.empty()) 
    { instance = 0;
      delete this;
    }      
    return 0;      
  }  

  void clear(event_item x)
  { data_struct& D = *(data_struct*) x->data;    
    list_item item = D.global;    
    
    while (!L[item].empty())
    { event_item x = L[item].pop(); 
      delete (data_struct*) x->data;
      delete x;
    }
    
    L.del_item(item);
    
    if (L.empty()) 
    { instance = 0;
      delete this;
    }      
  }

  static singleton<T>& get()
  { if (instance == 0) instance = new singleton<T>;
    return *(singleton<T>*) instance;
  }
};

template <class T> GenPtr singleton<T>::instance = 0;

}

// ---------------------------------------------------------------------------
// event
// ---------------------------------------------------------------------------

/*{\Manpage {event} {} {Events}}*/

/*{\Mdefinition
Events are designed to support all kinds of Observer Pattern. An event |E|
is able to store arbitrary receivers. Each receiver contains a reference to 
a global function or to a member function of an arbitrary data type. 
Functions can be added/removed by calling the non-member functions |attach|/|detach|. 
If an event occurs, all attached functions with a suitable parameter list
are called. 
}*/

class event : public BASE_EVENT
{ map<detail::base_singleton*,event_item> M;
  bool enabled;
  
  event(const event& e);
  event& operator=(const event& e);
 
  public:

/*{\Mcreation E }*/
  event() : M(0), enabled(true) {}
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
            to the empty receiver list. }*/
   
 ~event() 
  { event_item x = 0;
    forall(x,M)
    { if (!x) continue;
      detail::data_struct& d = *(detail::data_struct*) x->data;    
      detail::base_singleton& s = *d.instance;
      s.clear(x);      
    }
  }
  
  template <class RT>
  event_item attach(const RT& r)
  { typedef typename RT::param_type PT;
    detail::singleton<PT>& s = detail::singleton<PT>::get();    
    return s.attach(*this,M[&s],r);
  }

  template <class PT>
  void occur(const PT& x) const 
  { if (!enabled) return;
    detail::singleton<PT>& s = detail::singleton<PT>::get();    
    if (M[&s]) s.occur(M[&s],x); 
  }
  
  void detach(event_item x)
  { detail::data_struct& d = *(detail::data_struct*) x->data;    
    detail::base_singleton& s = *d.instance;
    M[&s] = s.detach(x);
  }
    
  void enable()  { enabled = true; }   
  void disable() { enabled = false; }    
  bool is_enabled() const { return enabled; } 
};

/*{\Moperations 1.3 2.7 }*/


/*{\Moptions nextwarning=no}*/
/*
void enable();
*/
/*{\Mop enables |E|, i. e., all attached functions will be notified
        when |E| occurs.}*/

/*{\Moptions nextwarning=no}*/
/*
void disable();
*/
/*{\Mop disables |E|, no function will be notified when |E| occurs.}*/

/*{\Moptions nextwarning=no}*/
/*
bool is_enabled();
*/
/*{\Mop returns |true| if |E| is enabled, otherwise |false|.}*/


/*{\Mtext
\bigskip
{\bf Non-Member Functions} 
}*/

/*{\Moptions nextwarning=no}*/
/*
event_item attach(event& E, observer& o, function f);
*/
/*{\Mfunc    appends a new member function |f| of object |o| to the  
             receiver list of |E| and returns an |event_item| for it. 
             If |E| occurs with suitable parameters, |f| is called 
             for object |o|. }*/ 

/*{\Moptions nextwarning=no}*/
/*
event_item attach(event& E, function f);
*/
/*{\Mfunc    appends a new function |f| to the list of receivers 
             of |E| and returns an |event_item| for it. }*/ 

/*{\Moptions nextwarning=no}*/
/*
template<class T>
void occur(event& E, T p);
*/
/*{\Mfunc  calls all attached functions with |p| as parameter
           of the associated event |E|. There are also |occur| functions 
           for more than one parameter.
}*/ 

/*{\Moptions nextwarning=no}*/
/*
void detach(event_item x)
*/
/*{\Mfunc   removes |x| from the list of receivers of the associated event.}*/

/*{\Moptions nextwarning=no}*/
/*
void enable(event_item x)
*/
/*{\Mfunc   enables |x| in the list of receivers of the associated event.}*/

/*{\Moptions nextwarning=no}*/
/*
void disable(event_item x)
*/
/*{\Mfunc   disables |x| in the list of receivers of the associated event.}*/

/*{\Moptions nextwarning=no}*/
/*
bool is_enabled(event_item x)
*/
/*{\Mfunc   returns |true| if |x| is an enabled receiver in the list of 
            the associated event, otherwise |false|.}*/


/*{\Moptions nextwarning=no}*/
/*
void detach(event_item x, int c) 
*/
/*{\Mfunc  removes |c| receivers handled by vector |x| from the list 
           of receivers of the associated event.}*/

/*{\Moptions nextwarning=no}*/
/*
void enable(event_item x, int c) 
*/
/*{\Mfunc  enables |c| receivers handled by vector |x| of the 
           associated event.}*/

/*{\Moptions nextwarning=no}*/
/*
void disable(event_item x, int c) 
*/
/*{\Mfunc  disables |c| receivers handled by vector |x| of the 
           associated event.}*/

/*{\Mexample  
  \begin{verbatim}
  #include <LEDA/system/event.h>

  #ifdef LEDA_NAMESPACE
  using namespace leda;
  #endif

  #ifdef LEDA_STD_IO_HEADERS
  using std::cout;
  #endif

  void by_reference(int& val)
  { cout << "Call by reference:  " << val <<  " Change value! \n";
    val = 3;
  }

  void by_value(int val)
  { cout << "Call by value:      " << val << "\n"; }

  int main()
  {
    event e;
  
    attach(e,by_value);
    attach(e,by_reference);
  
    int i = 10;
  
    occur(e,i);
    occur<int&>(e,i);  
    occur(e,i);
  
    return 0;
  }
  \end{verbatim}
}*/

// ---------------------------------------------------------------------------
// old LEDA events
// ---------------------------------------------------------------------------

template <class T>
inline void event_unused_warning(const T&) {}

struct EVENT0 : public base_event
{ template <class RT>
  event_item attach(const RT& r) 
  { void (RT::*call)(const param0&) = r.call; 
    event_unused_warning(call);
    return base_event::attach(r); 
  }  
   
  void occur() const { operator()();}  
  void operator()() const { base_event::occur(param0()); }  
};


template <class T>
struct EVENT1 : public base_event
{ template <class RT>
  event_item attach(const RT& r) 
  { void (RT::*call)(const param1<T>&) = r.call; 
    event_unused_warning(call);
    return base_event::attach(r); 
  }  
  
  void occur(T x) const { operator()(x);}  
  void operator()(T x) const 
  { base_event::occur(param1<T>(x)); }
};


template <class T1, class T2>
struct EVENT2 : public base_event
{ template <class RT>
  event_item attach(const RT& r) 
  { void (RT::*call)(const param2<T1,T2>&) = r.call; 
    event_unused_warning(call);
    return base_event::attach(r); 
  }  
  
  void occur(T1 x1, T2 x2) const { operator()(x1,x2);}  
  void operator()(T1 x1,T2 x2) const 
  { base_event::occur(param2<T1,T2>(x1,x2)); }
};


template <class T1, class T2, class T3>
struct EVENT3 : public base_event
{ template <class RT>
  event_item attach(const RT& r) 
  { void (RT::*call)(const param3<T1,T2,T3>&) = r.call; 
    event_unused_warning(call);
    return base_event::attach(r); 
  }  

  void occur(T1 x1, T2 x2, T3 x3) const { operator()(x1,x2,x3);}    
  void operator()(T1 x1, T2 x2, T3 x3) const 
  { base_event::occur(param3<T1,T2,T3>(x1,x2,x3)); }
};


template <class T1, class T2, class T3, class T4>
struct EVENT4 : public base_event
{ template <class RT>
  event_item attach(const RT& r) 
  { void (RT::*call)(const param4<T1,T2,T3,T4>&) = r.call; 
    event_unused_warning(call);
    return base_event::attach(r); 
  }  
  
  void occur(T1 x1, T2 x2, T3 x3, T4 x4) const { operator()(x1,x2,x3,x4);}    
  void operator()(T1 x1, T2 x2, T3 x3, T4 x4) const 
  { base_event::occur(param4<T1,T2,T3,T4>(x1,x2,x3,x4)); }
};


template <class T1, class T2, class T3, class T4, class T5>
struct EVENT5 : public base_event
{ template <class RT>
  event_item attach(const RT& r) 
  { void (RT::*call)(const param5<T1,T2,T3,T4,T5>&) = r.call; 
    event_unused_warning(call);
    return base_event::attach(r); 
  }  

  void occur(T1 x1, T2 x2, T3 x3, T4 x4, T5 x5) const 
  { operator()(x1,x2,x3,x4,x5);}  
  
  void operator()(T1 x1, T2 x2, T3 x3, T4 x4, T5 x5) const 
  { base_event::occur(param5<T1,T2,T3,T4,T5>(x1,x2,x3,x4,x5)); }
};


template <class T1, class T2, class T3, class T4, class T5, class T6>
struct EVENT6 : public base_event
{ template <class RT>
  event_item attach(const RT& r) 
  { void (RT::*call)(const param6<T1,T2,T3,T4,T5,T6>&) = r.call; 
    event_unused_warning(call);
    return base_event::attach(r); 
  }  
  
  void occur(T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6) const 
  { operator()(x1,x2,x3,x4,x5,x6);}  
  
  void operator()(T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6) const 
  { base_event::occur(param6<T1,T2,T3,T4,T5,T6>(x1,x2,x3,x4,x5,x6)); }
};


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
struct EVENT7 : public base_event
{ template <class RT>
  event_item attach(const RT& r) 
  { void (RT::*call)(const param7<T1,T2,T3,T4,T5,T6,T7>&) = r.call; 
    event_unused_warning(call);
    return base_event::attach(r); 
  }  
  
  void occur(T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6, T7 x7) const 
  { operator()(x1,x2,x3,x4,x5,x6,x7);}  
  
  void operator()(T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6, T7 x7) const 
  { base_event::occur(param7<T1,T2,T3,T4,T5,T6,T7>(x1,x2,x3,x4,x5,x6,x7)); }
};

// ---------------------------------------------------------------------------
// non-member functions
// ---------------------------------------------------------------------------

inline void detach(event_item x)
{ BASE_EVENT* e = x->e;
  if (e) e->detach(x);
}

inline void enable(event_item x)     { x->enable(); }

inline void disable(event_item x)    { x->disable(); }

inline bool is_enabled(event_item x) { return x->is_enabled(); }

inline void detach(event_item* x, int c) 
{ while (c > 0) 
  { BASE_EVENT* e = (x[--c]->e);
    if (e) e->detach(x[c]);
  }
}

inline void enable(event_item* x, int c) 
{ while (c > 0) x[--c]->enable(); }

inline void disable(event_item* x, int c) 
{ while (c > 0) x[--c]->disable(); }

//-----------------------------------------------------------------------------
// handler without arguments
//-----------------------------------------------------------------------------

template <class R, class ET>
inline event_item attach(ET& e, R& r, void (R::*f)()) 
{ typedef void (R::*F)(); 
  return e.attach(receiver<param0,F,R>(r,f)); 
}

template <class R, class ET>
inline event_item attach(ET& e, R& r, void (R::*f)() const) 
{ typedef void (R::*F)() const; 
  return e.attach(receiver<param0,F,R>(r,f)); 
}

template <class ET>
inline event_item attach(ET& e, void (*f)()) 
{ typedef void (*F)();
  return e.attach(receiver<param0,F>(f));
}

inline void occur(event& e) { e.occur(param0()); }

//-----------------------------------------------------------------------------
// handler with one argument
//-----------------------------------------------------------------------------

template <class R, class T, class ET>
inline event_item attach(ET& e, R& r, void (R::*f)(T)) 
{ typedef void (R::*F)(T);
  return e.attach(receiver<param1<T>,F,R>(r,f)); 
}

template <class R, class T, class ET>
inline event_item attach(ET& e, R& r, void (R::*f)(T) const) 
{ typedef void (R::*F)(T) const;
  return e.attach(receiver<param1<T>,F,R>(r,f)); 
}

template <class T, class ET>
inline event_item attach(ET& e, void (*f)(T)) 
{ typedef void (*F)(T);
  return e.attach(receiver<param1<T>,F>(f)); 
}

template <class T>
inline void occur(event& e, T x) { e.occur(param1<T>(x)); }

//-----------------------------------------------------------------------------
// handler with two arguments
//-----------------------------------------------------------------------------

template <class R, class T1, class T2, class ET>
inline event_item attach(ET& e, R& r, void (R::*f)(T1,T2)) 
{ typedef void (R::*F)(T1,T2);
  return e.attach(receiver<param2<T1,T2>,F,R>(r,f)); 
}

template <class R, class T1, class T2, class ET>
inline event_item attach(ET& e, R& r, void (R::*f)(T1,T2) const) 
{ typedef void (R::*F)(T1,T2) const;
  return e.attach(receiver<param2<T1,T2>,F,R>(r,f)); 
}

template <class T1, class T2, class ET>
inline event_item attach(ET& e, void (*f)(T1,T2)) 
{ typedef void (*F)(T1,T2);
  return e.attach(receiver<param2<T1,T2>,F>(f)); 
}

template <class T1, class T2>
inline void occur(event& e, T1 x1, T2 x2) { e.occur(param2<T1,T2>(x1,x2)); }

//-----------------------------------------------------------------------------
// handler with three arguments
//-----------------------------------------------------------------------------

template <class R, class T1, class T2, class T3, class ET>
inline event_item attach(ET& e, R& r, void (R::*f)(T1,T2,T3)) 
{ typedef void (R::*F)(T1,T2,T3);
  return e.attach(receiver<param3<T1,T2,T3>,F,R>(r,f)); 
}

template <class R, class T1, class T2, class T3, class ET>
inline event_item attach(ET& e, R& r, void (R::*f)(T1,T2,T3) const) 
{ typedef void (R::*F)(T1,T2,T3) const;
  return e.attach(receiver<param3<T1,T2,T3>,F,R>(r,f)); 
}

template <class T1, class T2, class T3, class ET>
inline event_item attach(ET& e, void (*f)(T1,T2,T3)) 
{ typedef void (*F)(T1,T2,T3);
  return e.attach(receiver<param3<T1,T2,T3>,F>(f)); 
}

template <class T1, class T2, class T3>
inline void occur(event& e, T1 x1, T2 x2, T3 x3) 
{ e.occur(param3<T1,T2,T3>(x1,x2,x3)); }

//-----------------------------------------------------------------------------
// handler with four arguments
//-----------------------------------------------------------------------------

template <class R, class T1, class T2, class T3, class T4, class ET>
inline event_item attach(ET& e, R& r, void (R::*f)(T1,T2,T3,T4)) 
{ typedef void (R::*F)(T1,T2,T3,T4);
  return e.attach(receiver<param4<T1,T2,T3,T4>,F,R>(r,f));
}

template <class R, class T1, class T2, class T3, class T4, class ET>
inline event_item attach(ET& e, R& r, void (R::*f)(T1,T2,T3,T4) const) 
{ typedef void (R::*F)(T1,T2,T3,T4) const;
  return e.attach(receiver<param4<T1,T2,T3,T4>,F,R>(r,f));
}

template <class T1, class T2, class T3, class T4, class ET>
inline event_item attach(ET& e, void (*f)(T1,T2,T3,T4)) 
{ typedef void (*F)(T1,T2,T3,T4);
  return e.attach(receiver<param4<T1,T2,T3,T4>,F>(f));
}

template <class T1, class T2, class T3, class T4>
inline void occur(event& e, T1 x1, T2 x2, T3 x3, T4 x4) 
{ e.occur(param4<T1,T2,T3,T4>(x1,x2,x3,x4)); }

//-----------------------------------------------------------------------------
// handler with five arguments
//-----------------------------------------------------------------------------

template <class R, class T1, class T2, class T3, class T4, class  T5, class ET>
inline event_item attach(ET& e, R& r, void (R::*f)(T1,T2,T3,T4,T5)) 
{ typedef void (R::*F)(T1,T2,T3,T4,T5);
  return e.attach(receiver<param5<T1,T2,T3,T4,T5>,F,R>(r,f));
}

template <class R, class T1, class T2, class T3, class T4, class  T5, class ET>
inline event_item attach(ET& e, R& r, void (R::*f)(T1,T2,T3,T4,T5) const) 
{ typedef void (R::*F)(T1,T2,T3,T4,T5) const;
  return e.attach(receiver<param5<T1,T2,T3,T4,T5>,F,R>(r,f));
}

template <class T1, class T2, class T3, class T4, class T5, class ET>
inline event_item attach(ET& e, void (*f)(T1,T2,T3,T4,T5)) 
{ typedef void (*F)(T1,T2,T3,T4,T5);
  return e.attach(receiver<param5<T1,T2,T3,T4,T5>,F>(f));
}

template <class T1, class T2, class T3, class T4, class T5>
inline void occur(event& e, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5) 
{ e.occur(param5<T1,T2,T3,T4,T5>(x1,x2,x3,x4,x5)); }

//-----------------------------------------------------------------------------
// handler with six arguments
//-----------------------------------------------------------------------------

template <class R, class T1, class T2, class T3, 
          class T4, class T5, class T6, class ET>
inline event_item attach(ET& e, R& r, void (R::*f)(T1,T2,T3,T4,T5,T6)) 
{ typedef void (R::*F)(T1,T2,T3,T4,T5,T6);
  return e.attach(receiver<param6<T1,T2,T3,T4,T5,T6>,F,R>(r,f));
}

template <class R, class T1, class T2, class T3, 
          class T4, class T5, class T6, class ET>
inline event_item attach(ET& e, R& r, void (R::*f)(T1,T2,T3,T4,T5,T6) const) 
{ typedef void (R::*F)(T1,T2,T3,T4,T5,T6) const;
  return e.attach(receiver<param6<T1,T2,T3,T4,T5,T6>,F,R>(r,f));
}

template <class T1, class T2, class T3, class T4, class T5, class T6, class ET>
inline event_item attach(ET& e, void (*f)(T1,T2,T3,T4,T5,T6)) 
{ typedef void (*F)(T1,T2,T3,T4,T5,T6);
  return e.attach(receiver<param6<T1,T2,T3,T4,T5,T6>,F>(f));
}

template <class T1, class T2, class T3, class T4, class T5, class T6>
inline void occur(event& e, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6) 
{ e.occur(param6<T1,T2,T3,T4,T5,T6>(x1,x2,x3,x4,x5,x6)); }


//-----------------------------------------------------------------------------
// handler with seven arguments
//-----------------------------------------------------------------------------

template <class R, class T1, class T2, class T3, 
          class T4, class T5, class T6, class T7, class ET>
inline event_item attach(ET& e, R& r, void (R::*f)(T1,T2,T3,T4,T5,T6,T7)) 
{ typedef void (R::*F)(T1,T2,T3,T4,T5,T6,T7);
  return e.attach(receiver<param7<T1,T2,T3,T4,T5,T6,T7>,F,R>(r,f));
}

template <class R, class T1, class T2, class T3, 
          class T4, class T5, class T6, class T7, class ET>
inline event_item attach(ET& e, R& r, void (R::*f)(T1,T2,T3,T4,T5,T6,T7) const) 
{ typedef void (R::*F)(T1,T2,T3,T4,T5,T6,T7) const;
  return e.attach(receiver<param7<T1,T2,T3,T4,T5,T6,T7>,F,R>(r,f));
}

template <class T1, class T2, class T3, class T4, 
          class T5, class T6, class T7, class ET>
inline event_item attach(ET& e, void (*f)(T1,T2,T3,T4,T5,T6,T7)) 
{ typedef void (*F)(T1,T2,T3,T4,T5,T6,T7);
  return e.attach(receiver<param7<T1,T2,T3,T4,T5,T6,T7>,F>(f));
}

template <class T1, class T2, class T3, class T4, 
          class T5, class T6, class T7>
inline void occur(event& e, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6, T7 x7) 
{ e.occur(param7<T1,T2,T3,T4,T5,T6,T7>(x1,x2,x3,x4,x5,x6,x7)); }

//-----------------------------------------------------------------------------
// man page comments
//-----------------------------------------------------------------------------

/*{\Manpage {EVENTx} {E} {Specialized Events}}*/

/*{\Mdefinition
|EVENTx| are specialized events. An instance |E| of |EVENTx| can only
call functions where the parameter list matches the template 
parameter of |EVENTx|. In difference to |event|, the attached 
receivers are notified by the parenthesis operator.
}*/

/*{\Mcreation E }*/

/*{\Mtext
|EVENT0|,

|EVENT1<type>|,

|EVENT2<type1,type2>|, 

\ldots

|EVENT7<type1,type2, ... ,type7>|
}*/ 

/*{\Moperations 1.3 2.7 }*/

/*{\Moptions nextwarning=no}*/
/*
void occur(type1 p1, type2 p2, ...);
*/

/*{\Mop calls all attached functions with |p1|,|p2|,\ldots as
        parameters. You can also use the parenthesis operator.}*/

/*{\Moptions nextwarning=no}*/
/*
void enable();
*/
/*{\Mop enables |E|, i. e., all attached functions will be notified
        when |E| occurs.}*/

/*{\Moptions nextwarning=no}*/
/*
void disable();
*/
/*{\Mop disables |E|, no function will be notified when |E| occurs.}*/

/*{\Moptions nextwarning=no}*/
/*
bool is_enabled();
*/
/*{\Mop returns |true| if |E| is enabled, otherwise |false|.}*/


/*{\Mtext
\bigskip
{\bf Non-Member Functions} 
}*/

/*{\Moptions nextwarning=no}*/
/*
event_item attach(EVENTx& E, observer& o, function f);
*/
/*{\Mfunc    appends a new member function |f| of object |o| to the  
             receiver list of |E| and returns an |event_item| for it. 
             If |E| occurs with suitable parameters, |f| is called 
             for object |o|. }*/ 

/*{\Moptions nextwarning=no}*/
/*
event_item attach(EVENTx& E, function f);
*/
/*{\Mfunc    appends a new function |f| to the list of receivers 
             of |E| and returns an |event_item| for it. }*/ 

LEDA_END_NAMESPACE

#endif

#if LEDA_ROOT_INCL_ID == 600229
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

#endif
