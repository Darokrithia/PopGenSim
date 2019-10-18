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

//hopefully cindy copy else i messed up branch lmao rip

void usage(void);
double get_fitness(double hat_size);
void calculate_diversity(Degnome* generation, double** percent_decent, double* diversity);

const char *usageMsg =
    "Usage:\n"
    "devosim [-c CL] [-p PS] [-g G] [-m MR] [-e ME] [-o CR] [-s]|[-u] [-v] [-r] [-b]\n"
    "\n"
    "\"CL\" is chromosome length.  \"PS\" is the population size, \"G\"\n"
    "is thenumber of generations that the simulation will run,\n"
    "\"MR\" is mutation rate, \"ME\" is how much a mutation will\n"
    "effect a gene on average, and \"CR\" is crossover rate.  If \"-s\"\n"
    "is present, there will be selection, while if \"-u\" is present\n"
    "all degnomes will contribute to two offspring.\n"
    "Note: \"-s\" and \"-u\" cannot be\n simultaneously active.\n"
    "If \"-v\" is present there will be output at every single\n"
    "generation, and if \"-r\" is present only percentages will be\n"
    "printed (you wont get to see the genomes of each degnome).  If\n"
    "\"-b\" is present, the program will break once all degnomes are\n"
    "identical. They can be in any order, and not all are needed.\n"
    "Default CL is 10, default PS is 10, the default G is 1000, and\n"
    "defualt CR is 2.\n";
    
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

	//printf("%u, %u, %u\n", chrom_size, pop_size, num_gens); //20 30 60 = "preset"
	printf("%u\n", pop_size); //this just prints out the pop size whihc data is based on

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
/*
// from here
	printf("\nGeneration 0:\n\n");
	for(int i = 0; i < pop_size; i++){
		//printf("Degnome %u allele values:\n", i);
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
	printf("\n\n");
//to here  -- test to see what code looks like nah not much for this
*/
	int final_gen;
	int broke_early = 0;

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

				Degnome_mate(children + j, parents + m, parents + d, rng, mutation_rate, mutation_effect, crossover_rate);
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

				Degnome_mate(children + j, parents + m, parents + d, rng, mutation_rate, mutation_effect, crossover_rate);
			}
		}
		temp = children;
		children = parents;
		parents = temp;

		//this part does the gen 0 parts as well -- dont need just yet

		/*
		if(verbose){
			calculate_diversity(parents, percent_decent, diversity);
			printf("\nGeneration %u:\n", i);
			for(int k = 0; k < pop_size; k++){
				printf("\n\nDegnome %u allele values:\n", k);

				if(!reduced){
					for(int j = 0; j < chrom_size; j++){
						printf("%lf\t", parents[k].dna_array[j]);
					}
					if(selective){
						printf("\nTOTAL HAT SIZE: %lg\n\n", parents[k].hat_size);
					}
					else{
						printf("\n");
					}
				}

				printf("\n\nDegnome %u ancestries:\n", k);
				if(!reduced){
					for(int j = 0; j < chrom_size; j++){
						printf("%u\t", parents[k].GOI_array[j]);
					}
					printf("\n");
				}

				for(int j = 0; j < pop_size; j++){
					if(percent_decent[k][j] > 0){
						printf("%lf%% Degnome %u\t", (100*percent_decent[k][j]), j);
					}
				}
			}
			printf("\nAverage population decent percentages:\n");
			for(int j = 0; j < pop_size; j++){
				if(percent_decent[pop_size][j] > 0){
					printf("%lf%% Degnome %u\t", (100*percent_decent[pop_size][j]), j);
				}
			}
			printf("\nPercent diversity: %lf\n", (100* (*diversity)));
		printf("\n\n");
		}
		*/
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

	for(int i = 0; i < pop_size; i++){

		//just working with % of Degnomes per generation -- dont need allele values yet

		/*
		printf("\n\nDegnome %u allele values:\n", i);		
		if(!reduced){
			for(int j = 0; j < chrom_size; j++){
				printf("%lf\t", parents[i].dna_array[j]);
			}
		}
		*/

		printf("\n\nDegnome %u ancestries:\n", i);

		//prints out the actual values ex: 18 not percent for how many are from 18 -- dont add just yet

		/*
		if(!reduced){
			for(int j = 0; j < chrom_size; j++){
				printf("%u\t", parents[i].GOI_array[j]);
			}
			printf("\n");
		}
		*/

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

	//add this in later -- rn, python graphics code focuses on degnome allele percentages/freq

	/*
	printf("Average population decent percentages:\n");
	for(int j = 0; j < pop_size; j++){
		if(percent_decent[pop_size][j] > 0){
			printf("%lf%% Degnome %u\t", (100*percent_decent[pop_size][j]), j);
		}
	}
	printf("\n");
	printf("\nPercent diversity: \n%lf\n", (100* (*diversity)));
	//printf("\n\n\n"); //just to see if commits change anyhting nope doesnt remove this  
	*/

	//free everything

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