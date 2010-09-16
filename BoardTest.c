#include "CuTest.h"
#include "Board.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

/* CuHelper */
void CuAssertPositionEquals(CuTest *tc, Position *a, Position *b) {
	CuAssertIntEquals(tc,a->x,b->x);
	CuAssertIntEquals(tc,a->y,b->y);
}

const char *DAY2_MAP = "OOOO\nO    O\nO OOO \nR OO W\nO  O O\nOO   O";

Board *BoardTestSetup() {
	uint8_t
		width = 6,
		height = 6;
	Board *b = BoardAlloc();
	BoardInit(b,width,height,DAY2_MAP);
	return b;
}

void TestBoardAlloc(CuTest *tc) {
	Board *b = BoardAlloc();
	CuAssertPtrNotNull(tc,b);
	BoardFree(b);
}

void TestBoardInit(CuTest *tc) {
	Board *b = BoardTestSetup();
	// char *p;
	CuAssertIntEquals(tc,6,b->width);
	CuAssertIntEquals(tc,6,b->height);

	/*
	for(p = b->data; *p != 0; p++) {
		fprintf(stderr, "%p 0x%x '%c'\n", p, *p, *p);
	}
	*/
	CuAssertStrEquals(tc,
		"OOOO  \nO    O\nO OOO \n  OO  \nO  O O\nOO   O",
		b->data);
	BoardFree(b);
}

void TestBoardIsFree(CuTest *tc) {
	Board *b = BoardTestSetup();
	Position p = {1,2};
	CuAssertTrue(tc,BoardIsFree(b,&p));
	BoardFree(b);
}

void TestPositionNew(CuTest *tc) {
	Position expected = {0,0},
				result = {1,1};
	result = PositionNew(result,NW);
	CuAssertPositionEquals(tc,&expected,&result);
}

void TestMovePlayerMovesPlayer(CuTest *tc) {
	Board *b = BoardTestSetup();
	Position expected;
	expected.x = 1;
	expected.y = 3;
	BoardMovePlayer(b,RABBIT,E);
	CuAssertPositionEquals(tc, &expected, &(b->players[RABBIT]));
}

void TestPositionDistance(CuTest *tc) {
	Position
		a = {3,7},
		b = {1,4};

	CuAssertIntEquals(tc,3,PositionDistance(&a,&b));
}

void TestPos2Hash(CuTest *tc) {
	Position
		a = {3,7},
		b = {1,4};

	CuAssertIntEquals(tc,3|7<<8|1<<16|4<<24,Pos2Hash(a,b));
}
