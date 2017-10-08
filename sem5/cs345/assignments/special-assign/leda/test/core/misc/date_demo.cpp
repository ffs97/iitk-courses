/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  date_demo.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/system/date.h>
#include <LEDA/system/stream.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif


int main()
{
  int i;

  date::set_output_format("dd.mm.yyyy DOW, 'week:'w, 'day in year:'diy");
  cout << "size of an object of type date (bytes): " << sizeof(date) << endl;
  cout << "current date: " << date().get_date() << endl; cout.flush();

  date d(4,date::Nov,1973); cout << d << endl; cout.flush();
  date d2("01.01.2010"); cout << d2 << endl; cout.flush();
  date* dp = new date(11,date::Mar,1996); cout << *dp << endl; cout.flush();
  delete dp;

  cout << "Writing dates ..." << endl;
  {
    file_ostream o("test");
    d.set_output_format("dd.mm.[yy]yy DOW");
    o << d << endl; cout << d << endl;
    o << d << endl; cout << d << endl;
    d.set_output_format("dow d.M.yy");
    o << d << endl; cout << d << endl;
    o << d2 << endl; cout << d2 << endl; // reference_year test
    d.set_output_format("DOW:3 d.M:3.yy");
    o << d << endl; cout << d << endl;
    d.set_output_format("dth M yyyy");
    o << d << endl; cout << d << endl;
    d.set_output_format("'Der 'd'.Tag des 'm'.Monats des Jahres 'yyyy.");
    o << d << endl; cout << d << endl;
  }
    
  cout << "Reading dates ..." << endl;
  d.set_output_format(date::hyphens);
  {
    file_istream i("test");
    d.set_input_format("delim:\n;mm/dd/[yy]yy *\n;dd.mm.[yy]yy *\n");
    i >> d; cout << d << endl; cout.flush();
    d.set_input_format("dd?mm?yyyy *\n");
    i >> d; cout << d << endl; cout.flush();
    d.set_input_format("* d.M.yy\n");
    i >> d; cout << d << endl; cout.flush();
    i >> d; cout << d << endl; cout.flush();
    d.set_input_format("* d.M:3.yy\n");
    i >> d; cout << d << endl; cout.flush();
    d.set_input_format("dth M yyyy\n");
    i >> d; cout << d << endl; cout.flush();
    d.set_input_format("'Der 'd'.Tag des 'm'.Monats des Jahres 'yyyy.\n");
    i >> d; cout << d << endl; cout.flush();
  }

  /* Some tests with different languages and formats */
  cout << "Language tests ..." << endl;

  date::set_output_format("d.M.yy DOW");
  date::set_language(date::english);
  cout << d << endl;
  date::set_language(date::french);
  cout << d << endl;
  date::set_language(date::german);
  cout << d << endl;
  date::set_language(date::english);
  cout << d2 << endl;
  date::set_language(date::french);
  cout << d2 << endl;
  date::set_language(date::german);
  cout << d2 << endl;

  const char* mn[] = 
  { "m1","m2","m3","m4","m5","m6","m7","m8","m9","m10","m11","m12" };

  const char* dn[] =
  { "d1","d2","d3","d4","d5","d6","d7" };

  date::set_output_format("DOW d.M.yy");
  date::set_month_names(mn);
  date::set_dow_names(dn);
  cout << date (3,date::Apr,1990) << endl;

  date::set_language(date::local);
  // reads local month and dow names
  cout << date (3,date::Mar,1990) << endl;

  date::set_output_format(date::german_standard);
  cout << date (3,date::Apr,1990) << endl;

  date::set_language(date::english);
  date::set_output_format("dth M yy");
  cout << date(1,date::Mar,98) << endl;
  cout << date(2,date::Mar,98) << endl;
  cout << date(3,date::Mar,98) << endl;
  cout << date(8,date::Mar,98) << endl;
  cout << date(11,date::Mar,98) << endl;
  cout << date(21,date::Mar,98) << endl;

  cout << "Multi-input formats ..." << endl;
  date::set_output_format(date::colons);
  date::set_input_format("m/d/[yy]yy;d?m?[yy]yy");
  cout << d.set_date("12/3/1984",false) << '#'; cout<< d << endl; cout.flush();
  cout << d.set_date("5.0.1974",false) << '#'; cout<< d << endl; cout.flush();
  cout << d.set_date("6,11,1994",false) << '#'; cout<< d << endl; cout.flush();
  cout << d.set_date("10/4/77",false) << '#'; cout<< d << endl; cout.flush();
  cout << d.set_date("4.11.74",false) << '#'; cout<< d << endl; cout.flush();
  cout << d.set_date("4.11.174",false) << '#'; cout<< d << endl; cout.flush();

  cout << "Some update functions ..." << endl;
  d.set_date(4,date::Nov,1974); cout << d << endl; cout.flush();
  d.add_to_month(10); cout <<"+10M " << d << endl; cout.flush();
  d.add_to_month(-11); cout <<"-11M " << d << endl; cout.flush();
  d.set_date(29,date::Feb,1996); cout << d << endl; cout.flush();
  d.add_to_year(1); cout <<"+1J " << d << endl; cout.flush();

  cout << "Some date arithmetics ..." << endl;
  date D1(28,date::Feb,1948);
  date D2(1,date::Mar,1948);
  cout << D2 << " - " << D1 << " = " << D2-D1 << " days" << endl;
  D2.set_date(1,date::Mar,1949);
  cout << D2 << " - " << D1 << " = " << D2-D1 << " days" << endl;
  D2.set_date(1,date::Mar,1952);
  cout << D2 << " - " << D1 << " = " << D2-D1 << " days" << endl;

  cout << D2 << " + 10 days = " << D2 + 10 << endl;
  cout << D1 << " + 20 days = " << D1 + 20 << endl;

  D1.set_date(28,date::Feb,1948);
  D2.set_date(31,date::Dec,1948);
  cout<<"months from "<<D1<<" until "<<D2<<" : "<<D1.months_until(D2)<<endl;
  D1.set_date(29,date::Feb,1948);
  D2.set_date(28,date::Feb,1949);
  cout<<"months from "<<D1<<" until "<<D2<<" : "<<D1.months_until(D2)<<endl;
  D2.set_date(29,date::Feb,1948);
  D1.set_date(28,date::Feb,1949);
  cout<<"months from "<<D1<<" until "<<D2<<" : "<<D1.months_until(D2)<<endl;
  D1.set_date(31,date::Mar,1948);
  D2.set_date(30,date::May,1948);
  cout<<"months from "<<D1<<" until "<<D2<<" : "<<D1.months_until(D2)<<endl;
  D1.set_date(31,date::Jan,1948);
  D2.set_date(29,date::Feb,1948);
  cout<<"months from "<<D1<<" until "<<D2<<" : "<<D1.months_until(D2)<<endl;

  D1.set_date(28,date::Feb,1948);
  D2.set_date(31,date::Dec,1948);
  cout<<"years from "<<D1<<" until "<<D2<<" : "<<D1.years_until(D2)<<endl;
  D1.set_date(29,date::Feb,1948);
  D2.set_date(28,date::Feb,1949);
  cout<<"years from "<<D1<<" until "<<D2<<" : "<<D1.years_until(D2)<<endl;
  D2.set_date(29,date::Feb,1948);
  D1.set_date(28,date::Feb,1949);
  cout<<"years from "<<D1<<" until "<<D2<<" : "<<D1.years_until(D2)<<endl;
  D1.set_date(29,date::Feb,1948);
  D2.set_date(28,date::Feb,1952);
  cout<<"years from "<<D1<<" until "<<D2<<" : "<<D1.years_until(D2)<<endl;
  D1.set_date(28,date::Feb,1951);
  D2.set_date(31,date::Mar,1948);
  cout<<"years from "<<D1<<" until "<<D2<<" : "<<D1.years_until(D2)<<endl;

  cout << "Some predicates ..." << endl;
  d.set_date(28,date::Feb,1979);
  cout << d << " last day ? " << d.is_last_day_in_month() << endl;
  d.set_date(28,date::Feb,1980);
  cout << d << " last day ? " << d.is_last_day_in_month() << endl;
  d.set_date(29,date::Feb,1980);
  cout << d << " last day ? " << d.is_last_day_in_month() << endl;
  d.set_date(30,date::Jun,1980);
  cout << d << " last day ? " << d.is_last_day_in_month() << endl;
  d.set_date(30,date::Jul,1980);
  cout << d << " last day ? " << d.is_last_day_in_month() << endl;

  cout << "Some iterations ..." << endl;
  D1.set_date(1,date::Jan,1800);
  for (i=1; i<= 365*400 + 97; ++i) {
    date old = D1;
    D1 += 1;
    if (D1-old != 1) cout << old << endl;
  }
  if (D1 != date(1,date::Jan,2200)) {
    cout << D1 << endl; cout.flush();
  }

  int days = 0;
  date start(1,date::Jan,1), end(1,date::Jan,401);
  float t; used_time(t);
  for (D1 = start; D1 < end; ++D1) { ++days; }
  cout << used_time(t) << endl;
  cout << end-start << '#' << days << endl;
  
  used_time(t);
  for (i=1; i<= 365*400 + 97; ++i) { --days; }
  cout << used_time(t) << endl;

//   D1.set_to_current_date();
//   date::set_input_format("delim:\n;d.m.\n;d.m.[yy]yy\n");
//   cin >> D1;
//   cout << D1 << endl;

  return 0;
}
