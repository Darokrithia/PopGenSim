/**
 * @file xqepo.c
 * @author Daniel R. Tabin
 * @brief Unit tests for Qepo
 */

#include "qepo.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv){
	int verbose = 0;

	if(argc == 2) {
        if(strncmp(argv[1], "-v", 2) != 0) {
            fprintf(stderr, "usage: xqepo [-v]\n");
            exit(EXIT_FAILURE);
        }
        verbose = 1;
    }
    else if(argc != 1){
        fprintf(stderr, "usage: xqepo [-v]\n");
        exit(EXIT_FAILURE);
    }

	chrom_size = 10;

	if(verbose){
		printf("Chromosome is length %u\n", chrom_size);
	}

	Qepo* bom_mom = Qepo_new(); // good parent
	Qepo* bad_dad = Qepo_new();	// bad parent
	Qepo* tst_bby = Qepo_new(); // their child

	for(int i = 0; i < chrom_size; i++){
		bom_mom->dna_array[i] = 2;
		bad_dad->dna_array[i] = 0;

		bom_mom->val += bom_mom->dna_array[i];
		bad_dad->val += bad_dad->dna_array[i];
	}

	if(verbose){
		printf("pre-mating values:\n");
		for(int i = 0; i < chrom_size; i++){
       	 	printf("Mom: %lf\t Dad: %f\n", bom_mom->dna_array[i], bad_dad->dna_array[i]);
		}
		printf("Mom val: %lf\t Dad val: %f\n", bom_mom->val, bad_dad->val);
	}

	Qepo_mate(tst_bby, bom_mom, bad_dad);


	if(verbose){
		printf("post-mating values:\n");
		for(int i = 0; i < chrom_size; i++){
       	 	printf("Mom: %lf\t Dad: %f\t Kid: %f\n", bom_mom->dna_array[i], bad_dad->dna_array[i], tst_bby->dna_array[i]);
		}
		printf("Mom val: %lf\t Dad val: %f\t Kid val: %f\n", bom_mom->val, bad_dad->val, tst_bby->val);
	}

	Qepo_free(bom_mom);
	Qepo_free(bad_dad);
	Qepo_free(tst_bby);

	printf("All tests for xqepo completed\n");
}