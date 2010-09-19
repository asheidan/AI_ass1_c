#ifndef __IL_HASH_H__
#define __IL_HASH_H__

//#include "debug.h"
#include <stdio.h>
#include <stdbool.h>

typedef int ILHashKey;

typedef struct {
	void
		*key_value;

	size_t
		allocated_size;
	unsigned int
		inserted_keys,
		allocated_elements;

	ILHashKey
		*data;
} ILHash;

ILHash *ILHashAlloc();
void ILHashFree(ILHash *hash);
void ILHashInit(ILHash *hash, unsigned int initial_size);

bool ILHashHasKey(ILHash *hash, ILHashKey key);
void ILHashInsertKey(ILHash *hash, ILHashKey key);
void ILHashRemoveKey(ILHash *hash, ILHashKey key);


#endif
