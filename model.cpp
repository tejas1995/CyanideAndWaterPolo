#include "model.h"
#define KEY_PRESS_ACCELERATION_WADE 10
#define KEY_PRESS_ACCELERATION_SWIM 10
#define GRAVITY_ACCELERATION 10
#define BASE_HEIGHT 280

int collisionReact( entity*, entity*, int, int, float);
int checkCollision(player* player1, player* player2)
{
	int x1 = player1->getX(); int x2 = player2->getX();
	int y1 = player1->getY(); int y2 = player2->getY();
	int r1 = player1->getRadius(); int r2 = player2->getRadius();

	x1 += r1; y1 += r1; x2 += r2; y2 += r2;

	float distance = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));

	if(distance <= (r1+r2)) 
	{
		return 1;
	}
	return 0;
}

int checkCollision(ball *ball, player *player)
{
	int x1 = player->getX(); int x2 = ball->getX();
	int y1 = player->getY(); int y2 = ball->getY();
	int r1 = player->getRadius(); int r2 = ball->getRadius();

	x1 += r1; y1 += r1; x2 += r2; y2 += r2;

	float distance = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));

	if(distance <= (r1+r2))
	{
		return 1;
	}
	return 0;
}

int checkCollision(ball* ball, hand* hand)
{
	float theta = hand->getAngle()*PI/180.0;
	float cosine = cos(theta); float sine = sin(theta);
	int radius = ball->getRadius();
	int h = ball->getX();
	int k = ball->getY();
	int x = hand->getX();
	int y = hand->getY();
	int width = hand->getWidth();
	int height = hand->getHeight();

	float distance = fabs(cosine*(h-x) - sine*(k-y));
	float pointOfContact = sqrt((h-x)*(h-x) + (k-y)*(k-y) - distance*distance);

	if(radius <= (distance - width/2) && pointOfContact < height)
	{
		return pointOfContact;
	}

	int xTip = x - height*sine; int yTip = y - height*cosine;
	float distance2 = sqrt((h-xTip)*(h-xTip) + (k-yTip)*(k-yTip));
	if(distance2 <= radius)
	{
		int angle = acos((float)abs(yTip-k)/distance2)*180/PI;
		return angle*(-1);
	}

	return 0;
}

int checkCollision(ball *ball, SDL_Rect rect)
{
	int ballTop = ball->getY();
	int ballBottom = ball->getY() + 2*ball->getRadius();
	int ballLeft = ball->getX();
	int ballRight = ball->getX() + 2*ball->getRadius();

	int rTop = rect.y;
	int rBottom = rect.y + rect.h;
	int rLeft = rect.x;
	int rRight = rect.x + rect.w;
	if((ballTop <= rBottom) && (ballBottom >= rBottom) && (ballLeft-rLeft)*(ballRight-rRight) <= 0) 
			return -1;
	if((ballBottom >= rTop) && (ballTop <= rTop) && (ballLeft-rLeft)*(ballRight-rRight)<=0) 
			return -1;
	if((ballLeft <= rRight) && (ballRight >= rRight) && (ballBottom-rBottom)*(ballTop-rTop)<=0)
			return -2;
	if((ballRight >= rLeft) && (ballLeft <= rLeft) && (ballBottom-rBottom)*(ballTop-rTop)<=0) 
			return -2;
	
	return 0;
}

int updateObjects(int* keystates, player player[], goal goals[], ball* ball, water* water, int pCode){
	//Get mode
	entity* eball;
	entity* eplayer[2];
	
	eball = ball;
	eplayer[0] = &player[0];
	eplayer[1] = &player[1];
	bool colFlag = false;
	if(checkCollision(ball, &player[USER])>0){
		collisionReact( eball, eplayer[USER], 1, 10000, 0.9);
		colFlag = true;
	}
	if(checkCollision(ball, &player[COMPUTER])>0){
		collisionReact( eball, eplayer[COMPUTER], 1, 10000, 0.9);
		colFlag = true;
	}

	if(checkCollision(&player[USER], &player[COMPUTER])>0){
		collisionReact( eplayer[COMPUTER], eplayer[USER], 1, 1000, 0.9);
		colFlag = true;
	}
	if (colFlag){
		return 0;
	}

	int uvx = player[USER].getVelocity() -> getX();
	int uvy = player[USER].getVelocity() -> getY();
	int cvx = player[COMPUTER].getVelocity() -> getX();
	int cvy = player[COMPUTER].getVelocity() -> getY();

	if (keystates[KEY_SHIFT] == 1)
	{
		player[pCode].setMode(SWIM);
	}

	else
	{
		player[pCode].setMode(WADE);
	}
	//Now we know the mode
	
	if(player[pCode].getMode() == WADE)
	{
		if (keystates[KEY_A] == 1)
		{
			if(-uvx < player[pCode].getMaxWadeVelocity())
				uvx -= KEY_PRESS_ACCELERATION_WADE;
		}

		else
		{
			if(uvx < 0)
				uvx += KEY_PRESS_ACCELERATION_WADE;
		}

		if (keystates[KEY_D] == 1)
		{
			if(uvx < player[pCode].getMaxWadeVelocity())
				uvx += KEY_PRESS_ACCELERATION_WADE;
		}

		else
		{
			if(uvx > 0)
				uvx -= KEY_PRESS_ACCELERATION_WADE;
		}

		if (keystates[KEY_W] == 1)
		{
			if(player[pCode].getY() == BASE_HEIGHT)
				uvy = player[pCode].getMaxJumpVelocity();
			else
				uvy += GRAVITY_ACCELERATION;
		}
	}
	else if(player[pCode].getMode() == SWIM)
	{
		if (keystates[KEY_A] == 1)
		{
			if(-uvx < player[pCode].getMaxSwimVelocity())
				uvx -= KEY_PRESS_ACCELERATION_SWIM;
		}

		else
		{
			if(uvx < 0)
				uvx += KEY_PRESS_ACCELERATION_SWIM;
		}

		if (keystates[KEY_D] == 1)
		{
			if(uvx < player[pCode].getMaxSwimVelocity())
				uvx += KEY_PRESS_ACCELERATION_SWIM;
		}

		else
		{
			if(uvx < 0)
				uvx += KEY_PRESS_ACCELERATION_SWIM;
		}

		if (keystates[KEY_S] == 1)
		{
			if(uvy < player[pCode].getMaxSwimVelocity())
				uvy += KEY_PRESS_ACCELERATION_SWIM;
		}
	}

	return 0;
}
int collisionReact( entity* A, entity* B, int m1, int m2, float e){
	mVector dir;
	dir.setX(B->getX()-A->getX());
	dir.setY(B->getY()-A->getY());
	int vNormalAf,vNormalAi,vNormalBf,vNormalBi;
	vNormalAi = A->getVelocity()->dot(dir);
	vNormalBi = A->getVelocity()->dot(dir);

	vNormalBf = (m1*vNormalAi*(1+e) + (m2-m1*e)*vNormalBi) / (m1+m2);
	vNormalAf = (m2*vNormalBi*(1+e) + (m1-m2*e)*vNormalAi) / (m1+m2);

	mVector dirP;
	dirP.setX(dir.getY());
	dirP.setY(-1*dir.getX());

	int vlaf = A->getVelocity()->dot(dirP);
	int vlbf = B->getVelocity()->dot(dirP);

	int vax = (vNormalAf*(dir.getX()/(dir.getMag()*dir.getMag()))) + vlaf*(dirP.getX()/(dirP.getMag()*dirP.getMag()));
	int vay = (vNormalAf*(dir.getY()/(dir.getMag()*dir.getMag()))) + vlaf*(dirP.getY()/(dirP.getMag()*dirP.getMag()));
	
	int vbx = (vNormalBf*(dir.getX()/(dir.getMag()*dir.getMag()))) + vlbf*(dirP.getX()/(dirP.getMag()*dirP.getMag()));
	int vby = (vNormalBf*(dir.getY()/(dir.getMag()*dir.getMag()))) + vlbf*(dirP.getY()/(dirP.getMag()*dirP.getMag()));
	
	A->setVelocity(vax,vay);
	B->setVelocity(vbx,vby);
	return 0;
}
