#include "Board.h"

#include <string.h>

#include "debug.h"

#define BoardDataPos(b,x,y)	((b)->data[((b)->width+1)*(y)+(x)])

char *ActionNames[] = {"SW","S","SE","W","E","NW","N","NE" };

Board *BoardAlloc() {
	return malloc(sizeof(Board));
}

void BoardInit(Board *b,uint8_t width,uint8_t height,const char *data) {/*{{{*/
	char *p;
	uint8_t x = 0,y = 0;

	b->width = width;
	b->height = height;
	b->data_size = sizeof(char)*(width+1)*height;
	b->data = malloc(b->data_size);
	memset(b->data,' ',b->data_size);
	
	for(p = (char *)data; *p != 0; p++) {
		switch(*p) {
			case ' ':
				x++;
				break;
			case '\n':
				BoardSet(b,width,y,'\n');
				y++;
				x = 0;
				break;
			case 'R':
				b->players[RABBIT].x = x;
				b->players[RABBIT].y = y;
				x++;
				break;
			case 'W':
				b->players[WOLF].x = x;
				b->players[WOLF].y = y;
				x++;
				break;
			default:
				BoardSet(b,x,y,*p);
				x++;
		}
		if((p - data) >= b->data_size) {
			break;
		}
	}
	for(;y < b->height; y++) {
		BoardSet(b,width,y,'\n');
	}

	b->data[b->data_size-1] = '\0';

	//ifdebug(BoardPrint(b));

} /*}}}*/

void BoardFree(Board *b) {/*{{{*/
	if(b != NULL) {
		if(b->data != NULL) {
			free(b->data);
		}
		free(b);
	}
}/*}}}*/

void BoardSet(Board *b,uint8_t x,uint8_t y,char value) {/*{{{*/
	//b->data[(b->width+1)*y+x] = value;
	BoardDataPos(b,x,y) = value;
	//dprintf(stderr,"------\n%s\n------\nx:%d y:%d\n",b->data,x,y);
}/*}}}*/

void BoardHeader(uint8_t width) {
	uint8_t i;

	putchar('+');
	for(i = 0; i < width; i++) {
		putchar('-');
	}
	putchar('+');
	putchar('\n');
}

void BoardPrint(Board *b) {/*{{{*/
	char *output = malloc(b->data_size);
	char *print,*s;
	memcpy(output,b->data,b->data_size);
	Position pos;

	// dprintf(stderr, "<%s>\n",output);
	BoardHeader(b->width);
	pos = b->players[RABBIT];
	output[(b->width+1)*pos.y+pos.x] = 'R';

	pos = b->players[WOLF];
	output[(b->width+1)*pos.y+pos.x] = 'W';

	for(s = output;(s-output)<b->data_size;s++) {
		for(print = s; *s != '\n' && *s != '\0'; s++);
		*s = '\0';
		fprintf(stderr,"|%s|\n", print);
	}

	BoardHeader(b->width);
	free(output);
}/*}}}*/


void BoardMovePlayer(Board *b,PLAYER p, ACTION direction) { /*{{{*/
	Position *pos = &(b->players[p]);
	Position n_pos = PositionNew(*pos,direction);

	if(BoardIsFree(b,&n_pos)) {
		*pos = n_pos;
	}
} /*}}}*/

int BoardPositionIsOnBoard(Board *b, Position *p) {
	return (p->x < b->width) && (p->y < b->height);
}
int BoardIsFree(Board *b,Position *p) {/*{{{*/
	return BoardPositionIsOnBoard(b,p) && (' ' == BoardDataPos(b,p->x,p->y));
}/*}}}*/

Position PositionMoveSW(Position p) { return (Position){ p.x-1, p.y+1 }; }
Position PositionMoveS(Position p)  { return (Position){ p.x, p.y+1 }; }
Position PositionMoveSE(Position p) { return (Position){ p.x+1, p.y+1 }; }
Position PositionMoveW(Position p)  { return (Position){ p.x-1, p.y }; }
Position PositionMoveE(Position p)  { return (Position){ p.x+1, p.y }; }
Position PositionMoveNW(Position p) { return (Position){ p.x-1, p.y-1 }; }
Position PositionMoveN(Position p)  { return (Position){ p.x, p.y-1 }; }
Position PositionMoveNE(Position p) { return (Position){ p.x+1, p.y-1 }; }

typedef Position(*PositionAction)(Position);

PositionAction PositionActions[8] = {/*{{{*/
	PositionMoveSW,
	PositionMoveS,
	PositionMoveSE,
	PositionMoveW,
	PositionMoveE,
	PositionMoveNW,
	PositionMoveN,
	PositionMoveNE
};/*}}}*/

Position PositionNew(Position p, ACTION a) {
	return (*PositionActions[a])(p);
}

uint8_t PositionDistance(Position *a, Position *b) {
	uint8_t x,y;
	x = abs((int)(a->x) - (int)(b->x));
	y = abs((int)(a->y) - (int)(b->y));

	return (x > y) ? x : y;
}

int PositionEqual(Position *a, Position *b) {
	return (a->x == b->x) && (a->y == b->y);
}

void PositionPrint(FILE *stream, Position *p) {
	fprintf(stream,"<%d,%d>",p->x,p->y);
}
