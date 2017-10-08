/* 
* last modified: 26.06.2003   
*/

#ifndef VGL_SMATRIX_H
#define VGL_SMATRIX_H

#include <LEDA/graphics/gl_basic.h>
/*
#include <iostream>
#include <cmath>
*/


VGL_BEGIN_NAMESPACE

//---------------------------------------------------------------------------
// workaround
//---------------------------------------------------------------------------

namespace vgl_detail {

#ifdef MSC_VER_6

inline double sqrt(double x) { return std::sqrt(x); }
inline double abs(double x) { return std::fabs(x); }
inline float abs(float x) { return float(std::fabs(x)); }
inline long abs(long x) { return std::labs(x); }
inline int abs(int x) { return std::abs(x); }

#else

inline double sqrt(double x) { return ::sqrt(x); }
inline double abs(double x) { return ::fabs(x); }
inline float abs(float x) { return float(::fabs(x)); }
inline long abs(long x) { return ::labs(x); }
inline int abs(int x) { return ::abs(x); }

#endif

}

template <int, typename> class svector;

/*! \class smatrix
    The data type \c smatrix <\c n,m,T> implements a static \f$ n \times m \f$ 
    matrix by a one-dimensional array of type \c T (default: \c double) with
    \f$M(i,j)=D[i \cdot m + j]\f$ for \f$0\leq i \leq n -1\f$ and \f$0\leq j \leq m-1\f$,
    where \a D is a pointer to an array of size \a nm. The space requirement is \a O(nm). */

template <int n, int m, typename T = double>
class smatrix {

  static int index;
  
public:
  
  enum
  { rows    = n,     /*!< Stores the number of rows. */
    columns = m,     /*!< Stores the number of columns. */
    size    = n * m  /*!< Stores the size.  */
  };
      
  typedef T        value_type;      /*!< Type name for value type. */
  typedef T&       reference;       /*!< Type name for reference type. */
  typedef const T& const_reference; /*!< Type name for const reference type. */
  typedef T*       pointer;         /*!< Type name for pointer type. */
  typedef const T* const_pointer;   /*!< Type name for const pointer type. */

  /*! Creates a \f$n \times m \f$ matrix \a M and initializes
      all entries of \a M to \a x.*/
  explicit smatrix(const_reference x = T())
  { for (int i = 0; i < size; ++i) D[i] = x; }

  /*! Copy constructor. */
  smatrix(const smatrix<n,m,T>& mat) { *this = mat; }

  /*! Assignment. */
  smatrix<n,m,T>& operator=(const smatrix<n,m,T>& M)
  { for (int i = 0; i < size; ++i) D[i] = M.D[i]; 
    return *this;
  }
 
  /*! Sets \f$D[0]\f$ to \a x and initializes the matrix 
      initialization index \f$\mathit{index}\f$.
      @see operator,(const_reference x) */
  smatrix<n,m,T>& operator=(const_reference x) 
  { index = 0;
    D[0] = x; 
    return *this;
  }

  /*! Sets \f$D[\mathit{index}]\f$ to \a x and
      increases the matrix initialization index 
      \f$\mathit{index}\f$. The following example shows
      a matrix initialization using the operator= and the
      operator, 
      \code 
      smatrix<3,3> I;
      I = 1, 0, 0,
          0, 1, 0,
          0, 0, 1;
      \endcode
      
      @see operator=(const_reference x) 
  */
  smatrix<n,m,T>& operator,(const_reference x)
  { if (++index == size) index = 0;
    D[index] = x;        
    return *this;
  }

  /*! Returns a const reference of \f$M_{ij}\f$. */
  const_reference operator()(int i, int j) const { return D[i * m + j]; } 

  /*! Returns a reference of \f$M_{ij}\f$. */
  reference       operator()(int i, int j)       { return D[i * m + j]; } 
  
  
  /*! Returns the \f$i\f$-th row of the matrix as a \a m 
      dimensional static vector. \pre \f$0\leq i \leq m-1\f$. */
  svector<m,T> row(int i) const
  { svector<m,T> v;
    for (int j = 0; j < m; ++j) v[j] = elem(i,j);
    return v;
  }
  
  /*! Returns the \f$i\f$-th row of the matrix as a \a m 
      dimensional reference vector. \pre \f$0\leq i \leq n-1\f$. */  
  svector<m,T>& operator[](int i) 
  { svector<m,T>* p = (svector<m,T>*) D + i; 
    new (p) svector<m,T>(bool());
    return *p; 
  }


  /*! Returns the \f$j\f$-th column of the matrix as a \a n 
      dimensional static vector. \pre \f$0\leq j \leq m-1\f$. */
  svector<n,T> col(int j) 
  { svector<n,T> v;
    for (int i = 0; i < n; ++i) v[i] = elem(i,j);
    return v;
  }
 
  /*! Addition.  
  */
  smatrix<n,m,T>& operator+=(const smatrix<n,m,T>& M)
  { for (int i = 0; i < n; ++i) 
      for (int j = 0; j < m; ++j)
        elem(i,j) += M(i,j);
    return *this;
  }


  /*! Subtraction.  
  */
  smatrix<n,m,T>& operator-=(const smatrix<n,m,T>& M)
  { for (int i = 0; i < n; ++i) 
      for (int j = 0; j < m; ++j)
        elem(i,j) -= M(i,j);
    return *this;
  }
 

  /*! Test of equality.*/
  bool operator==(const smatrix<n,m,T>& M) const
  { if (this == &M) return true;
    for(int i = 0; i < size; ++i) 
      if (D[i] != M.D[i]) return false;
    return true;
  }

  /*! Test of inequality. */
  bool operator!=(const smatrix<n,m,T>& M) const 
  { return !(*this == M); }


  /*! Returns the number rows. */
  int dim1() const { return n; }
  
  /*! Returns the number columns. */
  int dim2() const { return m; }

  /*! Returns the pointer pointing to the first 
      element of the matrix. */
  operator pointer() { return D; }

  /*! Returns the pointer pointing to the first constant 
      element of the matrix.*/
  operator const_pointer() const { return D; } 
     
  protected:
  
  const_reference elem(int i, int j) const { return D[i * m + j]; }
  reference       elem(int i, int j)       { return D[i * m + j]; }
  
  T D[n * m];
};

template <int n, int m, typename T>
int smatrix<n,m,T>::index = 0;


//----------------------------------------------------------------------------
//
// static_vector 
//
//----------------------------------------------------------------------------

/*! \class svector
    The data type \c svector <\c n,m,T> implements a static \a n dimensional
    vector \a v by a one-dimensional array of \a n variables of some
    type \c T. The default number type is \c double. The variables are indexed
    from 0 to \a n - 1 and \a v[i] denotes the variable with index \a i.*/

template <int n, typename T = double>
class svector {

  static int index;

public:

  enum
  { size = n  /*!< Stores the size of the vector.  */
  };
  
  typedef T        value_type;      /*!< Type name for value type. */
  typedef T&       reference;       /*!< Type name for reference type. */
  typedef const T& const_reference; /*!< Type name for const reference type. */
  typedef T*       pointer;         /*!< Type name for pointer type. */
  typedef const T* const_pointer;   /*!< Type name for const pointer type. */

  // don't initialize the vector
  
  /*! Creates a \a n dimensional vector and without
      an explicit initialization of the components. */
  explicit svector(bool) {}

  /*! Creates a \a n dimensional vector \a V and initializes
      all components of \a V to \a x.*/ 
  explicit svector(const_reference x = T()) { 
    for (int i = 0; i < size; ++i) V[i] = x; 
	}

  /*! Creates a \a n dimensional vector \a V and initializes
      the first two components of \a V with \a x0 and \a x1
      and all other components with \c T().
      \pre \f$ n \geq 2 \f$ */ 
  svector(const_reference x0, const_reference x1) 
  { V[0] = x0; V[1] = x1; 
    for (int i = 2; i < size; ++i) V[i] = T();       
  }

  /*! Creates a \a n dimensional vector \a V and initializes
      the first three components of \a V with \a x0, \a x1 and \a x2
      and all other components with \c T().
      \pre \f$ n \geq 3 \f$ */ 
  svector(const_reference x0, const_reference x1, const_reference x2) 
  { V[0] = x0; V[1] = x1; V[2] = x2; 
    for (int i = 3; i < size; ++i) V[i] = T();     
  }

  /*! Creates a \a n dimensional vector \a V and initializes
      the first three components of \a V with \a x0, \a x1, \a x2 and \a x3
      and all other components with \c T().
      \pre \f$ n \geq 4 \f$ */ 
  svector(const_reference x0, const_reference x1, 
          const_reference x2, const_reference x3) 
  { V[0] = x0; V[1] = x1; V[2] = x2; V[3] = x3; 
    for (int i = 4; i < size; ++i) V[i] = T();    
  }

  /*! Creates a \a n dimensional vector \a V and initializes
      \a V with the first \a i components of \a p. \pre \f$ n \geq i \f$ */ 
  svector(const_pointer p, int s = size) { 
    for (int i = 0; i < size && i < s; ++i) V[i] = p[i];
  }

  /*! Copy constructor. */
  svector(const svector<n,T>& vec) { *this = vec; }
  
  /*! Assignment. */
  svector<n,T>& operator=(const svector<n,T>& vec)
  { for (int i = 0; i < size; ++i) V[i] = vec.V[i]; 
    return *this;
  }
         
  /*! Sets \f$V[0]\f$ to \a x and initializes the vector
      initialization index \f$\mathit{index}\f$.
      @see operator,(const_reference x) */
  svector<n,T>& operator=(const_reference x) 
  { index = 0;
    V[0] = x;
    return *this;
  }

  /*! Sets \f$V[\mathit{index}]\f$ to \a x and
      increases the vector initialization index 
      \f$\mathit{index}\f$. The following example shows
      a vector initialization using the operator= and the
      operator, 
      \code 
      svector<3> V;
      V = 1, 4, 5;
      \endcode
      
      @see operator=(const_reference x) 
  */
  svector<n,T>& operator,(const_reference x)
  { if (++index == size) index = 0;
    V[index] = x;            
    return *this;
  }
           
  /*! Returns a constant reference of the \a i -th component of \a V. 
      \pre \f$0\leq i \leq n-1\f$. */
  const_reference operator[](int i) const { return V[i]; } 

  /*! Returns a reference of the \a i -th component of \a V. 
      \pre \f$0\leq i \leq n-1\f$. */
  reference       operator[](int i) { return V[i]; } 
      
  /*! Addition. */
  svector<n,T>& operator+=(const svector<n,T>& vec)
  { for (int i = 0; i < n; ++i) V[i] += vec.V[i];
    return *this;
  }

  /*! Subtraction. */
  svector<n,T>& operator-=(const svector<n,T>& vec)
  { for (int i = 0; i < n; ++i) V[i] -= vec.V[i];
    return *this;
  }

  /*! Multiplication by a scalar \a x. */
  svector<n,T>& operator*=(const_reference x)
  { for (int i = 0; i < n; ++i) V[i] *= x;
    return *this;
  }
 
   /*! Division by a scalar \a x. */
  svector<n,T>& operator/=(const_reference x)
  { for (int i = 0; i < n; ++i) V[i] /= x;
    return *this;
  }

  /*! Test of equality.*/
  bool operator==(const svector<n,T>& vec) const
  { if (this == &vec) return true;
    for(int i = 0; i < size; ++i) 
      if (V[i] != vec.V[i]) return false;
    return true;
  }

  /*! Test of inequality. */
  bool operator!=(const svector<n,T>& vec) const 
  { return !(*this == vec); }
  
   
  /*! Returns the dimension of the vector. */
  int dim() const { return n; }

  /*! Returns the pointer pointing to the first component.*/
  operator pointer() { return V; }

  /*! Returns the pointer to the constant first component.*/
  operator const_pointer() const { return V; } 
  
  /*! \section sec Additional Operations for vectors in two and three-dimensional space
  */
  
  /*! Retruns the first component. */  
  T xcoord() const { return V[0]; }

  /*! Retruns the second component. 
  */
  T ycoord() const { return V[1]; }

  /*! Retruns the third component. 
  */
  T zcoord() const { return V[2]; }

  /*! Retruns the fourth component. 
  */
  T wcoord() const { return V[3]; }

  /*! Retruns the first component.
  */ 
  T x() const { return V[0]; }
  
  /*! Retruns the second component.
  */ 
  T y() const { return V[1]; }
  
  /*! Retruns the third component.
  */ 
  T z() const { return V[2]; }
  
  /*! Retruns the fourth component.
  */ 
  T w() const { return V[3]; }
  
  protected:
 
  value_type V[n];
};

template <int n, typename T>
int svector<n,T>::index = 0;

//----------------------------------------------------------------------------
// vector functions
//----------------------------------------------------------------------------

template <int n, typename T>
ostream& operator<<(ostream& os, const svector<n,T>& V)
{ os << n << '\n';
  for (int i = 0; i < n; ++i) os << " " << V[i];
  os << '\n';
  return os;
}

template <int n, typename T>
svector<n,T> operator*(const T& x, const svector<n,T>& v)
{ svector<n,T> r = v;
  return r *= x;
}  

template <int n, typename T>
svector<n,T> operator*(const svector<n,T>& v, const T& x)
{ return x * v; }

template <int n, typename T>
svector<n,T> operator/(const T& x, const svector<n,T>& v)
{ svector<n,T> r = v;
  return r /= x;
}  

template <int n, typename T>
svector<n,T> operator/(const svector<n,T>& v, const T& x)
{ return x / v; }

template <int n, typename T>
inline smatrix<n,n,T> tensor_product(const svector<n,T>& v1, const svector<n,T>& v2)
{ smatrix<n,n,T> res;
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
       res(i,j) = v1[i] * v2[j];
  return res;  
}

template <int n, typename T>
inline svector<n,T> cross_product(const svector<n,T>& p, const svector<n,T>& q)
{ 
  if (p.dim() != 3 || q.dim() != 3) 
	  VGL_ERROR_HANDLER("cross_product(): wrong vector dimension!");
  
  svector<n,T> v;
  v[0] = p[1] * q[2] - p[2] * q[1];
  v[1] = p[2] * q[0] - p[0] * q[2];
  v[2] = p[0] * q[1] - p[1] * q[0];
  
	return v;
}                                 


template <int n, typename T>
inline T operator*(const svector<n,T>& v1, const svector<n,T>& v2) 
{ T r = 0;
  for (int i = 0; i < v1.dim(); ++i) r += v1[i] * v2[i];
  return r;
}

template <int n, typename T>
inline svector<n,T> operator+(const svector<n,T>& v1, const svector<n,T>& v2) 
{ svector<n,T> r = v1;
  return r += v2;
}

template <int n, typename T>
inline svector<n,T> operator-(const svector<n,T>& v1, const svector<n,T>& v2) 
{ svector<n,T> r = v1;
  return r -= v2;
}


template <int n, typename T>
inline T sqr_length(const svector<n,T>& v) { return v * v; }

template <int n, typename T>
inline svector<n,T> norm(const svector<n,T>& v) { 
  return v / T(vgl_detail::sqrt(sqr_length(v))); 
}

template <int n, typename T>
inline double length(const svector<n,T>& v) { 
  return vgl_detail::sqrt(sqr_length(v)); 
}

//----------------------------------------------------------------------------
// matrix functions
//----------------------------------------------------------------------------

template <int n, int m, class T>
ostream& operator<<(ostream& os, const smatrix<n,m,T>& M)
{ os << n << " " << m << '\n';
  for (int i = 0; i < n; ++i)
  { for (int j = 0; j < m; ++j) os << " " << M(i,j);
    os << endl;
  }
  return os;
}

template <int n, int m, class T>
inline smatrix<n,m,T> operator+(const smatrix<n,m,T>& M1, const smatrix<n,m,T>& M2)
{ smatrix<n,m,T> r = M1;
  return r += M2;
}

template <int n, int m, class T>
inline smatrix<n,m,T> operator-(const smatrix<n,m,T>& M1, const smatrix<n,m,T>& M2)
{ smatrix<n,m,T> r = M1;
  return r -= M2;
}

template <int n, int m, int m1, class T>
inline smatrix<n,m1,T> operator*(const smatrix<n,m,T>& M1, const smatrix<m,m1,T>& M2)
{ smatrix<n,m1,T> res;
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < m1; ++j)
      for (int k = 0; k < m; ++k)   
        res(i,j) += M1(i,k) * M2(k,j);
    return res;       
}
  
template <int n, int m, class T>
inline smatrix<n,m,T> operator*(const T& v, const smatrix<n,m,T>& M)
{ smatrix<n,m,T> res;
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j)
        res(i,j) = M(i,j) * v;
  return res;       
}

template <int n, int m, class T>
inline smatrix<n,m,T> operator*(const smatrix<n,m,T>& M, const T& v)
{ return v * M; }

template <int n, int m, class T>
inline svector<n,T> operator*(const smatrix<n,m,T>& M, const svector<m,T>& V) 
{ svector<n,T> res;
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j)
       res[i] += M(i,j) * V[j];
  return res;
}  

template <int n, int m, class T>   
inline smatrix<m,n,T> trans(const smatrix<n,m,T>& M) 
{ smatrix<m,n,T> res;    
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j)
      res(j,i) = M(i,j);
  return res;
}

template <class matrix>
inline void identity(matrix& M)
{ typedef typename matrix::value_type T; 
  int n = M.dim1();
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
       M(i,j) = (i == j) ? T(1) : T(0);
}

template <class matrix1, class matrix2>
inline void sub_matrix(const matrix1& M, matrix2& S, int i, int j)
{ int n1 = S.dim1(), m1 = S.dim2();	
  for (int k = 0; k < n1; ++i, ++k)
    for (int l = 0, p = j; l < m1; ++p, ++l) S(k,l) = M(i,p);
}

template <class matrix, class vector>
inline int lu_decomposition(const matrix& A, matrix& L, matrix& U, vector& p)
{
  bool B = false;

  int i, j, k, n = A.dim1(), c = 0;  
  for (i = 0; i < n; ++i)
  { p[i] = i;
    for (j = 0; j < n; ++j)
    { if (i == j && A(i,i) == 0) 
      { B = false; 
        i = n; 
        break; 
      }
    }
  }
  
  typedef typename matrix::value_type T;

  identity(L);
  U = A;    

  if (B)
  { for (k = 0; k < n; ++k)
    { 
      T max = T(0);
      int m = k;
      
      for (i = k; i < n; ++i)
      { T sum = T(0);  
        for (j = k; j < n; ++j) sum += vgl_detail::abs(U(i,j));      
      
       sum = vgl_detail::abs(U(i,k)) / sum;
        if (sum > max) { max = sum; m = i; }      
      }
      
      if (m != k) 
      { c++;
        std::swap(p[k],p[m]);
      }
      
      for (i = k + 1;  i < n; ++i)
      {  T z = U(p[i],k) / U(p[k],k);
         L(p[i],k) = z;
    
		     for (j = k + 1; j < n; ++j)
				   U(p[i],j) = U(p[i],j) - z * U(p[k],j);
      }
		}
		
    return c;
  }

  for (k = 0; k < n; ++k)
  {
    for (i = k + 1; i < n; ++i)
    { L(i,k) = U(i,k) / U(k,k);
    
      for (j = k + 1; j < n; ++j)
        U(i,j) = U(i,j) - L(i,k) * U(k,j);    
        
      U(i,k) = 0;
    }
  } 
	
	return 0;
}

template <class MatrixType, class VectorType>
inline int lu_decomposition(MatrixType& A, VectorType& p)
{
  bool B = false;

  int i, j, k, n = A.dim1(), c = 0;  
  for (i = 0; i < n; ++i)
  { p[i] = i;
    for (j = 0; j < n; ++j)
    { if (i == j && A(i,i) == 0) 
      { B = false; 
        i = n; 
        break; 
      }
    }
  }
  
  typedef typename MatrixType::value_type T;
  
  if (B)
  { 
    for (k = 0; k < n; ++k)
    { 
      T max = T(0);
      int m = k;
      
      for (i = k; i < n; ++i)
      { T sum = T(0);  
        for (j = k; j < n; ++j) sum += vgl_detail::abs(A(i,j));      
      
        sum = vgl_detail::abs(A(i,k)) / sum;
        if (sum > max) { max = sum; m = i; }      
      }
      
      if (m != k) 
      { c++;
	    
		T swv = p[k];
		p[k] = p[m];
		p[m] = (int)swv;
        //std::swap(p[k],p[m]);
      }
      
      for (i = k + 1;  i < n; ++i)
      {  T z = A(p[i],k) / A(p[k],k);
         A(p[i],k) = z;
    
		     for (j = k + 1; j < n; ++j)
				   A(p[i],j) = A(p[i],j) - z * A(p[k],j);
      }
		}
		
    return c;
  }

  for (k = 0; k < n; ++k)
  { for (i = k + 1; i < n; ++i)
    { A(i,k) = A(i,k) / A(k,k);    
      for (j = k + 1; j < n; ++j)
        A(i,j) = A(i,j) - A(i,k) * A(k,j);    
    }
  } 
      
  return 0;
}


template <class MatrixType>
inline typename MatrixType::value_type det(const MatrixType& M) 
{ 
  MatrixType A = M;

	int n = A.dim1(), c = 0;
	int*p = new int[n];	

  try {
    c = lu_decomposition(A,p);
  }
  catch (...) { 
    VGL_ERROR_HANDLER("det(): matrix is singular!");  
  }
		
	typedef typename MatrixType::value_type T;

	T val = c % 2 > 0 ? T(-1) : T(1);
  
  int i, j;  
  for (i = 0; i < n; ++i)
    for (j = 0; j < n; ++j)
      if (i == j) val *= A(i,i); 
  
	delete [] p;	
  return val;
}


inline smatrix<2,2,float> inv(const smatrix<2,2,float>& M)
{ 
  float D = det(M);
  if (D == 0) 
	  VGL_ERROR_HANDLER("inv(): matrix isn't invertible!");

  smatrix<2,2,float> r;
  r =  M(1,1), -M(0,1),
      -M(1,0),  M(0,0);
  
  return r * (1/D);
}

inline smatrix<2,2,double> inv(const smatrix<2,2,double>& M)
{ 
  double D = det(M);
  if (D == 0) 
	  VGL_ERROR_HANDLER("inv(): matrix isn't invertible!");

  smatrix<2,2,double> res;
  res =  M(1,1), -M(0,1),
        -M(1,0),  M(0,0);
  
  return res * (1/D);
}
 
template <int n, class T>
inline smatrix<n,n,T> inv(const smatrix<n,n,T>& M)
{ VGL_ERROR_HANDLER("inv(): inv() not implemented!");
  return smatrix<n,n,T>();
}


//----------------------------------------------------------------------------
// typedefs
//----------------------------------------------------------------------------

typedef svector<2>         vector2d;
typedef svector<2,float>   vector2f;
typedef svector<2,int>     vector2i;

typedef svector<3>         vector3d;
typedef svector<3,float>   vector3f;
typedef svector<3,int>     vector3i;

typedef svector<4>         vector4d;
typedef svector<4,float>   vector4f;
typedef svector<4,int>     vector4i;


typedef smatrix<2,2>       matrix22d;
typedef smatrix<2,2,float> matrix22f;
typedef smatrix<2,2,int>   matrix22i;

typedef smatrix<3,3>       matrix33d;
typedef smatrix<3,3,float> matrix33f;
typedef smatrix<3,3,int>   matrix33i;

typedef smatrix<4,4>       matrix44d;
typedef smatrix<4,4,float> matrix44f;
typedef smatrix<4,4,int>   matrix44i;

VGL_END_NAMESPACE

#endif
