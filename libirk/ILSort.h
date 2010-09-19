#ifndef __IL_SORT_H__
#define __IL_SORT_H__

#include <stdlib.h>

typedef int (*ILComparator)(void *, void *);
typedef void (*ILSwapper)(void *, void *);
typedef void (*ILTrans)(void *, void *);

int ILIntCompare(void *a, void *b);
void ILIntSwapper(void *a, void *b);
void ILIntTrans(void *from, void *to);

void ILMergeSort(void *array, int size, size_t data_size,ILComparator compare, ILTrans trans);
void ILMerge(void *array,int a_size, int b_size,size_t data_size,ILComparator compare, ILTrans trans);

#endif
