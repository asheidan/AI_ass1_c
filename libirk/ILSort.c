#include "ILSort.h"
#include <string.h>
#include <stdio.h>

int ILIntCompare(void *a, void *b) {
	return *((int *)a) > *((int *)b);
}

void ILIntSwapper(void *a, void *b) {
	int tmp = *(int *)a;
	*(int *)a = *(int *)b;
	*(int *)b = tmp;
}

void ILIntTrans(void *from, void *to) {
	*(int*)to = *(int*)from;
}

void ILMergeSort(void *array, int size, size_t data_size, ILComparator compare, ILSwapper swap) {
	int a_size,b_size;
	if( size < 2 ) {
		return;
	}
	else {
		a_size = size / 2;
		b_size = size - a_size;

		ILMergeSort(array,a_size,data_size,compare,swap);
		ILMergeSort(array+data_size*a_size,b_size,data_size,compare,swap);

		ILMerge(array,a_size,b_size,data_size,compare,swap);
	}
}

void ILMerge(void *array,int a_size, int b_size,size_t data_size,ILComparator compare, ILTrans trans) {/*{{{*/
	void *tmp,
		*a_p,*b_p,
		 *r_p;

	tmp = malloc(data_size * a_size);
	if(NULL == tmp) {
		fprintf(stderr, "Malloc failed\n");
		return;
	}
	memcpy(tmp,array,data_size * a_size);
	a_p = tmp;
	b_p = array + data_size*a_size;
	//r_p = array;

	for(
			r_p = array;
			
			(r_p < (array + data_size*(a_size + b_size))) &&
			(a_p < (tmp + data_size * a_size)) &&
			(b_p < (array + data_size * (a_size + b_size)));
			
			r_p += data_size
	) {
		if((*compare)(a_p,b_p)) {
			trans(b_p,r_p);
			b_p += data_size;
		}
		else {
			trans(a_p,r_p);
			a_p += data_size;
		}
	}
	for(;a_p < tmp + data_size*a_size;a_p += data_size) {
		trans(a_p,r_p);
		r_p += data_size;
	}
	for(;b_p < array + data_size*(a_size+b_size);b_p += data_size) {
		trans(b_p,r_p);
		r_p += data_size;
	}

	free(tmp);
}/*}}}*/
