#ifndef QEPO
#define QEPO

#include <stdlib.h>

//Qepodia Rogerus
typedef struct Qepo Qepo;
struct Qepo {
	double* dna_array;
	double val;

};

Qepo* Qepo_new(void);
void Qepo_mate(Qepo* location, Qepo* p1, Qepo* p2);
void Qepo_free(Qepo* q);

int chrom_size;

#endif