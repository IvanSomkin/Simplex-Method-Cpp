# Educational Simplex Method for C++11
A bare bones algorithm for finding max and min values of a function based on the system of conditions for the variables (linear optimization task).

**src/main.cpp** - Main entry point into the program with a code example.

**src/simplex/simplex.h** - Header file containing declarations for the SimplexTask class.

**src/simplex/simplex.cpp** - Implementation of the SimplexTask class.

**src/simplex/vector/vector_print.h** - Header file containing vector printing functions.

**src/simplex/vector/vector_print.cpp** - Implementation of the printing functions.

**src/simplex/vector/vector_get.h** - Header file containing vector reading functions.

**src/simplex/vector/vector_get.cpp** - Implementation of the reading functions.

**examples** - A collection of examples that can be used to check the algorithm.

### Input file format:

*Number of variables, number of conditions*

**4 2**

*Coefficients of the function*

**-6 -1 -4 5**

*System coefficients*

**3	1 -1 1**  
**5	1 1 -1**

*System constant terms*

**4 4**

*System signs: -1 - "<=", 0 - "=", 1 - ">="*

**0 0**

*Task type: 0 - min, 1 - max*

**0**
