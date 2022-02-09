#ifndef SIMPLEX_H
#define SIMPLEX_H

#include <string>
#include <vector>

// Types of result

enum SimplexResult { one, many, unrestricted, invalid, none };

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
	
	SimplexResult result = none;
	
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
		
};

#endif
