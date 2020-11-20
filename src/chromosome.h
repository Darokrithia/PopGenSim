#ifndef CHROMOSOME
#define CHROMOSOME

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

typedef struct Chromosome Chromosome;
struct Chromosome {
	double* dna_array;
	int mutation_rate;
	int mutation_effect;
	int chrom_size;
};

Chromosome* Chromosome_new(int mutation_rate, int mutation_effect, int chrom_size);
void Chromosome_mutate(Chromosome* chr, gsl_rng* rng);
void Chromosome_free(Chromosome* c);

#endif