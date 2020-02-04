#ifndef FITFUNC
#define FITFUNC

typedef double* (*fit_func_ptr)(double);		//pointer to a fitness function which takes and returns a double

//char func_name[8];							//used for command line args (may not be needed on second thought delete later)
double input;
fit_func_ptr func_to_run;
//(*func_to_run)(hat_height)

void set_function(char*);


double linear_returns(double input);
double sqrt_returns(double input);
double close_returns(double input);
double ceiling_returns(double input);

#endif