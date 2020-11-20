/**
@file new_degnome.c
@page new_degnome
@author Daniel R. Tabin
@brief Digital Genomes aka Degnomes

This program will be used to simulated Polygenic evoltion of
quantitative traits by using Degnomes as defined above.

*/
#include "uni_degnome.h"
#include "chromosome.h"
#include "misc.h"
#include <string.h>
#include <stdio.h>


Degnome* Degnome_new() {
	Degnome* q = malloc(sizeof(Degnome));
	q->chromosome_array = malloc(num_o_chroms*sizeof(Chromosome));

	int i = 0;	// track all chromosomes
	// go through each group
	for (int j = 0; j < chrom_groups; j++) {
		num_o_chroms_in_group = chrom_g_szs[i];

		// go through each chromosome within each group
		for(int k = 0; k < num_o_chroms_in_group; k++) {
			Chromosome_fill((q->chromosome_array + i), mutat_rates[i], mutat_effct[i], chrom_sizes[j]);
			i++;

			//minor basic errorchecking
			if (i >= num_o_chroms){
				fprintf(stderr, "Error, invalid chromsome inputs\n");
				return NULL;
			}
		}
	}

	return q;
}

void Degnome_mate(Degnome* child, Degnome* p1, Degnome* p2, gsl_rng* rng) {
	// printf("mating\n");
	//Cross over
	int chrom_itr = 0;  // track all chromosomes
	for (int j = 0; j < chrom_groups; j++) {		//go through each group
		int num_crossover = gsl_ran_poisson(rng, chrom_co_rs[j]);	//randomize number of chrossovers on jth group
		int crossover_locations[num_crossover];
		int distance;
		int diff;

		for (int i = 0; i < num_crossover; i++) {
			crossover_locations[i] = gsl_rng_uniform_int(rng, chrom_sizes[j]); //randomize location of chrossovers on jth group
		}
		if (num_crossover > 0) {
			int_qsort(crossover_locations, num_crossover);		//sort crossover locations to 
		}

		num_o_chroms_in_group = chrom_g_szs[j];		//get number in this group
		for (int k = 0; k < num_o_chroms_in_group; k++) {		//go through each chromosome in this group
			int distance = 0;

			//get the locations of the DNA
			child_chrome = (child->chromosome_array + chrom_itr)->dna_array
			p1_chrome = (p1->chromosome_array + chrom_itr)->dna_array
			p2_chrome = (p2->chromosome_array + chrom_itr)->dna_array

			for (int i = 0; i < num_crossover; i++) {
				diff = crossover_locations[i] - distance;	//find amount to copy

				if (i % 2 == 0) {
					memcpy(child_chrome+distance, p1_chrome+distance, (diff*sizeof(double)));
				}
				else {
					memcpy(child_chrome+distance, p2_chrome+distance, (diff*sizeof(double)));
				}
				distance = crossover_locations[i];
			}
			//see how far we are from the end
			if (num_crossover > 0) {
				diff = chrom_size - crossover_locations[num_crossover-1];
			}
			else {
				diff = chrom_size;
			}

			// do the final cross over
			if (num_crossover % 2 == 0) {
				memcpy(child_chrome+distance, p1_chrome+distance, (diff*sizeof(double)));
			}
			else {
				memcpy(child_chrome+distance, p2_chrome+distance, (diff*sizeof(double)));
			}

			chrom_itr++; // update number of chromosomes we've been through
		}
	}

	child->hat_size = 0;

	// mutate chromosomes
	for (int i = 0; i < num_o_chroms; i++) {
		Chromosome_mutate(chromosome_array + i, rng);

		//calculate hat_size

		for (int j = 0;  j< num_o_chroms; j++) {
			if(contribs[i]) {
				child->hat_size += child->chromosome_array[i]->dna_array[j];
			}
		}
	}
	//and we are done!
}

void Degnome_free(Degnome* q) {
	free(q->dna_array);
	free(q->GOI_array);
	free(q);
}