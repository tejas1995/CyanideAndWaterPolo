#include <math.h>

#include "model.h"
#include "enums.h"
#include "structures.h"

#define WATER_DEPTH 320
#define MIN_DIVE_DISTANCE 40
#define DEFENSE_FACTOR 1.2
#define WADE_FACTOR 0.3

float distance(player *Player)
{
    return Player[USER].getX() - Player[COMPUTER].getX();
}

float distance(player Player, ball Ball)
{
    return Ball.getX() - Player.getX();
}

float distance(player Player, goal Goal)
{
    return Goal.getBlankSpace().getX() - Player.getX();
}

float distance(goal Goal, ball Ball)
{
    return Goal.getBlankSpace.getX() - Ball.getX();
}

int compKeyStates[KEY_TOTAL];

void initCompKeyStates()
{
    for(int i = 0; i < KEY_TOTAL; i++)
        compKeyStates[i] = 0;
}

int* getCompKeyStates(player* Player, goal* Goal, ball Ball)
{
    float sUserCompDistance = distance(Player);
    float sUserBallDistance = distance(Player[USER], Ball);
    float sCompBallDistance = distance(Player[COMPUTER], Ball);
    float sUGoalBallDistance = distance(Goal[USER], Ball);
    float sCGoalBallDistance = distance(Goal[COMPUTER], Ball);
    float sUserUGoalDistance = distance(Player[USER], Goal[USER]);
    float sCompCGoalDistance = distance(Player[COMPUTER], Goal[COMPUTER]);
    float sCompUGoalDistance = distance(Player[COMPUTER], Goal[USER]);

    if(sUserBallDistance*sCompBallDistance > 0)
    {
        compKeyStates[KEY_SHIFT] = 1; //Always be swimming

        if(sCompBallDistance < 0)   //Move computer player to left towards the ball
            compKeyStates[KEY_A] = 1;
        else                        //Move computer player to right towards the ball
            compKeyStates[KEY_D] = 1;

        if(fabs(sUserCompDistance) <= MIN_DIVE_DISTANCE && 
            Player[COMPUTER].getY() + Player[COMPUTER].getTexture()->getHeight() <= WATER_DEPTH)
        {
            compKeyStates[KEY_S] = 1;
        }

        if(Player[COMPUTER].getY() + Player[COMPUTER].getTexture()->getHeight() > WATER_DEPTH && compKeyStates[KEY_S] == 1)
        {
            if(fabs(sCompBallDistance) >= fabs(sUserBallDistance))  //Computer is closer to the ball than user
            {
                if(fabs(sUGoalBallDistance) < fabs(sCompUGoalDistance)) //Ball closer to the user goal than computer => computer should resurface
                {
                    compKeyStates[KEY_S] = 0;   //Release the down key
                }
                else if(fabs(sCGoalBallDistance) < fabs(sCompCGoalDistance) &&
                        fabs(sCompBallDistance) > fabs(sUserCompDistance)) //Ball close to the computer goal than computer & computer midway between user and ball => computer should resurface
                {
                    compKeyStates[KEY_S] = 0;
                }
            }
            
        }
    }
    else
    {
        compKeyStates[KEY_SHIFT] = 1; //By default, swim

        if(fabs(sCompBallDistance) >= DEFENSE_FACTOR*fabs(sUserBallDistance) && fabs(sCGoalBallDistance) > fabs(sCompCGoalDistance))
        {
            if(sCompBallDistance < 0)   //Move computer player to right away from the ball
                compKeyStates[KEY_D] = 1;
            else                        //Move computer player to left away from the ball
                compKeyStates[KEY_A] = 1;
        }
        else
        {
            if(sCompBallDistance < 0)   //Move computer player to left towards the ball
                compKeyStates[KEY_A] = 1;
            else                        //Move computer player to rightt towards the ball
                compKeyStates[KEY_D] = 1;

            if(fabs(sCompBallDistance) < WADE_FACTOR*fabs(sUserBallDistance))
            {
                compKeyStates[KEY_SHIFT] = 0;   //Make computer wade
            }
            
        }
    }
}