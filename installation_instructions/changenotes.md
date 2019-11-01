###Replace all calls to int_merge_sort() with calls to int_qsort()

- Has original instance of int_merge_sort

misc.c

- NEED CHECK 






- CHECKED 
dprintf.h

polygensim.c
genancesim.c

ance_degnome.h

degnome.h
xdegnome.c

jobqueue.c
jobqueue.h
xjobqueue.c


- CHANGED
ance_degnome.c<-
degnome.c <-
misc.h <-
xmisc.c <-



misc.h:6:6: error: conflicting types for ‘int_qsort’
 void int_qsort(int * array, size_t num);
      ^~~~~~~~~
misc.h:3:6: note: previous declaration of ‘int_qsort’ was here
 void int_qsort(int* array, int min, int max);
      ^~~~~~~~~
ance_degnome.c: In function ‘Degnome_mate’:
ance_degnome.c:36:3: error: too many arguments to function ‘int_qsort’
   int_qsort(crossover_locations, 0, num_crossover-1);
   ^~~~~~~~~
In file included from ance_degnome.c:11:0:
misc.h:6:6: note: declared here
 void int_qsort(int * array, size_t num);
      ^~~~~~~~~
Makefile:35: recipe for target 'ance_degnome.o' failed
make: *** [ance_degnome.o] Error 1




