/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  calen.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


/*
#include "date.h"
*/
#include <LEDA/system/date.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif


const int m1_start = 5;
const int m2_start = 32;
const int m3_start = 59;

void fill_to_col(string& s, int c)
{ while (s.length() < c) s+= ' '; }

void print_week(date& D, string& s, int start_col)
{
  fill_to_col(s, start_col - 5);
  date::set_output_format("[w]");
  s += D.get_date();
  fill_to_col(s, start_col + (D.get_day_of_week()-1)*3);
  date::set_output_format("dd");
  do {
    s += D.get_date() + string(' ');
    ++D;
  } while (D.get_day_of_week() != 1 && D.get_day() != 1);
}

int main(int argc, char* argv[])
// syntax: calen [year [language]]
//  - year may be specified with 2 or 4 digits (if left out: current year) 
//  - language is a number in [0;3] (= local, english, german, french)
{
  // get the year
  date D;
  if (argc >= 2) {
    date::set_input_format("[yy]yy");
    D.set_day(1);
    D.set_date(argv[1], false);
  }
  int y = D.get_year();

  // get the language
  if (argc >= 3)
    date::set_language(date::language(1+atoi(argv[2])));
  else
    date::set_language(date::local);

  // create header of abbreviated dow names
  D += 7 - D.get_day_of_week(); // jump to next Sunday
  string dow_names, dow_header;
  date::set_output_format("DOW:2 ");
  for(int i=1; i<=7; ++i) dow_names += (++D).get_date();
  fill_to_col(dow_header, m1_start); dow_header += dow_names; 
  fill_to_col(dow_header, m2_start); dow_header += dow_names; 
  fill_to_col(dow_header, m3_start); dow_header += dow_names; 

  // print calendar
  string line;
  for(int m=1; m<=12; m+=3) {
    date D1(1,date::month(m),y), 
         D2(1,date::month(m+1),y), 
         D3(1,date::month(m+2),y);

    // print header
    line = ""; date::set_output_format("M");
    fill_to_col(line, m1_start); line += D1.get_date(); 
    fill_to_col(line, m2_start); line += D2.get_date(); 
    fill_to_col(line, m3_start); line += D3.get_date(); 
    cout << line << endl;
    cout << dow_header << endl;

    // print 3 months
    while ((D1.get_month()==m)||(D2.get_month()==m+1)||(D3.get_month()==m+2)) {
      line = "";
      if (D1.get_month() == m)   print_week(D1, line, m1_start);
      if (D2.get_month() == m+1) print_week(D2, line, m2_start);
      if (D3.get_month() == m+2) print_week(D3, line, m3_start);
      cout << line << endl;
    }
    cout << endl;
  }

  return 0;
}
