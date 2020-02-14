/**
 REMEMBER TO ADD COMMAND LINE ARGS ONCE THIS IS DONE!!!
 NOTE: it wouuld be very nice if this compartment had its own
 usage which could / would be called by anything that uses this

*/

/**
@file fitness_function.c
@page fitness_function
@author Daniel R. Tabin
@brief Transforms fitness via multiple functions

*/

#import "fitness_function.h"

void set_function(char* func_name) {
	if(strcmp(func_name, "linear")) {
		func_to_run = &linear_returns;
	}
	else if (strcmp(func_name, "sqrt")){
		func_to_run = &sqrt_returns;
	}
	else if (strcmp(func_name, "close")){
		func_to_run = &close_returns;
	}
	else if (strcmp(func_name, "ceiling")){
		func_to_run = &ceiling_returns;
	}
	else {
		func_to_run = &linear_returns;
	}
}

double linear_returns(double input) {
	return input;
}

double sqrt_returns(double input) {
	return sqrt(input);
}

double close_returns(double input) {
	return (target_num - abs(target_num - input));
}

double ceiling_returns(double input) {
	if (input < target_num) {
		return input;
	}
	else {
		(target_num) - 10 * abs(target_num - input);
	}
}