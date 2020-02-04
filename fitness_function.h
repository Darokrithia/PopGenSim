#ifndef FITFUNC
#define FITFUNC

typedef double* (*func)(double*) fit_func_ptr;		//pointer to a fitness function which takes and returns a double

char func_name[8];									//used for command line args
double input;
fit_func_ptr func_to_run;


#endif