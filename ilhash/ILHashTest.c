#include "CuTest.h"
#include "ILHash.h"

#include "debug.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

extern uint32_t ILHashCollisionCount;

ILHash *ILHashTestSetup() {
	ILHash *h;
	h = ILHashAlloc();
	return h;
}
void ILHashTestTearDown(ILHash *hash) {
	ILHashFree(hash);
}

void TestHashAllocFree(CuTest *tc) {
	ILHash *hash = ILHashTestSetup();
	CuAssertPtrNotNullMsg(tc,"Allocation of hash failed",hash);
	ILHashTestTearDown(hash);
}

void TestHashInit(CuTest *tc) {
	ILHash *hash = ILHashTestSetup();
	unsigned int size = 32;

	ILHashInit(hash,size);

	ILHashTestTearDown(hash);
}

void TestHashInsertKey(CuTest *tc) {
	ILHash *hash = ILHashTestSetup();

	ILHashInit(hash,100);

	CuAssertTrue(tc, !ILHashHasKey(hash,0));
	CuAssertTrue(tc, !ILHashHasKey(hash,32));

	ILHashInsertKey(hash, 32);
	CuAssertTrue(tc, ILHashHasKey(hash,32));
	CuAssertIntEquals(tc,1,hash->inserted_keys);

	ILHashTestTearDown(hash);
}

void TestHashInsertSHouldNotOwerwriteOtherKey(CuTest *tc) {
	ILHash *hash = ILHashTestSetup();
	ILHashInit(hash, 2);

	ILHashKey
		a = 15,
		b = 32;

	ILHashInsertKey(hash, a);
	ILHashInsertKey(hash, b);

	CuAssertTrue(tc, ILHashHasKey(hash,a));
	CuAssertTrue(tc, ILHashHasKey(hash,b));

	ILHashTestTearDown(hash);
}

void TestHashShouldHandleInitialSize(CuTest *tc) {
	ILHash *hash = ILHashTestSetup();
	int i,n = 100000;
	
	ifdebug(ILHashCollisionCount = 0);

	ILHashInit(hash, n);

	for(i = 0; i < n; i++)
		ILHashInsertKey(hash, i);

	dprintf(stderr, "Collisioncount: %d\n", ILHashCollisionCount);

	for(i = 0; i < n; i++) {
		CuAssertTrue(tc,ILHashHasKey(hash,i));
	}

	ILHashTestTearDown(hash);
}

void TestHashRemoveKeyShouldRemoveKey(CuTest *tc) {
	ILHash *hash = ILHashTestSetup();
	ILHashKey
		a = 23;

	ILHashInit(hash,10);

	ILHashInsertKey(hash,a);
	ILHashRemoveKey(hash,a);
	CuAssertTrue(tc,!ILHashHasKey(hash,a));
	CuAssertIntEquals(tc,0,hash->inserted_keys);

	ILHashTestTearDown(hash);
}

