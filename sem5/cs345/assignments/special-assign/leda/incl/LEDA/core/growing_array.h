/******************************************************************************
+
+  LEDA 6.3  
+
+
+  growing_array.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_GROWING_ARRAY_H
#define LEDA_GROWING_ARRAY_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600037
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/core/array.h>

LEDA_BEGIN_NAMESPACE

template <class E> 
class growing_array {
public:
	typedef int index_type;

	growing_array(index_type sz = 0) { init(sz, sz); }
	growing_array(index_type sz, index_type cap) { init(sz, cap); }
	growing_array(const growing_array& v) { copy(v); }
	growing_array(const array<E>& a) { copy(a); }
	~growing_array() { clear(); }

	const growing_array& operator=(const growing_array& v) 
	{ if (this != &v) { clear(); copy(v); } return *this; }

	const growing_array& operator=(const array<E>& a) 
	{ clear(); copy(a); return *this; }

	const E& operator[](index_type i) const { return Elements[i]; }
	      E& operator[](index_type i)       { return Elements[i]; }

	index_type size() const { return Size; }
	index_type capacity() const { return Capacity; }

	void clear()
	{ Size = Capacity = 0; delete[] Elements; Elements = 0; }

	void push_back(const E& x) { if (Capacity == Size) reserve(2*Size+1); Elements[Size++] = x; }
	void pop_back() { --Size; }

	const E& front() const { return *Elements; }
	      E& front() { return *Elements; }
	const E& back() const { return Elements[Size-1]; }
	      E& back() { return Elements[Size-1]; }

	void resize(index_type sz)
	{ reserve(sz); Size = sz; }

	void reserve(index_type cap)
	{
		if (cap <= Capacity) return;
		E* new_elems = new E[cap];
		E* pn = new_elems + Size; E* p = Elements + Size;
		while (p > Elements) (*--pn) = (*--p);
		delete[] Elements;
		Elements = new_elems; Capacity = cap;
	}

	friend ostream& operator<<(ostream& os, const growing_array<E>& v)
	{ for (int i = 0; i < v.size(); ++i) { if (i > 0) os << ' '; os << v[i]; } return os; }

	friend istream& operator>>(istream& is, growing_array<E>& v)
	{ v.clear(); E x; while (is) { is >> x; v.push_back(x); } return is; }

private:
	void init(index_type sz, index_type cap)
	{ Size = sz <= cap ? sz : cap; Capacity = cap; Elements = cap > 0 ? new E[cap] : 0; }

	void copy(const growing_array& v)
	{
		Size = Capacity = v.Size;
		Elements = Size > 0 ? new E[Size] : 0;
		E* p = Elements + Size; E* pv = v.Elements + Size;
		while (p > Elements) (*--p) = (*--pv);
	}

	void copy(const array<E>& a)
	{
		Size = Capacity = a.size();
		E* p = Elements = Size > 0 ? new E[Size] : 0;
		for (int i = a.low(); i <= a.high(); ++i, ++p) *p = a[i];
	}

private:
	E* Elements;
	index_type Size, Capacity;
};

#if LEDA_ROOT_INCL_ID == 600037
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

LEDA_END_NAMESPACE

#endif
