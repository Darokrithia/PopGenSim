# header

## Step 0: Install WSL


[how to link](https://github.com/michaeltreat/Windows-Subsystem-For-Linux-Setup-Guide)

## Step 1: Install GSL 
```
code block 
```

## Step 2: Run MakeFile
```
make
```

## Step 3: Run some example simulations
```
./PolyGenSim
./genancesim
./devosim
```

Try giving it some parameters as well
```
./PolyGenSim -c 10 -p 5 -g 30
```

DevoSim
 Simulation will stop when all degnomes are identical.

-c chromosome_length
Set chromosome length for the current simulation.
Default chromosome length is 10.


-e mutation_effect
Set how much a mutation will effect a gene on average.
Default mutation effect is 2.



-g num_generations\n"
Set how many generations this simulation will run for.\n"
Default number of generations is 1000.\n\n"

-h\t Display this help menu.\n\n"

-m mutation_rate
Set the mutation rate for the current simulation.
Default mutation rate is 1.


"\t -o crossover_rate\n"
"\t\t Set the crossover rate for the current simulation.\n"
"\t\t Default crossover rate is 2.\n\n"
"\t -p population_size\n"
"\t\t Set the population size for the current simulation.\n"
"\t\t Default population size is 10.\n\n"
"\t -r\t Only show percentages of descent from the original genomes.\n\n"
"\t -s\t Degnome selection will occur.\n\n"
"\t -u\t All degnomes contribute to two offspring.\n\n"
"\t -v\t Output will be given for every generation.\n";

