#include "jobqueue.h"
#include "degnome.h"
#include "flagparse.c"
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
void help_menu(void);
int jobfunc(void* p, void* tdat);
double get_fitness(double hat_size);

const char* usageMsg =
	"Usage: polygensim [-h] [-c chromosome_length] [-e mutation_effect]\n"
	"\t\t  [-g num_generations] [-m mutation_rate]\n"
	"\t\t  [-o crossover_rate] [-p population_size]\n";

const char* helpMsg =
	"OPTIONS\n"
	"\t -c chromosome_length\n"
	"\t\t Set chromosome length for the current simulation.\n"
	"\t\t Default chromosome length is 10.\n\n"
	"\t -e mutation_effect\n"
	"\t\t Set how much a mutation will effect a gene on average.\n"
	"\t\t Default mutation effect is 2.\n\n"
	"\t -g num_generations\n"
	"\t\t Set how many generations this simulation will run for.\n"
	"\t\t Default number of generations is 1000.\n\n"
	"\t -h\t Display this help menu.\n\n"
	"\t -m mutation_rate\n"
	"\t\t Set the mutation rate for the current simulation.\n"
	"\t\t Default mutation rate is 1.\n\n"
	"\t -o crossover_rate\n"
	"\t\t Set the crossover rate for the current simulation.\n"
	"\t\t Default crossover rate is 2.\n\n"
	"\t -p population_size\n"
	"\t\t Set the population size for the current simulation.\n"
	"\t\t Default population size is 10.\n";

pthread_mutex_t seedLock = PTHREAD_MUTEX_INITIALIZER;
unsigned long rngseed = 0;

//	there is no need for the line 'int chrom_size' as it is declared as a global variable in degnome.h
int pop_size;
int num_gens;
int mutation_rate;
int mutation_effect;
int crossover_rate;

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

void help_menu(void) {
	fputs(helpMsg, stderr);
	exit(EXIT_FAILURE);
}

double get_fitness(double hat_size) {
	return hat_size;
}

int main(int argc, char **argv) {

	int * flags = NULL;

	if (parse_flags(argc, argv, 1, flags) == -1) {
		usage();
	}

	if (flags[2] == 1) {
		help_menu();
	}

	chrom_size = flags[6];
	mutation_effect = flags[7];
	num_gens = flags[8];
	mutation_rate = flags[9];
	crossover_rate = flags[10];
	pop_size = flags[11];

	free(flags);

	if (num_threads <= 0) {
		if (num_threads < 0) {
			#ifdef DEBUG_MODE
				fprintf(stderr, "Error invalid number of threads: %u\n", num_threads);
			#endif
		}
		num_threads = (3*getNumCores()/4);
	}
	#ifdef DEBUG_MODE
		fprintf(stderr, "Final number of threads: %u\n", num_threads);
	#endif

	time_t currtime = time(NULL);                  // time
	unsigned long pid = (unsigned long) getpid();  // process id
	rngseed = currtime ^ pid;                      // random seed
	gsl_rng* rng = gsl_rng_alloc(gsl_rng_taus);    // rand generator
	gsl_rng_set(rng, rngseed);

	Degnome* parents;
	Degnome* children;
	Degnome* temp;

	printf("%u, %u, %u\n", chrom_size, pop_size, num_gens);

	parents = malloc(pop_size*sizeof(Degnome));
	children = malloc(pop_size*sizeof(Degnome));

	for (int i = 0; i < pop_size; i++) {
		parents[i].dna_array = malloc(chrom_size*sizeof(double));
		children[i].dna_array = malloc(chrom_size*sizeof(double));
		parents[i].hat_size = 0;

		for (int j = 0; j < chrom_size; j++) {
			parents[i].dna_array[j] = (i+j);	//children isn't initiilized
			parents[i].hat_size += (i+j);
		}
	}

	printf("Generation 0:\n");
	for (int i = 0; i < pop_size; i++) {
		printf("Degnome %u\n", i);
		for (int j = 0; j < chrom_size; j++) {
			printf("%lf\t", parents[i].dna_array[j]);
		}
		printf("\nTOTAL HAT SIZE: %lg\n\n", parents[i].hat_size);
	}

	jq = JobQueue_new(num_threads, NULL, ThreadState_new, ThreadState_free);

	JobData* dat = malloc(pop_size*sizeof(JobData));

	for (int i = 0; i < num_gens; i++) {

		double fit = get_fitness(parents[0].hat_size);

		double total_hat_size = fit;
		double cum_hat_size[pop_size];
		cum_hat_size[0] = fit;

		for (int j = 1; j < pop_size; j++) {
			fit = get_fitness(parents[j].hat_size);

			total_hat_size += fit;
			cum_hat_size[j] = (cum_hat_size[j-1] + fit);
		}

		for (int j = 0; j < pop_size; j++) {

			int m, d;

			double win_m = gsl_rng_uniform(rng);
			win_m *= total_hat_size;
			double win_d = gsl_rng_uniform(rng);
			win_d *= total_hat_size;

			// printf("win_m:%lf, wind:%lf, max: %lf\n", win_m,win_d,total_hat_size);

			for (m = 0; cum_hat_size[m] < win_m; m++) {
				continue;
			}

			for (d = 0; cum_hat_size[d] < win_d; d++) {
				continue;
			}

			// printf("m:%u, d:%u\n", m,d);
			dat[j].child = (children + j);
			dat[j].p1 = (parents + m);
			dat[j].p2 = (parents + d);

			JobQueue_addJob(jq, jobfunc, dat + j);
		}

		JobQueue_waitOnJobs(jq);
		temp = children;
		children = parents;
		parents = temp;
	}

	JobQueue_noMoreJobs(jq);

	printf("Generation %u:\n", num_gens);
	for (int i = 0; i < pop_size; i++) {
		printf("Degnome %u\n", i);
		for (int j = 0; j < chrom_size; j++) {
			printf("%lf\t", parents[i].dna_array[j]);
		}
		printf("\nTOTAL HAT SIZE: %lg\n\n", parents[i].hat_size);
	}

	//free everything

	JobQueue_free(jq);
	free(dat);

	for (int i = 0; i < pop_size; i++) {
		free(parents[i].dna_array);
		free(children[i].dna_array);
		parents[i].hat_size = 0;
	}

	free(parents);
	free(children);

	gsl_rng_free (rng);
}
