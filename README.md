# C++ - Polynomials
This is the solution to a three day assignment to implement a program in C++ that can perform various operations on polynomials. For detailed requirements, see Assignments.pdf.


## 1. COMPILATION
This project is made in Visual Studio 2013 (v120), and should be compiled with the default VS13 compiler, which includes most C++11 functionality and some C++14 functionality.

For Test to compile, the Boost paths in the project properties must be correct. Right click the "Test" project and select "Properties". Go to "VC++ Directories" and change "Include Directories" and "Library Directories" accordingly.

To run unit tests, set the "Test" project as StartUp Project (right click) and build it.

To get console output with demonstration of demonstratable assignments, set "dspe11_ap_mp2" as StartUp Project and compile (Ctrl + F5).


## 2. INPUT / OUTPUT
Inputs should be typed directly in the main function in dspe11_ap_mp2.cpp. Expected outputs are written there as well.

Unit tests are found in the Test project in the file TestPolynomial.cpp.


## 3. DETAILED REQUIREMENTS
All detailed requirements have been implemented, though I am not proud of the implementation of 1. (j).

Includes are handled in the stdafx.h for each project.
Aside from that, interesting files are:
- dspe11_ap_mp2.cpp (main)
- Polynomial.h (template class)
- TestPolynmial.cpp (boost unit tests)
