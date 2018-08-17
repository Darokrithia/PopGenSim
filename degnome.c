/**
@file degnome.c
@page degnome
@author Daniel R. Tabin
@brief Digital Genomes aka Degnomes

This program will be used to simulated Polygenic evoltion of
quantitative traits by using Degnomes as defined above.
*/
#include "degnome.h"
#include <string.h>

Degnome* Degnome_new(){
	Degnome* q = malloc(sizeof(Degnome));
	q->dna_array = malloc(chrom_size*sizeof(double));

	return q;
}

void Degnome_mate(Degnome* child, Degnome* p1, Degnome* p2, gsl_rng* rng){
	
	//Cross over
	//NOTE: this crossover algorith is very simple and will be suject to change
	int split = (chrom_size/2);		//half and half
	memcpy(child->dna_array, p1->dna_array, (split*sizeof(double)));	//get from p1
	memcpy((child->dna_array + split), p2->dna_array + split, (chrom_size-split)*sizeof(double));	//get from p2

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