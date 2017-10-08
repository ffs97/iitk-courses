/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  test_event.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef USE_STL
#include <LEDA/system/event.h>
#else
#include "stl_event.h"
#endif

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif



// LEDA/system/event.h>
//    g++ -I/LEDA/BETA/incl test_event.c -lL
//    CC -pto -I/LEDA/BETA/incl test_event.c -lL

// STL:  #include "stl_event.h"
//    g++ test_event.c



// global events

EVENT0             E0;
EVENT1<int>        E1;
EVENT2<const char*,int> E2;

// global receivers

void on_E0 () {
  cout << "::on_E0" << endl;
}
void on_E1 (int a) {
  cout << "::on_E1 with arg " << a << endl;
}
void on_E2 (const char* s, int a) {
  cout << "::on_E2 with args " << s << " and " << a << endl;
}


// a Subject with events

class Subject {

  public :

  EVENT0             E0;
  EVENT1<int>        E1;
  EVENT2<const char*,int> E2;

  static EVENT1<Subject&> create_event;

  Subject() { create_event(*this); }

};

EVENT1<Subject&> Subject::create_event;

class BaseObserver {

  public :

  virtual void on_gcc_2_7_bug(const char* s,int) { 
    cout << s << " failed!" << endl; 
  }

};

class Observer : public BaseObserver {

  public :

  void on_E0 () {
    cout << "Observer::on_E0" << endl;
  }
  void on_E1 (int a) {
    cout << "Observer::on_E1 with arg " << a << endl;
  }
  void on_E2 (const char* s, int a) {
    cout << "Observer::on_E2 with args " << s << " and " << a << endl;
  }
  
  void on_gcc_2_7_bug(const char* s, int) { cout << s << " passed." << endl; }

  void on_create_S(const Subject&) { cout << "Subject created." << endl; }


};


// template classes


template <class T>
class SUBJECT {

  public :

  EVENT1<T> E;

  // simulating:     static EVENT1<SUBJECT<T>&> create_event;

  CREATE_EVENT(const SUBJECT<T>);  
            // same as CLASS_EVENT(create_event,SUBJECT<T>);

  SUBJECT() { 
    create_event(*this); 
  }

};


template <class T>
class OBSERVER {

  public :

  void on_create_S(const SUBJECT<T>&) { 
    cout << "SUBJECT<T> created." << endl;
  }

  void on_E(const T& t) { 
    cout << "OBSERVER::on_E with arg " << t << endl;
  }
  

};


// class for testing self-loop

class MyEvent : public  EVENT1<int> {
  typedef EVENT1<int> inherited;
  static int c;
  public : 
  void occur(int a) { 
    cout << ++c << ": occuring with " << a << endl;
    inherited::occur(a);
  }
};

int MyEvent::c = 0;

main() {

  event_item it[10];

  Subject  S;
  Observer O;

  it[0] = attach(E0,on_E0);
  it[1] = attach(E1,on_E1);
  it[2] = attach(E2,on_E2);

  cout << endl 
       << "Testing events E0(), E1(5), E2(\"two\",2) ..." 
       << endl << endl;

  E0();
  E1(5);
  E2("two",2);

  cout << endl 
       << "Testing done." 
       << endl << endl;

  it[3] = attach(S.E0,on_E0);
  it[4] = attach(S.E1,on_E1);
  it[5] = attach(S.E2,on_E2);
  it[6] = attach(S.E0,O,&Observer::on_E0);
  it[7] = attach(S.E1,O,&Observer::on_E1);
  it[8] = attach(S.E2,O,&Observer::on_E2);

  cout << endl 
       << "Testing events S.E0(), S.E1(7), S.E2(\"four\",4) of Subject S ..." 
       << endl << endl;

  S.E0();
  S.E1(7);
  S.E2("four",4);

  cout << endl 
       << "Testing done." 
       << endl << endl;

  detach(it,9);


  BaseObserver *bO = new Observer;
  it[0] = attach(E2,*bO,&BaseObserver::on_gcc_2_7_bug);
  E2.occur("Testing virtual receive-methods ...",0);
  detach(it[0]);
  delete bO;

  {
    cout << endl 
	 << "Testing static [create-] event of Subject ..." 
	 << endl << endl;
    
    it[0] = attach(Subject::create_event,O,&Observer::on_create_S);
    Subject *s = new Subject;
    delete s;
    detach(it[0]);
    
    cout << endl 
	 << "Testing done." 
	 << endl << endl;
  }

  {
    
    OBSERVER<double> tO;
    cout << endl 
	 << "TEMPLATES : Testing event of SUBJECT<double> ..." 
	 << endl << endl;

    SUBJECT<double> dummy; 
    it[0] = attach(dummy.E,tO,&OBSERVER<double>::on_E);
    dummy.E(1.2345);
    detach(it[0]);
 
    cout << endl 
	 << "Testing done." 
	 << endl << endl;

    cout << endl 
	 << "TEMPLATES : Testing static [create] event of SUBJECT<double> ..." 
	 << endl << endl;
    

    cout << "attaching ...";

    //    { string s; cin >> s; }
   
    it[0] = attach(SUBJECT<double>::create_event,tO,&OBSERVER<double>::on_create_S);


    cout << "done." << endl;
    SUBJECT<double> *s = new SUBJECT<double>;
    delete s;
    detach(it[0]);
    
    cout << endl 
	 << "Testing done." 
	 << endl << endl;
  }


  {

    EVENT1<int> E3;

    it[0] = attach(E1,E3,&EVENT1<int>::occur);
    it[1] = attach(E3,on_E1);
    E1(9);
    detach(it,2);

  }

#ifdef DERIVED_EVENT  
  {
    
//   This part may demonstrate a selfloop, but some compilers (g++-2.7,CC) 
//   do not compile this because of an non-existing attach-template.

    MyEvent E;
    it[0] = attach(E,E,&MyEvent::occur);
    E.occur(11);  // selfloop
    detach(it[0]);

  }
#endif


  return 0;
}

