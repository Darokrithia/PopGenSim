#include "chromosome.h"
#include <string.h>
#include <stdio.h>

Chromosome* Chromosome_new(int mutation_rate, int mutation_effect, int chrom_size) {
	Chromosome* c = malloc(sizeof(Chromosome));

	c->dna_array = malloc(chrom_size*sizeof(double));
	c->mutation_rate = mutation_rate;
	c->mutation_effect = mutation_effect;
	c->chrom_size = chrom_size;

	return c;
}

void Chromosome_mutate(Chromosome* chr, gsl_rng* rng) {
	double mutation;
	int num_mutations = gsl_ran_poisson(rng, chr->mutation_rate);
	int mutation_location;

	int chrom_size = chr->chrom_size;
	int mutation_effect = chr->mutation_effect;

	for (int i = 0; i < num_mutations; i++) {
		mutation_location = gsl_rng_uniform_int(rng, chrom_size);
		mutation = gsl_ran_gaussian_ziggurat(rng, mutation_effect);
		chr->dna_array[mutation_location] += mutation;
	}
}

void Chromosome_free(Chromosome* c) {
	free(c->dna_array);
	free(c);
}