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

void set_function(char*){
	func_to_run = &linear_returns;
}