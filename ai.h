#ifndef AI_H
#define AI_H

#include "structures.h"
#include "enums.h"

float distance(player* Player);
float distance(player* Player, ball* Ball);
float distance(player* Player, goal* Goal);
float distance(ball* Ball, goal* Goal);
void initCompKeyStates();
int* getCompKeyStates(int*, player*, goal*, ball*);

#endif