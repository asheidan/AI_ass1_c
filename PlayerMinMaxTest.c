#include "CuTest.h"
#include "Board.h"
#include "PlayerMinMax.h"

#include <stdlib.h>
#include <stdio.h>

const char *PlayerMinMaxTestMap = "OOOO\nO    O\nO OOO \nR OO W\nO  O O\nOO   O";

Board *PlayerMinMaxTestSetup() {
	uint8_t
		width = 6,
		height = 6;
	Board *b = BoardAlloc();
	BoardInit(b,width,height,PlayerMinMaxTestMap);
	PlayerMinMaxInit(b);
	return b;
}

void PlayerMinMaxTestTearDown(Board *b) {
	PlayerMinMaxCleanup();
	BoardFree(b);
}

void TestPositionCompare(CuTest *tc) {
	Position a = {4,4};
	Position b = {4,5};
	Position wolf = {3,3};

	PlayerMinMaxSetWolfPos(&wolf);

	CuAssertTrue(tc,PlayerMinMaxCompPos((void*)&b,(void*)&a));
	CuAssertTrue(tc,!PlayerMinMaxCompPos((void*)&a,(void*)&b));
}
