/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  date_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/system/date.h>
#include <LEDA/system/regtest.h>
#include <LEDA/core/random_source.h>
#include <LEDA/core/list.h>
#include <LEDA/core/dictionary.h>
#include <LEDA/core/h_array.h>
#include <time.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif


/*****************************************************************************
  install my own error handler
*****************************************************************************/

class err {
  static LedaErrorHandler old_error_handler;
  static bool   error_fetched;
  static int    cur_num;
  static string cur_msg;

public:
  static bool no_new_err() { return error_fetched; }

  static string fetch()
  // fetch the last error and reset it
  {
    string last_err = cur_msg;
    cur_num = 0; cur_msg = ""; error_fetched = true;
    return last_err;
  }

  friend void my_error_handler(int, const char*);
};

void my_error_handler(int err_num, const char* err_msg)
{
  if (err::error_fetched) {
    err::cur_num = err_num; 
    err::cur_msg = err_msg; 
    err::error_fetched = false;
  }
  else
   err::old_error_handler(err::cur_num, "Unfetched error: "+err::cur_msg);
}

LedaErrorHandler err::old_error_handler = set_error_handler(my_error_handler);
bool   err::error_fetched = true;
int    err::cur_num;
string err::cur_msg;

/*****************************************************************************
  misc. supporting functions
*****************************************************************************/

void test_get_current_date(int& d, int& m, int& y, int& dow, int& diy)
{
  time_t clock;
  time(&clock);
  tm* T = localtime(&clock); 

  d = T->tm_mday; m = T->tm_mon+1; y = 1900 + T->tm_year;
  dow = (T->tm_wday == 0 ? 7 : T->tm_wday);
  diy = T->tm_yday + 1;
}

string test_get_todays_local_M_and_DOW()
{
  time_t clock;
  time(&clock);
  tm* T = localtime(&clock); 

  char buf[100];
  strftime(buf, 99, "%B-%A", T);
  return buf;
}

static const int ds_in_mth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

int test_last_day_in_month(int m, int y)
{
  if (date::is_leap_year(y) && m == 2) 
    return 29;
  else
    return ds_in_mth[m-1];
}

bool test_is_last_day_in_month(const date& d)
{ return d.get_day() == test_last_day_in_month(d.get_month(),d.get_year()); } 

int test_get_last_week(int year)
{
  int dow = date(1,date::Jan,year).get_day_of_week();
  if ( dow == 4 || (dow == 3 && date::is_leap_year(year)) ) return 53;
  else return 52;
}

int test_get_weeknum_of_last_day(int y)
{
  date D(31,date::Dec,y);
  if (7 - D.get_day_of_week() >= 4)
    return 1;
  else
    return (3 + D.get_day_in_year() + 7 - D.get_day_of_week()) / 7;
}

int test_get_weeknum_of_first_day(int y)
{
  if (date(1,date::Jan,y).get_day_of_week() <= 4) return 1;
  else return test_get_weeknum_of_last_day(y-1);
}

random_source S;

date draw_date()
{
  int y = S(1,9999);
  int m = S(1,12);
  // days on the egde of the valid interval are a little bit more likely
  int d = S(-1,33); 
  if (d<1) d = 1;
  if (d>test_last_day_in_month(m,y)) d = test_last_day_in_month(m,y);

  return date(d,date::month(m),y);
}

int test_hash(const date& d)
{ return 400 * d.get_year() + 32 * d.get_month() + d.get_day(); }

/*****************************************************************************
  THE TEST
*****************************************************************************/

int main()
{
  LEDA_TEST_START(date, test);

  // init_random_source (output seed in order to make test reproducible) ////
  {
    time_t seed;
    time(&seed);
    S.set_seed(int(seed));
    F_log << "Seed for random source : " << int(seed) << endl << endl;
  }

  // constructors ///////////////////////////////////////////////////////////

  int d, m, y, dow, diy;
  date today;
  test_get_current_date(d, m, y, dow, diy);
  LEDA_TEST(d == today.get_day(), today:day);
  LEDA_TEST(m == today.get_month(), today:month);
  LEDA_TEST(y == today.get_year(), today:year);
  LEDA_TEST(dow == today.get_day_of_week(), today:dow);
  LEDA_TEST(diy == today.get_day_in_year(), today:diy);

  {
    date* dp = new date();
    LEDA_TEST(*dp == today, operator new);
    delete dp;
  }

  date D1(d,date::month(m),y);
  LEDA_TEST(err::no_new_err(), constructor: (d m y));
  LEDA_TEST(D1 == today, cmp:D1&today);

  {
    date dummy1(29,date::Feb,1973);
    LEDA_TEST(err::fetch() == "date: invalid date", constructor: (d m y));
    date dummy2(31,date::Apr,1973);
    LEDA_TEST(err::fetch() == "date: invalid date", constructor: (d m y));
    date dummy3(31,date::month(13),1973);
    LEDA_TEST(err::fetch() == "date: invalid date", constructor: (d m y));
  }

  date D2("04.11.73");
  LEDA_TEST(err::no_new_err(), constructor:string_swallow);
  LEDA_TEST(D2 == date(4,date::Nov,1973), cmp:D2);
  {
    date dummy1("28aa2.1973");
    LEDA_TEST(err::fetch() == "date: invalid date", constructor:string);
    date dummy2("31.11.1973");
    LEDA_TEST(err::fetch() == "date: invalid date", constructor:string);
    date dummy3("31.12.0");
    LEDA_TEST(err::fetch() == "date: invalid date", constructor:string);
  }
  LEDA_TEST(D2.get_week() == 44, D2:week);
    
  date::set_input_format("yy+m-d");
  date D3("73+11-4", false);
  LEDA_TEST(err::no_new_err(), constructor:string_inp_fmt);
  LEDA_TEST(D3 == D2, cmp:D2&D3);

  date::set_input_format(date::US_standard);
  LEDA_TEST(err::no_new_err(), no pending err.);

  // languages //////////////////////////////////////////////////////////////

  date D4(5,date::Jul,1998);  
  date::set_output_format("M-DOW");

  date::set_language(date::user_def_lang);
  LEDA_TEST(err::fetch() == "date: invalid language", set_language);

  date::set_language(date::local);
  LEDA_TEST(today.get_date() == test_get_todays_local_M_and_DOW(), local);
  
  date::set_language(date::english);
  LEDA_TEST(date::get_language() == date::english, english);
  LEDA_TEST(D4.get_date() == "July-Sunday", english);
  
  date::set_language(date::german);
  LEDA_TEST(date::get_language() == date::german, german);
  LEDA_TEST(D4.get_date() == "Juli-Sonntag", german);
  
  date::set_language(date::french);
  LEDA_TEST(date::get_language() == date::french, french);
  LEDA_TEST(D4.get_date() == "julliet-dimanche", french);
  
  const char* mn[] = 
  { "m1","m2","m3","m4","m5","m6","m7","m8","m9","m10","m11","m12" };
  const char* dn[] =
  { "d1","d2","d3","d4","d5","d6","d7" };
  date::set_month_names(mn);
  date::set_dow_names(dn);
  LEDA_TEST(date::get_language() == date::user_def_lang, user_def_lang);
  LEDA_TEST(D2.get_date() == "m11-d7", user_def_lang);

  date::set_language(date::english);
  date::set_output_format(date::US_standard);
  LEDA_TEST(err::no_new_err(), no pending err.);

  // input/output ///////////////////////////////////////////////////////////

  D2.set_date(4,date::Nov,1973);
  date::set_input_format("dd.mm.[yy]yy ?x");
  LEDA_TEST(date::get_input_format() == date::user_def_fmt, input);
  LEDA_TEST(date::get_input_format_str() == "dd.mm.[yy]yy ?x", input);
  LEDA_TEST(D3.set_date("04.11.73 ,x", false) && D3==D2, input1a);
  LEDA_TEST(!D3.set_date("04.11:73 ,x", false), input1b);
  date::set_input_format("''d.M.''yy '''xyz'''");
  LEDA_TEST(D3.set_date("4.November.73 xyz", false) && D3==D2, input2a);
  LEDA_TEST(!D3.set_date("4.Dezember.73 xyz", false), input2b);
  date::set_input_format("d.m.yy;dth M:3, yyyy");
  LEDA_TEST(D3.set_date("4th Nov, 1973", false) && D3==D2, input3a);
  LEDA_TEST(!D3.set_date("4.11.1973", false), input3b);
  date::set_input_format("*$ m \"@!\"");
  LEDA_TEST(D3.set_date("foobar$ 11 @!", false) && D3==D2, input4a);
  LEDA_TEST(!D3.set_date("test$ 31 @!", false), input4b);

  date::set_input_format(date::user_def_fmt);
  LEDA_TEST(err::fetch() == "date: invalid format", input5);
  date::set_input_format(date::US_standard);
  LEDA_TEST(date::get_input_format() == date::US_standard, input6ai);
  LEDA_TEST(date::get_input_format_str() == "mm/dd/[yy]yy", input6aii);
  LEDA_TEST(D3.set_date("11/04/1973", false), input6b);
  LEDA_TEST(!D3.set_date("1104/1973", false), input6c);
  date::set_input_format(date::german_standard);
  LEDA_TEST(date::get_input_format() == date::german_standard, input7a);
  LEDA_TEST(D3.set_date("04.11.1973", false), input7b);
  LEDA_TEST(!D3.set_date("04.111973", false), input7c);
  date::set_input_format(date::colons);
  LEDA_TEST(date::get_input_format() == date::colons, input8a);
  LEDA_TEST(D3.set_date("04:11:1973", false), input8b);
  LEDA_TEST(!D3.set_date("04-11-1973", false), input8c);
  date::set_input_format(date::hyphens);
  LEDA_TEST(date::get_input_format() == date::hyphens, input9a);
  LEDA_TEST(D3.set_date("04-11-1973", false), input9b);
  LEDA_TEST(!D3.set_date("04.11.1973", false), input9c);

  date::set_output_format("dd.mm.[yy]yy DOW diy");
  LEDA_TEST(date::get_output_format() == date::user_def_fmt, output);
  LEDA_TEST(date::get_output_format_str() == "dd.mm.[yy]yy DOW diy", output);
  LEDA_TEST(D2.get_date() == "04.11.1973 Sunday 308", output1);
  date::set_output_format("''w'' dow d.M.''yy''");
  LEDA_TEST(D2.get_date() == "44 7 4.November.73", output2);
  date::set_output_format("DOW:3 dth M:3, yyyy");
  LEDA_TEST(D2.get_date() == "Sun 4th Nov, 1973", output3);
  date::set_output_format("'Tag:'d \"'Monat\": m 'Jahr':yy';';d.m.yy");
  LEDA_TEST(D2.get_date() == "Tag:4 'Monat: 11 Jahr:73;", output4);
 
  date::set_output_format(date::user_def_fmt);
  LEDA_TEST(err::fetch() == "date: invalid format", output5);
  date::set_output_format(date::US_standard);
  LEDA_TEST(date::get_output_format() == date::US_standard, output6a);
  LEDA_TEST(D2.get_date() == "11/04/1973", output6b);
  date::set_output_format(date::german_standard);
  LEDA_TEST(date::get_output_format() == date::german_standard, output7ai);
  LEDA_TEST(date::get_output_format_str() == "dd.mm.[yy]yy", output7aii);
  LEDA_TEST(D2.get_date() == "04.11.1973", output7b);
  date::set_output_format(date::colons);
  LEDA_TEST(date::get_output_format() == date::colons, output8a);
  LEDA_TEST(D2.get_date() == "04:11:1973", output8b);
  date::set_output_format(date::hyphens);
  LEDA_TEST(date::get_output_format() == date::hyphens, output9a);
  LEDA_TEST(D2.get_date() == "04-11-1973", output9b);

  // test operator>> and operator<<
  date::set_output_format("dd.mm.[yy]yy DOW\n");
  date::set_input_format("delim:\n;d.M.yy ?\n;dd.mm.[yy]yy *\n");
  D3.set_to_current_date();
  LEDA_IO_TEST(D2, D3);

  date::set_output_format("DOW:3 d.M:3.yy\n");
  date::set_input_format("* d.M:3.yy\n");
  D3.set_to_current_date();
  LEDA_IO_TEST(D2, D3);
  
  date::set_output_format("'The 'dth' day of month 'm' of the year 'yyyy.\n");
  date::set_input_format("'The 'dth' day of month 'm' of the year 'yyyy.\n");
  D3.set_to_current_date();
  LEDA_IO_TEST(D2, D3);

  date::set_output_format("yyyymmdd\n");
  date::set_input_format("yyyymmdd\n");
  D3.set_to_current_date();
  LEDA_IO_TEST(D2, D3);

  date::set_input_format(date::US_standard);
  date::set_output_format(date::US_standard);
  LEDA_TEST(err::no_new_err(), no pending err.);

  // access & update ////////////////////////////////////////////////////////

  D3.set_to_current_date();
  LEDA_TEST(D3 == today, set_to_current_date);

  LEDA_TEST(D3.set_date(22,date::Jan,1973), set_date1);
  LEDA_TEST(D3.get_day()==22 && D3.get_month()==1 && D3.get_year()==1973, 
            set_date);
  LEDA_TEST(!D3.set_date(1,date::month(0),1923), set_date2);

  LEDA_TEST(D3.set_date("11/4/1973"), set_date3);
  LEDA_TEST(D3 == D2, set_date3);
  LEDA_TEST(D3.set_date("4$11$73"), set_date4);
  LEDA_TEST(D3 == D2, set_date4);
  
  LEDA_TEST(D3.set_day(11), set_day1);
  LEDA_TEST(D3.get_day()==11, set_day2);
  LEDA_TEST(!D3.set_day(31), set_day3);
  LEDA_TEST(!D3.set_day(0), set_day4);

  LEDA_TEST(D3.get_month_name() == "November", get_month_name1);
  LEDA_TEST(D3.set_month(date::month(1)), set_month1);
  LEDA_TEST(D3.get_month_name() == "January", get_month_name2);
  LEDA_TEST(D3.get_month()==1, set_month2);
  LEDA_TEST(!D3.set_month(date::month(13)), set_month3);
  LEDA_TEST(!D3.set_month(date::month(-1)), set_month4);

  LEDA_TEST(D3.set_year(1411), set_year1);
  LEDA_TEST(D3.get_year()==1411, set_year2);
  LEDA_TEST(!D3.set_year(10000), set_year3);
  LEDA_TEST(!D3.set_year(0), set_year4);

  D3.set_date(28,date::Feb,1980);
  LEDA_TEST(D3.add_to_day(30), add_to_day1a);
  LEDA_TEST(D3 == date(29,date::Mar,1980), add_to_day1b);
  LEDA_TEST(D3.add_to_day(-58), add_to_day2a);
  LEDA_TEST(D3 == date(31,date::Jan,1980), add_to_day2b);
  LEDA_TEST(D3.add_to_month(-11), add_to_month1a);
  LEDA_TEST(D3 == date(28,date::Feb,1979), add_to_month1b);
  LEDA_TEST(D3.add_to_month(24), add_to_month2a);
  LEDA_TEST(D3 == date(28,date::Feb,1981), add_to_month2b);

  LEDA_TEST(D3.get_day_of_week() == 6, get_dow);
  LEDA_TEST(D3.get_dow_name() == "Saturday", get_dow_name);
  LEDA_TEST(D3.get_week() == 9, get_week1);
  LEDA_TEST(D3.get_day_in_year() == 59, get_day_in_year);

  {
    bool ok = true;
    int year;
    for (year=1800; year<=2200; ++year)
      if (date(1,date::Jan,year).get_week()!=test_get_weeknum_of_first_day(year) ||
          date(31,date::Dec,year).get_week()!=test_get_weeknum_of_last_day(year)) 
        ok = false;
    LEDA_TEST(ok, get_week2);

    ok = true;
    year = 1993;
    date D(1,date::Jan,year); 
    int week = test_get_weeknum_of_first_day(year);
    int last_week;
    if (week == 1) last_week = test_get_last_week(year);
    else last_week = test_get_last_week(--year);
    for (int i=0; i<530; ++i) {
      if (D.get_week() != week) ok = false;
      if (week == last_week) { 
        week = 1; last_week = test_get_last_week(++year);
      }
      else ++week;
      D += 7;
    }
    LEDA_TEST(ok, get_week3);
  }

  LEDA_TEST(err::no_new_err(), no pending err.);

  // arithmetic ops /////////////////////////////////////////////////////////

  // test arithmetic operators
  {
    bool ok = true;
    int i;
    date start(1,date::Jan,5000); // start somewhere in the middle
    date D(start);
    int offset = 0;
    for (i=0; i<1000; ++i) {
      int diff = S(-1000,1000);
      if (diff >= 0)
        D = D + diff;
      else
        D = D - (-diff);
      offset += diff;
      if (D-start != offset) ok = false;
      diff = S(-1000,1000);
      if (diff >= 0)
        D += diff;
      else
        D -= - diff;
      offset += diff;
      if (start.days_until(D) != offset) ok = false;
    }
    LEDA_TEST(ok, copy-cons op= op+ op-(both) op+= op-= days_until);

    ok = true;
    date d1(1,date::Jan,1600), d2(1,date::Jan,1600), 
         d3(1,date::Jan,2400), d4(1,date::Jan,2400); 
    for (i=0; i<400*365 + 97; ++i) {
      if ((++d1) - (d2++) != 1 ) ok = false; 
      if ((--d3) - (d4--) != -1) ok = false;
    }
    if (d1!=date(1,date::Jan,2000) || d1!=d2 || d2!=d3 || d3!=d4) ok = false;
    LEDA_TEST(ok, op++(both) op--(both));

    LEDA_TEST((start + 59)    == date(1,date::Mar,5000), arith1); 
    LEDA_TEST((start - 365)   == date(1,date::Jan,4999), arith2); 
    LEDA_TEST((start += 765)  == date(5,date::Feb,5002), arith3); 
    LEDA_TEST((start -= 1876) == date(16,date::Dec,4996), arith4); 
    LEDA_TEST(date(18,date::Apr,1998)-date(4,date::Nov,1973) == 8931, arith5);
  }

  // test the edges of the valid range
  D3.set_date(2,date::Jan,1); --D3;
  LEDA_TEST(err::no_new_err(), lower range end 1);
  --D3;
  LEDA_TEST(err::fetch() == 
    "date: date out of range in an arithmetic operation", lower range end 2);

  D3.set_date(30,date::Dec,9999); ++D3;
  LEDA_TEST(err::no_new_err(), upper range end 1);
  ++D3;
  LEDA_TEST(err::fetch() == 
    "date: date out of range in an arithmetic operation", upper range end 2);

  // test months_until and years_until
  {
    bool ok = true;
    int i;
    for (i=0; i<1000; ++i) {
      date d1 = draw_date(); date d2 = draw_date();
      int m = d1.months_until(d2);
      if (d1>d2)
        { if (m != - d2.months_until(d1) || m>0) ok = false; }
      else {
        date d1_(d1), d1__(d1);
        if (!d1_.add_to_month(m)) ok = false; 
        bool in_range = d1__.add_to_month(m+1);
        if (d1_>d2 || (d1__<=d2 && in_range) || m<0) ok = false;
      }

      int y = d1.years_until(d2);
      if (d1>d2)
        { if (y != - d2.years_until(d1) || y>0) ok = false; }
      else {
        date d1_(d1), d1__(d1);
        if (!d1_.add_to_year(y)) ok = false;
        bool in_range = d1__.add_to_year(y+1);
        if (d1_>d2 || (d1__<=d2 && in_range) || y<0) ok = false; 
      }
    }
    LEDA_TEST(ok, months_until years_until);
  }
  LEDA_TEST(date(28,date::Feb,1948).months_until(date(31,date::Dec,1948))==10, months_until1);
  LEDA_TEST(date(29,date::Feb,1948).months_until(date(28,date::Feb,1949))==12, months_until2);
  LEDA_TEST(date(28,date::Feb,1949).months_until(date(29,date::Feb,1948))==-12, months_until3);
  LEDA_TEST(date(31,date::Mar,1948).months_until(date(30,date::May,1948))==1, months_until4);
  LEDA_TEST(date(31,date::Jan,1948).months_until(date(29,date::Feb,1948))==1, months_until5);
  LEDA_TEST(date(28,date::Feb,1948).years_until(date(31,date::Dec,1948))==0, years_until1);
  LEDA_TEST(date(29,date::Feb,1948).years_until(date(28,date::Feb,1949))==1, years_until2);
  LEDA_TEST(date(28,date::Feb,1949).years_until(date(29,date::Feb,1948))==-1, years_until3);
  LEDA_TEST(date(29,date::Feb,1948).years_until(date(28,date::Feb,1952))==3, years_until4);
  LEDA_TEST(date(28,date::Feb,1951).years_until(date(31,date::Mar,1948))==-2, years_until5);

  LEDA_TEST(err::no_new_err(), no pending err.);

  // miscellaneous //////////////////////////////////////////////////////////

  LEDA_TEST(date::is_valid(23,date::Dec,1904), is_valid1);
  LEDA_TEST(date::is_valid(29,date::Feb,2000), is_valid2);
  LEDA_TEST(!date::is_valid(30,date::Feb,2000), is_valid3);
  LEDA_TEST(!date::is_valid(29,date::Feb,1999), is_valid4);
  LEDA_TEST(!date::is_valid(29,date::month(0),1999), is_valid5);
  LEDA_TEST(!date::is_valid(29,date::month(13),1999), is_valid6);
  LEDA_TEST(!date::is_valid(-1,date::Jan,1999), is_valid7);
  LEDA_TEST(!date::is_valid(32,date::Jan,1999), is_valid8);
  LEDA_TEST(!date::is_valid(1,date::Jan,0), is_valid9);
  LEDA_TEST(!date::is_valid(29,date::Dec,10000), is_valid10);
  
  LEDA_TEST(date::is_valid("31,3,1904"), is_valid_str1);
  LEDA_TEST(!date::is_valid("31,3,2000", false), is_valid_str2);
  LEDA_TEST(date::is_valid("3/31/04"), is_valid_str3);

  LEDA_TEST(date::is_leap_year(1904), leap_year1);
  LEDA_TEST(date::is_leap_year(2000), leap_year2);
  LEDA_TEST(!date::is_leap_year(1900), leap_year3);
  LEDA_TEST(!date::is_leap_year(1582), leap_year4);

  {
    bool ok = true;
    D3.set_date(1,date::Jan,1904); // leap year !
    for (int i=0; i<366+365; ++i) {
      D3++;
      if (D3.is_last_day_in_month() != test_is_last_day_in_month(D3))
        ok = false;
    }
    LEDA_TEST(D3 == date(1,date::Jan,1906),last_day_in_mth (arithm.)); 
    LEDA_TEST(ok, last_day_in_mth);
  }

  LEDA_TEST(err::no_new_err(), no pending err.);

  // compatibility with LEDA ////////////////////////////////////////////////

  // compatibility with list
  {
    bool ok = true;
    list<date> L;
    date D(1,date::Jan,1998);
    int i;
    for(i=0; i<100; ++i)
      L.append(D--);
    if (L.size() != 100) ok = false;
    L.sort();
    date D_;
    forall(D_, L)
      if (D_ != ++D) ok = false;
    while (! L.empty())
      if (L.Pop() != D--) ok = false;
    LEDA_TEST(ok, compatibility with list);
  }

  // compatibility with dictionary
  {
    bool ok = true;
    dictionary<date, int> dic;
    int i; date D;
    for(i=0; i<100; ++i) {
      do { D=draw_date(); } while (dic.lookup(D) != nil);
      dic.insert(D, test_hash(D));
    }
    if (dic.size() != 100) ok = false;
    dic_item it;
    forall_items(it, dic) {
      D = dic.key(it);
      if (dic[it] != test_hash(D)) ok = false;
    }
    LEDA_TEST(ok, compatibility with dictionary);
  }

  // compatibility with h_array
  {
    bool ok = true;
    h_array<date, int> h;
    int i; date D;
    for(i=0; i<100; ++i) {
      do { D = draw_date(); } while (h.defined(D));
      h[D] = test_hash(D);
    }
    forall_defined(D, h)
      if (h[D] != test_hash(D)) ok = false;
    LEDA_TEST(ok, compatibility with h_array);
  }

  LEDA_TEST(err::no_new_err(), no pending err.);

  // That's it ! ////////////////////////////////////////////////////////////

  LEDA_TEST_END;

  if (leda_test_success)
    return 0;
  else
    return 1;
}

