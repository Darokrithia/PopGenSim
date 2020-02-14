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

Degnome* Degnome_new() {
	Degnome* q = malloc(sizeof(Degnome));
	q->dna_array = malloc(chrom_size*sizeof(double));

	return q;
}

void Degnome_mate(Degnome* child, Degnome* p1, Degnome* p2, gsl_rng* rng,
	int mutation_rate, int mutation_effect, int crossover_rate) {
	// printf("mating\n");
	
	//Cross over
	int num_crossover = gsl_ran_poisson(rng, crossover_rate);
	int crossover_locations[num_crossover];
	int distance = 0;
	int diff;

	for (int i = 0; i < num_crossover; i++) {
		crossover_locations[i] = gsl_rng_uniform_int(rng, chrom_size);
	}
	if(num_crossover > 0) {
		int_qsort(crossover_locations, num_crossover);//changed
	}

	for (int i = 0; i < num_crossover; i++) {
		diff = crossover_locations[i] - distance;

		if (i % 2 == 0) {
			memcpy(child->dna_array+distance, p1->dna_array+distance, (diff*sizeof(double)));
		}
		else{
			memcpy(child->dna_array+distance, p2->dna_array+distance, (diff*sizeof(double)));
		}
		distance = crossover_locations[i];
	}

	if(num_crossover > 0) {
		diff = chrom_size - crossover_locations[num_crossover-1];
	}
	else{
		diff = chrom_size;
	}

	if (num_crossover % 2 == 0) {
		memcpy(child->dna_array+distance, p1->dna_array+distance, (diff*sizeof(double)));
	}
	else{
		memcpy(child->dna_array+distance, p2->dna_array+distance, (diff*sizeof(double)));
	}

	child->hat_size = 0;

	//mutate
	double mutation;
	int num_mutations = gsl_ran_poisson(rng, mutation_rate);
	int mutation_location;

	for (int i = 0; i < num_mutations; i++) {
		mutation_location = gsl_rng_uniform_int(rng, chrom_size);
		mutation = gsl_ran_gaussian_ziggurat(rng, mutation_effect);
		child->dna_array[mutation_location] += mutation;
	}

	//calculate hat_size

	for (int i = 0; i < chrom_size; i++) {
		child->hat_size += child->dna_array[i];
	}
	//and we are done!
}

void Degnome_free(Degnome* q) {
	free(q->dna_array);
	free(q);
}