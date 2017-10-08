/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  param_handler.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef PARAM_HANDLER_H
#define PARAM_HANDLER_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600235
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/core/d_array.h>
#include <LEDA/core/list.h>

LEDA_BEGIN_NAMESPACE


class __exportC param {
public: 

  LEDA_MEMORY(param);

  string Label;
  string Flag;
  string Type;
  string Value;
  int    Offset;
  list<string> Enums;
 
  param() {}
  param(string input);
  ~param() {}

  friend __exportF ostream& operator<<(ostream& O, const param& p);
  friend __exportF istream& operator>>(istream& I, param& p);

};


/*{\Manpage {param_handler} {} {Clever parameter handling} 
            {P}}*/
/*{\Mtext \label{Param_Handler}}*/

class __exportC param_handler{
  /*{\Mdefinition An instance of data type |\Mname| allows to maintain
                  the configuration of an executable uniformly. It
                  stores a list of configuration variables of the
                  types |bool|, |int|, |double|, |string| and |enum|
                  which can be accessed via a long mnemonic label or a
                  short flag. The actual list of configuration
                  variables is set by the operation
                  |add_parameter()|. If there exists a parameter file
                  in the execution directory of the program using
                  |\Mname| then the initialization values for |\Mvar|
                  can be taken from the file instead of the list filled
                  by |add_parameter()|.}*/

public:
  /*{\Mcreation 4}*/

  param_handler(const char* paramfile=".last", bool initialRead = true);
  /*{\Mcreate introduces an unitialized parameter handler |\Mvar| of
              type |\Mname|. |paramfile| is the default file where the
              configuration information is stored at the end of
              program execution or by a call to
              |write_parameter_file()|.  If |initialRead| is true the
              object is also initialized from the default file, if it
              exists.}*/

  param_handler(int argc, char** argv, const char* paramfile=".last", 
                    bool initialRead = true);
  /*{\Mcreate introduces an unitialized parameter handler |\Mvar| of
              type |\Mname|. |paramfile| is the default file where the
              configuration information is stored at the end of
              program execution or by a call to
              |write_parameter_file()|.
              If |initialRead| is true the object is also initialized
              from the default file, if it exists.}*/


  ~param_handler();


  /*{\Moperations 2 3}*/

  void add_parameter(string param);
  /*{\Mop incrementally extends |\Mvar| by adding a new parameter to
          it.  The string |param| must have the form "Label:Flag:Type:Value"
          where |Label| is a mnemonic name for the parameter, |Flag|
          is an abbreviation for it, |Type| is one of 
          |\{bool, double, int, string,enum(...)\}| and |Value| is 
          the initialization value of the parameter.\\
          \precond |\Mvar.init_all()| has not been called.}*/

  static void init_all();
  /*{\Mstatic initializes the expected parameter setting of the
              application.  Afterwards the list of parameters added by
              |add_parameter| is fixed for all existing objects of type
              |\Mname| and cannot be changed anymore. 
              This function also triggers the parsing of the command line
              parameters; when the flag |-h| is encountered, a help
              message is displayed for all |\Mname|s and the program is
              terminated.}*/

  void  get_parameter(const string& label, double& value) const
  { value = get_double(label); } 
  /*{\Mop sets |val| to the variable referenced by |label| which can
          be |Label| or |Flag| of the definitions in
          |add_parameter|. This operation exists also for |bool&|,
          |int&| and |string&|. The |int&| operation can also be used
          for the |enum| type. \\
          \precond |\Mvar.init_all()| has been called.}*/

  void  get_parameter(const string& label, bool& value) const
  { value = get_bool(label); } 

  void  get_parameter(const string& label, int& value) const
  {
    int pt = Type[label];
    int i = Index[label];
    value = (pt==INT) ?  get_int(label) : (get_enum(label)+POffset[i]);
  }

  void  get_parameter(const string& label, string& value) const
  { value = get_string(label); } 

  void set_parameter(const string& label, double value);
  /*{\Mop sets a the variable referenced by |label| which can be
          |Label| or |Flag| of the definitions in |add_parameter| to
          the value |val|. This operation exists also for |bool|, and
          |string|. \\
          \precond |\Mvar.init_all()| has been called.}*/

  void set_parameter(const string& label, bool   value);
  void set_parameter(const string& label, int    value);
  void set_parameter(const string& label, string value);
  void set_parameter(const string& label, char* value)
  { set_parameter(label, string(value)); }

  void print_help() const;
  /*{\Mop prints a formated list of all parameters to the standard
          output. \\  
          \precond |\Mvar.init_all()| has been called.}*/

  void write_parameter_file() const;
  /*{\Mop All configuration information is written to the parameter
          file set by the constructor or the operation
          |set_parameter_file|. \\
          \precond |\Mvar.init_all()| has been called.}*/

  void write_parameter_file(const string& paramfile) const;
  /*{\Mop All configuration information is stored in file
          |paramfile|.}*/

  void set_parameter_file(string paramfile) 
  { File = paramfile; }
  /*{\Mop All configuration information is stored in the parameter
          file |paramfile|.}*/

  friend __exportF istream& operator>>(istream& from, param_handler& P);
  friend __exportF ostream& operator<<(ostream& to, const param_handler& P);

private:
  static list<param_handler*> Objects;
  static leda_lock Obj_mutex;

  enum paramtype { UNDEF, BOOL, DOUBLE, INT, STRING , ENUM };

  //d_array<string,paramtype> Type;

  d_array<string,int> Type;

  d_array<string,int>       Index;
  list<param> ParamList;

  bool initialized;

  bool*   PBool;
  int*    PInt;
  double* PDouble;
  string* PString;
  int*    PEnum;
  int*    POffset;

  int     AnzBool;
  int     AnzDouble;
  int     AnzInt;
  int     AnzString;
  int     AnzEnum;

  int     Argc;
  char**  Argv;
  string  File;
  bool    FileInit;


  bool&   get_bool(const string& label) const;
  int&    get_enum(const string& label) const;
  int&    get_int(const string& label) const;
  double& get_double(const string& label) const;
  string& get_string(const string& label) const;

  bool   is_bool(const string& s) 
    { return ( s == "true" || s == "false"); }
  bool   is_flag(const string& s) { return (Index[s] != -1); }
  int    has_flag_prefix(const string& s) { 
    for (int i=1; i< s.length(); i++)
      if (is_flag(s.head(i))) return i;
    return 0;
  }
  int atoe(param& p);

  string argument(int i) 
    { if( Argc > i ) return Argv[i]; else return "Gibts nicht"; }

  void toggle(bool& b) { b = !b; }

  void init(bool&);
  static void print_help_all();

  friend class param_panel;
};

/*{\Mimplementation |\Mname| is implemented by an internal hashing
    scheme which maps labeled configuration variables to internal
    storage places. The access operations take constant time.}*/

/*{\Mexample Consider a program \textit{foo.c} with the following content.
\begin{verbatim}
  main(int argc, char* argv[])
  {
    enum {Morning=10,Noon,Evening};
    param_handler P(argc,argv,".myparams");
    P.add_parameter("Input File:-f:string:I-File");
    P.add_parameter("Rounds:-n:int:10");
    P.add_parameter("Start Value:-s:double:0.1");
    P.add_parameter("Read from file:-r:bool:false");
    P.add_parameter("Daytime:-dt:enum(Morning=10,Noon,Evening):Morning");
    param_handler::init_all();
    int rounds;
    P.get_parameter("-n",rounds);
    for (int i = 0; i < rounds; i++)
      cout << "hello world\n";
  }
\end{verbatim}

After compiling \textit{foo.c} into \textit{foo*} you can use the
program now in different modes. \texttt{foo} will just iterate
10 times. \texttt{foo -n 100} will iterate 100 times and store
the 100 in \texttt{.myparams} on exit. Another call of foo
will iterate 100 times stemming from the entry in \texttt{.myparams}.
A call of \texttt{foo -h} will display all possible flags of the
program including the init values.}*/


#if LEDA_ROOT_INCL_ID == 600235
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif


