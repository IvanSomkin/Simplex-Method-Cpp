#ifndef VECTOR_GET_H
#define VECTOR_GET_H

#include <vector>
#include <fstream>

// Functions for getting vector and vector<vector> out of std::ifstream

std::vector<double> get_vector(std::ifstream& input, int vector_length) {
	
	std::vector<double> vec(vector_length);
	
	for (int i = 0; i < vector_length; i++)
		input >> vec[i];
	
	return vec;
	
}

std::vector<std::vector<double> > get_matrix(std::ifstream& input, int matrix_width, int matrix_height) {
	
	std::vector<std::vector<double> > mat;
	
	for (int i = 0; i < matrix_height; i++) {
		
		std::vector<double> row(matrix_width);
		
		for(int j = 0; j < matrix_width; j++)
			input >> row[j];
			
		mat.push_back(row);
		
	}
	
	return mat;
	
}

#endif
