#include "jobqueue.h"
#include "qepo.h"
#include <stdio.h>
#include <string.h>

void usage(void);

const char *usageMsg =
    "Usage: polygensim [-c] CL [-p] PS [-g] G \n"
    "\n"
    "\"CL\" is chromosome length.  \"PS\" is the population size, and \"G\" is the\n"
    "number of generations that the simulation will run  They can be in\n"
    "any order, and not all are needed.  Default CL is 50, default PS is\n"
    "100, and the default G is 1000.\n";

int pop_size;
int num_gens;

void usage(void) {
	fputs(usageMsg, stderr);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv){

	chrom_size = 50;
	pop_size = 100;
	num_gens = 1000;

	if(argc > 7 || (argc%2) == 0){
		printf("\n");
		usage();
	}
	for(int i = 1; i < argc; i += 2){
		if(argv[i][0] == '-'){
			if (strcmp(argv[i], "-c") == 0){
				sscanf(argv[i+1], "%u", &chrom_size);
			}
			else if (strcmp(argv[i], "-p") == 0){
				sscanf(argv[i+1], "%u", &pop_size);
			}
			else if (strcmp(argv[i], "-g") == 0){
				sscanf(argv[i+1], "%u", &num_gens);
			}
			else{
				usage();
			}
		}
		else{
			usage();
		}
	}

	printf("%u, %u, %u\n", chrom_size, pop_size, num_gens);

	Qepo* generation_e;			//even generations 
	Qepo* generation_o;			//odd generations
	printf("yeet\n");
}