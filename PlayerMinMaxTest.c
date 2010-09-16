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

