#include <stdio.h>
#include <iostream>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;

#include "structures.h"
#include "graphics.h"
#include "model.h"
#include "enums.h"
#include "ai.h"

#define WATER_DEPTH 400
#define GOAL_WIDTH 80
#define GOAL_HEIGHT 160
#define GOAL_THICK 10
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600
#define BASE_HEIGHT 320
#define GRAVITY_ACCELERATION 0.2
#define JUMP_VELOCITY 4.0
#define WADE_VELOCITY 4.0
#define SWIM_VELOCITY 3.5
#define INIT_ANGLE 150

player Player[2];
ball Ball;
goal Goal[2];
int Score[2];
water Water;
SDL_Rect Wall[4];
Uint32 startTime = 0;

int userKeyStates[ KEY_TOTAL ];
int compKeyStates[ KEY_TOTAL ];

void initialize()
{
    //Initialize the user player
    Player[ USER ].setX(840);
    Player[ USER ].setY(BASE_HEIGHT+10);
    Player[ USER ].getVelocity()->setX(0);
    Player[ USER ].getVelocity()->setY(0);
    Player[ USER ].getHand()->setAngle(INIT_ANGLE);
    Player[ USER ].setAttributes(JUMP_VELOCITY,WADE_VELOCITY,SWIM_VELOCITY);

    //Initialize the computer player
    Player[ COMPUTER ].setX(80);
    Player[ COMPUTER ].setY(BASE_HEIGHT+10);
    Player[ COMPUTER ].getVelocity()->setX(0);
    Player[ COMPUTER ].getVelocity()->setY(0);
    Player[ COMPUTER ].getHand()->setAngle(360 - INIT_ANGLE);
    Player[ COMPUTER ].setAttributes(JUMP_VELOCITY,WADE_VELOCITY,SWIM_VELOCITY);

    //Initialize the Ball object
    Ball.setX(478);
    Ball.setY(160);
    Ball.getVelocity()->setX(0);
    Ball.getVelocity()->setY(0);

    //Initialize the user goal
    Goal[ USER ].setX(920);
    Goal[ USER ].setY(200);
    Goal[ USER ].defineTopNet( Goal[ USER ].getX(), Goal[ USER ].getY(), GOAL_WIDTH, GOAL_THICK );
    Goal[ USER ].defineBackNet( SCREEN_WIDTH - GOAL_THICK, Goal[ USER ].getY(), GOAL_THICK, GOAL_HEIGHT );
    Goal[ USER ].defineBlankSpace( Goal[ USER ].getX(), Goal[ USER ].getY() + GOAL_THICK, GOAL_WIDTH - GOAL_THICK, GOAL_HEIGHT - GOAL_THICK);

    //Initialize the computer goal
    Goal[ COMPUTER ].setX(0);
    Goal[ COMPUTER ].setY(200);
    Goal[ COMPUTER ].defineTopNet( Goal[ COMPUTER ].getX(), Goal[ COMPUTER ].getY(), GOAL_WIDTH, GOAL_THICK );
    Goal[ COMPUTER ].defineBackNet( Goal[COMPUTER].getX(), Goal[ COMPUTER ].getY(), GOAL_THICK, GOAL_HEIGHT );
    Goal[ COMPUTER ].defineBlankSpace( Goal[ COMPUTER ].getX() + GOAL_THICK, Goal[ COMPUTER ].getY() + GOAL_THICK, GOAL_WIDTH - GOAL_THICK, GOAL_HEIGHT - GOAL_THICK);

    //Initializethe water object
    Water.setDepth(WATER_DEPTH);

    Score[ USER ] = 0;
    Score[ COMPUTER ] = 0;

    Wall[0].x = 0;
    Wall[0].y = 0;
    Wall[0].w = SCREEN_WIDTH;
    Wall[0].h = 1;
    Wall[1].x = 0;
    Wall[1].y = 0;
    Wall[1].w = 1;
    Wall[1].h = SCREEN_HEIGHT;
    Wall[2].x = 0;
    Wall[2].y = SCREEN_HEIGHT-1;
    Wall[2].w = SCREEN_WIDTH;
    Wall[2].h = 1;
    Wall[3].x = SCREEN_WIDTH-1;
    Wall[3].y = 0;
    Wall[3].w = 1;
    Wall[3].h = SCREEN_HEIGHT;

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
        if( currentKeyStates[ SDL_SCANCODE_A ] )
        {
            userKeyStates[ KEY_A ] = 1;
        }
        if( currentKeyStates[ SDL_SCANCODE_S ] )
        {
            userKeyStates[ KEY_S ] = 1;
        }
        if( currentKeyStates[ SDL_SCANCODE_D ] )
        {
            userKeyStates[ KEY_D ] = 1;
        }
        if( currentKeyStates[ SDL_SCANCODE_LSHIFT ] || currentKeyStates[ SDL_SCANCODE_RSHIFT ])
        {
            userKeyStates[ KEY_SHIFT ] = 1;
        }
        if( currentKeyStates[ SDL_SCANCODE_K ] )
        {
            userKeyStates[ KEY_K ] = 1;
        }
        if( currentKeyStates[ SDL_SCANCODE_L ] )
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
    int seconds_remaining;
    
    init();
    loadMedia(Player, Goal, &Ball, &Water);

    while(!quit)
    {
        if(pollForUserInput() == true) //false return corresponds to game being quit
        {
            quit = true;
        }
        
        //Send userKeyStates to the physics model
        updateObjects(userKeyStates, Player, Goal, &Ball, &Water, Wall, USER);
        getCompKeyStates(compKeyStates, Player, Goal, &Ball);
        updateObjects(compKeyStates, Player, Goal, &Ball, &Water, Wall, COMPUTER);
        
        //Check for Score updation
        bool scored = false;

        if(checkCollision(&Ball, Goal[ USER ].getBlankSpace()) != 0)
        {
            Score[ COMPUTER ] += 1;
            scored = true;
        }
        else if(checkCollision(&Ball, Goal[ COMPUTER ].getBlankSpace()) != 0)
        {
            scored = true;
            Score[ USER ] += 1;
        }

        if(scored)
        {
            scoreUpdate(Score);

            Player[ USER ].setX(840);
            Player[ USER ].setY(BASE_HEIGHT+10);
            Player[ USER ].getVelocity()->setX(0);
            Player[ USER ].getVelocity()->setY(0);
            Player[ USER ].getHand()->setAngle(0);

            //Initialize the computer player
            Player[ COMPUTER ].setX(80);
            Player[ COMPUTER ].setY(BASE_HEIGHT+10);
            Player[ COMPUTER ].getVelocity()->setX(0);
            Player[ COMPUTER ].getVelocity()->setY(0);
            Player[ COMPUTER ].getHand()->setAngle(0);

            //Initialize the Ball object
            Ball.setX(478);
            Ball.setY(160);
            Ball.getVelocity()->setX(0);
            Ball.getVelocity()->setY(0);

        }

        resetKeyStates();
        seconds_remaining = setTime(SDL_GetTicks(), startTime);

        if(seconds_remaining == 0)
        {
            quit = true;
            if(Score[USER] > Score[COMPUTER])
                printf("User wins\n");
            else if(Score[USER] < Score[COMPUTER])
                printf("Computer wins!\n");
            else
                printf("Draw!\n");
        }
        frameRender(Player, &Ball);

        if(scored)
        {
            SDL_Delay(2000);
            scored = false;
        }
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
