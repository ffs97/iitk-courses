/*******************************************************************************
+
+  LEDA 4.5  
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


/*{\Manpage {EVENTx} {E} {Events}}*/


/*{\Mdefinition
Events are designed to support some kind of Observer Pattern. An event |E|
contains receivers, a list of |event_items|. Each item contains a reference
to a function. 
Functions can be added/removed by using the non-member functions
|attach|/|detach|.
If an event occurs, all attached functions are called. 
}*/


/*{\Mcreation E }*/

/*{\Mtext
|EVENT0|

|EVENT1<type>|

|EVENT2<type1,type2>|, ...
}*/ 


/*{\Moperations 2 5 }*/

/*{\Moptions nextwarning=no}*/
/*
void occur(type1 p1, type2 p2, ..);
*/
/*{\Mop      calls all attached functions with |p1|,|p2|,\ldots as parameters.
}*/ 

/*{\Mtext
\bigskip
{\bf Non-Member Functions} 
}*/

/*{\Moptions nextwarning=no}*/
/*
event_item attach(EVENTx<type1,..,typex>& E, observer& o, function f);
*/
/*{\Mfunc    appends a new item |<o,f>| to the list of receivers of |E|
             and returns it. 
	     |f| is member-function of class |observer| with argument-types 
	     corresponding to the arguments of event |E|. If |E| occurs,
	     |f| is called for object |o|. }*/ 

/*{\Moptions nextwarning=no}*/
/*
event_item attach(EVENTx<type1,..,typex>& E, function f);
*/
/*{\Mfunc    appends a new item |<f>| to the list of receivers of |E|
             and returns it. |f| is a global function with argument-types 
	     corresponding to the arguments of event |E|.}*/ 

/*{\Moptions nextwarning=no}*/
/*
void detach(event_item it);
*/
/*{\Mfunc  removes |it| from the receivers-list of the associated event.}*/


#include <LEDA/core/list.h>

/*
#if defined(...)
#include <LEDA/event_dummy.h>
#else ...
#endif
*/


LEDA_BEGIN_NAMESPACE


class base_event;

class base_receiver {
  bool _enabled;
  public :
  base_receiver() : _enabled(true) {}
  virtual ~base_receiver() {}
  virtual base_event* get_event() const { return 0; }
  void enable() { _enabled = true; }
  void disable() { _enabled = false; }
  bool is_enabled() const { return _enabled; }
  friend istream& operator>>(istream& istr, base_receiver*) { return istr; }
};

typedef base_receiver* event_item;

inline void detach(event_item it) { delete it; }
inline void detach(event_item it_p[], int c) {
  while (c > 0) delete(it_p[--c]);
}

class base_event {
  public :
  virtual ~base_event() {}
  virtual void detach(event_item it) {
    if (it->get_event() == this) delete it;
    else LEDA_EXCEPTION(1,"detach: event_item not attached!");
  }
  friend istream& operator>>(istream& istr, base_event*) { return istr; }
};

LEDA_END_NAMESPACE

#include <LEDA/system/event_old/event_nomacs.h>
/*
#include <LEDA/event_macros.h>
*/


LEDA_BEGIN_NAMESPACE

// some hacks for static events

#define CLASS_EVENT(name,type) \
  static EVENT1<type&>& name(type& t) { \
    static EVENT1<type&> _name; \
    if (&t) _name.occur(t); \
    return _name; \
  }

#define CREATE_EVENT(type) CLASS_EVENT(create_event,type)

template<class etype, class type, class obs, class func>
inline event_item attach(etype& (*e)(type&), obs& c, func f) {
  return attach(e(*(type*)0),c,f);
}
template<class etype, class type, class func>
inline event_item attach(etype& (*e)(type&), func f) {
  return attach(e(*(type*)0),f);
}

// end hacks
LEDA_END_NAMESPACE

