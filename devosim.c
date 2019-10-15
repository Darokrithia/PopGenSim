#include "jobqueue.h"
#include "ance_degnome.h"
#include <stdio.h>
#include <string.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <limits.h>

typedef struct JobData JobData;
struct JobData {
	Degnome* child;
	Degnome* p1;
	Degnome* p2;
};

void usage(void);
int jobfunc(void* p, void* tdat);
double get_fitness(double hat_size);
void calculate_diversity(Degnome* generation, double** percent_decent, double* diversity);

const char *usageMsg =
    "Usage:\n"
    "devosim [-c CL] [-p PS] [-g G] [-o CR] [-s]|[-u] [-v] [-r] [-b]\n"
    "\n"
    "\"CL\" is chromosome length.  \"PS\" is the population size, \"G\"\n"
    "is thenumber of generations that the simulation will run,\n"
    "and \"CR\" is crossover rate.  If \"-s\" is present, there will\n"
    "be selection, while if \"-u\" is present all degnomes will\n"
    "contribute to two offspring.  Note: \"-s\" and \"-u\" cannot be\n"
    "simultaneously active. If \"-v\" is present there will be\n"
    "output at every single generation, and if \"-r\" is present\n"
    "only percentages will be printed (you wont get to see the"
    "genomes of each degnome).  If \"-b\" is present, the program\n"
    "will break once all degnomes are identical."
    "They can be in any order, and not all are needed. Default CL\n"
    "is 10, default PS is 10, the default G is 1000, and defualt\n"
    "CR is 2.\n";

pthread_mutex_t seedLock = PTHREAD_MUTEX_INITIALIZER;
unsigned long rngseed=0;

//	there is no need for the line 'int chrom_size' as it is declared as a global variable in degnome.h
int pop_size;
int num_gens;
int mutation_rate;
int mutation_effect;
int crossover_rate;
int selective;
int uniform;
int verbose;
int reduced;
int break_at_zero_diversity;

int num_threads = 0;
JobQueue* jq;

void *ThreadState_new(void *notused);
void ThreadState_free(void *rng);

void *ThreadState_new(void *notused) {
	// Lock seed, initialize random number generator, increment seed,
	// and unlock.
	gsl_rng *rng = gsl_rng_alloc(gsl_rng_taus);

	pthread_mutex_lock(&seedLock);
	gsl_rng_set(rng, rngseed);
	rngseed = (rngseed == ULONG_MAX ? 0 : rngseed + 1);
	pthread_mutex_unlock(&seedLock);

	return rng;
}

void ThreadState_free(void *rng) {
	gsl_rng_free((gsl_rng *) rng);
}

int jobfunc(void* p, void* tdat) {
	gsl_rng* rng = (gsl_rng*) tdat;
	JobData* data = (JobData*) p;																				//get data out
	Degnome_mate(data->child, data->p1, data->p2, rng, mutation_rate, mutation_effect, crossover_rate);			//mate

	return 0;		//exited without error
}

void usage(void) {
	fputs(usageMsg, stderr);
	exit(EXIT_FAILURE);
}


double get_fitness(double hat_size){
	return hat_size;
}

void calculate_diversity(Degnome* generation, double** percent_decent, double* diversity){
	*diversity = 0;
	for(int i = 0; i < pop_size; i++){			//calculate percent decent for each degnome
		for(int j = 0; j < pop_size; j++){
			percent_decent[i][j] = 0;
			for(int k = 0; k < chrom_size; k++){
				if(generation[i].GOI_array[k] == j){
					percent_decent[i][j]++;
				}
			}
			percent_decent[i][j] /= chrom_size;
		}
	}
	for(int j = 0; j < pop_size; j++){			//sum and average
		percent_decent[pop_size][j] = 0;
		for(int k = 0; k < pop_size; k++){
			percent_decent[pop_size][j] += percent_decent[k][j];
		}
		percent_decent[pop_size][j] /= pop_size;
	}

	for(int i = 0; i < pop_size; i++){			//calculate percent diversity for the entire generation
		for(int j = 0; j < pop_size; j++){
			if(i == j){
				continue;
			}
			for(int k = 0; k < chrom_size; k++){
				if(generation[i].GOI_array[k] != generation[j].GOI_array[k]){
					(*diversity)++;
				}
			}
		}
	}
	*diversity /= ((pop_size-1) * pop_size * chrom_size);
}

int main(int argc, char **argv){

	chrom_size = 20;
	pop_size = 30;
	num_gens = 60;
	mutation_rate = 1;
	mutation_effect = 2;
	crossover_rate = 2;
	selective = 0;
	uniform = 0;
	verbose = 0;
	reduced = 0;
	break_at_zero_diversity = 0;

	if(argc > 17){
		printf("\n");
		usage();
	}
	for(int i = 1; i < argc; i++){
		if(argv[i][0] == '-'){
			if (strcmp(argv[i], "-c" ) == 0 && argc > (i+1)){
				sscanf(argv[i+1], "%u", &chrom_size);
				i++;
			}
			else if (strcmp(argv[i], "-p") == 0 && argc > (i+1)){
				sscanf(argv[i+1], "%u", &pop_size);
				i++;
			}
			else if (strcmp(argv[i], "-g") == 0 && argc > (i+1)){
				sscanf(argv[i+1], "%u", &num_gens);
				i++;
			}
			else if (strcmp(argv[i], "-m") == 0){
				sscanf(argv[i+1], "%u", &mutation_rate);
				i++;
			}
			else if (strcmp(argv[i], "-e") == 0){
				sscanf(argv[i+1], "%u", &mutation_effect);
				i++;
			}
			else if (strcmp(argv[i], "-o") == 0 && argc > (i+1)){
				sscanf(argv[i+1], "%u", &crossover_rate);
				i++;
			}
			else if (strcmp(argv[i], "-s") == 0){
				if(uniform){
					usage();
				}
				selective = 1;
			}
			else if (strcmp(argv[i], "-u") == 0){
				if(selective){
					usage();
				}
				uniform = 1;
			}
			else if (strcmp(argv[i], "-v") == 0){
				verbose = 1;
			}
			else if (strcmp(argv[i], "-r") == 0){
				reduced = 1;
			}
			else if (strcmp(argv[i], "-b") == 0){
				break_at_zero_diversity = 1;
			}
			else{
				printf("\n");
				usage();
			}
		}
		else{
			usage();
		}
	}

    time_t currtime = time(NULL);                  // time
    unsigned long pid = (unsigned long) getpid();  // process id
    rngseed = currtime ^ pid;                      // random seed
    gsl_rng* rng = gsl_rng_alloc(gsl_rng_taus);    // rand generator

	Degnome* parents;
	Degnome* children;
	Degnome* temp;

	printf("%u, %u, %u\n", chrom_size, pop_size, num_gens);

	parents = malloc(pop_size*sizeof(Degnome));
	children = malloc(pop_size*sizeof(Degnome));

	for (int i = 0; i < pop_size; i++){
		parents[i].dna_array = malloc(chrom_size*sizeof(double));
		parents[i].GOI_array = malloc(chrom_size*sizeof(int));

		children[i].dna_array = malloc(chrom_size*sizeof(double));
		children[i].GOI_array = malloc(chrom_size*sizeof(int));

		parents[i].hat_size = 0;

		for(int j = 0; j < chrom_size; j++){
			parents[i].dna_array[j] = 10;	//children aren't initialized
			parents[i].hat_size += 10;
			parents[i].GOI_array[j] = (i);	//track ancestries
		}
	}

	double* diversity;
	double** percent_decent;

	diversity = malloc(sizeof(double));
	*diversity = 1;
	percent_decent = malloc((pop_size+1)*sizeof(double*));
	for(int i = 0; i < pop_size+1; i++){
		percent_decent[i] = malloc(pop_size*sizeof(double));
		if(i == pop_size){
			continue;
		}
		for(int j = 0; j < pop_size; j++){
			if(i == j){
				percent_decent[i][j] = 1;
			}
			else{
				percent_decent[i][j] = 0;
			}
		}
	}

	if(!reduced){
		printf("\nGeneration 0:\n\n");
		for(int i = 0; i < pop_size; i++){
			printf("Degnome %u allele values:\n", i);
			if(!reduced){
				for(int j = 0; j < chrom_size; j++){
					printf("%lf\t", parents[i].dna_array[j]);
				}
				printf("\n");
			}
			else{
				printf("%lf\n", parents[i].dna_array[0]);
			}

			printf("Degnome %u ancestries:\n", i);
			if(!reduced){
				for(int j = 0; j < chrom_size; j++){
					printf("%u\t", parents[i].GOI_array[j]);
				}
				printf("\n");
			}
			else{
				printf("%u\n", parents[i].GOI_array[0]);
			}
		}
	}
	printf("\n\n");

	int final_gen;
	int broke_early = 0;

	jq = JobQueue_new(num_threads, NULL, ThreadState_new, ThreadState_free);

	JobData* dat = malloc(pop_size*sizeof(JobData));

	for(int i = 0; i < num_gens; i++){
		if(break_at_zero_diversity){
			calculate_diversity(parents, percent_decent, diversity);
			if((*diversity) <= 0){
				final_gen = i;
				broke_early = 1;
				break;
			}
		}
		if(!uniform){
			double fit;
			if(selective){
				fit = get_fitness(parents[0].hat_size);
			}
			else{
				fit = 100;			//in runs withoutslection, everybody is equally fit
			}

			double total_hat_size = fit;
			double cum_hat_size[pop_size];
			cum_hat_size[0] = fit;

			for(int j = 1; j < pop_size; j++){
				if(selective){
					fit = get_fitness(parents[j].hat_size);
				}
				else{
					fit = 100;
				}

				total_hat_size += fit;
				cum_hat_size[j] = (cum_hat_size[j-1] + fit);
			}

			for(int j = 0; j < pop_size; j++){

			    pthread_mutex_lock(&seedLock);
				gsl_rng_set(rng, rngseed);
			    rngseed = (rngseed == ULONG_MAX ? 0 : rngseed + 1);
	    		pthread_mutex_unlock(&seedLock);

				int m, d;

				double win_m = gsl_rng_uniform(rng);
				win_m *= total_hat_size;
				double win_d = gsl_rng_uniform(rng);
				win_d *= total_hat_size;

				// printf("win_m:%lf, wind:%lf, max: %lf\n", win_m,win_d,total_hat_size);

				for (m = 0; cum_hat_size[m] < win_m; m++){
					continue;
				}

				for (d = 0; cum_hat_size[d] < win_d; d++){
					continue;
				}

				// printf("m:%u, d:%u\n", m,d);


				dat[j].child = (children + j);
				dat[j].p1 = (parents + m);
				dat[j].p2 = (parents + d);

       			JobQueue_addJob(jq, jobfunc, dat + j);
			}
		}
		else{
			// printf("uniform!!!\n");

			int moms[pop_size];
			int dads[pop_size];
			int mom_max = pop_size;
			int dad_max = pop_size;

			int m, d;

			for(int j = 0; j < pop_size; j++){
				moms[j] = j;
				dads[j] = j;
			}

			for(int j = 0; j < pop_size; j++){
				pthread_mutex_lock(&seedLock);
				gsl_rng_set(rng, rngseed);
			    rngseed = (rngseed == ULONG_MAX ? 0 : rngseed + 1);

	    		pthread_mutex_unlock(&seedLock);

				int index_m = (int) gsl_rng_uniform_int (rng, mom_max);
				int index_d = (int) gsl_rng_uniform_int (rng, dad_max);

				m = moms[index_m];
				d = dads[index_d];

				// printf("m:\t%u\nd:\t%u\n", m, d);

				//reduce the pool of available degnomes
				//in order to make sure everybody get's two chances to mate
				//one as a dad and one as a mom
			    
				int temp_m = moms[index_m];
				int temp_d = dads[index_d];
				moms[index_m] = moms[mom_max-1];
				dads[index_d] = dads[dad_max-1];
				moms[mom_max-1] = temp_m;
				dads[dad_max-1] = temp_d;

				mom_max--;
				dad_max--;

				dat[j].child = (children + j);
				dat[j].p1 = (parents + m);
				dat[j].p2 = (parents + d);
				
       			JobQueue_addJob(jq, jobfunc, dat + j);
			}
		}
		temp = children;
		children = parents;
		parents = temp;
		if(verbose){
			calculate_diversity(parents, percent_decent, diversity);
			printf("\nGeneration %u:\n", i);
			if(!reduced){
				for(int k = 0; k < pop_size; k++){
					printf("\n\nDegnome %u allele values:\n", k);

					for(int j = 0; j < chrom_size; j++){
						printf("%lf\t", parents[k].dna_array[j]);
					}
					if(selective){
						printf("\nTOTAL HAT SIZE: %lg\n\n", parents[k].hat_size);
					}
					else{
						printf("\n");
					}

					printf("\n\nDegnome %u ancestries:\n", k);
					for(int j = 0; j < chrom_size; j++){
						printf("%u\t", parents[k].GOI_array[j]);
					}
					printf("\n");

					for(int j = 0; j < pop_size; j++){
						if(percent_decent[k][j] > 0){
							printf("%lf%% Degnome %u\t", (100*percent_decent[k][j]), j);
						}
					}
				}
			}
			printf("\nAverage population descent percentages:\n");
			for(int j = 0; j < pop_size; j++){
				if(percent_decent[pop_size][j] > 0){
					printf("%lf%% Degnome %u\t", (100*percent_decent[pop_size][j]), j);
				}
			}
			printf("\nPercent diversity: %lf\n", (100* (*diversity)));
		printf("\n\n");
		}
	}
	if(verbose){
		printf("\n");
	}

	calculate_diversity(parents, percent_decent, diversity);
	// printf("\n\n DIVERSITY%lf\n\n\n", *diversity);
	if(broke_early){
		printf("Generation %u:\n", final_gen);
	}
	else{
		printf("Generation %u:\n", num_gens);
	}
	if(!reduced){
		for(int i = 0; i < pop_size; i++){
			printf("\n\nDegnome %u allele values:\n", i);		
			if(!reduced){
				for(int j = 0; j < chrom_size; j++){
					printf("%lf\t", parents[i].dna_array[j]);
				}
			}

			printf("\n\nDegnome %u ancestries:\n", i);
			if(!reduced){
				for(int j = 0; j < chrom_size; j++){
					printf("%u\t", parents[i].GOI_array[j]);
				}
				printf("\n");
			}

			for(int j = 0; j < pop_size; j++){
				if(percent_decent[i][j] > 0){
					printf("%lf%% Degnome %u\t", (100*percent_decent[i][j]), j);
				}
			}
			printf("\n");

			if(selective){
				printf("\nTOTAL HAT SIZE: %lg\n\n", parents[i].hat_size);
			}
			else{
				printf("\n\n");
			}
		}
	}
	printf("Average population decent percentages:\n");
	for(int j = 0; j < pop_size; j++){
		if(percent_decent[pop_size][j] > 0){
			printf("%lf%% Degnome %u\t", (100*percent_decent[pop_size][j]), j);
		}
	}
	printf("\nPercent diversity: %lf\n", (100* (*diversity)));
	printf("\n\n\n");

	//free everything

	JobQueue_free(jq);
	free(dat);

	for (int i = 0; i < pop_size; i++){
		free(parents[i].dna_array);
		free(children[i].dna_array);
		parents[i].hat_size = 0;

		free(percent_decent[i]);
	}
	free(percent_decent[pop_size]);

	free(parents);
	free(children);

	free(percent_decent);
	free(diversity);

	gsl_rng_free (rng);
}