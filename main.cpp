#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;

#include "structures.h"
#include "graphics.h"
#include "model.h"
#include "enums.h"
#include "ai.h"

#define WATER_DEPTH 320
#define GOAL_WIDTH 80
#define GOAL_HEIGHT 160
#define GOAL_THICK 10
#define CROSS_WADE_TIME 16
#define CROSS_SWIM_TIME 8
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

player Player[2];
ball Ball;
goal Goal[2];
int Score[2];
water Water;
Uint32 startTime = 0;

int userKeyStates[ KEY_TOTAL ];
int compKeyStates[ KEY_TOTAL ];

void initialize()
{
    //Initialize the user player
    Player[ USER ].setX(560);
    Player[ USER ].setY(WATER_DEPTH);
    Player[ USER ].getVelocity()->setX(0);
    Player[ USER ].getVelocity()->setY(0);

    //Initialize the computer player
    Player[ COMPUTER ].setX(80);
    Player[ COMPUTER ].setY(WATER_DEPTH);
    Player[ COMPUTER ].getVelocity()->setX(0);
    Player[ COMPUTER ].getVelocity()->setY(0);
    
    //Initialize the Ball object
    Ball.setX(306);
    Ball.setY(160);
    Ball.getVelocity()->setX(0);
    Ball.getVelocity()->setY(0);

    //Initialize the user goal
    Goal[ USER ].setX(560);
    Goal[ USER ].setY(160);
    Goal[ USER ].defineTopNet( Goal[ USER ].getX(), Goal[ USER ].getY(), GOAL_WIDTH, GOAL_THICK );
    Goal[ USER ].defineBackNet( SCREEN_WIDTH - GOAL_THICK, Goal[ USER ].getY(), GOAL_THICK, GOAL_HEIGHT );
    Goal[ USER ].defineBlankSpace( Goal[ USER ].getX(), Goal[ USER ].getY() - GOAL_THICK, GOAL_WIDTH - GOAL_THICK, GOAL_HEIGHT - GOAL_THICK);

    //Initialize the computer goal
    Goal[ COMPUTER ].setX(0);
    Goal[ COMPUTER ].setY(160);
    Goal[ COMPUTER ].defineTopNet( Goal[ COMPUTER ].getX(), Goal[ COMPUTER ].getY(), GOAL_WIDTH, GOAL_THICK );
    Goal[ COMPUTER ].defineBackNet( Goal[COMPUTER].getX(), Goal[ COMPUTER ].getY(), GOAL_THICK, GOAL_HEIGHT );
    Goal[ COMPUTER ].defineBlankSpace( Goal[ COMPUTER ].getX() + GOAL_THICK, Goal[ COMPUTER ].getY() - GOAL_THICK, GOAL_WIDTH - GOAL_THICK, GOAL_HEIGHT - GOAL_THICK);

    //Initializethe water object
    Water.setDepth(WATER_DEPTH);

    Score[ USER ] = 0;
    Score[ COMPUTER ] = 0;

    while(!init())
    {
        SDL_Delay(500); //Wait for 0.5 seconds between each init()
    }

    for(int i = 0; i < KEY_TOTAL; i++)
    {
        userKeyStates[i] = 0;
        compKeyStates[i] = 0;
    }
}

bool pollForUserInput()
{
    bool quit = false;
    SDL_Event e;
    while(SDL_PollEvent( &e ) != 0)
    {
        if(e.type == SDL_QUIT)
        {
            quit = true;
        }   

        const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
        if( currentKeyStates[ SDL_SCANCODE_W ] )
        {
            userKeyStates[ KEY_W ] = 1;
        }
        else if( currentKeyStates[ SDL_SCANCODE_A ] )
        {
            userKeyStates[ KEY_A ] = 1;
        }
        else if( currentKeyStates[ SDL_SCANCODE_S ] )
        {
            userKeyStates[ KEY_S ] = 1;
        }
        else if( currentKeyStates[ SDL_SCANCODE_D ] )
        {
            userKeyStates[ KEY_D ] = 1;
        }
        else if( currentKeyStates[ SDL_SCANCODE_LSHIFT ] || currentKeyStates[ SDL_SCANCODE_RSHIFT ])
        {
            userKeyStates[ KEY_SHIFT ] = 1;
        }
        else if( currentKeyStates[ SDL_SCANCODE_K ] )
        {
            userKeyStates[ KEY_K ] = 1;
        }
        else if( currentKeyStates[ SDL_SCANCODE_L ] )
        {
            userKeyStates[ KEY_L ] = 1;
        }
    }

    return quit;
}

void resetKeyStates()
{
    for(int i = 0; i < KEY_TOTAL; i++)
    {
        userKeyStates[i] = 0;
        compKeyStates[i] = 0;
    }
}

void game()
{
    startTime = SDL_GetTicks();
    bool quit = false;
    bool game_in_play = false;
    
    init();
    loadMedia(Player, Goal, &Ball, &Water);

    while(!quit)
    {
        if(pollForUserInput() == true) //false return corresponds to game being quit
        {
            quit = true;
        }
        
        //Send userKeyStates to the physics model
        updateObjects(userKeyStates, Player, Goal, &Ball, &Water, USER);

        //Check for Score updation
        if(checkCollision(Ball, Goal[ USER ].getBlankSpace()) != 0)
            Score[ COMPUTER ] += 1;
        else if(checkCollision(Ball, Goal[ COMPUTER ].getBlankSpace()) != 0)
            Score[ USER ] += 1;

        //Get compKeyStates and send to the physics model
        compKeyStates = getCompKeyStates(Player, Goal, Ball);
        updateObjects(compKeyStates, Player, Goal, &Ball, &Water, COMPUTER);

        resetKeyStates();
        setTime(SDL_GetTicks(), startTime);
        frameRender(Player, &Ball);
    }

    printf("User %d - %d Computer\n", Score[USER], Score[COMPUTER]);
    closeObjectTextures(Player, &Ball);
}

int main(int argc, char* args[])
{
    initialize();
    game();

    return 0;
}
