#ifndef DEGNOME
#define DEGNOME

#include <stdlib.h>

//Degnomedia Rogerus
typedef struct Degnome Degnome;
struct Degnome {
	double* dna_array;
	double hat_size;

};

Degnome* Degnome_new(void);
void Degnome_mate(Degnome* location, Degnome* p1, Degnome* p2);
void Degnome_free(Degnome* q);

int chrom_size;

#endif