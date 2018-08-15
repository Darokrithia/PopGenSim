/**
@file qepo.c
@page qepo
@author Daniel R. Tabin
@brief Quantitive Evolition Polygenic Organism

This program will be used to simulated Polygenic evoltion of
quantitative traits by using QEPOs as defined above.
*/
#include "qepo.h"
#include <string.h>

Qepo* Qepo_new(){
	Qepo* q = malloc(sizeof(Qepo));
	q->dna_array = malloc(chrom_size*sizeof(double));

	return q;
}

void Qepo_mate(Qepo* child, Qepo* p1, Qepo* p2){
	
	//Cross over
	//NOTE: this crossover algorith is very simple and will be suject to change
	int split = (chrom_size/2);		//half and half
	memcpy(child->dna_array, p1->dna_array, (split*sizeof(double)));	//get from p1
	memcpy(child->dna_array, (p2->dna_array + (split*sizeof(double))), (chrom_size-split*sizeof(double)));	//get from p2

	child-> val = 0;

	for (int i = 0; i < chrom_size; i++){
		//mutate
		child->dna_array[i] += 0.0;
		//calculate value
		child->val += child->dna_array[i];
	}

	//and we are done!
}

void Qepo_free(Qepo* q){
	free(q->dna_array);
	free(q);
}