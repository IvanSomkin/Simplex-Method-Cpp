#ifndef SIMPLEX_H
#define SIMPLEX_H

#include "vectorprint.h"
#include "vectorget.h"
#include <cmath>
#include <stdexcept>
#include <string>
#include <algorithm>

double epsilon = 1e-8;

double round_eps(double a) {
	
	double round_a = round(a);
	
	if (std::abs(a - round_a) < epsilon)
		return round_a;
	else
		return a;
		
}

// Types of result

enum Result { one, many, unrestricted, invalid, none };

// Main task class

class SimplexTask {
	
	// Task
	
	int n, m; // n - number of variables, m - number of conditions

	std::vector<double> f_vector_task;
	
	std::vector<std::vector<double>> a_matrix_task;
	
	std::vector<double> b_vector_task;
	
	std::vector<double> sign; // condition signs:   -1 - '<='   0 - '='   1 - '>='
	
	bool task_max; // task is max: 0 - min, 1 - max
	
	// Solving Simplex
	
	std::vector<double> f_vector;
	
	std::vector<std::vector<double>> a_matrix;
	
	std::vector<double> b_vector;
	
	std::vector<int> i_vector; // indexes of basis variables
	
	std::vector<double> d_vector; // coefficients for new function	
	
	std::vector<double> solution;
	
	Result result = none;
	
	bool solved = false;
	
	bool read = false;
	
	int pivot_column, pivot_row;
	
	double pivot_element;
	
	// Functions
	
	public:
	
		void read_task_from_file(std::string file_path);
		
		void print_task();
		
		bool solve();
		
		double get_result_value();
		
		std::vector<double> get_result_solution();
		
		std::string get_result_statement();
		
	private:
		
		void print_simplex_matrix();
		
		void print_preparation();
		
		void print_pivots();
		
		void print_solution();
		
		bool is_in(int i, std::vector<int> vec);
		
		bool is_bad(int i);
		
		bool find_bad_i_row();
		
		bool find_first_good_column();
		
		bool find_pivot_column();
		
		bool find_pivot_row();
		
		bool check_stop();
	
};

// Print simplex matrix with markings

void SimplexTask::print_simplex_matrix() {
	
	std::vector<std::vector<double>> simplex_matrix = a_matrix;
	
	for (int i = 0; i < m; i++) {
		simplex_matrix[i].push_back(b_vector[i]);
		simplex_matrix[i].push_back(i_vector[i]);
	}
	
	std::cout << "\t";
	for (int i = 0; i < n + m; i++) 
		std::cout << std::setw(MATRIX_CELL) << "x" + std::to_string(i + 1) << " ";
	std::cout << std::setw(MATRIX_CELL) << "b" << " " << std::setw(MATRIX_CELL) << "basis";
	std::cout << std::endl << std::endl;
	print_matrix(simplex_matrix);
	
	std::cout << "d";	
	print_matrix_row(d_vector);
	std::cout << std::endl;
	
}

// Print task

void SimplexTask::print_task() {
	
	std::cout << "F = ";
	print_vector(f_vector_task);
	std::cout << std::endl;

    std::cout << "A = ";
	print_matrix(a_matrix_task);
	std::cout << std::endl;
	
	std::cout << "B = ";
	print_vector(b_vector_task);
	std::cout << std::endl;
	
	std::cout << "signs = ";
	print_vector(sign);
	std::cout << std::endl;
	
	std::cout << "type = " << (task_max ? "max" : "min") << std::endl;
	std::cout << std::endl;
		
}

// Print preparation lines

void SimplexTask::print_preparation() {

	std::cout << "Simplex prepared." << std::endl << std::endl;	
	print_simplex_matrix();
	std::cout << std::endl;

	std::vector<double> basic_solution;

	basic_solution.assign(n + m, 0);
	for (int i = 0; i < m; i++)
		basic_solution[n + i] = b_vector[i];
		
	std::cout << "First basic solution = ";
	print_vector(basic_solution);
	std::cout << std::endl;
	
}

// Print pivot_row, pivot_column and pivot_element

void SimplexTask::print_pivots() {

	std::cout << "Pivot column: " << pivot_column << std::endl;
	std::cout << "Pivot row: " << pivot_row << std::endl;
	std::cout << "Pivot element: " << pivot_element << std::endl << std::endl;

}

void SimplexTask::print_solution() {

	std::cout << std::endl;	
	std::cout << "Solution = ";
	print_vector(solution);
	std::cout << std::endl << std::endl;
	
}

// Is item in integer vector

bool SimplexTask::is_in(int i, std::vector<int> vec) {
	
	return std::find(vec.begin(), vec.end(), i) != vec.end();
	
}

// Is it an additional variable

bool SimplexTask::is_bad(int i) {	
	
	return i >= n && sign[i - n] == 0;
	
}

// Find first bad index in i_vector

bool SimplexTask::find_bad_i_row() {

	for (int i = 0; i < i_vector.size(); i++)
	
		if (is_bad(i_vector[i])) {
			
			pivot_row = i;
			return true;
			
		}
		
	return false;
	
}

// Find first

bool SimplexTask::find_first_good_column() { 
	 
	pivot_column = -1; 
	 
	for (int i = 0; i < d_vector.size(); i++) 
	 
		if (a_matrix[pivot_row][i] != 0 && !is_bad(i)) { 
		 
			pivot_column = i;
			return true; 
	 
		} 
	 
	return false; 
} 

// Get pivot column

bool SimplexTask::find_pivot_column() {
	
	pivot_column = -1;
	
	bool positive_element;
	
	double d = 0;
	
	for (int i = 0; i < d_vector.size(); i++)
	
		if (task_max == (d_vector[i] < 0) && d_vector[i] != 0)
		
			if (task_max == (d_vector[i] < d))
			
				if (!is_bad(i)) {
					
					positive_element = false;
				
					for (int j = 0; j < a_matrix.size(); j++)
						if (a_matrix[j][i] > 0)
							positive_element = true;
						
					if (positive_element) {
						
						pivot_column = i;
						d = d_vector[i];
						
					}
					
				}
	
	if (pivot_column == -1)
		return false;
		
	return true;
	
}

// Get pivot row in pivot column

bool SimplexTask::find_pivot_row() {

	pivot_row = -1;
	
	std::vector<double> b_ratio_vector = b_vector;
	
	double b_ratio = 0;
	
	for (int i = 0; i < b_vector.size(); i++) {
		
		b_ratio_vector[i] /= a_matrix[i][pivot_column];
		
		if (b_ratio_vector[i] >= 0 && (b_ratio_vector[i] < b_ratio || pivot_row == -1)) {
			
			pivot_row = i;
			b_ratio = b_ratio_vector[i];
			
		}
			
	}	
	
	if (pivot_row == -1) 
		return false;
	
	return true;
}

// Check method stop condition

bool SimplexTask::check_stop() {
	
	bool all_negatives, all_positives;
	
	// Check for invalid conditions
	
	for (int i = 0; i < m; i++) {
		
		all_negatives = true;
		all_positives = true;
		
		for (int j = 0; j < n; j++) {

			if (a_matrix[i][j] > 0)
				all_negatives = false;
			
			if (a_matrix[i][j] < 0)
				all_positives = false;
			
		}
	
		if ((all_negatives && b_vector[i] > 0) || (all_positives && b_vector[i] < 0)) {
	
			result = invalid;
			return true;
			
		}
		
	}
	
	result = one;
	
	// Check if the solution is optimal and whether the function is unrestricted
	
	for (int i = 0; i < n; i++) {
		
		if (task_max == (d_vector[i] < 0) && d_vector[i] != 0) {
			
			bool all_negatives = true;
		
			for (int j = 0; j < a_matrix.size(); j++)
				if (a_matrix[j][i] > 0)
					all_negatives = false;		
			
			if (all_negatives) {
		
				result = unrestricted;
				return true;
				
			}
			
			return false;
	
		}
		
		if (d_vector[i] == 0 && !is_in(i, i_vector))
			result = many;
	
	}
		
	solved = true;

	return true;
	
}

// Read from file

void SimplexTask::read_task_from_file(std::string file_path) {
	
	std::ifstream input;
	
	input.open(file_path);
	
	if (!input)
    	throw std::runtime_error("Could not open file!");
    	
	input >> n >> m;
	
	f_vector_task = get_vector(input, n);
    
    a_matrix_task = get_matrix(input, n, m);

	b_vector_task = get_vector(input, m);
	
	sign = get_vector(input, m);
	
	input >> task_max;
	
	input.close();
	
	solved = false;
	
	read = true;
	
}

bool SimplexTask::solve() {
	
	if (read) {
	
		solved = false;
	
		a_matrix = a_matrix_task;
		
		f_vector = f_vector_task;
		
		b_vector = b_vector_task;
		
		f_vector.resize(n + m, 0);
		
		// Set all additional variables as basis variables
		
		for (int i = n; i < n + m; i++)
			i_vector.push_back(i);
		
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
		
		print_preparation();
	
		bool stop = false;
		
		while (!stop) {
			
			// Find first bad variable in basis
		
			if (find_bad_i_row()) {
								
				// Bad variable in basis at pivot_row
				
				if (!find_first_good_column())
					break;
			
			} else {
		
				// No bad variables in basis
				
				if (!find_pivot_column())
					break;
				
				if (!find_pivot_row())
					break;
		
			}
			
			pivot_element = a_matrix[pivot_row][pivot_column];
			
			print_pivots();
			
			// Count new B vector
			
			b_vector[pivot_row] = round_eps(b_vector[pivot_row] / pivot_element);
			
			for (int i = 0; i < m; i++)
				if (i != pivot_row)
					b_vector[i] = round_eps(b_vector[i] - b_vector[pivot_row] * a_matrix[i][pivot_column]);
					
			
			// Count new A matrix coefficients
			
			for (int i = 0; i < n + m; i++)		
				a_matrix[pivot_row][i] = round_eps(a_matrix[pivot_row][i] / pivot_element);
				
			double column_coef;	
			
			for (int i = 0; i < m; i++)	
				if (i != pivot_row) {
					
					column_coef = a_matrix[i][pivot_column];
					for (int j = 0; j < n + m; j++)
						a_matrix[i][j] = round_eps(a_matrix[i][j] - a_matrix[pivot_row][j] * column_coef);
						
				}
				
			// Move variable to basis
			
			i_vector[pivot_row] = pivot_column;
			
			// Count new D coefficients
			
			double d_sum;	
			
			for (int i = 0; i < n + m; i++) {
				
				d_sum = 0;
				
				for (int j = 0; j < m; j++)
					d_sum += f_vector[i_vector[j]] * a_matrix[j][i];
	
				d_vector[i] = round_eps(d_sum - f_vector[i]);
				
			}
			
			print_simplex_matrix();
			
			solution.assign(n + m, 0);
		
			for (int i = 0; i < m; i++)
				solution[i_vector[i]] = b_vector[i];
			
			print_solution();
			
			stop = check_stop();
			
		}
	
		solution.assign(n, 0);
		
		for (int i = 0; i < m; i++)
			solution[i_vector[i]] = b_vector[i];
		
		return solved;	
		
	}
	
	throw new std::runtime_error("Can't solve. Task not read.");

}

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
		
	} else {
		
		throw new std::runtime_error("Can't get result. Task not solved.");
	
	}
	
}

std::string SimplexTask::get_result_statement() {
	
	switch (result) {

		case one: return "Single solution found for result value.";
			
		case many: return "Infinite solutions found for result value.";
			
		case unrestricted: return "Can't solve. Function not restricted by conditions.";
			
		case invalid: return "Can't solve. No possible solution.";
	
		default: return "No result. Did not attempt to solve.";
	}
		
}

#endif
