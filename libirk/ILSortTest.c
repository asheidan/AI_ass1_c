#include "CuTest.h"
#include "ILSort.h"

#include "debug.h"
#include <stdio.h>
#include <stdbool.h>


void *ILSortTestSetup() {
	return NULL;
}
void ILSortTestTearDown() {
}

void TestMergeSorted(CuTest *tc) {
	int i;
	int expected[]	= {0,1,2,3,4,5,6,7,8,9};
	int result[]	= {0,1,2,3,4,5,6,7,8,9};

	ILMerge(result,5,5,sizeof(int),ILIntCompare,ILIntTrans);
	for(i = 0; i < 10; i++) {
		// fprintf(stderr, "[%d] %d-%d\n",i,expected[i],result[i]);
		CuAssertIntEquals(tc,expected[i],result[i]);
	}
}

void TestMergeMinimal(CuTest *tc) {
	int expected[]	= {0,1};
	int result[]	= {1,0};

	ILMerge(result,1,1,sizeof(int),ILIntCompare,ILIntTrans);

	CuAssertIntEquals(tc, expected[0], result[0]);
	CuAssertIntEquals(tc, expected[1], result[1]);
}

void TestMergeInterleaved(CuTest *tc) {
	int i;
	int expected[]	= {0,1,2,3,4,5,6,7,8,9};
	int result[]	= {0,2,4,6,8,1,3,5,7,9};

	ILMerge(result,5,5,sizeof(int),ILIntCompare,ILIntTrans);
	for(i = 0; i < 10; i++) {
		// fprintf(stderr, "[%d] %d-%d\n",i,expected[i],result[i]);
		CuAssertIntEquals(tc,expected[i],result[i]);
	}
}

void TestMergeSemiReversed(CuTest *tc) {
	int i;
	int expected[]	= {0,1,2,3,4,5,6,7,8,9};
	int result[]	= {5,6,7,8,9,0,1,2,3,4};

	ILMerge(result,5,5,sizeof(int),ILIntCompare,ILIntTrans);

	for(i = 0; i < 10; i++) {
		// fprintf(stderr, "[%d] %d-%d\n",i,expected[i],result[i]);
		CuAssertIntEquals(tc,expected[i],result[i]);
	}
}

void TestMergeDifferentSized(CuTest *tc) {
	int i;
	int expected[]	= {0,1,2,3,4,5,6,7,8,9};
	int result[]	= {7,8,9,0,1,2,3,4,5,6};

	ILMerge(result,3,7,sizeof(int),ILIntCompare,ILIntTrans);

	for(i = 0; i < 10; i++) {
		// fprintf(stderr, "[%d] %d-%d\n",i,expected[i],result[i]);
		CuAssertIntEquals(tc,expected[i],result[i]);
	}
}

void TestMergeSortMinimal(CuTest *tc) {
	int expected[]	= {0,1};
	int result[]	= {1,0};

	ILMerge(result,1,1,sizeof(int),ILIntCompare,ILIntTrans);

	CuAssertIntEquals(tc, expected[0], result[0]);
	CuAssertIntEquals(tc, expected[1], result[1]);
}

void TestMergeSortReversed(CuTest *tc) {
	int i;
	int expected[]	= {0,1,2,3,4,5,6,7,8,9};
	int result[]	= {9,8,7,6,5,4,3,2,1,0};

	ILMergeSort(result,10,sizeof(int),ILIntCompare,ILIntTrans);

	for(i = 0; i < 10; i++) {
		//fprintf(stderr, "[%d] %d-%d\n",i,expected[i],result[i]);
		CuAssertIntEquals(tc,expected[i],result[i]);
	}
}

void TestMergeLargeRandomized(CuTest *tc) {
	int n = 1000,
		 previous,
		 i;
	int
		randomized[n];

	for(i = 0; i < n; i++) {
		randomized[i] = rand();
	}

	ILMergeSort(randomized,n,sizeof(int),ILIntCompare,ILIntTrans);

	previous = 0;
	for(i = 0; i < n; i++) {
		CuAssertTrue(tc,previous <= randomized[i]);
		previous = randomized[i];
	}
}

