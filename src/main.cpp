#include "simplex/simplex.h"
#include "simplex/vector/vector_print.h"

#include <iostream>

int main() {
	
	SimplexTask task;
	
	task.read_task_from_file("../examples/input1.txt");
	
	task.print_task();
	
	bool success = task.solve();
	
	if (success) {
	
		std::cout << "Resulting solution = ";
		print_vector(task.get_result_solution());
		std::cout << std::endl << std::endl;
		
		std::cout << "Result = " << task.get_result_value();
		std::cout << std::endl << std::endl;

	}
	
	std::cout << task.get_result_statement();

	return 0;
	
}
