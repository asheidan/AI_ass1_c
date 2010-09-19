#include "Board.h"
#include "debug.h"
#include "PlayerMinMax.h"
#include "ILHash.h"
#include <string.h>

ILHash *PlayerMinMaxExplored;

uint8_t PlayerDepthLimit = 0;
uint32_t PlayerUnexplored = 0;
uint32_t PlayerExplored = 0;
uint32_t PlayerCaptured = 0;
uint32_t PlayerPruned = 0;

Position PlayerMinMaxWolfPos;

void PlayerMinMaxSetWolfPos(Position *p) {
	PlayerMinMaxWolfPos = *p;
}

int PlayerMinMaxCompPos(void *a, void *b) {
	return PositionDistance(&PlayerMinMaxWolfPos,a) > PositionDistance(&PlayerMinMaxWolfPos,b);
}


int PlayerMinMaxUtility(Board *b, Position *my_pos, Position *enemy_pos) {
	return PositionDistance(my_pos,enemy_pos);
}

int PlayerMinMaxCost(Board *b,Position my_pos, Position enemy_pos, unsigned int cost, bool maximize, unsigned int p_min, unsigned int p_max) {/*{{{*/
	ACTION a;
	Position pos;
	ILHashKey key;
	int c, opt_cost;

	if(PositionEqual(&my_pos,&enemy_pos)) {
		PlayerCaptured++;
		// PositionPrint(stderr, &my_pos);
		// PositionPrint(stderr, &enemy_pos);
		// dprintf(stderr,"Captured at depth: %d\n",cost);
		return cost;
	}
	if(cost >= PlayerDepthLimit) {
		PlayerUnexplored++;
		return cost + PlayerMinMaxUtility(b,&my_pos,&enemy_pos);
	}
	//cost++;
	opt_cost = maximize ? 0 : INT32_MAX;
	
	for(a = 0; a < 8; a++) {
		pos = maximize ? PositionNew(my_pos,a) : PositionNew(enemy_pos,a);
		key = maximize ? Pos2Hash(my_pos,pos) : Pos2Hash(pos,enemy_pos);
		if(BoardIsFree(b,&pos) && !ILHashHasKey(PlayerMinMaxExplored,key)) {
			ILHashInsertKey(PlayerMinMaxExplored,key);
			PlayerExplored++;
			c = maximize ?
				PlayerMinMaxCost(b,pos,enemy_pos,cost+1,WOLF,p_min,p_max)
				:
				PlayerMinMaxCost(b,my_pos,pos,cost+1,RABBIT,p_min,p_max) ;
			if((maximize && (c > opt_cost)) || (!maximize && (c < opt_cost))) {
				opt_cost = c;
				if(maximize && opt_cost == INT32_MAX) {
					fprintf(stderr, "---------ERRROER: \n");
				}
				if(maximize) {
					if(opt_cost > p_max) {
						p_max = opt_cost;
					}
					if( opt_cost >= p_min ) {
						PlayerPruned++;
						ILHashRemoveKey(PlayerMinMaxExplored,key);
						break;
					}
				}
				else {
					if(opt_cost < p_min) {
						p_min = opt_cost;
					}
					if( opt_cost <= p_max ) {
						PlayerPruned++;
						ILHashRemoveKey(PlayerMinMaxExplored,key);
						break;
					}
				}
			}
			ILHashRemoveKey(PlayerMinMaxExplored,key);
		}
	}
	if(opt_cost == 0 || opt_cost == INT32_MAX) {
		/* dead-end */
		return cost + PlayerMinMaxUtility(b,&my_pos,&enemy_pos);
	}
	return opt_cost;
}/*}}}*/

void PlayerMinMaxInit(Board *b) {
}

void PlayerMinMaxCleanup() {
}

ACTION PlayerMinMaxNextMove(Board *b) {/*{{{*/
	ACTION a, direction;
	Position rabbit = b->players[RABBIT];
	Position enemy_pos = b->players[WOLF];
	Position pos;
	int max_cost = 0,
		 cost;

	for(PlayerDepthLimit = 1; PlayerDepthLimit <= 15; PlayerDepthLimit++) {
		PlayerMinMaxExplored = ILHashAlloc();
		ILHashInit(PlayerMinMaxExplored,PlayerDepthLimit+3);

		PlayerExplored = PlayerPruned = PlayerCaptured = PlayerUnexplored = 0;
		for(a = 0; a < 8; a++) {
			pos = PositionNew(rabbit,a);
			if(BoardIsFree(b,&pos)) {
				ILHashInsertKey(PlayerMinMaxExplored,Pos2Hash(pos,enemy_pos));

				PlayerExplored++;
				cost = PlayerMinMaxCost(b,pos,enemy_pos,1,false,INT32_MAX,0);
				if(cost > max_cost) {
					max_cost = cost;
					direction = a;
				}
				dprintf(stderr,"[%-2s%2d] ",ActionNames[a],cost);

				ILHashRemoveKey(PlayerMinMaxExplored,Pos2Hash(pos,enemy_pos));
			}
		}
		// dprintf(stderr," d:%3d |",max_cost);
		dprintf(stderr," l:%2d | e:%7d | p:%4d | u:%6d | c:%5d | %s\n",
				PlayerDepthLimit, PlayerExplored, PlayerPruned,
				PlayerUnexplored, PlayerCaptured, ActionNames[direction]);
		ILHashFree(PlayerMinMaxExplored);
		if(PlayerUnexplored == 0) {
			dprintf(stderr, "Dr. Livingstone I presume...\n");
			break;
		}
	}

	return direction;
}/*}}}*/
