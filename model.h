#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

int updateObjects(int* keystates, player* players, goal* goals, ball* ball, water* water);																										
int checkCollision(player player1, player player2);
int checkCollision(ball ball, player player);
int checkCollision(ball ball, hand, hand);
int checkCollision(ball ball, SDL_Rect goal);
