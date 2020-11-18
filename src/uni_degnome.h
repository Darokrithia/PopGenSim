#ifndef DEGNOME
#define DEGNOME

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

//Degnomedia Multisomatus
typedef struct Degnome Degnome;
struct Degnome {
	Chromosome* chromosome_array;

	double hat_size;
};

Degnome* Degnome_new(void);
void Degnome_mate(Degnome* location, Degnome* p1, Degnome* p2, gsl_rng* rng);
void Degnome_free(Degnome* q);

int chrom_groups;
int* chrom_g_szs;
int* chrom_sizes;
int* chrom_co_rs;

#endif