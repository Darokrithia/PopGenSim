#include <stddef.h>

void int_qsort(int* array, int min, int max);
//Note: A quicker sort should be possible, but for now, we'll just use the tried-and-true standard qsort.
int int_qsort_comparator(const void * ptra, const void * ptrb);
void int_qsort(int * array, size_t num);