#ifndef __BOARD_H__
#define __BOARD_H__
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define Pos2Hash(a,b)	((a).x|((a).y<<8)|((b).x<<16)|((b).y<<24))

typedef struct {
	uint8_t x,y;
} Position;

typedef enum { WOLF,RABBIT } PLAYER;
typedef enum { SW,S,SE,W,E,NW,N,NE } ACTION;
extern char *ActionNames[];

typedef struct {
	uint8_t width;
	uint8_t height;
	char *data;
	size_t data_size;
	Position players[2];
} Board;

Board *BoardAlloc();
void BoardInit(Board *b,uint8_t width,uint8_t height,const char *data);
void BoardFree(Board *b);
void BoardSet(Board *b,uint8_t x,uint8_t y,char value);
void BoardPrint(Board *b);
int BoardIsFree(Board *b,Position *p);
void BoardMovePlayer(Board *b, PLAYER p, ACTION direction);

Position PositionNew(Position p, ACTION a);
uint8_t PositionDistance(Position *a, Position *b);
int PositionEqual(Position *a, Position *b);
void PositionPrint(FILE *stream, Position *p);

#endif
