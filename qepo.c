/**
@file qepo.c
@page qepo
@author Daniel R. Tabin
@brief Quantitive Evolition Polygenic Organism

This program will be used to simulated Polygenic evoltion of
quantitative traits by using QEPOs as defined above.
*/


//Qepodia Rogerus
struct Qepo {
	double* dna_array;
	double val;

};

Qepo* Qepo_new();
void Qepo_mate(Qepo* location, Qepo* p1, Qepo p2);
void Qepo_free(Qepo* q);

int size;

Qepo* Qepo_new(){
	Qepo* q = malloc(sizeof(Qepo));
	q->dna_array = malloc(size*sizeof(double));

	return q;
}

void Qepo_mate(Qepo* child, Qepo* p1, Qepo p2){
	
	//Cross over
	//NOTE: this crossover algorith is very simple and will be suject to change
	int split = (size/2);		//half and half
	memcpy(child->dna_array, p1->dna_array, (split*sizeof(double)));	//get from p1
	memcpy(child->dna_array, (p2->dna_array + (split*sizeof(double))), (size-split*sizeof(double)));	/get from p2

	child-> val = 0;

	for (int i = 0; i < size; i++){
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


int main(int argc, char **argv) {
	printf("yeet\n");
}