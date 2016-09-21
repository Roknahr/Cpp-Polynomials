// Author: Dan Skøtt Petersen
// E-mail: dspe11@student.aau.dk
#pragma once
#include "stdafx.h"
#include "Polynomial.h"

//using namespace std;

int main()
{
    /*
     * A demonstration that the implementation of the 
     * assignments actually work.
     */

    //1. (a) Trivial polynomial: 0
    Polynomial<double> pol;
    std::cout << "1. (a) - Trivial polynomial: " << pol << std::endl;

    //1. (b)  Constructor for specific degree term coefficients.
    //Expect: 1 + 2x^1 + 3x^2
    //Also works with any container type, 
    //e.g. Polynomial<double> pol2b(std::vector<double>{1, 2, 3})
    Polynomial<double> pol2{ 1.0, 2.0, 3.0 };
    std::cout << "1. (b) - Specific degree: " << pol2 << std::endl;

    //1. (c) Scale the polynomial
    //Expect: 2 + 4x^1 + 6x^2
    pol2.scale(2);
    std::cout << "1. (c) - Scale: " << pol2 << std::endl;

    //1. (d) Add root
    //Expect: -5 + 1x^1
    Polynomial<double> pol3(std::vector<double>{ 1.0 });
    pol3.add_root(5.0);
    std::cout << "1. (d) - Add root: " << pol3 << std::endl;

    //1. (e) Add several roots
    //Expect: -10 + 17x^1 + -8x^2 + 1x^3
    double c_array[2] = { 1, 2 };
    pol3.add_roots(c_array);
    std::cout << "1. (e) - Add roots: " << pol3 << std::endl;

    //1. (f) Evaluate
    //Expect: 0 at 2 (2 is a root), 60680 at 42
    std::cout << "1. (f) - Evaluate at 2: " << pol3.eval(2) 
        << " and 42: " << pol3.eval(42) << std::endl;

    //1. (g) Derivative
    //Expect: 4 + 12x^1
    std::cout << "1. (g) - Derivative of 1. (c): " 
        << pol2.derive() << std::endl;

    //1. (h) Integral
    //Expect: 520
    std::cout << "1. (h) - Integrate 1. (c) in range -5 to 5: "
        << pol2.integrate(-5, 5) << std::endl;

    //1. (i) Plus operator
    //Expect: 4 + 8x^1 + 12x^2
    std::cout << "1. (i) - Plus operator: "<< pol2 + pol2 << std::endl;

    //1. (j) Star operator
    //Expect 4 + 16x^1 + 40x^2 + 48x^3 + 36x^4
    //Note: Due to it's (not so good) implementation it calls some
    //      move operators and a move ctor. This is printed as well.
    std::cout << "1. (j) - Star operator: ";
    std::cout << pol2 * pol2 << std::endl;

    //5. Member functions accepting containers should accept any kind
    //Let's add more roots to pol3!
    //Expect 3 with more roots:
    //-300 + 620x^1 + -437x^2 + 135x^3 + -19x^4 + 1x^5
    pol3.add_roots(std::vector<double> {5, 6});
    std::cout << "5. - Any container: " << pol3 << std::endl;

    //6. Cache integral
    //Expect: 2 messages "Fetched from cache!" when values
    //        for -5 and 5 are fetched. Same result: 520
    std::cout << "6. - Integrate from cache: ";
    std::cout << pol2.integrate(-5, 5) << std::endl;

    //7. Implement move semantics
    //Expect: Move, then copy.
    std::cout << "7. - Move semantics: ";
    Polynomial<double> pol4(std::move(pol2)); // Move ctor.
    Polynomial<double> pol5(pol4); // Copy ctor.
    std::cout << std::endl;

    //8. Type traits
    //Cannot integrate int polynomials. If you uncomment the
    //lines below, you will get an error from static_assert.

    /*Polynomial<int> pol6{ 1, 2 };
    pol6.integrate(-1, 1);*/

    return 0;
}

