/**
 * @file xfitfunc.c
 * @author Daniel R. Tabin
 * @brief Unit tests for fitness_function
 */

#include "fitfunc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	int verbose = 0;

	if (argc == 2) {
		if (strncmp(argv[1], "-v", 2) != 0) {
			fprintf(stderr, "usage: xfitfunc [-v]\n");
			exit(EXIT_FAILURE);
		}
		verbose = 1;
	}
	else if (argc != 1) {
		fprintf(stderr, "usage: xfitfunc [-v]\n");
		exit(EXIT_FAILURE);
	}

	double x;
	double y;
	x = 100;
	set_function("linear");

	y = get_fitness(x);

	if (verbose){
		printf("%lf = %lf\n", x,y);
	}

	set_function("sqrt");

	y = get_fitness(x);

	if (verbose){
		printf("sqrt(%lf) = %lf\n", x,y);
	}

	set_function("close");

	target_num = 100;

	y = get_fitness(x);

	if (verbose) {
		printf("%lf is \"%lf close\" to %lf\n", x, y, target_num);
	}

	target_num = 90;

	y = get_fitness(x);

	if (verbose){
		printf("%lf is \"%lf close\" to %lf\n", x, y, target_num);
	}

	target_num = 110;

	y = get_fitness(x);

	if (verbose){
		printf("%lf is \"%lf close\" to %lf\n", x, y, target_num);
	}

	set_function("ceiling");

	target_num = 100;

	y = get_fitness(x);

	if (verbose){
		printf("%lf gets a score of %lf for %lf\n", x, y, target_num);
	}

	target_num = 90;

	y = get_fitness(x);

	if (verbose){
		printf("%lf gets a score of %lf for %lf\n", x, y, target_num);
	}

	target_num = 110;

	y = get_fitness(x);

	if (verbose){
		printf("%lf gets a score of %lf for %lf\n", x, y, target_num);
	}

	printf("All tests for xfitfunc completed\n");
}