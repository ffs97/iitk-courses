/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  part_pers.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_PART_PERS_H
#define LEDA_PART_PERS_H

// sn  02/02/2001: some small changes for AIX xlC

#if defined(__xlC__)
#define __const_cast(T) (T)
#else
#define __const_cast(T) const_cast<T>
#endif


#include <LEDA/system/basic.h>
#include <LEDA/core/array.h>

LEDA_BEGIN_NAMESPACE

typedef unsigned TimeStamp;
typedef unsigned RefVector;

typedef int FieldIndex;
typedef FieldIndex PtrIndex;
typedef FieldIndex InfoIndex;



class __exportC PPersBase;
class __exportC Anchor;

class PPersNode;
class PPersVersion;
class PPersVersion_rep;
class LAnchor;

class PPersNode {
public:
  union Field {
    PPersNode* ptr;
    GenPtr     info;
  };

public:
  void mark_orig_ref(InfoIndex idx)
  { orig_ref_vector |= (1 << idx); }

  bool is_orig_ref(InfoIndex idx) const
  { return (orig_ref_vector & (1 << idx)) != 0; }

  PPersNode(Anchor& a, TimeStamp s) :
    my_anchor(a), stamp(s), pred(0), orig_ref_vector(0) {}

  PPersNode(PPersNode* n, TimeStamp s) :
    my_anchor(n->my_anchor), stamp(s), pred(n), orig_ref_vector(0) {}

  PPersNode*& ptr(PtrIndex idx) { return fields[idx].ptr; }
  GenPtr& info(InfoIndex idx) { return fields[idx].info; }

  Anchor& get_anchor() const { return my_anchor; }
  TimeStamp get_stamp() const { return stamp; }
  PPersNode* get_pred() const { return pred; }


private:
  Anchor&    my_anchor;
  TimeStamp  stamp;
  PPersNode* pred;

  RefVector  orig_ref_vector; 
  Field      fields[1]; // must be the last attribute !

public:
  LEDA_MEMORY(PPersNode)
};

class __exportC Anchor {
public:
  struct ReversePtr {
    Anchor*  target;
    PtrIndex lp_idx;

    ReversePtr() {}
    ReversePtr(Anchor* a, PtrIndex i) : target(a), lp_idx(i) {}
    bool is_unused() const { return lp_idx < 0; }
  };
  enum { bridge_build_parameter = 2 };

public:
    virtual void clear_info(GenPtr& p, InfoIndex idx) = 0;

  static void p_delete(Anchor* a) { a->p_delete(); }

  void dock_to_node(PPersNode* n, TimeStamp new_cur_stamp)
  { docked_node = n; cur_stamp = new_cur_stamp; }

  PPersNode* get_docked_node() const { return docked_node; }
  TimeStamp get_current_stamp() const { return cur_stamp; }

  void dock_to_live_node(TimeStamp newest_ver) 
  { dock_to_node(live_node, newest_ver); }

  void register_ptr(PtrIndex idx, Anchor* init_val)
  { 
    install_live_ptr(idx, init_val); 
  }

  void register_info(InfoIndex) {}
  void register_info(InfoIndex idx, GenPtr init_val)
  {
    live_node->mark_orig_ref(idx); live_node->info(idx) = init_val;
  }

  void assign_live_ptr(PtrIndex idx, Anchor* new_val);
  void assign_live_info(InfoIndex idx, GenPtr new_val);

  const GenPtr& access_info(InfoIndex idx) const
  { return docked_node->info(idx); }

  void set_pred(Anchor* s) { pred = s; }
  Anchor* get_pred() const { return pred; }

  Anchor(PPersBase& base, int num_ps, int num_is, int num_rps);
  virtual ~Anchor();

  void destroy_nodes();

  void dock_to_live_node();
  void find_node_and_dock(TimeStamp new_cur_stamp, PPersNode* hint);

  int get_num_ptrs() const { return num_ptrs; }
  int get_num_infos() const { return num_fields-num_ptrs; }

  PPersNode* get_live_node() const { return live_node; }
  PPersBase& get_base() const { return my_base; }

  Anchor* follow_ptr(PtrIndex idx) const;

  void p_delete();

  virtual int get_id() const { return -1; }


private:

  Anchor(const Anchor&);
  Anchor& operator=(const Anchor&) { return *this; }

  PtrIndex ptrs_begin() const { return 0; }
  PtrIndex ptrs_end() const { return num_ptrs; }

  InfoIndex infos_begin() const { return num_ptrs; }
  InfoIndex infos_end() const { return num_fields; }

  int node_size() const 
  { return sizeof(PPersNode) + (num_fields-1) * sizeof(PPersNode::Field); }

  void free_rev_ptr(PtrIndex idx) 
  { rev_ptrs[idx].lp_idx = - first_free_rp; first_free_rp = idx; }

  void move_live_ptr_to(Anchor* new_target, PtrIndex lp_idx)
  {
    delete_corresp_rp_for_lp(lp_idx);
    install_live_ptr(lp_idx, new_target);
  }

  PtrIndex allocate_rev_ptr();

  void delete_corresp_rp_for_lp(PtrIndex lp_idx);
  void install_live_ptr(PtrIndex lp_idx, Anchor* target);

  void copy_live_node();


private:
  PPersBase&  my_base;

  const int   num_fields;
  const int   num_ptrs;
  int         num_rev_ptrs;

  TimeStamp   cur_stamp;
  PPersNode*  docked_node;

  PPersNode*  live_node;

  ReversePtr* rev_ptrs;
  PtrIndex* corresp_rev_ptr_for_lp;

  PtrIndex first_free_rp;

  Anchor* pred;
};

class __exportC PPersBase : public handle_rep {
public:
  typedef PPersVersion Version;

public:
explicit PPersBase(int num_eps);
  virtual ~PPersBase() { destroy_anchors(); }

protected:
  void destroy_anchors();

public:
  void begin_update_at(Version& v, Anchor* a)
  { begin_update(v); a->dock_to_live_node(get_current_stamp()); }

  void begin_update_at(Version& v, PPersNode* n)
  { begin_update_at(v, &(n->get_anchor()) ); }

  void access_version_at(const Version& v, PPersNode* n) const
  { access_version(v); n->get_anchor().dock_to_node(n, get_current_stamp()); }

  static void p_delete(Anchor* a) { a->p_delete(); }

  Version get_empty_version() const;

  void begin_update(Version& v);
  void end_update();

  inline void access_version(const Version& v) const;

  TimeStamp register_anchor(Anchor* a);

  void register_entry_ptr(PtrIndex) 
  { 
  }
  Anchor* follow_entry_ptr(PtrIndex idx) const;
  void assign_entry_ptr(PtrIndex idx, Anchor* new_val);

  inline TimeStamp get_current_stamp() const;
  TimeStamp get_newest_stamp() const { return newest_time_stamp; }

  int entry_ptrs_begin() const { return 0; }
  int entry_ptrs_end() const { return num_entry_ptrs; }
  int get_num_entry_ptrs() const { return num_entry_ptrs; }


private:
  int num_entry_ptrs;

  TimeStamp newest_time_stamp;

  //mutable 
  PPersVersion_rep* cur_version_rep;

  Anchor* last_anchor;
};

class PPersVersion_rep : public handle_rep, public HANDLE_BASE(PPersBase) {

PPersBase* ptr() const { return (PPersBase*)PTR; }

public:
  PPersVersion_rep(PPersBase* my_base, TimeStamp s, int num_entry_ptrs) 
    : stamp(s), entry_ptrs(num_entry_ptrs) 
  { 
    PTR = my_base;
  }

  virtual ~PPersVersion_rep() {}

  PPersVersion_rep(const PPersVersion_rep& v)
    : HANDLE_BASE(PPersBase)(v), stamp(v.stamp), entry_ptrs(v.entry_ptrs) {}

  PPersBase* get_base() const { return ptr(); }
  int get_ref_count() const { return ref_num(); }

  TimeStamp get_stamp() const { return stamp; }
  void increment_stamp() { ++stamp; }

  PPersNode*& entry_ptr(PtrIndex idx) { return entry_ptrs[idx]; }

private:
  TimeStamp stamp;
  array<PPersNode*> entry_ptrs;

private:
  // Version_reps may not be assigned, only Versions
  PPersVersion_rep& operator=(const PPersVersion_rep&);

public:
  
 LEDA_MEMORY(PPersVersion_rep)
};

/*
   Note: We do not derive PPersVersion from HANDLE_BASE(PPersVersion_rep),
         because we need versions having NULL-pointers as rep-pointers!
*/
class PPersVersion {
public:
  PPersVersion(PPersVersion_rep* vr = 0) : PTR(vr) {}

  /*virtual*/ ~PPersVersion() { if (PTR && PTR->ref_minus() == 0) delete PTR; }

  PPersVersion(const PPersVersion& v) : PTR(v.PTR)
  { if (PTR) PTR->ref_plus(); }

  PPersVersion& operator=(const PPersVersion& v) 
  { if (v.PTR) v.PTR->ref_plus(); if (PTR) PTR->ref_minus(); PTR = v.PTR; return *this; }

  PPersBase* get_base() const { return get_rep()->get_base(); }
  TimeStamp get_stamp() const { return get_rep()->get_stamp(); }
  bool is_sole_reference() const { return get_rep()->get_ref_count() == 1; }

  PPersVersion create_next() const 
  {
    PPersVersion_rep* next = new PPersVersion_rep(*get_rep());
    next->increment_stamp();
    
    return PPersVersion(next);
  }

private:
  PPersVersion_rep* PTR;
private:
  PPersVersion_rep* get_rep() const { return PTR; }

friend class PPersBase;

// for use with LEDA:
friend ostream& operator<<(ostream& out, const PPersVersion&) { return out; }
friend istream& operator>>(istream& in, PPersVersion&) { return in; }
};

template <class Node>
class PPersEntryPtrRef {
public:
  PPersEntryPtrRef(PPersBase& my_b, PtrIndex my_idx)
    : my_base(my_b), my_index(my_idx) {}

  PPersEntryPtrRef(const PPersEntryPtrRef<Node>& ref)
    : my_base(ref.my_base), my_index(ref.my_index) {}

  Node& operator*() const 
  { return (Node&) *my_base.follow_entry_ptr(my_index); }
  Node* operator->() const 
  { return (Node*) my_base.follow_entry_ptr(my_index); }
  operator Node*() const 
  { return (Node*) my_base.follow_entry_ptr(my_index); }

  const Node* operator=(const Node* n) 
  { my_base.assign_entry_ptr(my_index, __const_cast(Node*)(n)); return n; }

private:
  PPersBase& my_base;
  PtrIndex   my_index;

public:
  LEDA_MEMORY(PPersEntryPtrRef<Node>)
};

template <class Node, PtrIndex Idx>
class PPersEntryPtr {
public:
  PPersEntryPtr(PPersBase& my_b) : my_base(my_b)
  { my_base.register_entry_ptr(Idx); }

  Node& operator*() const { return (Node&) *my_base.follow_entry_ptr(Idx); }
  Node* operator->() const { return (Node*) my_base.follow_entry_ptr(Idx); }
  operator Node*() const { return (Node*) my_base.follow_entry_ptr(Idx); }

  operator PPersEntryPtrRef<Node>() const
  { return PPersEntryPtrRef<Node>(my_base, Idx); }

  const Node* operator=(const Node* n) 
  { my_base.assign_entry_ptr(Idx, __const_cast(Node*)(n)); return n; }

private:
  PPersBase& my_base;

private:
  PPersEntryPtr(const PPersEntryPtr<Node, Idx>&);

public:
  typedef PPersEntryPtr<Node, Idx> self;
  LEDA_MEMORY(self)
};

template <class Node>
class PPersPtrRef {
public:
  PPersPtrRef(Anchor& my_a, PtrIndex my_idx) 
    : my_anchor(my_a), my_index(my_idx) {}

  PPersPtrRef(const PPersPtrRef<Node>& ref) 
    : my_anchor(ref.my_anchor), my_index(ref.my_index) {}

  Node& operator*() const 
  { return (Node&) *my_anchor.follow_ptr(my_index); }

  Node* operator->() const 
  { return (Node*) my_anchor.follow_ptr(my_index); }

  operator Node*() const 
  { return (Node*) my_anchor.follow_ptr(my_index); }

  const Node* operator=(const Node* n) 
  { my_anchor.assign_live_ptr(my_index, __const_cast(Node*)(n)); return n; }

  Anchor&  get_anchor() const { return my_anchor; }
  PtrIndex get_index()  const { return my_index; }

private:
  Anchor&  my_anchor;
  PtrIndex my_index;

public:
  LEDA_MEMORY(PPersPtrRef<Node>)
};

template <class Node, PtrIndex Idx>
class PPersPtr {
public:
  enum { index = Idx };

public:
  PPersPtr(Anchor& my_a, Node* val = 0) : my_anchor(my_a)
  { my_anchor.register_ptr(Idx, val); }

  Node& operator*() const 
  { return (Node&) *my_anchor.follow_ptr(Idx); }

  Node* operator->() const 
  { return (Node*) my_anchor.follow_ptr(Idx); }

  operator Node*() const 
  { return (Node*) my_anchor.follow_ptr(Idx); }

  operator PPersPtrRef<Node>() const 
  { return PPersPtrRef<Node>(my_anchor, Idx); }

  const Node* operator=(const Node* n) 
  { my_anchor.assign_live_ptr(Idx, __const_cast(Node*)(n)); return n; }

  Anchor&  get_anchor() const { return my_anchor; }
  PtrIndex get_index()  const { return Idx; }

private:
  Anchor&  my_anchor;

private:
  PPersPtr(const PPersPtr<Node, Idx>&);

public:
  typedef PPersPtr<Node, Idx> self;
  LEDA_MEMORY(self)
};

template <class Node>
class EphPtr {
public:
  EphPtr(Node* init_val = 0) : ptr(init_val) {}

  Node& operator*() const { return *dock_pointee_and_get_ptr(); }
  Node* operator->() const { return dock_pointee_and_get_ptr(); }
  operator Node*() const { return dock_pointee_and_get_ptr(); }

  const Node* operator=(const Node* n) 
  { ptr = __const_cast(Node*)(n); return ptr; }

  bool is_nil() const { return ptr == nil; }

private:
  Node* dock_pointee_and_get_ptr() const
  { 
    if (ptr) ptr->dock_to_live_node(); 
    return ptr; 
  }

private:
  Node* ptr;

public:
  LEDA_MEMORY(EphPtr<Node>)
};

template <class T>
class PPersInfoRef {
public:
  PPersInfoRef(Anchor& my_a, InfoIndex my_idx) 
    : my_anchor(my_a), my_index(my_idx) {}

  PPersInfoRef(const PPersInfoRef<T>& ref)
    : my_anchor(ref.my_anchor), my_index(ref.my_index) {}

  operator const T&() const
  { return LEDA_CONST_ACCESS(T, my_anchor.access_info(my_index)); }

  const PPersInfoRef<T>& operator=(const T& inf)
  { my_anchor.assign_live_info(my_index, leda_copy(inf)); return *this; }

  Anchor&   get_anchor() const { return my_anchor; }
  InfoIndex get_index()  const { return my_index; }
  
private:
  Anchor&  my_anchor;
  InfoIndex my_index;

public:
  LEDA_MEMORY(PPersInfoRef<T>)
};
template <class T, InfoIndex Idx>
class PPersInfo {
public:
  enum { index = Idx };

public:
  PPersInfo(Anchor& my_a, const T& val = T()) : my_anchor(my_a)
  { my_anchor.register_info(Idx, leda_copy(val)); }

  operator const T&() const
  { return LEDA_CONST_ACCESS(T, my_anchor.access_info(Idx)); }

  operator PPersInfoRef<T>() const 
  { return PPersInfoRef<T>(my_anchor, Idx); }

  const PPersInfo<T, Idx>& operator=(const T& inf)
  { my_anchor.assign_live_info(Idx, leda_copy(inf)); return *this; }

  static void clear_info(GenPtr& p)
  { LEDA_CLEAR(T, p); }

  Anchor&   get_anchor() const { return my_anchor; }
  InfoIndex get_index()  const { return Idx; }
  
private:
  Anchor&  my_anchor;

private:
  PPersInfo(const PPersInfo<T, Idx>&);

public:
  typedef PPersInfo<T, Idx> self;
  LEDA_MEMORY(self)
};

class LPPersBase : public PPersBase {
public:
explicit LPPersBase(int num_aps) : PPersBase(num_aps) { }

  virtual void copy_info(GenPtr& p, InfoIndex idx, Anchor& a) = 0;
  virtual void clear_info(GenPtr& p, InfoIndex idx, Anchor& a) = 0;

protected:
  void destroy() { destroy_anchors(); }

public:
  typedef PPersNode* pers_item;

  static pers_item convert_item(Anchor* a) 
  { return a == nil ? nil : a->get_docked_node(); }

};

class LAnchor : public Anchor {
public:
  LAnchor(LPPersBase& base, int num_ps, int num_is, int num_rps) :
    Anchor(base, num_ps, num_is, num_rps) { }

  virtual void clear_info(GenPtr& p, InfoIndex idx)
  { get_base().clear_info(p, idx, *this); }

  virtual int get_id() const { return 0; }

  LPPersBase& get_base() const { return (LPPersBase&) Anchor::get_base(); }
};

class PPersGenPtrRef {
public:
  PPersGenPtrRef(LAnchor& my_a, InfoIndex my_idx) 
    : my_anchor(my_a), my_index(my_idx) {}

  PPersGenPtrRef(const PPersGenPtrRef& ref) 
    : my_anchor(ref.my_anchor), my_index(ref.my_index) {}

  ~PPersGenPtrRef() {}

  operator const GenPtr&() const
  { return my_anchor.access_info(my_index); }

  const PPersGenPtrRef& operator=(GenPtr val)
  { my_anchor.assign_live_info(my_index, val); return *this; }

  const PPersGenPtrRef& operator=(const PPersGenPtrRef& p)
  { return operator=(copy_info(p)); }

  LAnchor&  get_anchor() const { return my_anchor; }
  InfoIndex get_index()  const { return my_index; }
  
private:
  LAnchor&  my_anchor;
  InfoIndex my_index;

private:
  inline GenPtr copy_info(const GenPtr& p) const;

public:
  LEDA_MEMORY(PPersGenPtrRef)
};

template <InfoIndex Idx>
class PPersGenPtr {
public:
  enum { index = Idx };

public:
  PPersGenPtr(LAnchor& my_a) : my_anchor(my_a) 
  { my_anchor.register_info(Idx); }

  ~PPersGenPtr() {}

  PPersGenPtr(LAnchor& my_a, GenPtr val) : my_anchor(my_a)
  { my_anchor.register_info(Idx, val); }

  /*const*/ PPersGenPtr<Idx>& operator=(GenPtr val)
  { my_anchor.assign_live_info(Idx, val); return *this; }

  PPersGenPtr(LAnchor& my_a, const PPersGenPtr<Idx>& p) : my_anchor(my_a)
  { my_anchor.register_info(Idx, copy_info(p)); }

  /*const*/ PPersGenPtr<Idx>& operator=(const PPersGenPtr<Idx>& p)
  { return operator=(copy_info(p)); }

  PPersGenPtr(LAnchor& my_a, const PPersGenPtrRef& p) : my_anchor(my_a)
  { my_anchor.register_info(Idx, copy_info(p)); }

  const PPersGenPtr<Idx>& operator=(const PPersGenPtrRef& p)
  { return operator=(copy_info(p)); }


  operator const GenPtr&() const
  { return my_anchor.access_info(Idx); }

  operator PPersGenPtrRef() const { return PPersGenPtrRef(my_anchor, Idx); }

  LAnchor&  get_anchor() const { return my_anchor; }
  InfoIndex get_index()  const { return Idx; }
  
private:
  LAnchor&  my_anchor;

private:
  inline GenPtr copy_info(const GenPtr& p) const;

  PPersGenPtr(const PPersGenPtr<Idx>&);

public:
  LEDA_MEMORY(PPersGenPtr<Idx>)
};


inline
void Anchor::dock_to_live_node()
{ dock_to_node(live_node, get_base().get_newest_stamp()); }

inline
void PPersBase::access_version(const Version& v) const
{
  (PPersVersion_rep*&)cur_version_rep = v.get_rep();
}

inline
void PPersBase::assign_entry_ptr(PtrIndex idx, Anchor* new_val)
{
  cur_version_rep->entry_ptr(idx) = new_val ? new_val->get_live_node() : nil;
}

inline 
TimeStamp PPersBase::get_current_stamp() const
{ return cur_version_rep->get_stamp(); }

template <InfoIndex Idx>
inline
GenPtr PPersGenPtr<Idx>::copy_info(const GenPtr& p) const 
{
  LPPersBase& my_base = my_anchor.get_base();
  GenPtr pc = p;
  my_base.copy_info(pc, Idx, my_anchor);
  return pc;
}

inline
GenPtr PPersGenPtrRef::copy_info(const GenPtr& p) const 
{
  LPPersBase& my_base = my_anchor.get_base();
  GenPtr pc = p;
  my_base.copy_info(pc, my_index, my_anchor);
  return pc;
}


LEDA_END_NAMESPACE

#endif // LEDA_PART_PERS_H
