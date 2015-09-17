#ifndef MODEL_H
#define MODEL_H
#define PI 3.141592

#include <model.h>
#include <enums.h>

int checkCollision(player player1, player player2)
{
	int x1 = player1.getX(); int x2 = player2.getX();
	int y1 = player1.getY(); int y2 = player2.getY();
	int r1 = player1.getRadius(); int r2 = player2.getRadius();

	x1 += r1; y1 += r1; x2 += r2; y2 += r2;

	float distance = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2))

	if(distance <= (r1+r2)) 
	{
		angle = acos((float)abs(y2-y1)/(float)distance)
		return angle*(-1);
	}
	return 0;
}

int checkCollision(ball ball, player player)
{
	int x1 = player.getX(); int x2 = ball.getX();
	int y1 = player.getY(); int y2 = ball.getY();
	int r1 = player.getRadius(); int r2 = ball.getRadius();

	x1 += r1; y1 += r1; x2 += r2; y2 += r2;

	float distance = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2))

	if(distance <= (r1+r2))
	{
		angle = acos((float)abs(y2-y1)/(float)distance)
		return angle*(-1);
	}
	return 0;
}

int checkCollision(ball ball, hand hand)
{
	float theta = hand.getAngle()*PI/180.0;
	float cosine = cos(theta); float sine = sin(theta);
	int radius = ball.getRadius();
	int h = ball.getX();
	int k = ball.getY();
	int x = hand.getX();
	int y = hand.getY();
	int width = hand.getWidth();
	int height = hand.getHeight();

	float distance = abs(cosine*(h-x) - sine*(k-y))
	float pointOfContact = sqrt((h-x)*(h-x) + (k-y)*(k-y) - distance*distance)

	if(radius <= (distance - width/2) && pointOfContact < height)
	{
		return pointOfContact;
	}

	int xTip = x - height*sine; int yTip = y - height*cosine;

	if(sqrt((h-xTip)*(h-xTip) + (k-yTip)*(k-yTip)) <= radius)
	{
		angle = acos((float)abs(y2-y1)/(float)distance)
		return angle*(-1);
	}

	return 0;
}

int checkCollision(ball ball, SDL_Rect rect)
{
	int ballTop = ball.getY();
	int ballBottom = ball.getY() + 2*ball.getRadius();
	int ballLeft = ball.getX();
	int ballRight = ball.getX() + 2*ball.getRadius();

	int rTop = rect.y;
	int rBottom = rect.y + rect.h;
	int rLeft = rect.x;
	int rRight = rect.x + rect.w;

	if(ballTop <= rBottom) return -1;
	if(ballBottom >= rTop) return -2;
	if(ballLeft <= rRight) return -3;
	if(ballRight >= rLeft) return -4;

	return 0;
}

#endif