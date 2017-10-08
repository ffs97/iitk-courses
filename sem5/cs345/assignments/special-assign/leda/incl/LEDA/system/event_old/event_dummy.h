/*******************************************************************************
+
+  LEDA 4.5  
+
+
+  event_dummy.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_DUMMY_EVENT_H
#define LEDA_DUMMY_EVENT_H

typedef char* event_item;

inline void detach(event_item) {}
inline void detach(event_item*, int)  {}

class EVENT0 {
 public:
  void attach(event_item) {}
  void detach(event_item) {}
  void occur() {}
  void operator() () {}
  void shrink () {}
  int number_of_attachments() const { return 0; }
  int number_of_empty_attachments() const { return 0; }
  int occur_level() const { return 0; } 
};

template < class obs, class func >
inline event_item attach(EVENT0 &, obs &, func)
{ return NULL; } 

template < class func >
inline event_item attach(EVENT0 &, func)
{ return NULL; } 


template <class argt>
class EVENT1 {
 public:
  void attach(event_item) {}
  void detach(event_item) {}
  void occur(argt) {}
  void operator() (argt) {}
  void shrink () {}
  int number_of_attachments() const { return 0; }
  int number_of_empty_attachments() const { return 0; }
  int occur_level() const { return 0; } 
};

template <class obs, class func, class argt>
inline event_item attach(EVENT1 < argt > &, obs &, func)
{ return NULL; } 

template <class func, class argt>
inline event_item attach(EVENT1 < argt > &, func)
{ return NULL; }


template < class argt2, class argt1 >
class EVENT2 {
 public:
  void attach(event_item) {}
  void detach(event_item) {}
  void occur(argt2, argt1) {}
  void operator() (argt2, argt1) {}
  void shrink () {}
  int number_of_attachments() const { return 0; }
  int number_of_empty_attachments() const { return 0; }
  int occur_level() const { return 0; } 
};

template <class obs, class func, class argt2, class argt1>
inline event_item attach(EVENT2 < argt2, argt1 > &, obs &, func)
{ return NULL; } 

template <class func, class argt2, class argt1>
inline event_item attach(EVENT2<argt2,argt1>&, func)
{ return NULL; }

template < class argt3, class argt2, class argt1 >
class EVENT3 {
 public:
  void attach(event_item) {}
  void detach(event_item) {}
  void occur(argt3, argt2, argt1) {}
  void operator() (argt3, argt2, argt1) {}
  void shrink () {}
  int number_of_attachments() const { return 0; }
  int number_of_empty_attachments() const { return 0; }
  int occur_level() const { return 0; } 
};

template < class obs, class func, class argt3, class argt2, class argt1 >
inline event_item attach(EVENT3 < argt3, argt2, argt1 > &, obs &, func)
{ return NULL; } 

template < class func, class argt3, class argt2, class argt1 >
inline event_item attach(EVENT3 < argt3, argt2, argt1 > &, func)
{ return NULL; }

template < class argt4, class argt3, class argt2, class argt1 >
class EVENT4 {
 public:
  void attach(event_item) {}
  void detach(event_item) {}
  void occur(argt4, argt3, argt2, argt1) {}
  void operator() (argt4, argt3, argt2, argt1) {}
  void shrink () {}
  int number_of_attachments() const { return 0; }
  int number_of_empty_attachments() const { return 0; }
  int occur_level() const { return 0; } 
};

template <class obs,class func,class argt4,class argt3,class argt2,class argt1>
inline event_item attach(EVENT4 < argt4, argt3, argt2, argt1 > &, obs &, func)
{ return NULL; } 


template <class func, class argt4, class argt3, class argt2, class argt1>
inline event_item attach(EVENT4 < argt4, argt3, argt2, argt1 > &, func)
{ return NULL; }

template < class argt5, class argt4, class argt3, class argt2, class argt1 >
class EVENT5 {
 public:
  void attach(event_item) {}
  void detach(event_item) {}
  void occur(argt5, argt4, argt3, argt2, argt1) {}
  void operator() (argt5, argt4, argt3, argt2, argt1) {}
  void shrink () {}
  int number_of_attachments() const { return 0; }
  int number_of_empty_attachments() const { return 0; }
  int occur_level() const { return 0; } 
};

template <class obs,class func,class argt5, class argt4,class argt3,class argt2,class argt1>
inline event_item attach(EVENT5 <argt5,argt4,argt3,argt2,argt1>&,obs &,func)
{ return NULL; } 


template <class func,class argt5,class argt4,class argt3,class argt2,class argt1>
inline event_item attach(EVENT5 <argt5,argt4,argt3,argt2,argt1>&,func)
{ return NULL; }


class VOID_EVENT {
 public:
  void attach(event_item) {}
  void detach(event_item) {}
  void occur() {}
  void operator() () {}
  void shrink () {}
  int number_of_attachments() const { return 0; }
  int number_of_empty_attachments() const { return 0; }
  int occur_level() const { return 0; } 
};


template <class obs, class func>
inline event_item attach(VOID_EVENT &, obs &, func)
{ return NULL; }

template <class func>
inline event_item attach(VOID_EVENT &, func)
{ return NULL; }

#endif







