/**
 REMEMBER TO ADD COMMAND LINE ARGS ONCE THIS IS DONE!!!
 NOTE: it wouuld be very nice if this compartment had its own
 usage which could / would be called by anything that uses this

*/

/**
@file fitfunc.c
@page fitfunc
@author Daniel R. Tabin
@brief Transforms fitness via multiple functions

*/

#include "fitfunc.h"
#include "string.h"
#include "math.h"
#include "stdlib.h"

fit_func_ptr func_to_run = &linear_returns;

void set_function(const char* func_name) {
	if (strcmp(func_name, "linear") == 0) {
		func_to_run = &linear_returns;
	}
	else if (strcmp(func_name, "sqrt") == 0){
		func_to_run = &sqrt_returns;
	}
	else if (strcmp(func_name, "close") == 0){
		func_to_run = &close_returns;
	}
	else if (strcmp(func_name, "ceiling") == 0){
		func_to_run = &ceiling_returns;
	}
	else {
		func_to_run = &linear_returns;
	}
}

double linear_returns(double x) {
	return x;
}

double sqrt_returns(double x) {
	return sqrt(x);
}

double close_returns(double x) {
	return (target_num - abs(target_num - x));
}

double ceiling_returns(double x) {
	if (x < target_num) {
		return x;
	}
	else {
		return (target_num) - 5 * abs(target_num - x);
	}
}

double get_fitness(double hat_size) {
	return (*func_to_run)(hat_size);
}