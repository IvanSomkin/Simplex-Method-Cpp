#include "simplex.h"
#include "vector/vector_print.h"
#include "vector/vector_read.h"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <stdexcept>


// Needed Math


double epsilon = 1e-8;

double round_eps(double a) {
	
	double round_a = round(a);
	
	if (std::abs(a - round_a) < epsilon) {
		return round_a;
	} else {
		return a;
	}
		
}

bool system_consistent(std::vector<std::vector<double>> a_mat, std::vector<double> b_vec) {
	
	int rows = a_mat.size();
	
	if (rows == 0) {
		throw new std::runtime_error("Can't check system consistency. No rows.");
	}
	
	if (rows != b_vec.size()) {
		throw new std::runtime_error("Can't check system consistency. B vector height not equal to A matrix height.");
	}
	
	int columns = a_mat[0].size();
	
	if (columns == 0) {
		throw new std::runtime_error("Can't check system consistency. No columns in first row.");
	}
		
	for (int i = 1; i < rows; i++) {
		
		if (a_mat[i].size() != columns) {		
			throw new std::runtime_error("Can't check system consistency. Rows have different lengths.");
		}
	}
	
	bool all_zeroes;
	
	// Initial 'all zeroes' check
	
	for (int i = 0; i < rows; i++) {
		
		all_zeroes = std::all_of(a_mat[i].begin(), a_mat[i].end(), [](int i) { return i == 0; });
		
		if (all_zeroes && b_vec[i] != 0) {
			return false;	
		}
	}
	
	// Gausian Elimination
	
	int last;
	
	for (int j = 0; j < columns; j++) {
		
		last = -1;
		
		// Normalize rows below and including 'j' by their element in column 'j'
		
		for (int i = j; i < rows; i++) {
		
			if (a_mat[i][j] != 0) {
			
				double divider = a_mat[i][j];
			
				for (int k = j; k < columns; k++)
					a_mat[i][k] /= divider;
					
				b_vec[i] /= divider;
				
				last = i;
			}
		}
		
		// Check that column 'j' is not all zeroes
		
		if (last != -1) {
			
			// Place at row 'j' the last row with a non-zero element in column 'j'
			
			a_mat[j].swap(a_mat[last]);
			
			bool all_zero_row;
			
			// Subtract row 'j' from every row below with a non-zero in column 'j'
			
			for (int i = j + 1; i < rows; i++) {
				
				all_zero_row = true;
				
				if (a_mat[i][j] != 0) {
				
					for (int k = j; k < columns; k++) {
						
						a_mat[i][k] -= a_mat[j][k];
						
						if (a_mat[i][k] != 0) {
							all_zero_row = false;
						}
					}
					
					b_vec[i] -= b_vec[j];
					
					if (all_zero_row && b_vec[i] != 0) {
						return false;	
					}
				}
			}
		}	
	}
	
	return true;
	
}

// Reading


void SimplexTask::read_task_from_file(std::string file_path) {
	
	std::ifstream input;
	
	input.open(file_path);
	
	if (!input) {
    	throw std::runtime_error("Could not open file!");
	}
    	
	input >> n >> m;
	
	f_vector_task = read_vector(input, n);
    
    a_matrix_task = read_matrix(input, n, m);

	b_vector_task = read_vector(input, m);
	
	sign = read_vector(input, m);
	
	input >> task_max;
	
	input.close();
	
	solved = false;
	
	read = true;
	
}


// Printing


void SimplexTask::print_task() {
	
	std::cout << "F = ";
	print_vector(f_vector_task);
	std::cout << std::endl << std::endl;

    std::cout << "A = ";
	print_matrix(a_matrix_task);
	
	std::cout << "B = ";
	print_vector(b_vector_task);
	std::cout << std::endl << std::endl;
	
	std::cout << "signs = ";
	print_vector(sign);
	std::cout << std::endl << std::endl;
	
	std::cout << "type = " << (task_max ? "max" : "min");
	std::cout << std::endl << std::endl;
		
}

void SimplexTask::print_simplex_matrix() {
	
	std::vector<std::vector<double>> simplex_matrix = a_matrix;
	
	for (int i = 0; i < m; i++) {
		simplex_matrix[i].push_back(b_vector[i]);
	}
	
	std::cout << "\t";
	
	for (int i = 0; i < n + m; i++) {	
		std::cout << std::setw(MATRIX_CELL) << "x" + std::to_string(i + 1);
	}
	
	std::cout << std::setw(MATRIX_CELL) << "b";
	std::cout << std::endl << std::endl;

	print_matrix(simplex_matrix);
	
	std::cout << "d";	
	print_matrix_row(d_vector);
	std::cout << std::endl << std::endl;
	
	std::cout << "Basis variables (from 0) = ";
	print_vector(i_vector);
	std::cout << std::endl << std::endl;
	
}

void SimplexTask::print_preparation() {

	std::cout << "Simplex prepared:" << std::endl << std::endl;	
	print_simplex_matrix();
	std::cout << std::endl;

	std::vector<double> basic_solution;

	basic_solution.assign(n + m, 0);
	for (int i = 0; i < m; i++)
		basic_solution[n + i] = b_vector[i];
		
	std::cout << "First basic solution = ";
	print_vector(basic_solution);
	std::cout << std::endl << std::endl << std::endl;
	
}

void SimplexTask::print_pivots() {

	std::cout << "Pivot column: " << pivot_column << std::endl;
	std::cout << "Pivot row: " << pivot_row << std::endl;
	std::cout << "Pivot element: " << pivot_element << std::endl << std::endl;

}

void SimplexTask::print_solution() {

	std::cout << std::endl;	
	std::cout << "Solution = ";
	print_vector(solution);
	std::cout << std::endl << std::endl << std::endl;
	
}


// Solving


bool SimplexTask::is_in(int i, std::vector<int> vec) {
	
	return std::find(vec.begin(), vec.end(), i) != vec.end();
	
}

bool SimplexTask::is_bad(int i) {	
	
	return i >= n && sign[i - n] == 0;
	
}

bool SimplexTask::find_bad_i_row() {

	for (int i = 0; i < m; i++) {
	
		if (is_bad(i_vector[i]) || b_vector[i_vector[i]] < 0) {
			pivot_row = i;
			return true;	
		}
	}
	
	return false;
	
}

bool SimplexTask::find_first_good_column() { 
	 
	pivot_column = -1; 
	 
	for (int i = 0; i < n + m; i++) {
	 
		if (a_matrix[pivot_row][i] != 0 && !is_bad(i)) { 
		
			pivot_column = i;
			return true; 
		}
	}
	
	result = invalid;
	return false; 
	
} 

bool SimplexTask::find_pivot_column() {
	
	pivot_column = -1;
	
	double d = 0;	

	result = one;

	for (int i = 0; i < n + m; i++) {
	
		if (task_max == (d_vector[i] < d) && d_vector[i] != 0 && !is_bad(i)) {	
				
			pivot_column = i;
			d = d_vector[i];	
		}
		
		if (d_vector[i] == 0 && !is_in(i, i_vector)) {
			result = many;
		}
	}
	
	if (pivot_column == -1) {
		return false;
	}
	
	result = none;
	return true;
	
}

bool SimplexTask::find_pivot_row() {

	pivot_row = -1;
	
	std::vector<double> b_ratio_vector = b_vector;
	
	double b_ratio = 0;
	
	for (int i = 0; i < b_vector.size(); i++) {
		
		b_ratio_vector[i] /= a_matrix[i][pivot_column];
		
		if (b_ratio_vector[i] >= 0 && (b_ratio_vector[i] < b_ratio || b_ratio == 0)) {
			
			pivot_row = i;
			b_ratio = b_ratio_vector[i];
		}	
	}	
	
	if (pivot_row == -1) {	
		result = unrestricted;
		return false;
	}
	
	return true;
	
}

bool SimplexTask::solve() {
	
	if (read) {
	
		solved = false;
	
		a_matrix = a_matrix_task;
		
		f_vector = f_vector_task;
		
		b_vector = b_vector_task;
		
		f_vector.resize(n + m, 0);
		
		// Set all additional variables as basis variables
		
		for (int i = n; i < n + m; i++) {
			i_vector.push_back(i);
		}
		
		// Set D vector with start coefficients
		
		d_vector.clear();
		
		for (int i = 0; i < n; i++)
			d_vector.push_back(-f_vector[i]);
			
		for (int i = 0; i < m; i++)
			d_vector.push_back(0);
		
		// Add 1 additional variable to each row
		
		for (int i = 0; i < m; i++) {
			
			a_matrix[i].resize(n + m);
			a_matrix[i][n + i] = (sign[i] == 0 ? 1 : -sign[i]);	
		}	
		
		result = none;
		
		if (!system_consistent(a_matrix_task, b_vector_task)) {
			
			result = invalid;
		}
		
		print_preparation();
		
		while (result == none) {
			
			// Find first bad variable in basis
		
			if (find_bad_i_row()) {
								
				// Bad variable in basis at pivot_row
				
				if (!find_first_good_column()) {
					
					// Result: invalid
					
					break;
				}
		    }
			
			else {
		
				// No bad variables in basis
				
				if (!find_pivot_column()) {
					
					// Result: one || many
					
					break;
				}
				
				if (!find_pivot_row()) {
					
					// Result: unrestricted
					
					break;
				}	
			}
			
			pivot_element = a_matrix[pivot_row][pivot_column];
			
			print_pivots();
			
			// Count new B vector
			
			b_vector[pivot_row] = round_eps(b_vector[pivot_row] / pivot_element);
			
			for (int i = 0; i < m; i++) {
					
				if (i != pivot_row) {	
							
					b_vector[i] = round_eps(b_vector[i] - b_vector[pivot_row] * a_matrix[i][pivot_column]);
				}
			}
			
			// Count new A matrix coefficients
			
			for (int i = 0; i < n + m; i++)	{	
			
				a_matrix[pivot_row][i] = round_eps(a_matrix[pivot_row][i] / pivot_element);
			}
				
			double column_coef;	
			
			for (int i = 0; i < m; i++)	{
			
				if (i != pivot_row) {
					
					column_coef = a_matrix[i][pivot_column];
					
					for (int j = 0; j < n + m; j++) {
						
						a_matrix[i][j] = round_eps(a_matrix[i][j] - a_matrix[pivot_row][j] * column_coef);	
					}
				}
			}
				
			// Move variable to basis
			
			i_vector[pivot_row] = pivot_column;
			
			// Count new D coefficients
			
			double d_sum;	
			
			for (int i = 0; i < n + m; i++) {
				
				d_sum = 0;
				
				for (int j = 0; j < m; j++) {
					d_sum += f_vector[i_vector[j]] * a_matrix[j][i];
				}
	
				d_vector[i] = round_eps(d_sum - f_vector[i]);	
			}
			
			print_simplex_matrix();
			
			solution.assign(n + m, 0);
		
			for (int i = 0; i < m; i++) {
				solution[i_vector[i]] = b_vector[i];
			}
			
			print_solution();
		}
	
		if (result == one || result == many) {
			
			solution.assign(n, 0);
			
			for (int i = 0; i < m; i++) {
				solution[i_vector[i]] = b_vector[i];	
			}
		
			solved = true;
		}
		
		return solved;	
	}
	
	throw new std::runtime_error("Can't solve. Task not read.");

}


// Result


std::vector<double> SimplexTask::get_result_solution() {
	
	if (solved) {	
		return solution;
	} else {
		throw new std::runtime_error("Can't get result solution. Task not solved.");
	}
	
}

double SimplexTask::get_result_value() {
	
	if (solved) {
	
		double result = 0;
		
		for (int i = 0; i < n; i++)
			result += f_vector[i] * solution[i];
			
		return result;	
	}	
	
	else {	
	
		throw new std::runtime_error("Can't get result. Task not solved.");
	}
	
}

std::string SimplexTask::get_result_statement() {
	
	switch (result) {

		case one: return "Single solution found for result value.";
			
		case many: return "Infinite solutions found for result value.";
			
		case unrestricted: return "Can't solve. Function not restricted by conditions.";
			
		case invalid: return "Can't solve. No possible solution.";
	
		default: return "No result. Did not attempt to solve() or unknown error.";
	}
	
}
