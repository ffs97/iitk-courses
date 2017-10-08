#include <LEDA/system/basic.h>
#include <LEDA/numbers/integer.h>
#include <LEDA/numbers/rational.h>
#include <LEDA/numbers/bigfloat.h>
#include <LEDA/numbers/real.h>
#include <LEDA/numbers/polynomial.h>

using namespace LEDA;
#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif

#define LEDA_VECTOR growing_array

template <class T>
void datastru() {
    int i;

    // uninitialized construction
    polynomial<T> p;

    // construction from iterator range, assignment
    LEDA_VECTOR<T> V(6);
    V[0] = T(integer(3));
    V[1] = T(integer(1));
    V[2] = T(integer(4));
    V[3] = T(integer(1));
    V[4] = T(integer(5));
    V[5] = T(integer(9));

    polynomial<T> q(V);
    p = q;

    // immediate construction, reduce(), copy construction, (in)equality
    LEDA_VECTOR<T> W(8);
    W[0] = T(integer(3));
    W[1] = T(integer(1));
    W[2] = T(integer(4));
    W[3] = T(integer(1));
    W[4] = T(integer(5));
    W[5] = T(integer(9));
    W[6] = T(integer(0));
    W[7] = T(integer(0));

    polynomial<T> r(W);
    assert( q.degree() == r.degree() );
    assert( q == r );
    assert( r == r );

    LEDA_VECTOR<T> X(5);
    X[0] = T(integer(2));
    X[1] = T(integer(7));
    X[2] = T(integer(1));
    X[3] = T(integer(8));
    X[4] = T(integer(1));

    q = polynomial<T>(X);
    assert( q != r );
    polynomial<T> s(p);
    assert( r == s );

    // selection of coefficients
    for (i = 0; i <= p.degree(); i++) {
        assert( p[i] == V[i] );
    }

    // zero test
    assert( !p.is_zero() );
    q = polynomial<T>(T(integer(0)));
    assert( q.is_zero() );
}

template <class T>
void arithmetic() {

    // evaluation
    LEDA_VECTOR<T> V(3);
    V[0] = T(integer(6));
    V[1] = T(integer(-5));
    V[2] = T(integer(1));

    polynomial<T> p(V);
    assert( p.evaluate(T(integer(1))) == T(integer(2)) );
    assert( p.evaluate(T(integer(2))) == T(integer(0)) );
    assert( p.evaluate(T(integer(3))) == T(integer(0)) );

    // diff
    LEDA_VECTOR<T> W(3);
    W[0] = T(integer(3));
    W[1] = T(integer(2));
    W[2] = T(integer(-2));
    polynomial<T> q(W);
    polynomial<T> z(integer(0)); 

    assert( diff(z) == z );           
    assert( p.degree() == 2 );

    LEDA_VECTOR<T> Vd(2);
    Vd[0] = T(integer(-5));
    Vd[1] = T(integer(2));
    polynomial<T> pd(Vd);
    assert( diff(p) == pd );
    assert( p.degree() == 2 );
    assert( q.degree() == 2 );
    q.diff();
    LEDA_VECTOR<T> Wd(2);
    Wd[0] = T(integer(2));
    Wd[1] = T(integer(-4));
    polynomial<T> qd(Wd);
    assert( q == qd );

       // scale and translate
    p.scale_up(T(integer(4)));
    V[0] = T(integer(6));
    V[1] = T(integer(-20));
    V[2] = T(integer(16));
    polynomial<T> pt(V);
    assert( p == pt );
    V[0] = T(integer(24));
    V[1] = T(integer(-40));
    V[2] = T(integer(16));
    polynomial<T> ptt(V);
    assert( scale_down(p, T(integer(2))) == ptt);
    assert( p == pt);
    assert( translate_by_one(p) == translate(p, T(integer(1))) );
    V[0] = T(integer(3));
    V[1] = T(integer(2));
    V[2] = T(integer(1));
    p = polynomial<T>(V);
    p.translate(T(integer(-2)));
    V[0] = T(integer(3));
    V[1] = T(integer(-2));
    V[2] = T(integer(1));
    pt = polynomial<T>(V);
    assert( p == pt);

    // addition
    polynomial<T> r;
    LEDA_VECTOR<T> Va(4);
    Va[0] = T(integer(3));
    Va[1] = T(integer(2));
    Va[2] = T(integer(1));
    Va[3] = T(integer(1));
    polynomial<T> pa(Va);
    LEDA_VECTOR<T> Wa(4);
    Wa[0] = T(integer(1));
    Wa[1] = T(integer(2));
    Wa[2] = T(integer(-1));
    Wa[3] = T(integer(-1));
    polynomial<T> qa(Wa);

    r = pa + qa;
    assert( r.degree() == 1 );
    assert( r[0] == T(integer(4)) );
    assert( r[1] == T(integer(4)) );

    // subtraction
    Va[0] = T(integer(3));
    Va[1] = T(integer(2));
    Va[2] = T(integer(1));
    Va[3] = T(integer(1));
    pa = polynomial<T>(Va);
    Wa[0] = T(integer(1));
    Wa[1] = T(integer(2));
    Wa[2] = T(integer(-1));
    Wa[3] = T(integer(1));
    qa = polynomial<T>(Wa);
    r = pa - qa;
    assert( r.degree() == 2 );

    V[0] = T(integer(2));
    V[1] = T(integer(0));
    V[2] = T(integer(2));
    p = polynomial<T>(V);
    assert( r == p);
    r = r -(-qa);
    assert( r == pa );

    // multiplication
    V[0] = T(integer(1));
    V[1] = T(integer(2));
    V[2] = T(integer(3));
    p = polynomial<T>(V);
    W[0] = T(integer(-2));
    W[1] = T(integer(5));
    W[2] = T(integer(2));
    q = polynomial<T>(W);
    r = p * q;
    LEDA_VECTOR<T> Vb(5);
    Vb[0] = T(integer(-2));
    Vb[1] = T(integer(1));
    Vb[2] = T(integer(6));
    Vb[3] = T(integer(19));
    Vb[4] = T(integer(6));
    pt = polynomial<T>(Vb);
    assert( r == pt);
    q =q* p;
    assert( q == r );

}

template <class POLY>
void test_greater(POLY a, POLY b) {
    // check .compare()
    assert( a.compare(b) == 1 );
    assert( a.compare(a) == 0 );
    assert( b.compare(a) == -1 );

    // check that all comparison operators reflect a > b
    assert(   a >  b  );
    assert( !(b >  a) );
    assert( !(a >  a) );
    assert(   b <  a  );
    assert( !(a <  b) );
    assert( !(a <  a) );
    assert(   a >= b  );
    assert( !(b >= a) );
    assert(   a >= a  );
    assert(   b <= a  );
    assert( !(a <= b) );
    assert(   a <= a  );
}

template <class T>
void signs() {

    LEDA_VECTOR<T> V(3);
    V[0] = T(integer(6));
    V[1] = T(integer(-5));
    V[2] = T(integer(1));
    polynomial<T> p(V);

    LEDA_VECTOR<T> W(3);
    W[0] = T(integer(3));
    W[1] = T(integer(2));
    W[2] = T(integer(-2));
    polynomial<T> q(W);

    T c(integer(0));
    polynomial<T> z(c); 

    // sign
    assert( z.sign() == 0 );
    assert( p.sign() == 1 );
    assert( q.sign() == -1 );

    // comparison operators

    LEDA_VECTOR<T> Vc(3);
    Vc[0] = T(integer(7));
    Vc[1] = T(integer(-5));
    Vc[2] = T(integer(1));
    polynomial<T> pc(Vc);
    test_greater(pc, p);

    LEDA_VECTOR<T> X(4);
    X[0] = T(integer(1));
    X[1] = T(integer(2));
    X[2] = T(integer(3));
    X[3] = T(integer(4));
    polynomial<T> r(X);
    test_greater(r, p);
}

template <class T>
void division() {

    // integral division (remainder zero)
    LEDA_VECTOR<T> V(5);
    V[0] = T(integer(-4));
    V[1] = T(integer(2));
    V[2] = T(integer(12));
    V[3] = T(integer(38));
    V[4] = T(integer(12));
    polynomial<T> r(V);

    r /= T(integer(2));
    LEDA_VECTOR<T> Vd(5);
    Vd[0] = T(integer(-2));
    Vd[1] = T(integer(1));
    Vd[2] = T(integer(6));
    Vd[3] = T(integer(19));
    Vd[4] = T(integer(6));
    polynomial<T> rd(Vd);

    assert( r == rd );

    // division with remainder
    LEDA_VECTOR<T> Wp(3);
    Wp[0] = T(integer(2));
    Wp[1] = T(integer(-3));
    Wp[2] = T(integer(1));
    polynomial<T> p(Wp); // (x-1)(x-2)

    LEDA_VECTOR<T> Wq(3);
    Wq[0] = T(integer(24));
    Wq[1] = T(integer(-14));
    Wq[2] = T(integer(2));
    polynomial<T> q(Wq); // 2(x-3)(x-4)

    LEDA_VECTOR<T> Wr(2);
    Wr[0] = T(integer(-5));
    Wr[1] = T(integer(1));
    r = polynomial<T>(Wr); // (x-5)

    LEDA_VECTOR<T> Ws(5);
    Ws[0] = T(integer(43));
    Ws[1] = T(integer(-99));
    Ws[2] = T(integer(70));
    Ws[3] = T(integer(-20));
    Ws[4] = T(integer(2));
    polynomial<T> s(Ws); // s = p*q + r

    polynomial<T> Q, R;
    s.euclidean_division(p, Q, R);  // works, since p.lcoeff() == 1
    assert( Q == q );
    assert( R == r );

}

template <class T>
void gcds() {

    // test univariate polynomial gcd and resultant over a field
    LEDA_VECTOR<T> V(6);
    V[0] = T(integer(2));
    V[1] = T(integer(7));
    V[2] = T(integer(1));
    V[3] = T(integer(8));
    V[4] = T(integer(1));
    V[5] = T(integer(8));
    polynomial<T> f(V);

    LEDA_VECTOR<T> W(6);
    W[0] = T(integer(3));
    W[1] = T(integer(1));
    W[2] = T(integer(4));
    W[3] = T(integer(1));
    W[4] = T(integer(5));
    W[5] = T(integer(9));
    polynomial<T> g(W);

    LEDA_VECTOR<T> X(4);
    X[0] = T(integer(3));
    X[1] = T(integer(4));
    X[2] = T(integer(7));
    X[3] = T(integer(7));
    polynomial<T> h(X);

    f /= T(integer(3));
    g /= T(integer(5));
    h /= h.lcoeff();

    polynomial<T> d;

    d = poly_gcd(f, g);
    T c(integer(1));
    assert( d == polynomial<T>(c) );

    /* This is ok for rationals but takes too long for reals
        For bigfloats the result is not correct.

    LEDA_VECTOR<T> Vh(9);
    Vh[0] = T(integer(6));
    Vh[1] = T(integer(29));
    Vh[2] = T(integer(45));
    Vh[3] = T(integer(91));
    Vh[4] = T(integer(91));
    Vh[5] = T(integer(91));
    Vh[6] = T(integer(95));
    Vh[7] = T(integer(63));
    Vh[8] = T(integer(56));
    polynomial<T> fh(Vh); // f*h
    fh /= T(integer(9));

    LEDA_VECTOR<T> Wh(9);
    Wh[0] = T(integer(9));
    Wh[1] = T(integer(15));
    Wh[2] = T(integer(37));
    Wh[3] = T(integer(47));
    Wh[4] = T(integer(54));
    Wh[5] = T(integer(82));
    Wh[6] = T(integer(78));
    Wh[7] = T(integer(98));
    Wh[8] = T(integer(63));
    polynomial<T> gh(Wh); // g*h
    gh /= T(integer(15));

    d = poly_gcd(fh, gh);
    assert( d == h );

    */
}





template <class T>
void basic_tests() {
    datastru<T>(); 
    datastru<polynomial<T> >(); 
    arithmetic<T>(); 
    signs<T>();
    division<T>();
    gcds<T>();
}

int main() {

    cout << "\nStart program polynomial_test. \n";

    cout << "rational test: ";
    basic_tests<rational>();
    cout << " ... done \n";

    cout << "bigfloat test: ";
    basic_tests<bigfloat>();
    cout << " ... done \n";

    cout << "real test: ";
    basic_tests<real>();
    cout << " ... done \n";

    cout << "Time: " << used_time() << endl;
    return 0;
}


