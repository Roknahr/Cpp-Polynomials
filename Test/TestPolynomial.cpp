#include "stdafx.h"
#include "Polynomial.h"

#define BOOST_TEST_MODULE TestPolynomial

//The program has not been extensively tested, but all methods have
//at least one test (except operator<<).

BOOST_AUTO_TEST_CASE(test_default_constuctor)
{
    // The default constructor should create a trivial polynomial: 0
    Polynomial<int> pol;
    auto coefficients = pol.get_coefficients();
    
    BOOST_CHECK_EQUAL(coefficients.size(), 1);
    BOOST_CHECK_EQUAL(coefficients.back(), 0);
}


BOOST_AUTO_TEST_CASE(test_specific_degree_constructor)
{
    // Test the constructor for specific degree term coefficients.
    Polynomial<int> pol{ 1, 2, 3, 4 };
    auto coefficients = pol.get_coefficients();
    int expected_value{ 1 };

    BOOST_CHECK_EQUAL(coefficients.size(), 4);


    for (auto it = coefficients.cbegin(); it != coefficients.cend(); ++it)
    {
        BOOST_CHECK_EQUAL(*it, expected_value++);
    }
}

BOOST_AUTO_TEST_CASE(test_scale_trivial)
{
    Polynomial<int> pol;

    pol.scale((int)2);

    auto coefficients = pol.get_coefficients();

    BOOST_CHECK_EQUAL(coefficients.size(), 1);
    BOOST_CHECK_EQUAL(coefficients.back(), 0);
}

BOOST_AUTO_TEST_CASE(test_scale_non_trivial)
{
    Polynomial<float> pol{ 2.0, 2.0 };

    pol.scale(2.0);

    auto coefficients = pol.get_coefficients();

    BOOST_CHECK_EQUAL(coefficients.size(), 2);
    BOOST_CHECK_EQUAL(coefficients.back(), 4.0);

    coefficients.pop_back();
    BOOST_CHECK_EQUAL(coefficients.back(), 4.0);
}

BOOST_AUTO_TEST_CASE(test_add_root)
{
    Polynomial<int> pol{ 1, 0, 2, 0, 1 };

    pol.add_root(5);

    auto actual = pol.get_coefficients();
    const std::vector<int> expected{ -5, 1, -10, 2, -5, 1 };

    BOOST_CHECK_EQUAL_COLLECTIONS(
        actual.cbegin(), actual.cend(),
        expected.begin(), expected.end());
}

  

BOOST_AUTO_TEST_CASE(test_add_roots)
{
    Polynomial<int> pol{ 1, 0, 2, 0, 1 };

    pol.add_roots(std::vector<int> {5, -4});

    const auto actual = pol.get_coefficients();
    const auto expected{ -20, -1, -39, -2, -18, -1, 1 };

    BOOST_CHECK_EQUAL_COLLECTIONS(
        actual.begin(), actual.end(),
        expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(test_eval)
{
    Polynomial<double> pol{ 1.0, 0.0, 2.0 };

    BOOST_CHECK_EQUAL(pol.eval(42), 3529);
}

BOOST_AUTO_TEST_CASE(test_derivative_observation)
{
    Polynomial<int> pol(std::vector<int>{ 1, 0, 2, 0, 1 });

    auto derived = pol.derive();
    
    auto actual = derived.get_coefficients();
    const auto expected{ 0, 4, 0, 4 };

    BOOST_CHECK_EQUAL_COLLECTIONS(
        actual.cbegin(), actual.cend(),
        expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(test_derivative_math)
{
    //Testing based on:
    //p(x)=(x - r1)(x - r2) <=> p'((r1 + r2) / 2) = 0

    Polynomial<int> pol{ 1 };

    pol.add_roots(std::vector <int> {4, 2});

    //(4+2)/2 = 3
    BOOST_CHECK_EQUAL(pol.derive().eval(3), 0);
}


BOOST_AUTO_TEST_CASE(test_integrate_math)
{
    Polynomial<double> pol{ 1 };

    pol.add_roots(std::vector<double> {5, -5});

    // -4/3*5^3 = -166.667
    const double expected = -166.667;

    BOOST_CHECK_CLOSE(pol.integrate(-5.0, 5.0), expected, 0.001);
}

BOOST_AUTO_TEST_CASE(test_plus_operator)
{
    Polynomial<int> pol1{ 1, 2, 3, 12 };
    Polynomial<int> pol2{ 1, 2, 3 };
    auto pol3 = pol1 + pol2;

    const auto actual = pol3.get_coefficients();
    const auto expected{ 2, 4, 6, 12 };

    BOOST_CHECK_EQUAL_COLLECTIONS(
        actual.begin(), actual.end(),
        expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(test_multiply_operator)
{
    Polynomial<int> pol1{ 1, 2, 3, 4 };
    Polynomial<int> pol2{ 1, 2, 3 };
    auto pol3 = pol1 * pol2;

    const auto actual = pol3.get_coefficients();
    const auto expected{ 1, 4, 10, 16, 17, 12 };

    BOOST_CHECK_EQUAL_COLLECTIONS(
        actual.begin(), actual.end(),
        expected.begin(), expected.end());
}
