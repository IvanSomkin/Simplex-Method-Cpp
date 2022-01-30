# Educational Simplex Method for C++11
A bare bones algorithm for finding max and min values of a function based on the system of conditions for the variables (linear optimization task).

simplex.cpp - An example of a program that uses the SimplexTask class.

simplex.h - Header file containing SimplexTask and it's realization.

vectorprint.h - Header file containing vector printing functions.

vectorget.h - Header file containing vector reading functions.

examples - A collection of examples that can be used to check the algorithm.

Input file format:

4 2 // Number of variables in the function, number of conditions in the system

-6 -1 -4 5 // Coefficients of the function

// System coefficients 3 1 -1 1 5 1 1 -1

4 4 // System constant terms

0 0 // System signs: -1 - "<=", 0 - "=", 1 - ">="

0 // Task type: 0 - min, 1 - max