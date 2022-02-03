#ifndef VECTOR_PRINT_H
#define VECTOR_PRINT_H

#include <vector>

extern int MATRIX_CELL;

void print_vector(std::vector<int> vec);

void print_vector(std::vector<double> vec);

void print_matrix_row(std::vector<double> vec);

void print_matrix(std::vector<std::vector<double>> mat);

#endif
