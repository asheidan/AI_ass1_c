#include "ILHash.h"

#include "debug.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

ifdebug(uint32_t ILHashCollisionCount = 0;)

ILHash *ILHashAlloc() {
	ILHash *hash = malloc(sizeof(ILHash));
	if(hash != NULL) {
		hash->data = NULL;
	}
	return hash;
}

void ILHashInit(ILHash *hash, unsigned int initial_size) {
	int i;
	hash->allocated_elements = initial_size;
	hash->allocated_size = sizeof(ILHashKey)*hash->allocated_elements;
	hash->data = malloc(hash->allocated_size);
	hash->inserted_keys = 0;

	if(NULL != hash->data) {
		for(i = 0; i < hash->allocated_elements; i++) {
			hash->data[i] = -1;
		}
	}
}

void ILHashFree(ILHash *hash) {
	if(NULL != hash->data) {
		free(hash->data);
	}
	free(hash);
}

ILHashKey *ILHashLocateKey(ILHash *hash, ILHashKey key) {
	int offset,initial_offset;
	offset = ((key + 153) * 3) % hash->allocated_elements;
	ifdebug(initial_offset = offset);

	for(; (hash->data[offset] != -1) &&
			(hash->data[offset] != key);
			offset = (offset + 1) % hash->allocated_elements) {
		(ILHashCollisionCount++);
	}
	// dprintf(stderr, "Keylocation: [k: %3d | h: %3d | o: %3d ] >> %p\n",key,initial_offset,offset,hash->data + offset);
	return hash->data + offset;
}

bool ILHashHasKey(ILHash *hash, ILHashKey key) {
	if(hash->data == NULL)
		return false;
	
	// dprintf(stderr, "Has key? key = %d >> %p [%d]\n",key,result,*result);
	return *ILHashLocateKey(hash,key) == key;
}

void ILHashInsertKey(ILHash *hash, ILHashKey key) {
	if(hash->data != NULL) {
		*ILHashLocateKey(hash,key) = key;
		hash->inserted_keys++;
		// dprintf(stderr, "Inserted key %d >> %p = %d\n",key,result,*result);
	}
}

void ILHashRemoveKey(ILHash *hash, ILHashKey key) {
	if(hash->data != NULL) {
		*ILHashLocateKey(hash,key) = -1;
		hash->inserted_keys--;
	}
}

