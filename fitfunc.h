#ifndef FITFUNC
#define FITFUNC

typedef double (*fit_func_ptr)(double);		//pointer to a fitness function which takes and returns a double

//char func_name[8];							//used for command line args (may not be needed on second thought delete later)
double input;
double target_num;

void set_function(const char*);
double get_fitness(double hat_size);


double linear_returns(double x);
double sqrt_returns(double x);
double close_returns(double x);
double ceiling_returns(double x);

#endif