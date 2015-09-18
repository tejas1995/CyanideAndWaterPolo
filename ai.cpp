#include <math.h>

#include "model.h"
#include "enums.h"
#include "structures.h"

#define WATER_DEPTH 320
#define MIN_DIVE_DISTANCE 40
#define DEFENSE_FACTOR 1.2
#define WADE_FACTOR 0.3
#define GRAVITY_ACCELERATION 0.4

float distance(player *Player)
{
    return float(Player[USER].getX() - Player[COMPUTER].getX());
}

float distance(player* Player, ball* Ball)
{
    return float(Ball->getX() - Player->getX());
}

float distance(player* Player, goal* Goal)
{
    return float(Goal->getBlankSpace().x - Player->getX());
}

float distance(goal* Goal, ball* Ball)
{
    return float(Goal->getBlankSpace().x - Ball->getX());
}

void getCompKeyStates(int* compKeyStates, player* Player, goal* Goal, ball* Ball)
{
    
    float sUserCompDistance = distance(Player);
    float sUserBallDistance = distance(&Player[USER], Ball);
    float sCompBallDistance = distance(&Player[COMPUTER], Ball);
    float sUGoalBallDistance = distance(&Goal[USER], Ball);
    float sCGoalBallDistance = distance(&Goal[COMPUTER], Ball);
    float sCompCGoalDistance = distance(&Player[COMPUTER], &Goal[COMPUTER]);
    float sCompUGoalDistance = distance(&Player[COMPUTER], &Goal[USER]);
    
    if(sCompBallDistance <= 100)
    {
        compKeyStates[KEY_K] = 1;
    }

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