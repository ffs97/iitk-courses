/*******************************************************************************
+
+  LEDA 4.5  
+
+
+  event_nomacs.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



LEDA_BEGIN_NAMESPACE

class EVENT0;
class BASE_RECEIVER_EVENT0:public base_receiver {
  friend class EVENT0;
  typedef EVENT0 event;
  typedef BASE_RECEIVER_EVENT0 receiver;
  event *_e;
  list_item _it;
   public:BASE_RECEIVER_EVENT0():_e(0) {} 
  inline ~BASE_RECEIVER_EVENT0();
  base_event *get_event() const;
  virtual void notify() = 0;
};

template < class obs, class func > event_item attach(EVENT0 & e, obs & c, func f);
template < class func > event_item attach(EVENT0 & e, func f);
class EVENT0:public base_event {
  typedef BASE_RECEIVER_EVENT0 receiver;
   list < receiver * >_receivers;
  int _occur_level;
  int _empty_attachments;
  void init() {
    _occur_level = 0;
    _empty_attachments = 0;
  } protected:virtual void notify_receivers() {
    receiver *r;
     forall(r, _receivers) if (r && r->is_enabled())
       r->notify();
  }
  public:EVENT0() {
    init();
  }
  EVENT0(const EVENT0 & e) {
    init();
    *this = e;
  } ~EVENT0() {
    receiver *r;
    forall(r, _receivers) if (r)
      delete r;
  }
  virtual void append(receiver & r) {
    r._it = _receivers.append(&r);
    r._e = this;
  } virtual void remove(receiver & r) {
    if (_occur_level) {
      _receivers[r._it] = 0;
      _empty_attachments++;
    }
    else
       _receivers.erase(r._it);
    r._e = 0;
  }
  void occur() {
    if (!_receivers.empty()) {
      _occur_level++;
      notify_receivers();
      _occur_level--;
    }
  }
  void remove_empty_attachments() {
    list_item it;
     forall_items(it, _receivers) if (_receivers[it] == nil)
       _receivers.erase(it);
     _empty_attachments = 0;
  }
  void operator() () {
    occur();
  } EVENT0 & operator = (const EVENT0 &) {
    return *this;
  }
  int number_of_attachments() const {
    return _receivers.size();
  }
  int number_of_empty_attachments() const {
    return _empty_attachments;
  }
  int occur_level() const {
    return _occur_level;
  }
};
template < class obs, class func > class RECEIVER_EVENT0:public BASE_RECEIVER_EVENT0 {
  obs *_c;
  func _f;
public:void notify() {
    (_c->*_f) ();
  } RECEIVER_EVENT0(obs & c, func f):_c(&c), _f(f) {
  }
};

template < class func > class GLOBAL_RECEIVER_EVENT0:public BASE_RECEIVER_EVENT0 {
  func _f;
public:void notify() {
    (*_f) ();
  } GLOBAL_RECEIVER_EVENT0(func f):_f(f) {
  }
};
inline BASE_RECEIVER_EVENT0::~BASE_RECEIVER_EVENT0()
{
  if (_e)
    _e->remove(*this);
}
inline base_event *BASE_RECEIVER_EVENT0::get_event()
const {
  return _e;
}
template < class obs, class func > inline event_item attach(EVENT0 & e, obs & c, func f)
{
  typedef RECEIVER_EVENT0 < obs, func > receiver;
  receiver *r = new receiver(c, f);
  e.append(*r);
  return r;
}
template < class func > inline event_item attach(EVENT0 & e, func f)
{
  typedef GLOBAL_RECEIVER_EVENT0 < func > receiver;
  receiver *r = new receiver(f);
  e.append(*r);
  return r;
}














template < class argt > class EVENT1;
template < class argt > class BASE_RECEIVER_EVENT1:public base_receiver {
  friend class EVENT1 < argt >;
  typedef EVENT1 < argt > event;
  typedef BASE_RECEIVER_EVENT1 < argt > receiver;
  event *_e;
  list_item _it;
public: BASE_RECEIVER_EVENT1():_e(0) {
  } ~BASE_RECEIVER_EVENT1();
  base_event *get_event() const;
  virtual void notify(argt) = 0;
};

template < class obs, class func, class argt > event_item attach(EVENT1 < argt > &e, obs & c, func f);
template < class func, class argt > event_item attach(EVENT1 < argt > &e, func f);
template < class argt > class EVENT1:public base_event {
  typedef BASE_RECEIVER_EVENT1 < argt > receiver;
  list < receiver * >_receivers;
  int _occur_level;
  int _empty_attachments;
  void init() {
    _occur_level = 0;
    _empty_attachments = 0;
  } protected:virtual void notify_receivers(argt a) {
    receiver *r;
     forall(r, _receivers) if (r && r->is_enabled())
       r->notify(a);
  }
  public:EVENT1() {
    init();
  }
  EVENT1(const EVENT1 & e) {
    init();
    *this = e;
  } ~EVENT1() {
    receiver *r;
    forall(r, _receivers) if (r)
      delete r;
  }
  virtual void append(receiver & r) {
    r._it = _receivers.append(&r);
    r._e = this;
  } virtual void remove(receiver & r) {
    if (_occur_level) {
      _receivers[r._it] = 0;
      _empty_attachments++;
    }
    else
       _receivers.erase(r._it);
    r._e = 0;
  }
  void occur(argt a) {
    if (!_receivers.empty()) {
      _occur_level++;
      notify_receivers(a);
      _occur_level--;
    }
  }
  void remove_empty_attachments() {
    list_item it;
     forall_items(it, _receivers) if (_receivers[it] == nil)
       _receivers.erase(it);
     _empty_attachments = 0;
  }
  void operator() (argt a) {
    occur(a);
  } EVENT1 & operator = (const EVENT1 &) {
    return *this;
  }
  int number_of_attachments() const {
    return _receivers.size();
  }
  int number_of_empty_attachments() const {
    return _empty_attachments;
  }
  int occur_level() const {
    return _occur_level;
  }
};
template < class obs, class func, class argt > class RECEIVER_EVENT1:public BASE_RECEIVER_EVENT1 < argt > {
  obs *_c;
  func _f;
public:void notify(argt a) {
    (_c->*_f) (a);
  } RECEIVER_EVENT1(obs & c, func f):_c(&c), _f(f) {
  }
};

template < class func, class argt > class GLOBAL_RECEIVER_EVENT1:public BASE_RECEIVER_EVENT1 < argt > {
  func _f;
public:void notify(argt a) {
    (*_f) (a);
  } GLOBAL_RECEIVER_EVENT1(func f):_f(f) {
  }
};

template < class argt > inline BASE_RECEIVER_EVENT1 < argt >::~BASE_RECEIVER_EVENT1()
{
  if (_e)
    _e->remove(*this);
}
template < class argt > inline base_event * BASE_RECEIVER_EVENT1 < argt >::get_event()
const {
  return _e;
}
template < class obs, class func, class argt > inline event_item attach(EVENT1 < argt > &e, obs & c, func f)
{
  typedef RECEIVER_EVENT1 < obs, func, argt > receiver;
  receiver *r = new receiver(c, f);
  e.append(*r);
  return r;
}
template < class func, class argt > inline event_item attach(EVENT1 < argt > &e, func f)
{
  typedef GLOBAL_RECEIVER_EVENT1 < func, argt > receiver;
  receiver *r = new receiver(f);
  e.append(*r);
  return r;
}
template < class argt1, class argt2 > class EVENT2;
template < class argt1, class argt2 > class BASE_RECEIVER_EVENT2:public base_receiver {
  friend class EVENT2 < argt1, argt2 >;
  typedef EVENT2 < argt1, argt2 > event;
  typedef BASE_RECEIVER_EVENT2 < argt1, argt2 > receiver;
  event *_e;
  list_item _it;
public: BASE_RECEIVER_EVENT2():_e(0) {
  } ~BASE_RECEIVER_EVENT2();
  base_event *get_event() const;
  virtual void notify(argt1, argt2) = 0;
};

template < class obs, class func, class argt1, class argt2 > event_item attach(EVENT2 < argt1, argt2 > &e, obs & c, func f);
template < class func, class argt1, class argt2 > event_item attach(EVENT2 < argt1, argt2 > &e, func f);
template < class argt1, class argt2 > class EVENT2:public base_event {
  typedef BASE_RECEIVER_EVENT2 < argt1, argt2 > receiver;
  list < receiver * >_receivers;
  int _occur_level;
  int _empty_attachments;
  void init() {
    _occur_level = 0;
    _empty_attachments = 0;
  } protected:virtual void notify_receivers(argt1 a1, argt2 a2) {
    receiver *r;
     forall(r, _receivers) if (r && r->is_enabled())
       r->notify(a1, a2);
  }
  public:EVENT2() {
    init();
  }
  EVENT2(const EVENT2 & e) {
    init();
    *this = e;
  } ~EVENT2() {
    receiver *r;
    forall(r, _receivers) if (r)
      delete r;
  }
  virtual void append(receiver & r) {
    r._it = _receivers.append(&r);
    r._e = this;
  } virtual void remove(receiver & r) {
    if (_occur_level) {
      _receivers[r._it] = 0;
      _empty_attachments++;
    }
    else
       _receivers.erase(r._it);
    r._e = 0;
  }
  void occur(argt1 a1, argt2 a2) {
    if (!_receivers.empty()) {
      _occur_level++;
      notify_receivers(a1, a2);
      _occur_level--;
    }
  }
  void remove_empty_attachments() {
    list_item it;
     forall_items(it, _receivers) if (_receivers[it] == nil)
       _receivers.erase(it);
     _empty_attachments = 0;
  }
  void operator() (argt1 a1, argt2 a2) {
    occur(a1, a2);
  } EVENT2 & operator = (const EVENT2 &) {
    return *this;
  }
  int number_of_attachments() const {
    return _receivers.size();
  }
  int number_of_empty_attachments() const {
    return _empty_attachments;
  }
  int occur_level() const {
    return _occur_level;
  }
};
template < class obs, class func, class argt1, class argt2 > class RECEIVER_EVENT2:public BASE_RECEIVER_EVENT2 < argt1, argt2 > {
  obs *_c;
  func _f;
public:void notify(argt1 a1, argt2 a2) {
    (_c->*_f) (a1, a2);
  } RECEIVER_EVENT2(obs & c, func f):_c(&c), _f(f) {
  }
};

template < class func, class argt1, class argt2 > class GLOBAL_RECEIVER_EVENT2:public BASE_RECEIVER_EVENT2 < argt1, argt2 > {
  func _f;
public:void notify(argt1 a1, argt2 a2) {
    (*_f) (a1, a2);
  } GLOBAL_RECEIVER_EVENT2(func f):_f(f) {
  }
};

template < class argt1, class argt2 > inline BASE_RECEIVER_EVENT2 < argt1, argt2 >::~BASE_RECEIVER_EVENT2()
{
  if (_e)
    _e->remove(*this);
}
template < class argt1, class argt2 > inline base_event * BASE_RECEIVER_EVENT2 < argt1, argt2 >::get_event()
const {
  return _e;
}
template < class obs, class func, class argt1, class argt2 > inline event_item attach(EVENT2 < argt1, argt2 > &e, obs & c, func f)
{
  typedef RECEIVER_EVENT2 < obs, func, argt1, argt2 > receiver;
  receiver *r = new receiver(c, f);
  e.append(*r);
  return r;
}
template < class func, class argt1, class argt2 > inline event_item attach(EVENT2 < argt1, argt2 > &e, func f)
{
  typedef GLOBAL_RECEIVER_EVENT2 < func, argt1, argt2 > receiver;
  receiver *r = new receiver(f);
  e.append(*r);
  return r;
}

template < class argt1, class argt2, class argt3 > class EVENT3;
template < class argt1, class argt2, class argt3 > class BASE_RECEIVER_EVENT3:public base_receiver {
  friend class EVENT3 < argt1, argt2, argt3 >;
  typedef EVENT3 < argt1, argt2, argt3 > event;
  typedef BASE_RECEIVER_EVENT3 < argt1, argt2, argt3 > receiver;
  event *_e;
  list_item _it;
public: BASE_RECEIVER_EVENT3():_e(0) {
  } ~BASE_RECEIVER_EVENT3();
  base_event *get_event() const;
  virtual void notify(argt1, argt2, argt3) = 0;
};

template < class obs, class func, class argt1, class argt2, class argt3 > event_item attach(EVENT3 < argt1, argt2, argt3 > &e, obs & c, func f);
template < class func, class argt1, class argt2, class argt3 > event_item attach(EVENT3 < argt1, argt2, argt3 > &e, func f);
template < class argt1, class argt2, class argt3 > class EVENT3:public base_event {
  typedef BASE_RECEIVER_EVENT3 < argt1, argt2, argt3 > receiver;
  list < receiver * >_receivers;
  int _occur_level;
  int _empty_attachments;
  void init() {
    _occur_level = 0;
    _empty_attachments = 0;
  } protected:virtual void notify_receivers(argt1 a1, argt2 a2, argt3 a3) {
    receiver *r;
     forall(r, _receivers) if (r && r->is_enabled())
       r->notify(a1, a2, a3);
  }
  public:EVENT3() {
    init();
  }
  EVENT3(const EVENT3 & e) {
    init();
    *this = e;
  } ~EVENT3() {
    receiver *r;
    forall(r, _receivers) if (r)
      delete r;
  }
  virtual void append(receiver & r) {
    r._it = _receivers.append(&r);
    r._e = this;
  } virtual void remove(receiver & r) {
    if (_occur_level) {
      _receivers[r._it] = 0;
      _empty_attachments++;
    }
    else
       _receivers.erase(r._it);
    r._e = 0;
  }
  void occur(argt1 a1, argt2 a2, argt3 a3) {
    if (!_receivers.empty()) {
      _occur_level++;
      notify_receivers(a1, a2, a3);
      _occur_level--;
    }
  }
  void remove_empty_attachments() {
    list_item it;
     forall_items(it, _receivers) if (_receivers[it] == nil)
       _receivers.erase(it);
     _empty_attachments = 0;
  }
  void operator() (argt1 a1, argt2 a2, argt3 a3) {
    occur(a1, a2, a3);
  } EVENT3 & operator = (const EVENT3 &) {
    return *this;
  }
  int number_of_attachments() const {
    return _receivers.size();
  }
  int number_of_empty_attachments() const {
    return _empty_attachments;
  }
  int occur_level() const {
    return _occur_level;
  }
};
template < class obs, class func, class argt1, class argt2, class argt3 > class RECEIVER_EVENT3:public BASE_RECEIVER_EVENT3 < argt1, argt2, argt3 > {
  obs *_c;
  func _f;
public:void notify(argt1 a1, argt2 a2, argt3 a3) {
    (_c->*_f) (a1, a2, a3);
  } RECEIVER_EVENT3(obs & c, func f):_c(&c), _f(f) {
  }
};

template < class func, class argt1, class argt2, class argt3 > class GLOBAL_RECEIVER_EVENT3:public BASE_RECEIVER_EVENT3 < argt1, argt2, argt3 > {
  func _f;
public:void notify(argt1 a1, argt2 a2, argt3 a3) {
    (*_f) (a1, a2, a3);
  } GLOBAL_RECEIVER_EVENT3(func f):_f(f) {
  }
};

template < class argt1, class argt2, class argt3 > inline BASE_RECEIVER_EVENT3 < argt1, argt2, argt3 >::~BASE_RECEIVER_EVENT3()
{
  if (_e)
    _e->remove(*this);
}
template < class argt1, class argt2, class argt3 > inline base_event * BASE_RECEIVER_EVENT3 < argt1, argt2, argt3 >::get_event()
const {
  return _e;
}
template < class obs, class func, class argt1, class argt2, class argt3 > inline event_item attach(EVENT3 < argt1, argt2, argt3 > &e, obs & c, func f)
{
  typedef RECEIVER_EVENT3 < obs, func, argt1, argt2, argt3 > receiver;
  receiver *r = new receiver(c, f);
  e.append(*r);
  return r;
}
template < class func, class argt1, class argt2, class argt3 > inline event_item attach(EVENT3 < argt1, argt2, argt3 > &e, func f)
{
  typedef GLOBAL_RECEIVER_EVENT3 < func, argt1, argt2, argt3 > receiver;
  receiver *r = new receiver(f);
  e.append(*r);
  return r;
}

template < class argt1, class argt2, class argt3, class argt4 > class EVENT4;
template < class argt1, class argt2, class argt3, class argt4 > class BASE_RECEIVER_EVENT4:public base_receiver {
  friend class EVENT4 < argt1, argt2, argt3, argt4 >;
  typedef EVENT4 < argt1, argt2, argt3, argt4 > event;
  typedef BASE_RECEIVER_EVENT4 < argt1, argt2, argt3, argt4 > receiver;
  event *_e;
  list_item _it;
public: BASE_RECEIVER_EVENT4():_e(0) {
  } ~BASE_RECEIVER_EVENT4();
  base_event *get_event() const;
  virtual void notify(argt1, argt2, argt3, argt4) = 0;
};

template < class obs, class func, class argt1, class argt2, class argt3, class argt4 > event_item attach(EVENT4 < argt1, argt2, argt3, argt4 > &e, obs & c, func f);
template < class func, class argt1, class argt2, class argt3, class argt4 > event_item attach(EVENT4 < argt1, argt2, argt3, argt4 > &e, func f);
template < class argt1, class argt2, class argt3, class argt4 > class EVENT4:public base_event {
  typedef BASE_RECEIVER_EVENT4 < argt1, argt2, argt3, argt4 > receiver;
  list < receiver * >_receivers;
  int _occur_level;
  int _empty_attachments;
  void init() {
    _occur_level = 0;
    _empty_attachments = 0;
  } protected:virtual void notify_receivers(argt1 a1, argt2 a2, argt3 a3, argt4 a4) {
    receiver *r;
     forall(r, _receivers) if (r && r->is_enabled())
       r->notify(a1, a2, a3, a4);
  }
  public:EVENT4() {
    init();
  }
  EVENT4(const EVENT4 & e) {
    init();
    *this = e;
  } ~EVENT4() {
    receiver *r;
    forall(r, _receivers) if (r)
      delete r;
  }
  virtual void append(receiver & r) {
    r._it = _receivers.append(&r);
    r._e = this;
  } virtual void remove(receiver & r) {
    if (_occur_level) {
      _receivers[r._it] = 0;
      _empty_attachments++;
    }
    else
       _receivers.erase(r._it);
    r._e = 0;
  }
  void occur(argt1 a1, argt2 a2, argt3 a3, argt4 a4) {
    if (!_receivers.empty()) {
      _occur_level++;
      notify_receivers(a1, a2, a3, a4);
      _occur_level--;
    }
  }
  void remove_empty_attachments() {
    list_item it;
     forall_items(it, _receivers) if (_receivers[it] == nil)
       _receivers.erase(it);
     _empty_attachments = 0;
  }
  void operator() (argt1 a1, argt2 a2, argt3 a3, argt4 a4) {
    occur(a1, a2, a3, a4);
  } EVENT4 & operator = (const EVENT4 &) {
    return *this;
  }
  int number_of_attachments() const {
    return _receivers.size();
  }
  int number_of_empty_attachments() const {
    return _empty_attachments;
  }
  int occur_level() const {
    return _occur_level;
  }
};
template < class obs, class func, class argt1, class argt2, class argt3, class argt4 > class RECEIVER_EVENT4:public BASE_RECEIVER_EVENT4 < argt1, argt2, argt3, argt4 > {
  obs *_c;
  func _f;
public:void notify(argt1 a1, argt2 a2, argt3 a3, argt4 a4) {
    (_c->*_f) (a1, a2, a3, a4);
  } RECEIVER_EVENT4(obs & c, func f):_c(&c), _f(f) {
  }
};

template < class func, class argt1, class argt2, class argt3, class argt4 > class GLOBAL_RECEIVER_EVENT4:public BASE_RECEIVER_EVENT4 < argt1, argt2, argt3, argt4 > {
  func _f;
public:void notify(argt1 a1, argt2 a2, argt3 a3, argt4 a4) {
    (*_f) (a1, a2, a3, a4);
  } GLOBAL_RECEIVER_EVENT4(func f):_f(f) {
  }
};

template < class argt1, class argt2, class argt3, class argt4 > inline BASE_RECEIVER_EVENT4 < argt1, argt2, argt3, argt4 >::~BASE_RECEIVER_EVENT4()
{
  if (_e)
    _e->remove(*this);
}
template < class argt1, class argt2, class argt3, class argt4 > inline base_event * BASE_RECEIVER_EVENT4 < argt1, argt2, argt3, argt4 >::get_event()
const {
  return _e;
}
template < class obs, class func, class argt1, class argt2, class argt3, class argt4 > inline event_item attach(EVENT4 < argt1, argt2, argt3, argt4 > &e, obs & c, func f)
{
  typedef RECEIVER_EVENT4 < obs, func, argt1, argt2, argt3, argt4 > receiver;
  receiver *r = new receiver(c, f);
  e.append(*r);
  return r;
}
template < class func, class argt1, class argt2, class argt3, class argt4 > inline event_item attach(EVENT4 < argt1, argt2, argt3, argt4 > &e, func f)
{
  typedef GLOBAL_RECEIVER_EVENT4 < func, argt1, argt2, argt3, argt4 > receiver;
  receiver *r = new receiver(f);
  e.append(*r);
  return r;
}

/* EVENT 5 ------------------------------------------------------------------ */
template < class argt1, class argt2, class argt3, class argt4, class argt5 > class EVENT5;
template < class argt1, class argt2, class argt3, class argt4, class argt5 > class BASE_RECEIVER_EVENT5:public base_receiver {
  friend class EVENT5 < argt1, argt2, argt3, argt4, argt5 >;
  typedef EVENT5 < argt1, argt2, argt3, argt4, argt5 > event;
  typedef BASE_RECEIVER_EVENT5 < argt1, argt2, argt3, argt4, argt5 > receiver;
  event *_e;
  list_item _it;
public: BASE_RECEIVER_EVENT5():_e(0) {
  } ~BASE_RECEIVER_EVENT5();
  base_event *get_event() const;
  virtual void notify(argt1, argt2, argt3, argt4, argt5) = 0;
};

template < class obs, class func, class argt1, class argt2, class argt3, class argt4, class argt5 > event_item attach(EVENT5< argt1, argt2, argt3, argt4, argt5 > &e, obs & c, func f);
template < class func, class argt1, class argt2, class argt3, class argt4, class argt5 > event_item attach(EVENT5 < argt1, argt2, argt3, argt4, argt5 > &e, func f);
template < class argt1, class argt2, class argt3, class argt4, class argt5 > class EVENT5:public base_event {
  typedef BASE_RECEIVER_EVENT5 < argt1, argt2, argt3, argt4, argt5 > receiver;
  list < receiver * >_receivers;
  int _occur_level;
  int _empty_attachments;
  void init() {
    _occur_level = 0;
    _empty_attachments = 0;
  } protected:virtual void notify_receivers(argt1 a1, argt2 a2, argt3 a3, argt4 a4, argt5 a5) {
    receiver *r;
     forall(r, _receivers) if (r && r->is_enabled())
       r->notify(a1, a2, a3, a4, a5);
  }
  public:EVENT5() {
    init();
  }
  EVENT5(const EVENT5 & e) {
    init();
    *this = e;
  } ~EVENT5() {
    receiver *r;
    forall(r, _receivers) if (r)
      delete r;
  }
  virtual void append(receiver & r) {
    r._it = _receivers.append(&r);
    r._e = this;
  } virtual void remove(receiver & r) {
    if (_occur_level) {
      _receivers[r._it] = 0;
      _empty_attachments++;
    }
    else
       _receivers.erase(r._it);
    r._e = 0;
  }
  void occur(argt1 a1, argt2 a2, argt3 a3, argt4 a4, argt5 a5) {
    if (!_receivers.empty()) {
      _occur_level++;
      notify_receivers(a1, a2, a3, a4, a5);
      _occur_level--;
    }
  }
  void remove_empty_attachments() {
    list_item it;
     forall_items(it, _receivers) if (_receivers[it] == nil)
       _receivers.erase(it);
     _empty_attachments = 0;
  }
  void operator() (argt1 a1, argt2 a2, argt3 a3, argt4 a4, argt5 a5) {
    occur(a1, a2, a3, a4, a5);
  } EVENT5 & operator = (const EVENT5 &) {
    return *this;
  }
  int number_of_attachments() const {
    return _receivers.size();
  }
  int number_of_empty_attachments() const {
    return _empty_attachments;
  }
  int occur_level() const {
    return _occur_level;
  }
};

template <class obs, class func, class argt1, class argt2, class argt3, class argt4, class argt5> 
class RECEIVER_EVENT5 : 
              public BASE_RECEIVER_EVENT5 < argt1, argt2, argt3, argt4,argt5 > 
{
  obs *_c;
  func _f;

public:
  void notify(argt1 a1, argt2 a2, argt3 a3, argt4 a4, argt5 a5) {
    (_c->*_f) (a1, a2, a3, a4, a5);
  } 

  RECEIVER_EVENT5(obs & c, func f):_c(&c), _f(f) {}
};


template <class func, class argt1, class argt2, class argt3, class argt4, class argt5> 
class GLOBAL_RECEIVER_EVENT5 : 
            public BASE_RECEIVER_EVENT5 <argt1, argt2, argt3, argt4, argt5> {
  func _f;

public:

  void notify(argt1 a1, argt2 a2, argt3 a3, argt4 a4, argt5 a5) {
    (*_f) (a1, a2, a3, a4, a5);
  } 
  GLOBAL_RECEIVER_EVENT5(func f):_f(f) { }
};

template < class argt1, class argt2, class argt3, class argt4, class argt5 > 
inline BASE_RECEIVER_EVENT5 <argt1, argt2, argt3, argt4, argt5>::~BASE_RECEIVER_EVENT5()
{
  if (_e)
    _e->remove(*this);
}

template < class argt1, class argt2, class argt3, class argt4, class argt5 > 
inline base_event* BASE_RECEIVER_EVENT5 <argt1, argt2, argt3, argt4, argt5>::get_event() const {
  return _e;
}

template <class obs, class func, class argt1, class argt2, class argt3, class argt4, class argt5> 
inline event_item attach(EVENT5 <argt1, argt2, argt3, argt4, argt5> &e, obs & c, func f)
{
  typedef RECEIVER_EVENT5 < obs, func, argt1, argt2, argt3, argt4, argt5 > receiver;
  receiver *r = new receiver(c, f);
  e.append(*r);
  return r;
}

template <class func, class argt1, class argt2, class argt3, class argt4, class argt5> 
inline event_item attach(EVENT5 <argt1, argt2, argt3, argt4, argt5> &e, func f)
{
  typedef GLOBAL_RECEIVER_EVENT5 <func, argt1, argt2, argt3, argt4, argt5> receiver;
  receiver *r = new receiver(f);
  e.append(*r);
  return r;
}

LEDA_END_NAMESPACE

