/**
 * @file xdegnome.c
 * @author Daniel R. Tabin
 * @brief Unit tests for Degnome
 */

#include "degnome.h"
#include <stdio.h>
#include <string.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>

unsigned long rngseed=0;

int main(int argc, char **argv) {
	int verbose = 0;
	int seeded =0;

	if (argc == 2) {
        if (strncmp(argv[1], "-v", 2) != 0) {
            fprintf(stderr, "usage: xdegnome [-v] [-s] [0000000000]\n");
            exit(EXIT_FAILURE);
        }
        verbose = 1;
    }
	else if (argc == 3) {
		if (strncmp(argv[1], "-s", 2) != 0) {
            fprintf(stderr, "usage: xdegnome [-v] [-s] [0000000000]\n");
            exit(EXIT_FAILURE);
        }
		rngseed = (unsigned long) atoi(argv[2]);
	}
	else if (argc == 4) {
		if (strncmp(argv[1], "-v", 2) != 0) {
            fprintf(stderr, "usage: xdegnome [-v] [-s] [0000000000]\n");
            exit(EXIT_FAILURE);
        }
		else if (strncmp(argv[2], "-s", 2) != 0) {		
            fprintf(stderr, "usage: xdegnome [-v] [-s]  [0000000000]\n");
            exit(EXIT_FAILURE);
        }
        verbose = 1;
		seeded = 1;
		rngseed = (unsigned long) atoi(argv[3]);
		
	}
    else if (argc != 1) {
        fprintf(stderr, "usage: xdegnome [-v] [-s] [seed]\n");
        exit(EXIT_FAILURE);
    }

	chrom_size = 15;

	if (verbose) {
		printf("Chromosome is length %u\n", chrom_size);
	}

	Degnome* bom_mom = Degnome_new(); // good parent
	Degnome* bad_dad = Degnome_new();	// bad parent
	Degnome* tst_bby = Degnome_new(); // their child
	
	// ============ change this to seed rn
	if (seeded == 0) {
		time_t currtime = time(NULL);                  // time
		unsigned long pid = (unsigned long) getpid();  // process id
		rngseed = currtime ^ pid;					   // random seed	
	}
	gsl_rng* rng = gsl_rng_alloc(gsl_rng_taus);    // rand generator
	
	gsl_rng_set(rng, rngseed);
	rngseed = (rngseed == ULONG_MAX ? 0 : rngseed + 1);
	
	// ============ change this to seed rn
	
	
	
	for (int i = 0; i < chrom_size; i++) {
		bom_mom->dna_array[i] = 2*i;
		bad_dad->dna_array[i] = 1*i;

		bom_mom->hat_size += bom_mom->dna_array[i];
		bad_dad->hat_size += bad_dad->dna_array[i];
	}

	if (verbose) {
		printf("pre-mating values:\n");
		for (int i = 0; i < chrom_size; i++) {
       	 	printf("Mom: %lf\t Dad: %f\n", bom_mom->dna_array[i], bad_dad->dna_array[i]);
		}
		printf("Mom hat_size: %lf\t Dad hat_size: %f\n", bom_mom->hat_size, bad_dad->hat_size);
	}

	Degnome_mate(tst_bby, bom_mom, bad_dad, rng, 0, 0,2);


	if (verbose) {
		printf("post-mating values:\n");
		for (int i = 0; i < chrom_size; i++) {
       	 	printf("Mom: %lf\t Dad: %f\t Kid: %f\n", bom_mom->dna_array[i], bad_dad->dna_array[i], tst_bby->dna_array[i]);
		}
		printf("Mom hat_size: %lf\t Dad hat_size: %f\t Kid hat_size: %f\n", bom_mom->hat_size, bad_dad->hat_size, tst_bby->hat_size);
	}

	Degnome_free(bom_mom);
	Degnome_free(bad_dad);
	Degnome_free(tst_bby);

	gsl_rng_free(rng);

	printf("All tests for xdegnome completed\n");
}