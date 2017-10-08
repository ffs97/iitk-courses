/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  additional_tests.cpp
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/numbers/bigfloat.h>
#include <LEDA/numbers/fp.h>
#include <LEDA/numbers/interval_constants.h>

#include <LEDA/numbers/interval.h>
#include <LEDA/numbers/real.h>
#include <LEDA/numbers/rational.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::flush;
using std::streambuf;
using std::ostream;
using std::istream;
using std::ifstream;
#endif

#define FAILURE exit(1)

list<double> generate_test_doubles()
{
	const unsigned long max_exp = 2046; // Note: 2047 is infinity / NaN
	const unsigned long max_most_sig  = 0xFFFFF;    // 20 bits all set to 1
	const unsigned long max_least_sig = 0xFFFFFFFF; // 32 bits all set to 1

	const double MaxDbl = fp::compose_parts(0, max_exp, max_most_sig, max_least_sig);
	const double DenormalizedMaxDbl = fp::compose_parts(0, 0, max_most_sig, max_least_sig);

	list<double> result;

	// test zeros
	result.append( fp::pZero );
	result.append( fp::nZero );

	// test numbers from interval_constants.h
	result.append( eps );                // = 2^(-52)
	result.append( DenormalizedMinDbl ); // = 2^(-1074)
	result.append( MinDbl );             // = 2^(-1022)
	result.append( MaxDbl );             //
	result.append( twoMinDbl ); 
	result.append( threeMinDbl );
	result.append( MaxError );
	result.append( roundup_factor );
	result.append( rounddown_factor );

	// test border cases
	result.append( MinDbl ); // = 2^(-1022)
	result.append( MaxDbl );
	result.append( DenormalizedMinDbl ); // = 2^(-1074)
	result.append( DenormalizedMaxDbl );

	// test something else
	result.append( 1.0 );
	result.append( -10.45 );
	result.append( 13.321 );

	return result;
}

list<bigfloat> generate_test_bigfloats()
{
	list<bigfloat> result;

	// add all test doubles
	list<double> test_doubles = generate_test_doubles();
	double d;
	forall(d, test_doubles) result.append( bigfloat(d) );

	{ // mainly denormalized doubles ...
		for (int i = 1; i <= 57; ++i) {        // at least [1;53]
			for (int j=1000; j <= 1100; ++j) { // at least [1022;1075]
				bigfloat b1(1, i);
				b1 -=1; // 2^(i)-1 = 111...1 (i ones)
				bigfloat b2(1, -(i-1) + (-j));
				bigfloat b = b1 * b2; // 1 . 1 1 ... 1 * 2^(-j) (i ones in total)
				result.append(b);
			}
		}
	}

	{ // very large doubles ...
		for (int i = 1; i <= 57; ++i) {        // at least [1;53]
			for (int j=1000; j <= 1080; ++j) { // around 1022
				bigfloat b1(1, i);
				b1 -=1; // 2^(i)-1 = 111...1 (i ones)
				bigfloat b2(1, -(i-1) + j);
				bigfloat b = b1 * b2; // 1 . 1 1 ... 1 * 2^(-j) (i ones in total)
				result.append(b);
			}
		}
	}

	return result;
}

list<rational> generate_test_rationals()
{
	list<rational> result;

	// add all test doubles
	list<double> test_doubles = generate_test_doubles();
	double d;
	forall(d, test_doubles) result.append( rational(d) );

	result.append( rational(2, 10) );

	return result;
}

bool verify_conversion(const bigfloat& b, double d, double abs_err)
{
	bigfloat::set_rounding_mode(EXACT);
	bigfloat abs_err_bf(abs_err);
	bigfloat exact_abs_err_bf = abs(b - bigfloat(d));
	if ( exact_abs_err_bf != abs_err_bf && exact_abs_err_bf > abs_err_bf ) {
		// Note: first test handles infinity!
		return false;
	}
	return true;
}

bool verify_conversion(const rational& r, double d, double abs_err)
{
	int prec = 53;
	bigfloat::set_rounding_mode(EXACT);
	bigfloat abs_err_bf(abs_err);
	bool is_exact = false;
	bigfloat b = bigfloat::from_rational(r,prec,TO_NEAREST,is_exact);
	bigfloat exact_abs_err_bf = abs(b - bigfloat(d));
	if (! is_exact) exact_abs_err_bf += bigfloat(1, -prec) * abs(b); 
	if ( exact_abs_err_bf != abs_err_bf && exact_abs_err_bf > abs_err_bf ) {
		// Note: first test handles infinity!
		return false;
	}
	return true;
}


/// bigfloat -> double

void test_bf_to_dbl_mode(const bigfloat& b, rounding_modes m)
{
	// convert b to a double value in the given rounding mode
	double abs_err;
	double d = b.to_double(abs_err, m);

	bigfloat d_bf(d);

	// check abs_err
	if (! verify_conversion(b, d, abs_err)) {
		bigfloat exact_abs_err_bf = abs(b - d_bf);
		cout << "error bound too small, m = " << m
			 << " / exact_abs_err = " << exact_abs_err_bf
			 << " / abs_err = " << bigfloat(abs_err) << endl;
		FAILURE;
	}

	if (m == TO_P_INF) {
		m = sign(b) > 0 ? TO_INF : TO_ZERO;
	}
	if (m == TO_N_INF) {
		m = sign(b) < 0 ? TO_INF : TO_ZERO;
	}

	if (m == TO_ZERO) {
		if (d_bf != b && abs(d_bf) > abs(b)) {
			cout << "error TO_ZERO, d_bf = " << d_bf << " / b = " << b << endl;
			FAILURE;
		}
	}
	if (m == TO_INF) {
		if (d_bf != b && abs(d_bf) < abs(b)) {
			cout << "error TO_INF, d_bf = " << d_bf << " / b = " << b << endl;
			FAILURE;
		}
	}

	if (m == TO_NEAREST) {
		double d2 = b.to_double();
		if (d != d2) {
			cout << "error TO_NEAREST, d = " << d << " != d2 / d2 = " << d2 << endl;
			FAILURE;
		}
	}
}

void test_bf_to_dbl_raw(const bigfloat& b)
{
	test_bf_to_dbl_mode(b, TO_NEAREST);
	test_bf_to_dbl_mode(b, TO_ZERO);
	test_bf_to_dbl_mode(b, TO_INF);
	test_bf_to_dbl_mode(b, TO_P_INF);
	test_bf_to_dbl_mode(b, TO_N_INF);
	test_bf_to_dbl_mode(b, EXACT);
}

void test_bf_to_dbl(const bigfloat& b)
{
  test_bf_to_dbl_raw( b);
  test_bf_to_dbl_raw(-b);
}

void test_dbl_to_bf_to_dbl_raw(double d)
{
  bigfloat b(d);
//  test_bf_to_dbl(b);

  double abs_err;
  double d1 = b.to_double(abs_err, TO_NEAREST);

  if (d1 != d) {
    cout << "d = " << d << " != d1 / d1 = " << d1;
    FAILURE;
  }

  if (abs_err != 0) {
    cout << "d = " << d << " / abs_err = " << abs_err;
    FAILURE;
  }
}

void test_dbl_to_bf_to_dbl(double d)
{
  test_dbl_to_bf_to_dbl_raw( d);
  test_dbl_to_bf_to_dbl_raw(-d);
}

void test_conversion_bigfloat_to_double()
{
	bigfloat::set_rounding_mode(EXACT);

	//*
	list<double> test_doubles = generate_test_doubles();
	double d;
	forall(d, test_doubles) test_dbl_to_bf_to_dbl( d );
	//*/

	//*
	list<bigfloat> test_bigfloats = generate_test_bigfloats();
	bigfloat b;
	forall(b, test_bigfloats) test_bf_to_dbl( b );
	//*/

}


/// bigfloat -> interval

template <class interval>
void test_bf_to_intvl(const bigfloat& b, bool may_change = false)
{
	double abs_err;
	double d = b.to_double(abs_err, TO_NEAREST);

	interval I1(b);
	double d1 = I1.to_double();
	double abs_err1 = I1.get_double_error();
	if (! verify_conversion(b, d1, abs_err1)) {
		cout << "error in conversion b -> interval with b = " << b << endl;
		cout << "d = " << d << " / d1 = " << d1 << endl;
		cout << "abs_err = " << abs_err << " / abs_err1 = " << abs_err1 << endl;
		FAILURE;
	}
	if ( (d1 != d || abs_err1 != abs_err) && !may_change ) {
		cout << "change in conversion b -> interval with b = " << b << endl;
		cout << "d = " << d << " / d1 = " << d1 << endl;
		cout << "abs_err = " << abs_err << " / abs_err1 = " << abs_err1 << endl;
	}

	real r(b);
	interval I2(r);
	double d2 = I2.to_double();
	double abs_err2 = I2.get_double_error();
	if (fp::is_nan(d2)) return;
	if (! verify_conversion(b, d2, abs_err2)) {
		cout << "error in conversion b -> interval with r = " << r << endl;
		cout << "d = " << d << " / d2 = " << d2 << endl;
		cout << "abs_err = " << abs_err << " / abs_err2 = " << abs_err2 << endl;
		FAILURE;
	}
}

template <class interval>
void test_conv_to_intvl(bool may_change = false)
{
	list<bigfloat> test_bigfloats = generate_test_bigfloats();

	test_bf_to_intvl<interval>( bigfloat(0) );

	{
		bigfloat b1(1, 55);
		b1 -= 1;
		bigfloat b2(1, -(55-1) + 100);
		bigfloat b = b1 * b2;

		test_bf_to_intvl<interval>(b, may_change);
	}

	test_bf_to_intvl<interval>(bigfloat(1, -1076), may_change);

	bigfloat b;
	forall(b, test_bigfloats) {
		test_bf_to_intvl<interval>(b, may_change);
	}
}

void test_conversion_to_interval()
{
#if defined(_MSC_VER) && _MSC_VER < 1300
	bool bound_abs_may_change = true;
#else
	bool bound_abs_may_change = false;
#endif
	
	test_conv_to_intvl<interval_bound_absolute>(bound_abs_may_change);
	test_conv_to_intvl<interval_bound_relative>(true);
#if !defined(LEDA_NO_FPU)
	test_conv_to_intvl<interval_round_inside>(true);
#endif
}


/// rational -> double

void test_rat_to_dbl_raw(const rational& r)
{
	// convert r to a double value
	double abs_err;
	double d = r.to_double_precise(abs_err);

	bigfloat d_bf(d);

	// check abs_err
	if (! verify_conversion(r, d, abs_err)) {
		cout << "error bound too small, r = " << r
			 << " / abs_err = " << bigfloat(abs_err) << endl;
		FAILURE;
	}
}

void test_rat_to_dbl(const rational& r)
{
  test_rat_to_dbl_raw( r);
  test_rat_to_dbl_raw(-r);
}

void test_dbl_to_rat_to_dbl_raw(double d)
{
  rational r(d);

  double abs_err;
  double d1 = r.to_double_precise(abs_err);

  if (d1 != d) {
    cout << "d = " << d << " != d1 / d1 = " << d1;
    FAILURE;
  }

  if (abs_err != 0) {
    cout << "d = " << d << " / abs_err = " << abs_err;
    FAILURE;
  }
}

void test_dbl_to_rat_to_dbl(double d)
{
  test_dbl_to_rat_to_dbl_raw( d);
  test_dbl_to_rat_to_dbl_raw(-d);
}

void test_conversion_rational_to_double()
{
	//*
	list<double> test_doubles = generate_test_doubles();
	double d;
	forall(d, test_doubles) test_dbl_to_rat_to_dbl( d );
	//*/

	//*
	list<rational> test_rationals = generate_test_rationals();
	rational r;
	forall(r, test_rationals) test_rat_to_dbl( r );
	//*/
}

int main()
{
	cout << "test conversion bigfloat -> double ..." << endl;
	test_conversion_bigfloat_to_double();
	cout << endl;

	cout << "test conversion bigfloat -> interval ..." << endl;
	test_conversion_to_interval();
	cout << endl;

	cout << "test conversion rational -> double ..." << endl;
	test_conversion_rational_to_double();
	cout << endl;

	cout << "all tests completed successfully!" << endl;

	return 0;
}
