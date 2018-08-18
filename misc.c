/**
 * @file xdegnome.c
 * @author Daniel R. Tabin
 * @brief Misc functions
 */

#include "misc.h"
#include <stdlib.h>
#include <stdio.h>

void int_merge_sort(int* array, int min, int max){
	if(min < max) {
		int mid = (min + max) / 2;

		int_merge_sort(array, min, mid);
		int_merge_sort(array, (mid+1), max);

		int temp[max];

		int j = min;
		int k = mid+1;
		int i = min;

		while(j <= mid && k <= max) {
			if(array[j] <= array[k]){
				temp[i++] = array[j++];
			}
			else{
				temp[i++] = array[k++];
			}
		}

		while(j <= mid){
			temp[i++] = array[j++];
		}
		while(k <= max){
			temp[i++] = array[k++];
		}
		for(i = min; i <= max; i++){
			array[i] = temp[i];
		}
	}
}