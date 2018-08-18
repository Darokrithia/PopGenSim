/**
 * @file xdegnome.c
 * @author Daniel R. Tabin
 * @brief Unit tests for misc
 */

#include "misc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
	int verbose = 0;

	if(argc == 2) {
        if(strncmp(argv[1], "-v", 2) != 0) {
            fprintf(stderr, "usage: xmisc [-v]\n");
            exit(EXIT_FAILURE);
        }
        verbose = 1;
    }
    else if(argc != 1){
        fprintf(stderr, "usage: xmisc [-v]\n");
        exit(EXIT_FAILURE);
    }

	int a_1[] = {5, 4, 3, 10, 10, 0, 6, 2, 16, 1};
	int max_1 = 10;
	int a_2[] = {1};
	int max_2 = 1;
	int a_3[] = {10, 60, 9};
	int max_3 = 3;

	if(verbose){
		for(int i = 0; i < max_1; i++){
			printf("a_1[%u] = %u\n", i, a_1[i]);
		}
		printf("\n\n");

		for(int i = 0; i < max_2; i++){
			printf("a_2[%u] = %u\n", i, a_2[i]);
		}
		printf("\n\n");

		for(int i = 0; i < max_3; i++){
			printf("a_3[%u] = %u\n", i, a_3[i]);
		}
		printf("\n\n");
	}

	int_merge_sort(a_1, 0, max_1-1);
	int_merge_sort(a_2, 0, max_2-1);
	int_merge_sort(a_3, 0, max_3-1);

	if(verbose){
		for(int i = 0; i < max_1; i++){
			printf("a_1[%u] = %u\n", i, a_1[i]);
		}
		printf("\n\n");

		for(int i = 0; i < max_2; i++){
			printf("a_2[%u] = %u\n", i, a_2[i]);
		}
		printf("\n\n");

		for(int i = 0; i < max_3; i++){
			printf("a_3[%u] = %u\n", i, a_3[i]);
		}
		printf("\n\n");
	}

	printf("All tests for xmisc completed\n");
}