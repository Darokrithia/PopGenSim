/**
@file degnome.c
@page degnome
@author Daniel R. Tabin
@brief Digital Genomes aka Degnomes

This program will be used to simulated Polygenic evoltion of
quantitative traits by using Degnomes as defined above.
*/
#include "degnome.h"
#include "misc.h"
#include <string.h>

Degnome* Degnome_new(){
	Degnome* q = malloc(sizeof(Degnome));
	q->dna_array = malloc(chrom_size*sizeof(double));

	return q;
}

void Degnome_mate(Degnome* child, Degnome* p1, Degnome* p2, gsl_rng* rng){
	
	//Cross over
	int num_crossover = gsl_ran_poisson(rng, 1.0);
	int crossover_locations[num_crossover];
	int current_end_loc = 0;

	for (int i = 0; i < num_crossover; i++){
		crossover_locations[i] = gsl_rng_uniform_int(rng, chrom_size);
		printf("CROSS LOCATION%u: %u\n", i, crossover_locations[i]);
	}

	int_merge_sort(crossover_locations, 0, num_crossover);
	
	for (int i = 0; i < num_crossover; i++){
		printf("CROSS LOCATION%u: %u\n", i, crossover_locations[i]);
	}

	printf("a\n");
	for (int i = 0; i < num_crossover; i++){
		int diff = crossover_locations[i] - current_end_loc;
		if (i % 2 == 0){
			printf("3.1\ti:%u\tdiff:%u\tend_loc:%u\n", i,diff,current_end_loc);
			memcpy(child->dna_array+current_end_loc, p1->dna_array, (diff*sizeof(double)));
			printf("3.3\n");
		}
		else{
			printf("3.2\ti:%u\tdiff:%u\tend_loc:%u\n", i,diff,current_end_loc);
			memcpy(child->dna_array+current_end_loc, p2->dna_array, (diff*sizeof(double)));
			printf("3.4\n");
		}
		printf("4\n");
		current_end_loc = crossover_locations[i];
		printf("5\n");
	}
	printf("b\n");

	child-> hat_size = 0;

	//mutate
	double mutation;
	int num_mutations = gsl_ran_poisson(rng, 1.0);
	int mutation_location;

	for (int i = 0; i < num_mutations; i++){
		mutation_location = gsl_rng_uniform_int(rng, chrom_size);
		mutation = gsl_ran_gaussian_ziggurat(rng, 2.0);
		child->dna_array[mutation_location] += mutation;
	}

	//calculate hat_size

	for (int i = 0; i < chrom_size; i++){
		child->hat_size += child->dna_array[i];
	}
	//and we are done!
}

void Degnome_free(Degnome* q){
	free(q->dna_array);
	free(q);
}