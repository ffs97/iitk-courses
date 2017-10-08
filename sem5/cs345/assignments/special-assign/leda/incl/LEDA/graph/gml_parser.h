/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  gml_parser.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_GML_PARSER_H
#define LEDA_GML_PARSER_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 350202
#include <LEDA/internal/PREAMBLE.h>
#endif

//------------------------------------------------------------------- //
// class gml_parser, extendible parser for the GML format             //
//                                                                    //
// by David Alberts (1997)                                            //
//------------------------------------------------------------------- //

#include<LEDA/system/basic.h>
#include<LEDA/core/dictionary.h>
#include<LEDA/core/sortseq.h>
#include<LEDA/core/slist.h>
#include<LEDA/core/stack.h>

LEDA_BEGIN_NAMESPACE

typedef char* gml_charp;

#if defined(__WATCOMC__)
inline  int compare(const gml_charp& x, const gml_charp& y)
{ return string::cmp(x,y); }
#endif


// possible types of values in GML input
enum gml_value_type { gml_int=0, gml_double=1,
                      gml_string=2, gml_list=3,
                      gml_none=4 };

class gml_objecttree;
class gml_parser;


class gml_object
{
  public:

    gml_object() : where(0), key_pos(-1), the_key(0), parent(0), 
                   the_type(gml_none) {}
    ~gml_object();

    const char*              get_key()      const { return the_key; }

    gml_value_type           get_type()     const { return the_type; }
    int                      get_int()      const { return the_int; }
	double                   get_double()   const { return the_type == gml_double ? the_double : double(get_int()); }
    char*                    get_string()   const { return the_string; }

    const gml_object*        get_parent()   const { return parent; }

    bool                     has_children() const { return !the_list.empty(); }

  private:

    typedef slist<gml_object*>::item slist_item;

    union
    {
      int              the_int;
      double           the_double;
      char*            the_string;
    };

    slist<gml_object*> the_list;
    slist_item         where;

    int                key_pos;
    const char*        the_key;

    gml_object*        parent;

    gml_value_type     the_type;

  public:

    friend class gml_objecttree;

    friend ostream& operator<<(ostream&, const gml_object&);

    LEDA_MEMORY(gml_object)
};


class gml_objecttree
{
  typedef slist<gml_object*>::item slist_item;

  public:

    // ----- initialization ----- //
    gml_objecttree()
      : in_chars(0), syntax_errors(false), root_key("ROOT") {}
    gml_objecttree(istream& ins)
      : in_chars(0), syntax_errors(false), root_key("istream") { read(ins); }
    gml_objecttree(char* filename)
      : in_chars(0), syntax_errors(false), root_key(filename)
      { read(filename); }

    ~gml_objecttree() { delete[] in_chars; }

    // ----- building the tree ----- //
    bool read(istream& ins );
      // reads from ins.
    bool read(const char* filename);
      // reads from filename.
    bool read_string(string s);
      // reads from s.
    bool errors() { return syntax_errors; }
      // last parse successful?

    // ----- navigating in the tree ----- //
    gml_object& get_objects() { return objects; }

    gml_object* first_object();
    gml_object* next_object();
    gml_object* down();
    gml_object* up();
    gml_object* parent();
    bool        last_in_list();

    void line_col(int position, int& line, int& col);
      // computes line and column from position in input string.

    void line_col(const gml_object& gobj, int& line, int& col)
      { line_col(gobj.key_pos,line,col); }
      // computes line and column for gobj.


    const char* get_root_key() { return root_key.cstring(); }

  private:

    void print_error(const char* type);
      // prints an error message for an error of type
      // type and a comment to cerr

    bool check_brackets();
      // returns false, if impaired brackets in in_chars are detected.
    void init_lines();
    void init_tree();

    bool build();
      // builds the tree.
    bool read_key();
      // read next key word into current_key
    void read_value();
      // read next value into current_value

    char current_char() { return in_chars[pos]; }
      // returns the current character
    void advance(int n=1);
      // advances the current position by n characters
    void skip_white();
      // skips white space.
    int next_white();
      // returns position of next white space character in in_chars or -1
    int next_quot();
      // returns position of next " or -1
    int closing_bracket();
      // returns the position of a closing bracket for the current level or -1


    char*               in_chars;        // the input as a string
    int                 in_length;       // length of the input
    int                 pos;             // current position in in_chars
    int                 bracket_level;   // # open [
    sortseq<int,int>    lines;           // aux ds for computing line and
    dictionary<int,int> line_starts;     // column from position in input

    bool                syntax_errors;

    gml_object          objects;         // the object tree
    gml_object*         current_object;
    gml_object*         last_object;
    gml_object*         current_context;

    string              root_key;        // key of root node of object tree
};


typedef int gml_rule;


inline  int cmp_charp(const gml_charp& x, const gml_charp& y)
{ return string::cmp(x,y); }


class gml_pattern
{
  public:

    gml_pattern() : the_key(0),the_type(gml_none),parent(0),children(cmp_charp) {}
    virtual ~gml_pattern();

    gml_value_type get_type() const { return the_type; }
    const char*    get_key()  const { return the_key; }

  protected:

    void add_rule(gml_rule r, gml_value_type t)
      { rules[t].append(r); }

  private:

    const char*                         the_key;
    gml_value_type                      the_type;

    slist<gml_rule>                     rules[4];

    gml_pattern*                        parent;
    dictionary<gml_charp,gml_pattern*>  children;

  public:

    friend class gml_parser;

    LEDA_MEMORY(gml_pattern)
};


class gml_parser
{
  typedef slist<gml_object*>::item slist_item;

  public:

    LEDA_MEMORY(gml_parser)

    void reset_path() { current_pattern = &patterns; }

    gml_parser() : parse_error(false), T(0) { reset_path(); }
    gml_parser(char* filename) : parse_error(false), T(0)
      { parse(filename); }
    gml_parser(istream& ins) : parse_error(false), T(0) 
      { parse(ins); }
    gml_parser(gml_objecttree& obj) : parse_error(false), T(0)
      { parse(obj); }

    virtual ~gml_parser() {}

    void append(const char* key);
      // go to the pattern with key word key in the current context,
      // create it, if not already present

    void goback();
      // remove last key word from current context, this has no effect
      // at the root pattern

    void make_context(const slist<char*>& cont);

    const char* current_key() const
      { return (current_pattern ? current_pattern->get_key() : 0); }

    bool parse(const char* filename);
    bool parse(istream& ins);
    bool parse_string(string in_str);
    bool parse(gml_objecttree& obj);

    bool errors() { return parse_error; }

  protected:

    void add_rule(const slist<char*>& cont, gml_rule r, gml_value_type t);
    void add_rule(gml_rule r, gml_value_type t)
      { current_pattern->add_rule(r,t); }

    virtual void init_rules() {}
      // this is called before parsing.
    virtual void cleanup() {}
      // this is called after parsing.

    virtual bool interpret(gml_rule, const gml_object*)
      { return true; }
      // interpret an object.
    virtual bool list_end(gml_rule, const gml_object*)
      { return true; }
      // is called when the list of rule r is done.

    void print_error(const gml_object& gobj, const char* type);


  private:

    bool parse();
    void parse_object();
    void list_end();

    bool parse_error;

    gml_pattern  patterns;        // the pattern tree
    gml_objecttree* T;            // the object tree

    gml_object*  current_object;  // current node in the object tree
    gml_pattern* current_pattern; // current pattern in pattern tree

};

LEDA_END_NAMESPACE

#if LEDA_ROOT_INCL_ID == 350202
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

#endif
