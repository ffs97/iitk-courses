/*******************************************************************************
+
+  LEDA 4.5  
+
+
+  event_macros.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#define _E1FMT0(arg)
#define _E1FMT1(arg) arg
#define _E1FMT2(arg) arg##1 , arg##2
#define _E1FMT3(arg) _E1FMT2(##arg) , arg##3
#define _E1FMT4(arg) _E1FMT3(##arg) , arg##4
#define _E1FMT5(arg) _E1FMT4(##arg) , arg##5
#define _E1FMT6(arg) _E1FMT5(##arg) , arg##6 

/*
#define _E1FMT0(arg)
#define _E1FMT1(arg) arg
#define _E1FMT2(arg) arg##1 , arg##2
#define _E1FMT3(arg) arg##1 , arg##2 , arg##3
#define _E1FMT4(arg) arg##1 , arg##2 , arg##3 , arg##4
#define _E1FMT5(arg) arg##1 , arg##2 , arg##3 , arg##4 , arg##5
#define _E1FMT6(arg) arg##1 , arg##2 , arg##3 , arg##4 , arg##5 , arg##6
*/

 
#define _E2FMT0(type,arg)
#define _E2FMT1(type,arg) type arg
#define _E2FMT2(type,arg) type##1 arg##1 , type##2 arg##2
#define _E2FMT3(type,arg) _E2FMT2(type,arg) , type##3 arg##3
#define _E2FMT4(type,arg) _E2FMT3(type,arg) , type##4 arg##4
#define _E2FMT5(type,arg) _E2FMT4(type,arg) , type##5 arg##5
#define _E2FMT6(type,arg) _E2FMT5(type,arg) , type##6 arg##6

#define _DECLARE_BASE_RECEIVER(name,ctypes,types) \
ETEMPLATE(##ctypes) class name; \
ETEMPLATE(##ctypes) \
class BASE_RECEIVER_##name : public base_receiver { \
  friend class name EANGLES(##types); \
  typedef name EANGLES(##types)                 event; \
  typedef BASE_RECEIVER_##name EANGLES(##types) receiver; \
  event* _e; \
  list_item _it; \
  public : \
  BASE_RECEIVER_##name() : _e(0) {} \
  inline ~BASE_RECEIVER_##name(); \
  base_event* get_event() const { return (base_event*)_e; } \
  virtual void notify(types) = 0; \
};


#define _DECLARE_EVENTS(name,ctypes,types,type_args,args) \
\
template<class obs, class func ECOMMA(##ctypes)> \
event_item attach(name EANGLES(##types)& e, obs& c, func f); \
\
template<class func ECOMMA(##ctypes)> \
event_item attach(name EANGLES(##types)& e, func f); \
\
ETEMPLATE(##ctypes) \
class name : public base_event { \
  typedef BASE_RECEIVER_##name EANGLES(##types) receiver; \
  list<receiver*> _receivers; \
  int _occur_level; \
  int _empty_attachments; \
  void init() { _occur_level = 0; _empty_attachments = 0; } \
\
protected : \
  virtual void notify_receivers(type_args) { \
    receiver *r; \
    forall(r,_receivers) if (r && r->is_enabled()) r->notify(args); \
  } \
\
public : \
  name() { init(); } \
  name& operator = (const name&) { return *this; } \
  name(const name& e) : base_event() { init(); *this = e; } \
  ~name() { \
    receiver *r; \
    forall(r,_receivers) if (r) delete r; \
  } \
\
  virtual void append(receiver& r) { \
    r._it = _receivers.append(&r); \
    r._e  = this; \
  } \
\
  virtual void remove(receiver& r) { \
    if (_occur_level) { \
      _receivers[r._it] = 0; \
      _empty_attachments++; \
    } \
    else _receivers.erase(r._it); \
    r._e = 0; \
  } \
\
  void occur(type_args) { \
    if (!_receivers.empty()) { \
      _occur_level++; notify_receivers(args); _occur_level--; \
    } \
  } \
\
  void remove_empty_attachments () { \
    list_item it; \
    forall_items(it,_receivers) \
      if (_receivers[it] == nil) _receivers.erase(it); \
    _empty_attachments = 0; \
  } \
\
  void operator () (type_args) { occur(args); } \
  int number_of_attachments() const { return _receivers.size(); } \
  int number_of_empty_attachments() const { return _empty_attachments; } \
  int occur_level() const { return _occur_level; } \
  EVENT_MEMBER_TEMPLATES \
};                       


#ifdef LEDA_MEMBER_TEMPLATES
#define EVENT_MEMBER_TEMPLATES \
  template<class obs,class func> \
  event_item attach(obs & c, func f) { return ::attach(*this,c,f); } \
  template<class func> \
  event_item attach(func f) { return ::attach(*this,f); }
#else
#define EVENT_MEMBER_TEMPLATES
#endif


#define _DECLARE_EVENT_XDEFS(name,ctypes,types,type_args,args) \
ETEMPLATE(##ctypes) \
inline BASE_RECEIVER_##name EANGLES(##types)::~BASE_RECEIVER_##name() { \
  if (_e) _e->remove(*this); \
} 


#define _DECLARE_RECEIVERS(name,ctypes,types,type_args,args) \
\
template<class obs, class func ECOMMA(##ctypes)> \
class RECEIVER_##name : public BASE_RECEIVER_##name EANGLES(##types) { \
  obs *_c; func _f; \
  public : \
  void notify(type_args) { (_c->*_f)(args); } \
  RECEIVER_##name(obs& c, func f) :  _c(&c) , _f(f) {} \
}; \
\
template<class func ECOMMA(##ctypes)> \
class GLOBAL_RECEIVER_##name : public BASE_RECEIVER_##name EANGLES(##types) { \
  func _f; \
  public : \
  void notify(type_args) { (*_f)(args); } \
  GLOBAL_RECEIVER_##name(func f) : _f(f) {} \
};


#define _DECLARE_ATTACH_FUNCS(name,ctypes,types) \
\
template<class obs, class func ECOMMA(##ctypes)> \
inline event_item attach(name EANGLES(##types)& e, obs& c, func f) { \
  typedef RECEIVER_##name<obs,func ECOMMA(##types)> receiver; \
  receiver* r = new receiver (c,f); \
  e.append(*r); \
  return r; \
} \
\
template<class func ECOMMA(##ctypes)> \
inline event_item attach(name EANGLES(##types)& e, func f) { \
  typedef GLOBAL_RECEIVER_##name<func ECOMMA(##types)> receiver; \
  receiver* r = new receiver (f); \
  e.append(*r); \
  return r; \
}

#define _DECLARE_EVENT_SYSTEM(name,ctypes,types,type_args,args) \
  _DECLARE_BASE_RECEIVER ( name, ##ctypes, ##types                      ) \
  _DECLARE_EVENTS        ( name, ##ctypes, ##types, ##type_args, ##args ) \
  _DECLARE_RECEIVERS     ( name, ##ctypes, ##types, ##type_args, ##args ) \
  _DECLARE_EVENT_XDEFS   ( name, ##ctypes, ##types, ##type_args, ##args ) \
  _DECLARE_ATTACH_FUNCS  ( name, ##ctypes, ##types                      )

#define DECLARE_EVENT_SYSTEM(name,argc) \
  _DECLARE_EVENT_SYSTEM( name, \
		         _E1FMT##argc(class argt), \
		         _E1FMT##argc(argt), \
			 _E2FMT##argc(argt,a), \
			 _E1FMT##argc(a) )


#define ETEMPLATE(args)
#define EANGLES(args)
#define ECOMMA(args)

#if defined(_MSC_VER)
#pragma warning(disable: 4003)
#endif

DECLARE_EVENT_SYSTEM(EVENT0,0)   // makes EVENT0 available

#if defined(_MSC_VER)
#pragma warning(default: 4003)
#endif

#undef ETEMPLATE
#undef EANGLES
#undef ECOMMA


#define ETEMPLATE(args) template<args>
#define EANGLES(args)   <args>
#define ECOMMA(args)    ,args

DECLARE_EVENT_SYSTEM(EVENT1,1)   // makes EVENT1<type> available
DECLARE_EVENT_SYSTEM(EVENT2,2)   // makes EVENT2<type1,type2> available ...
DECLARE_EVENT_SYSTEM(EVENT3,3)        
DECLARE_EVENT_SYSTEM(EVENT4,4)        
DECLARE_EVENT_SYSTEM(EVENT5,5)        

// DECLARE_EVENT_SYSTEM(event_name,argument_count) 



