#include "Board.h"
#include "debug.h"
#include "PlayerMinMax.h"
#include "ILHash.h"

#include <string.h>
#include <pthread.h>
#include <unistd.h>

ILHash *PlayerMinMaxExplored;

uint8_t PlayerDepthLimit = 0;
uint32_t PlayerUnexplored = 0;
uint32_t PlayerExplored = 0;
uint32_t PlayerCaptured = 0;
uint32_t PlayerPruned = 0;

Position PlayerMinMaxWolfPos;

pthread_t PlayerMinMaxWorkerThreadID;

static void PlayerMinMaxIntHandler(int signal) {
	fprintf(stderr, "Got Int, aborting...\n");
	char response;
	if(PlayerMinMaxWorkerThreadID == 0) {
		fprintf(stderr, "\nYou sure you want to quit? (y/N)");
		fflush(stdin);
		response = getc(stdin); 
		if(response == 'y') {
			exit(0);
		}
	}
	else {
		pthread_cancel(PlayerMinMaxWorkerThreadID);
	}
}

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
	PlayerMinMaxWorkerThreadID = 0;
	if(signal(SIGINT, PlayerMinMaxIntHandler) == SIG_ERR) {
		fprintf(stderr, "Setting up PlayerMinMax mignal handler failed\n");
	}
}

void PlayerMinMaxCleanup() {
}

typedef struct {
	Board *b;
	ACTION *move;
} PlayerMinMaxWorkerArgs;

void *PlayerMinMaxWorker(void *args) {
	ACTION a, direction,*move;
	Board *b = ((PlayerMinMaxWorkerArgs*)args)->b;
	move = ((PlayerMinMaxWorkerArgs*)args)->move;

	Position rabbit = b->players[RABBIT];
	Position enemy_pos = b->players[WOLF];
	Position pos;
	int max_cost = 0,
		 cost;
	
	//  PlayerDepthLimit <= 15
	for(PlayerDepthLimit = 1;; PlayerDepthLimit++) {
		PlayerMinMaxExplored = ILHashAlloc();
		ILHashInit(PlayerMinMaxExplored,PlayerDepthLimit+3);

		PlayerExplored = PlayerPruned = PlayerCaptured = PlayerUnexplored = 0;
		for(a = 0; a < 8; a++) {
			pthread_testcancel();
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
		dprintf(stderr," l:%2d | e:%7d | p:%7d | u:%7d | c:%5d | %s\n",
				PlayerDepthLimit, PlayerExplored, PlayerPruned,
				PlayerUnexplored, PlayerCaptured, ActionNames[direction]);
		ILHashFree(PlayerMinMaxExplored);
		if(PlayerUnexplored == 0) {
			dprintf(stderr, "Dr. Livingstone I presume...\n");
			break;
		}
		*move = direction;
	}

	return (void*)0;
}/*}}}*/

ACTION PlayerMinMaxNextMove(Board *b) {/*{{{*/
	ACTION move;
	void *status;
	PlayerMinMaxWorkerArgs args;
	args.b = b;
	args.move = &move;

	pthread_create(&PlayerMinMaxWorkerThreadID, NULL, PlayerMinMaxWorker,&args);
	sleep(1);
	pthread_cancel(PlayerMinMaxWorkerThreadID);
	pthread_join(PlayerMinMaxWorkerThreadID,&status);
	PlayerMinMaxWorkerThreadID = 0;
	return move;
}
