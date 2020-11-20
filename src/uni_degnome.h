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

int chrom_groups;				// number of chromosome groups.  Multiple chromosomes with same crossover (for tracking)
int num_o_chroms;				// number of total chromosomes
int* chrom_g_szs;				// sizes of the groups.  How many are in each group
int* chrom_sizes;				// sizes of each chromosome (same over group, and so group length)
int* chrom_co_rs;				// chrossover rate (same over group, and so group length)
int* mutat_rates;				// mutation rate of each chromosome.  Can be set per chromosome
int* mutat_effct;				// mutation effect of each chromosome.  Can be set per chromosome

bool* contribs;					// Does any given chromosome contribute to hat height.  Can be set per chromosome

#endif