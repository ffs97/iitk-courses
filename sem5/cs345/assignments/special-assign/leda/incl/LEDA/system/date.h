/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  date.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_DATE_H
#define LEDA_DATE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600239
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/system/basic.h>
#include <LEDA/core/array.h>
#include <LEDA/core/slist.h>
#include <LEDA/system/error.h>

LEDA_BEGIN_NAMESPACE

class __exportC date {
public:
  enum { reference_year = 1950 };

  /*{\Manpage {date} {} {A date interface} {D}}*/
  /*{\Mdefinition
    An instance of the data type |\Mname| represents a date consisting of a day
    |d|, a month |m| and year |y|. It will be denoted by |d.m.y|.
    Valid dates range from |1.1.1| to |31.12.9999|. A date is |valid| if it lies
    in the range and is correct according to the gregorian calendar, i.e.
    a year |y| is considered to be a leap year iff |y| is divisible by 4 but not 
    by 100 or |y| is divisible by 400.\label{date_def_validity}
    The year part |y| is always a four digit number, so that each date in the
    valid range has an unambiguous representation.\\
    With the |\Mname| class there is associated an input and an output format, 
    each is described by a string which determines how instances of type |\Mname|
    are read from streams and how they are printed to streams. 
    Printing the date |4.11.1973| using the format string "dd.mm.yy" will result 
    in "04.11.73", whereas printing the same date using "mm/dd/yyyy" will 
    produce "11/04/1973".
    The |\Mname| type provides some predefined formats, it also allows 
    user-defined formats and supports different languages (for month names and
    weekday names).\\
    A format string consists of tokens, not all tokens are valid for both input
    and output formats. But any sequence of valid tokens forms a valid format
    string, the only exception to this rule is the |delim| token (see the table
    below). In order to avoid ambiguities when parsing a format string the 
    longest prefix rule is applied, which ensures that |dd| is parsed as a 
    single token and not as twice the token |d|.\\
    An input format does not have to refer to all the three parts (day, month
    and year) of a date; the parts which do not appear in the format are left
    unchanged when the format is used in an update operation.
    Applying the format "d.m.", for example, changes the day and the month 
    part but not the year part.
    (The result of using input formats referring twice to the same part as
    in "m M" is undefined.)
    Please see table \ref{Token Overview} for an overview of all possible tokens.
\begin{table}[h!]
    \caption{Token Overview}
    \label{Token Overview}
    \newlength{\rowhere}\setlength{\rowhere}{10cm}
    \begin{tabular}[t]{|l|c|c|l|}
     \hline
     token & input & output & description \\ \hline
     d  & yes & yes & day with 1 or 2 digits \\
     dd & yes & yes & day with 2 digits (possibly with leading zero)\\
     dth& yes & yes & 
     \parbox[t]{\rowhere}{
     day as abbreviated english ordinal number (1st, 2nd, 3rd, 4th, \ldots)} \\
     m  & yes & yes & month with 1 or 2 digits \\
     mm & yes & yes & month with 2 digits (possibly with leading zero)\\
     M  & yes & yes & 
     \parbox[t]{\rowhere}{
     month name (when used in an input format this token must be followed
     by a single char |c| which does not belong to any month name, |c| is used
     to determine the end of the name. e.g.: "d.\textbf{M.}yy")}\\
     M:|l|& yes & yes & 
     \parbox[t]{\rowhere}{
     the first |l| characters of the month name (|l| must be a single digit)}\\
     yy     & yes & yes & 
     \parbox[t]{\rowhere}{
     year with 2 digits (yy is considered to represent a year in [1950;2049])}\\
     yyyy   & yes & yes & year with 4 digits\\
     $[$yy$]$yy & yes & yes & 
     \parbox[t]{\rowhere}{
     input: year with 2 or 4 digits / output: same as yyyy}\\
     w      & no & yes & 
     \parbox[t]{\rowhere}{
     calendar week (in the range $[$1;53$]$) (see |get_week()| for details)}\\
     diy    & no & yes & day in the year (in the range $[$1,366$]$)\\
     dow    & no & yes & day of the week (1=Monday, \ldots, 7=Sunday)\\
     DOW    & no & yes & name of the weekday\\
     DOW:|l|& no & yes & 
     \parbox[t]{\rowhere}{
     the first |l| characters of the weekday name (|l| must be a single digit)}\\
     "|txt|" & yes & yes & 
     \parbox[t]{\rowhere}{
     matches/prints |txt| (|txt| must not contain a double quote)}\\
     '|txt|' & yes & yes & 
     \parbox[t]{\rowhere}{
     matches/prints |txt| (|txt| must not contain a single quote)}\\
     |c| & yes & yes & matches/prints |c| ($c \notin \{d, m, M, ?, \ast$, ;\})\\
     ?   & yes & no  & matches a single arbitrary character\\
     $\ast$|c| & yes & no & matches any sequence of characters ending with |c|\\
     ; & yes & yes & separates different formats, e.g. "d.M.yy;dd.mm.yy"\\
       &     &     & input: the first format that matches the input is used\\
       &     &     & output: all but the first format is ignored\\
     delim:|c| & yes & no & 
     \parbox[t]{\rowhere}{
     |c| serves as delimiter when reading input from streams
     (If this token is used, it must be the first in the format string.)
     When you use "delim:$\backslash$n;d.M.yy$\backslash$n;d.m.yyyy$\backslash$n" 
     as input format to read a date from a stream, everything until the first 
     occurence of "$\backslash$n" is read and then the format 
     "d.M.yy$\backslash$n;d.m.yyyy$\backslash$n" is applied.}\\
     \hline
    \end{tabular}
\end{table}
  }*/

  /*{\Mtypes}*/ 

    enum month { Jan=1,Feb,Mar,Apr,May,Jun,Jul,Aug,Sep,Oct,Nov,Dec }; 
    /*{\Menum The enumeration above allows to specify months by their name.
        Of course, one can also specify months by their number writing
        [[date::month(m)]].}*/

    enum language { user_def_lang, local, english, german, french };
    /*{\Menum When the language is set to |local|, the month names and weekday
       names are read from the local environment; the other identifiers are
       self-explanatory.}*/

    enum format { user_def_fmt, US_standard, german_standard, colons, hyphens };
    /*{\Menum The format |US_standard| is an abbreviation for |mm/dd/[yy]yy|, 
       the format |german_standard| is the same as |dd.mm.[yy]yy|, the other 
       formats are the same as the latter except that the periods are replaced 
       by colons/hyphens.}*/

    enum fmt_token 
    { _delim , _diy , _dow , _DOWc , _DOW , _dth , _dd , _d , _mm , _m , _Mc , _M ,
     _week, _yyyy , _yy , _yy_or_yyyy, _skip, _skip1, _text, _sep, 
     _next_fmt = -1, _bad_fmt = -2, _empty_txt = -3 };


  /*{\Mcreation}*/ 

    date() { set_to_current_date(); }
    /*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it 
       to the current date.}*/

    date(int d, month m, int y);
    /*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it 
       to |d.m.y|. \\
       \precond |d.m.y| represents a valid date.}*/

    date(string date_str, bool swallow = true);
    /*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it 
       to date given in |date_str|.\\
       If |swallow| is |true|, then the format |"m/d/[yy]yy;d?m?[yy]yy"|
       is used to parse |date_str|, otherwise the current input format is 
       applied.\\
       \precond |date_str| represents a valid date.}*/

    date(const date& d) : ymd(d.ymd) {}

    date& operator=(const date& d) { ymd = d.ymd; return *this; }

  /*{\Moperations}*/

  /*{\Mtext
  \headerline{4.1 Languages and Input/Output Formats}
  }*/

    static void set_language(language l);
    /*{\Mstatic sets the language to |l|, which means that the month names
       and the weekday names are set according to the language. \\
       \precond $|l| \ne |user_def_lang|$}*/

    static void set_month_names(const char* names[]);
    /*{\Mstatic sets the names for the months and changes the language to
       |user_def_lang|. \\
       \precond |names[0..11]| contains the names for the months from January to
       December.}*/

    static void set_dow_names(const char* names[]);
    /*{\Mstatic sets the names for the weekdays and changes the language to 
       |user_def_lang|. \\
       \precond |names[0..6]| contains the names for the weekdays from Monday to
       Sunday.}*/

    static language get_language();
    /*{\Mstatic returns the current language.}*/

    static void set_input_format(format f);
    /*{\Mstatic sets the input format to |f|. \\
       \precond $|f| \ne |user_def_fmt|$}*/

    static void set_input_format(string f);
    /*{\Mstatic sets the input format to the user-defined format in |f|. \\
       \precond |f| is a valid format string}*/

    static format get_input_format();
    /*{\Mstatic returns the current input format.}*/

    static string get_input_format_str();
    /*{\Mstatic returns the current input format string.}*/

    static void set_output_format(format f);
    /*{\Mstatic sets the output format to |f|. \\
       \precond $|f| \ne |user_def_fmt|$}*/

    static void set_output_format(string f);
    /*{\Mstatic sets the output format to the user-defined format in |f|. \\
       \precond |f| is a valid format string}*/

    static format get_output_format();
    /*{\Mstatic returns the current output format.}*/

    static string get_output_format_str();
    /*{\Mstatic returns the current output format string.}*/

  /*{\Mtext
  \headerline{4.2 Access and Update Operations}
  }*/

  /*{\Mtext
  All update operations which may fail have in common that the date is changed
  and |true| is returned if the new date is valid, otherwise |false| is returned
  and the date is left unchanged. 
  (Note that the functions |add_to_day|, |add_to_month| and |add_to_year| can 
  only fail if the valid range (1.1.1 -- 31.12.9999) is exceeded.)
  \medskip
  }*/

    void set_to_current_date();
    /*{\Mop sets |\Mvar| to the current date.}*/

    bool set_date(int d, month m, int y);
    /*{\Mop |\Mvar| is set to |d.m.y| (if |d.m.y| is valid).}*/

    bool set_date(const string date_str, bool swallow = true);
    /*{\Mop |\Mvar| is set to the date contained in |date_str|.
       If |swallow| is |true|, then the format |"m/d/[yy]yy;d?m?[yy]yy"|
       is used to parse |date_str|, otherwise the current input format is 
       applied.
    }*/

    string get_date() const;
    /*{\Mop returns a string representation of |\Mvar| in the current output
       format.}*/

    int get_day() const { return ymd&31; }
    /*{\Mop returns the day part of |\Mvar|, i.e. if |\Mvar| is |d.m.y| then
       |d| is returned.}*/

    month get_month() const { return month((ymd>>5)&15); }
    /*{\Mop returns the month part of |\Mvar|.}*/

    string get_month_name() const { return month_names[get_month()]; }
    /*{\Mop returns the name of the month of |\Mvar| in the current language.}*/

    int get_year() const { return ymd>>9; }
    /*{\Mop returns the year part of |\Mvar|.}*/


    bool set_day(int d) { return set_date(d, get_month(), get_year()); }
    /*{\Mop sets the day part of |\Mvar| to |d|, i.e. if |\Mvar| is |d'.m.y| 
       then |\Mvar| is set to |d.m.y|.}*/

    bool add_to_day(int d) { return update_date(days_since_zero() + d); }
    /*{\Mop adds |d| days to |\Mvar| (cf. arithmetic operations).}*/

    bool set_month(month m) { return set_date(get_day(), m, get_year()); }
    /*{\Mop sets the month part of |\Mvar| to |m|.}*/

    bool add_to_month(int m);
    /*{\Mop adds |m| months to the month part of |\Mvar|.\\
       Let |\Mvar| be |d.m'.y|, then it is set to |d.(m'+m).y|. 
       If this produces an overflow (i.e. $m'+m > 12$) then the month part is 
       repeatedly decremented by 12 and the year part is simultaneously 
       incremented by 1, until the month part is valid. 
       (An underflow (i.e. $m'+m < 1$) is treated analogously.)
       The day part of the result is set to the minimum of |d| and the number of
       days in the resulting month.
    }*/

    bool set_year(int y) { return set_date(get_day(), get_month(), y); }
    /*{\Mop sets the year part of |\Mvar| to |y|.}*/

    bool add_to_year(int y);
    /*{\Mop adds |y| years to the year part of |\Mvar|.\\
       (If |\Mvar| has the form |29.2.y'| and |y'+y| is no leap year, then 
       |\Mvar| is set to |28.2.(y'+y)|.)}*/

    int get_day_of_week() const { return 1 + days_since_zero() % 7; }
    /*{\Mop returns the day of the week of |\Mvar|.\\
       (1=Monday, 2=Tuesday, \ldots, 7=Sunday)}*/

    string get_dow_name() const { return dow_names[get_day_of_week()]; }
   /*{\Mop returns the name of the weekday of |\Mvar| in the current language.}*/

    int get_week() const;
    /*{\Mop returns the number of the calendar week of |\Mvar| 
       (range $[$1,53$]$).\\
       A week always ends with a Sunday. Every week belongs to the year which
       covers most of its days. (If the first Sunday of a year occurs before the
       fourth day of the year, then all days up to this Sunday belong to the 
       last week of the preceding year. Similarly, if there are less than 4 days
       left after the last Sunday of a year, then these days belong to the first
       week of the succeding year.)}*/

    int get_day_in_year() const 
    { return days_in_months(get_month()-1, get_year()) + get_day(); }
    /*{\Mop returns the number of the day in the year of |\Mvar| (range
       $[$1;366$]$).}*/

  /*{\Mtext
  \headerline{4.3 Arithmetic Operations}
  }*/

    date& operator+=(int d);
    date& operator-=(int d) { return *this += -d; }
    
    date& operator++() { return *this += 1; }
    date& operator--() { return *this -= 1; }
    date operator++(int) { date tmp = *this; ++(*this); return tmp; }
    date operator--(int) { date tmp = *this; --(*this); return tmp; }

    date operator+(int d) const { return date(*this) += d; }
    /*{\Mbinop returns the date |d| days after |\Mvar|.}*/
    date operator-(int d) const { return date(*this) -= d; }
    /*{\Mbinop returns the date |d| days before |\Mvar|.}*/
    /*{\Mtext The related operators |++|, |--|, |+=|, |-=| and all comparison
       operators are also provided. }*/

    int operator-(const date& D2) const 
    { return days_since_zero() - D2.days_since_zero(); }
    /*{\Mbinop returns the difference between |\Mvar| and |D2| in days.}*/

    int days_until(const date& D2) const { return D2 - *this; }
    /*{\Mop returns |D2 - ||\Mvar|. }*/

    int months_until(const date& D2) const;
    /*{\Mop if |D2| $\geq$ |\Mvar| then $\max\{m:|\Mvar|.|add_to_month(m)| ~
       \leq |D2|\}$ is returned; otherwise the result is 
       |- D2.months_until(||\Mvar||)|.
    }*/

    int years_until(const date& D2) const;
    /*{\Mop if |D2| $\geq$ |\Mvar| then $\max\{y:|\Mvar|.|add_to_year(y)| ~
       \leq |D2|\}$ is returned; otherwise the result is 
       |- D2.years_until(||\Mvar||)|.
    }*/

  /*{\Mtext
  \headerline{4.4 Miscellaneous Predicates}
  }*/

    static bool is_valid(int d, month m, int y);
    /*{\Mstatic returns |true| iff |d.m.y| represents a valid date.}*/

    static bool is_valid(string d, bool swallow=true);
    /*{\Mstatic returns |true| iff |d| represents a valid date. If |swallow| is
       |true| the swallow format (cf. |set_date|) is used, otherwise the 
       current input format is tried.}*/

    inline static bool is_leap_year(int y);
    /*{\Mstatic returns |true| iff |y| is a leap year.}*/
   
    bool is_last_day_in_month() const;
    /*{\Mop let |\Mvar| be |d.m.y|; the function return |true| iff |d| is the 
       last day in the month |m| of the year |y|.}*/

    // comparison
    static int cmp(const date& d1, const date& d2);

    // get ymd representation
    int get_ymd() const { return ymd; }

    // substitute for operator>> (returns whether the input succeeded)
    bool read(istream& i);

  /*{\Mexample
  We count the number of Sundays in the days from now to 1.1.2010 using the
  following code chunk:\\[-5ex]
  \begin{verbatim}
    int number_of_Sundays = 0;
    for (date D; D<=date(1,date::Jan,2010); ++D)
      if (D.get_day_of_week() == 7) ++number_of_Sundays;
  \end{verbatim}

  Now we show an example in which different output formats are used:\\[-5ex]
  \begin{verbatim}
    date D(2,date::month(11),1973);
    date::set_output_format(date::german_standard);
    cout << D << endl; // prints "02.11.1973"
    date::set_language(date::english);
    date::set_output_format("dth M yyyy");
    cout << D << endl; // prints "2nd November 1973"
  \end{verbatim}

  Finally, we give an example for the usage of a multi-format.
  One can choose among 3 different formats:\\[-5ex]
  \begin{enumerate}
  \item If one enters only day and month, then the year part is set to the 
        current year.
  \item If one enters day, month and year providing only 2 digits for the year,
        the year is considered to be in the range [1950, 2049].
        (Note that the date |1.1.10| must be written as "1.1.0010".)
  \item One may also specify the date in full detail by entering 4 digits
        for the year.
  \end{enumerate}
  The code to read the date in one of the formats described above looks like 
  this:\\[-5ex]
  \begin{verbatim}
    D.set_to_current_date(); // set year part to current year
    date::set_input_format("delim:\n;d.m.\n;d.m.[yy]yy\n");
    cin >> D; cout << D << endl;
  \end{verbatim}
  }*/

private:
  int ymd; // the representation of the date

    // language data
    static leda_lock mutex_language;
    static language _language;

#if defined(__BORLANDC__)
    static array<string> month_names;
    static array<string> dow_names;
#else
    static string month_names[13];
    static string dow_names[8];
#endif

    // input format
    static leda_lock mutex_i_fmt;
    static format i_fmt;
    static string i_fmt_str;
    static slist<int> i_fmt_tokens;

    // output format
    static leda_lock mutex_o_fmt;
    static format o_fmt;
    static string o_fmt_str;
    static slist<int> o_fmt_tokens;

    // swallow format
    static string swallow_fmt_str;
    static slist<int> swallow_tokens;

  /* access and update related functions */
    bool update_date(const string, const slist<int>&, const string&);
    bool update_date(int dsz);
    
    int days_since_zero() const; // days elapsed since 1.1.1

    static int days_in_month(int m, int y); 
    static int days_in_months(int m, int y); 
    static int days_in_year(int y); 
    static int days_in_years(int full_years); 

    static bool is_valid(int dsz);

    static int get_month_by_name(string month_name);

  /* input and output related functions */

    // language
    static void get_local_names();
    static void copy_month_names(const char* names[]);
    static void copy_dow_names(const char* names[]);

    // input and output
    static fmt_token get_token(string f, int& pos, int& i1, int& i2);
    static bool parse_input_format(const string&, slist<int>&);
    static bool parse_output_format(const string&, slist<int>&);

    // i/o support functions
    static int extract_number(const string&, int&, int&, int);
    static string ordinal_number(int n);
    static void comp_yyyy_from_yy(int& y);


public:
  LEDA_MEMORY(date)

  // the stream operators
  friend istream& operator>>(istream& i, date& d)
  { if(!d.read(i)) LEDA_EXCEPTION(1,"date: bad input from stream"); return i; } 
  friend ostream& operator<<(ostream& o, const date& d)
  { return o << d.get_date(); } 

  friend class __exportC date_init; // needed to initialize the static members
};


inline bool date::is_leap_year(int y)
{ return (y%4 == 0) && (y%100 != 0 || y%400 == 0); }

inline const char* leda_tname(const date*) { return "date"; }

inline int Hash(const date& d) { return int(d.get_ymd()); }

inline int date::cmp(const date& d1, const date& d2) 
{ return int(d1.ymd - d2.ymd); }

inline int compare(const date& d1, const date& d2)
{ return date::cmp(d1, d2); }

inline bool operator == (const date& d1, const date& d2) 
{ return date::cmp(d1, d2) == 0; }
inline bool operator != (const date& d1, const date& d2)
{ return date::cmp(d1, d2) != 0; }
inline bool operator <= (const date& d1, const date& d2)
{ return date::cmp(d1, d2) <= 0; }
inline bool operator >= (const date& d1, const date& d2)
{ return date::cmp(d1, d2) >= 0; }
inline bool operator <  (const date& d1, const date& d2)
{ return date::cmp(d1, d2) < 0; }
inline bool operator >  (const date& d1, const date& d2)
{ return date::cmp(d1, d2) > 0; }

#if LEDA_ROOT_INCL_ID == 600239
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
