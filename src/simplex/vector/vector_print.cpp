#include "vector_print.h"

#include <iostream>
#include <iomanip>

int MATRIX_CELL = 10;

void print_vector(std::vector<int> vec) {
	
	std::cout << "( ";
    for (int n : vec)
        std::cout << n << " ";
    std::cout << ")";

}

void print_vector(std::vector<double> vec) {
	
	std::cout << "( ";
    for (double n : vec)
        std::cout << n << " ";
    std::cout << ")";
    
}

void print_matrix_row(std::vector<double> vec) {

	std::cout.setf(std::ios::fixed);
	std::cout << std::setprecision(2);

	std::cout << "\t";
	for (double n : vec)
		std::cout << std::setw(MATRIX_CELL) << n;
		
	std::cout.unsetf(std::ios::fixed);
	std::cout << std::setprecision(6);

}

void print_matrix(std::vector<std::vector<double>> mat) {
	
	int matrix_height = mat.size();	
	
	for (int i = 0; i < matrix_height; i++) {
		
    	print_matrix_row(mat[i]);
    	std::cout << std::endl << std::endl;
	}
	
}
