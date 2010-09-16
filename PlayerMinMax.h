#ifndef __PLAYER_MIN_MAX_H__
#define __PLAYER_MIN_MAX_H__

#include <stdbool.h>

void PlayerMinMaxInit(Board *b);
void PlayerMinMaxCleanup();
ACTION PlayerMinMaxNextMove(Board *b);

#endif
