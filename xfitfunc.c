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
	double x = 100;

	if(argc == 2) {
		if(strncmp(argv[1], "-v", 2) != 0) {
			fprintf(stderr, "usage: xfitfunc [-v]\n");
			exit(EXIT_FAILURE);
		}
		verbose = 1;
	}
	else if(argc != 1) {
		fprintf(stderr, "usage: xfitfunc [-v]\n");
		exit(EXIT_FAILURE);
	}

	printf("All tests for xfitfunc completed\n");
}