#include <stdio.h>
#include <iostream>
using namespace std;

#include "structures.h"
#include "graphics.h"

#define WATER_DEPTH 320
#define CROSS_WADE_TIME 16
#define CROSS_SWIM_TIME 8

player Player[2] = {Player(60, SCREEN_WIDTH/CROSS_WADE_TIME, SCREEN_WIDTH/CROSS_SWIM_TIME),
                    Player(60, SCREEN_WIDTH/CROSS_WADE_TIME, SCREEN_WIDTH/CROSS_SWIM_TIME)};
ball Ball;

enum PlayerCodes
{
    USER,
    COMPUTER
};

void initialize()
{
    Player[ USER ].setX(560);
    Player[ USER ].setY(WATER_DEPTH);
    Player[ USER ].getVelocity().setX(0);
    Player[ USER ].getVelocity().setY(0);

    Player[ COMPUTER ].setX(80);
    Player[ COMPUTER ].setY(WATER_DEPTH);
    Player[ COMPUTER ].getVelocity().setX(0);
    Player[ COMPUTER ].getVelocity().setY(0);
    
    Ball.setX(320);
    Ball.setY(640);
    Ball.getVelocity().setX(0);
    Ball.getVelocity().setY(0);
}

void game()
{
    bool quit = false;

    while(!quit)
    {
        if(poll_for_user_input() == false) //false return corresponds to game being quit
        {
            quit = true;
        }

        
    }
}

void main()
{
    initialize();
    game();
}