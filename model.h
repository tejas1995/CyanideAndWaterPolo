#ifndef MODEL_H
#define MODEL_H

#define PI 3.141592

#include "enums.h"
#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "structures.h"
using namespace std;

int updateObjects(int* keystates, player* player, goal* goals, ball* ball, water* water, SDL_Rect wall[], int pCode);																										
int checkCollision(player* player1, player* player2);
int checkCollision(ball* ball, player* player);
int checkCollision(ball* ball, hand* hand);
int checkCollision(ball* ball, SDL_Rect rect);
int checkCollision(player* player, SDL_Rect rect);
int collisionReact( entity*, entity*, int, int, float);
int changePositions(entity* eball, entity* eplayer[]);
int handShot( player* player, ball* ball, float e, int r);
#endif
