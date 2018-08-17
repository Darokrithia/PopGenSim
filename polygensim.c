#include "jobqueue.h"
#include "degnome.h"
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

	Degnome* parents;
	Degnome* children;
	Degnome* temp;

	printf("%u, %u, %u\n", chrom_size, pop_size, num_gens);

	parents = malloc(pop_size*sizeof(Degnome));
	children = malloc(pop_size*sizeof(Degnome));

	for (int i = 0; i < pop_size; i++){
		parents[i].dna_array = malloc(chrom_size*sizeof(double));
		children[i].dna_array = malloc(chrom_size*sizeof(double));

		for(int j = 0; j < chrom_size; j++){
			parents[i].dna_array[j] = (i+j);	//children isn't initiilized
			parents[i].hat_size += (i+j);
		}
	}

	printf("Generation 0:\n");
	for(int i = 0; i < pop_size; i++){
		printf("Degnome %u\n", i);
		for(int j = 0; j < chrom_size; j++){
			printf("%lf\t", parents[i].dna_array[j]);
		}
		printf("\n");
	}

	for(int i = 0; i < num_gens; i++){
		for(int j = 0; j < pop_size; j++){
			Degnome_mate(&children[j], &parents[pop_size-(j+1)], &parents[pop_size-1]);		//Will be selective
		}
		temp = children;
		children = parents;
		parents = temp;
	}

	printf("Generation %u:\n", num_gens);
	for(int i = 0; i < pop_size; i++){
		printf("Degnome %u\n", i);
		for(int j = 0; j < chrom_size; j++){
			printf("%lf\t", parents[i].dna_array[j]);
		}
		printf("\n");
	}
}