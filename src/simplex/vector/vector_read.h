#ifndef VECTOR_GET_H
#define VECTOR_GET_H

#include <vector>
#include <fstream>

std::vector<double> read_vector(std::ifstream& input, int vector_length);

std::vector<std::vector<double>> read_matrix(std::ifstream& input, int matrix_width, int matrix_height);

#endif
