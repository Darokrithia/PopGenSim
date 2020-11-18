#ifndef DEGNOME
#define DEGNOME

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

//Degnomedia Rogerus
typedef struct Degnome Degnome;
struct Degnome {
	double* dna_array;
	double hat_size;
};

Degnome* Degnome_new(void);
void Degnome_mate(Degnome* location, Degnome* p1, Degnome* p2, gsl_rng* rng,
	int mutation_rate, int mutation_effect, int crossover_rate);
void Degnome_free(Degnome* q);

int chrom_size;

#endif