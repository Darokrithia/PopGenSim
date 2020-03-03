#include "flagparse.h"
#include <string.h>

int parse_flags(int argc, char ** argv, int * flags) {


	// Flags formatting:
	// flags[0] ->  	Caller program (0: popgensim, 1: genancesim, 2: devosim)
	// flags[1] ->		-b	break						(Default: None)
	// flags[2] ->		-h	help						(Default: None)
	// flags[3] ->		-r	reduced						(Default: None)
	// flags[4] ->		-v 	verbose						(Default: None)
	// flags[5] ->		-s | -u selective / uniform		(Default: None)
	// flags[6] ->		-c chromosome_length			(Default:   10)
	// flags[7] ->		-e mutation_effect				(Default:    2)
	// flags[8] ->		-g num_generations				(Default: 1000)
	// flags[9] ->		-m mutation_rate				(Default:    1)
	// flags[10] ->		-o crossover_rate				(Default:    2)
	// flags[11] ->		-p population_size				(Default:   10)

	return 0;
}