#include "flagparse.h"
#include <string.h>

int parse_flags(int argc, char ** argv, int caller, int ** ret_flags) {

	// Flags formatting:
	// flags[0] ->  	Caller program (1: popgensim, 2: genancesim, 3: devosim)
	// flags[1] ->		-b	break						(Default:  Off)
	// flags[2] ->		-h	help						(Default:  Off)
	// flags[3] ->		-r	reduced						(Default:  Off)
	// flags[4] ->		-v 	verbose						(Default:  Off)
	// flags[5] ->		-s | -u selective / uniform		(Default: None)
	// flags[6] ->		-c chromosome_length			(Default:   10)
	// flags[7] ->		-e mutation_effect				(Default:    2)
	// flags[8] ->		-g num_generations				(Default: 1000)
	// flags[9] ->		-m mutation_rate				(Default:    1)
	// flags[10] ->		-o crossover_rate				(Default:    2)
	// flags[11] ->		-p population_size				(Default:   10)
	// flags[12] ->		-t num_threads					(Default:	 0)
	// flags[13] ->		--sqrt/linear/close/ceiling		(Default: None)
	// flags[14] ->		--target						(Default: 9999)
	// flags[15] ->		--seed							(Default:	 0)


	if (caller == 0) {
		return -1;
	}

	int * flags = (int*)calloc(16, sizeof(int));

	flags[0] = caller;
	flags[1] = 0;
	flags[2] = 0;
	flags[3] = 0;
	flags[4] = 0;
	flags[5] = 0;
	flags[6] = 10;
	flags[7] = 2;
	flags[8] = 1000;
	flags[9] = 1;
	flags[10] = 2;
	flags[11] = 10;
	flags[12] = 0;
	flags[13] = 0;
	flags[14] = 9999;
	flags[15] = 0;

    *ret_flags = flags;

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-' && (i + 1 == argc || argv[i + 1][0] == '-')) {
			int j = 1;
			while (argv[i][j] != '\0') {
				if (argv[i][j] == 'b' && caller != 1) {
					flags[1] = 1;
				}
				else if (argv[i][j] == 'h') {
					flags[2] = 1;
				}
				else if (argv[i][j] == 'r' && caller != 1) {
					flags[3] = 1;
				}
				else if (argv[i][j] == 'v' && caller != 1) {
					flags[4] = 1;
				}
				else if (argv[i][j] == 's' && caller != 1) {
					if (flags[5] == 2) {
						return -1;
					}
					flags[5] = 1;
				}
				else if (argv[i][j] == 'u' && caller != 1) {
					if (flags[5] == 1) {
						return -1;
					}
					flags[5] = 2;
				}
				else {
					return -1;
				}
				j++;
			}
		}
		else if (argv[i][0] == '-' && argv[i][1] == '-') {
			if (strcmp(argv[i], "--linear") == 0) {
				flags[13] = 0;
			}
			else if (strcmp(argv[i], "--sqrt") == 0) {
				flags[13] = 1;
			}
			else if (strcmp(argv[i], "--close") == 0) {
				flags[13] = 2;
			}
			else if (strcmp(argv[i], "--ceiling") == 0) {
				flags[13] = 3;
			}
			else if (strcmp(argv[i], "--target") == 0) {
				sscanf(argv[i+1], "%u", &flags[14]);
				i++;
			}
			else if (strcmp(argv[i], "--seed") == 0) {
				sscanf(argv[i+1], "%u", &flags[15]);
				i++;
			}
		}

		else if (argv[i][0] == '-') {
			if (strcmp(argv[i], "-b") == 0 && caller != 1) {
				flags[1] = 1;
			}
			else if (strcmp(argv[i], "-h") == 0) {
				flags[2] = 1;
			}
			else if (strcmp(argv[i], "-r") == 0 && caller != 1) {
				flags[3] = 1;
			}
			else if (strcmp(argv[i], "-v") == 0 && caller != 1) {
				flags[4] = 1;
			}
			else if (strcmp(argv[i], "-s") == 0 && caller != 1) {
				if (flags[5] == 2) {
					return -1;
				}
				flags[5] = 1;
			}
			else if (strcmp(argv[i], "-u") == 0 && caller != 1) {
				if (flags[5] == 1) {
					return -1;
				}
				flags[5] = 2;
			}
			else if (strcmp(argv[i], "-c" ) == 0) {
				sscanf(argv[i+1], "%u", &flags[6]);
				i++;
			}
			else if (strcmp(argv[i], "-e") == 0 && caller != 2) {
				sscanf(argv[i+1], "%u", &flags[7]);
				i++;
			}
			else if (strcmp(argv[i], "-g") == 0) {
				sscanf(argv[i+1], "%u", &flags[8]);
				i++;
			}
			else if (strcmp(argv[i], "-m") == 0 && caller != 2) {
				sscanf(argv[i+1], "%u", &flags[9]);
				i++;
			}
			else if (strcmp(argv[i], "-o") == 0) {
				sscanf(argv[i+1], "%u", &flags[10]);
				i++;
			}
			else if (strcmp(argv[i], "-p") == 0) {
				sscanf(argv[i+1], "%u", &flags[11]);
				i++;
			}
			else if (strcmp(argv[i], "-t") == 0) {
				sscanf(argv[i+1], "%u", &flags[12]);
				i++;
			}
			else {
				return -1;
			}
		}
		else {
			return -1;
		}
	}

	if(flags[15] > 0 && flags[12] != 1){
		return -1;
	}

	return 0;
}