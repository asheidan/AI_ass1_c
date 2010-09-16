#include <stdio.h>
#include "debug.h"
#include "Board.h"
#include "PlayerHuman.h"
#include "PlayerMinMax.h"

void usage(const char *name){
	fprintf(stdout, "%s mapfile\n",name);
}

Board *initializeBoard(const char *filename) {/*{{{*/
	FILE *mapfile;
	Board *b;
	int
		width,
		height;
	size_t mapsize;
	char *mapdata;

	mapfile = fopen(filename,"r");
	if(NULL == mapfile) {
		perror(filename);
		return NULL;
	}

	b = BoardAlloc();

	if(0 == fscanf(mapfile,"%d %d",&height,&width)) {
		perror("Mapscanning");
		return NULL;
	}
	dprintf(stderr, "Read map: [w:%d,h:%d]\n",width,height);

	if(fgetc(mapfile) != '\n') {
		fprintf(stderr, "Map format error\n");
		return NULL;
	}

	mapsize = (height) * (width+1);
	mapdata = malloc(sizeof(char) * mapsize);
	if(NULL == mapdata) {
		perror("malloc");
		return NULL;
	}

	if(fread(mapdata,sizeof(char),mapsize,mapfile) <= 0) {
		perror("Reading mapfile");
		return NULL;
	}

	BoardInit(b,width,height,mapdata);
	//BoardPrint(b);

	free(mapdata);
	fclose(mapfile);

	return b;
}/*}}}*/

int main(int argc, char *argv[]) {
	Board *b;
	ACTION a;

	if(argc < 2) {
		usage(argv[0]);
		return 1;
	}

	b = initializeBoard(argv[1]);
	if(NULL == b) {
		return -1;
	}

	PlayerHumanInit();
	PlayerMinMaxInit(b);

	for(;;) {
		if((a = PlayerMinMaxNextMove(b)) >= 0) {
			BoardMovePlayer(b,RABBIT,a);
		}
		if(PositionEqual(&(b->players[RABBIT]),&(b->players[WOLF]))) {
			fprintf(stdout, "Game over!!!\n");
			break;
		}
		BoardPrint(b);

		if((a = PlayerHumanNextMove(b)) >= 0) {
			BoardMovePlayer(b,WOLF,a);
		}
		else {
			fprintf(stderr, "Bad input, quitting\n");
		}

		if(PositionEqual(&(b->players[RABBIT]),&(b->players[WOLF]))) {
			fprintf(stdout, "Game over!!!\n");
			break;
		}

	}

	BoardPrint(b);

	PlayerMinMaxCleanup();
	BoardFree(b);

	return 0;
}
