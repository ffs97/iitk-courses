/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  bin_search.t
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_BINSEARCH_T
#define LEDA_BINSEARCH_T

LEDA_BEGIN_NAMESPACE


template <class E>
inline E* BINARY_SEARCH_C(E* l, E* r, const E& x)
{ while (l <= r)
  { E* m = l + (r-l)/2;
    int c = compare(x,*m);
    if (c == 0) return m;
    if (c > 0)
       l = m + 1;
    else
       r = m - 1;
   }
  return r;
}


template <class E>
inline E* BINARY_SEARCH_CMP(E* l, E* r, const E& x, 
                                        int (*cmp)(const E&, const E&))
{ while (l <= r)
  { E* m = l + (r-l)/2;
    int c = cmp(x,*m);
    if (c == 0) return m;
    if (c > 0)
       l = m + 1;
    else
       r = m - 1;
   }
  return r;
}


template <class E>
inline E* BINARY_LOCATE_C(E* l, E* r, const E& x)
{ l--; r++;
  while (l < r+1)
  { E* m = l + (r-l)/2;
    if (compare(x,*m) >= 0)
		if (l == m)
			return m;
		else
			l = m;
    else
       r = m;
   }
  return l;
}


template <class E>
inline E* BINARY_LOCATE_CMP(E* l, E* r, const E& x, 
                                        int (*cmp)(const E&, const E&))
{ l--; r++;
  while (l < r-1)
  { E* m = l + (r-l)/2;
    if (cmp(x,*m) >= 0)
		if (l == m)
			return m;
		else
			l = m;
    else
       r = m;
   }
  return l;
}


LEDA_END_NAMESPACE

#endif

