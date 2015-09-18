#include "model.h"
#define KEY_PRESS_ACCELERATION_WADE 1.0
#define KEY_PRESS_ACCELERATION_SWIM 2.0
#define GRAVITY_ACCELERATION 0.4
#define BASE_HEIGHT 320
#define DOWNWARD_CONST_ACCELERATION 0.6
#define DRAG_COEFFICIENT 0.5
#define BUOYANCY 0.4
#define BALL_BASE_HEIGHT 360
#define ANGULAR_VELOCITY 10
#define MAX_ANGLE_FORWARD 60
#define MAX_ANGLE_BACKWARD 390
#define INIT_ANGLE 150

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

int checkCollision(player* player, SDL_Rect rect)
{
	int ballTop = player->getY();
	int ballBottom = player->getY() + 2*player->getRadius();
	int ballLeft = player->getX();
	int ballRight = player->getX() + 2*player->getRadius();

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

int updateObjects(int* keystates, player player[], goal goals[], ball* ball, water* water, SDL_Rect wall[], int pCode){
	
	entity* eball;
	entity* eplayer[2];
	
	eball = ball;
	eplayer[0] = &player[0];
	eplayer[1] = &player[1];
	bool colFlag = false;

	float uvx = player[pCode].getVelocity() -> getX();
	float uvy = player[pCode].getVelocity() -> getY();
	float bvx = ball->getVelocity()->getX();
	float bvy = ball->getVelocity()->getY();

	if (keystates[KEY_SHIFT] == 1)
	{
		player[pCode].setMode(SWIM);
	}

	else if(keystates[KEY_SHIFT] == 0)
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
			if(player[pCode].getY() < BASE_HEIGHT+5 && player[pCode].getY() > BASE_HEIGHT)
			{
				uvy = (-1)*player[pCode].getMaxJumpVelocity();
			}
		}

		if(keystates[KEY_K] == 1)
		{
			if(abs(pCode*360 - (player[pCode].getHand() -> getAngle())) > MAX_ANGLE_FORWARD)
			{
				player[pCode].getHand() -> setOmega(ANGULAR_VELOCITY*pow(-1,1+pCode));
			}

			else if(abs(pCode*360 - (player[pCode].getHand() -> getAngle())) <= MAX_ANGLE_FORWARD)
			{
				player[pCode].getHand() -> setOmega(0);
			}
		}
		else if(keystates[KEY_L] == 1)
		{
			if(abs(pCode*360 - (player[pCode].getHand() -> getAngle())) < MAX_ANGLE_BACKWARD)
			{
				player[pCode].getHand() -> setOmega(ANGULAR_VELOCITY*pow(-1,pCode));
			}

			else if(abs(pCode*360 - (player[pCode].getHand() -> getAngle())) >= MAX_ANGLE_BACKWARD)
			{
				player[pCode].getHand() -> setOmega(0);
			}
		}
		else
		{
			if(abs(pCode*360 - (player[pCode].getHand() -> getAngle())) > INIT_ANGLE + 5)
			{
				player[pCode].getHand() -> setOmega(ANGULAR_VELOCITY*pow(-1,1+pCode));
			}

			else if(abs(pCode*360 - (player[pCode].getHand() -> getAngle())) < INIT_ANGLE - 5)
			{
				player[pCode].getHand() -> setOmega(ANGULAR_VELOCITY*pow(-1,pCode));
			}

			else
			{
				player[pCode].getHand() -> setOmega(0);
			}
		}

		if(player[pCode].getY() < BASE_HEIGHT)
		{
			uvy += GRAVITY_ACCELERATION;
		}
		if(player[pCode].getY() > BASE_HEIGHT)
		{
			uvy -= (BUOYANCY + uvy*DRAG_COEFFICIENT);
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
			uvy += (DOWNWARD_CONST_ACCELERATION - DRAG_COEFFICIENT*uvy);
		}
		if(player[pCode].getY() < BASE_HEIGHT)
		{
			uvy += GRAVITY_ACCELERATION;
		}
		if(player[pCode].getY() > BASE_HEIGHT)
		{
			uvy -= (BUOYANCY + uvy*DRAG_COEFFICIENT);
		}

	}

	if(ball->getY() > BALL_BASE_HEIGHT)
	{
		bvx -= bvx*DRAG_COEFFICIENT;
		bvy -= BUOYANCY*2 + bvy*DRAG_COEFFICIENT*3;
	}

	else if(ball->getY() < BALL_BASE_HEIGHT)
	{
		bvy += GRAVITY_ACCELERATION;
	}

	if(ball->getY() == BALL_BASE_HEIGHT)
	{
		if(fabs(bvy) < 0.2)
		{
			bvy = 0;
		}
	}


	player[pCode].setVelocity(uvx, uvy);
	ball->setVelocity(bvx,bvy);

	if(checkCollision(ball, &player[USER])>0){
		collisionReact( eball, eplayer[USER], 1, 900000, 1);
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

	int collideAxis;
	
	if((collideAxis = checkCollision(ball, goals[USER].getTopNet())))
	{
		if(collideAxis == -1)
		{
			ball -> setVelocity(ball->getVelocity()->getX(),-(ball->getVelocity()->getY()));
		}
		else if(collideAxis == -2)
		{
			ball -> setVelocity(-(ball->getVelocity()->getX()),ball->getVelocity()->getY());
		}
		colFlag = true;
	}

	if((collideAxis = checkCollision(ball, goals[COMPUTER].getTopNet())))
	{
		if(collideAxis == -1)
		{
			ball -> setVelocity(ball->getVelocity()->getX(),-(ball->getVelocity()->getY()));
		}
		else if(collideAxis == -2)
		{
			ball -> setVelocity(-(ball->getVelocity()->getX()),ball->getVelocity()->getY());
		}
		colFlag = true;
	}

	for(int i = 0; i<4; i++)
	{
		int collideWall = checkCollision(ball, wall[i]);
		if(collideWall == -1)
		{
			ball -> setVelocity(ball->getVelocity()->getX(),-1*ball->getVelocity()->getY());
			colFlag = true;
		}
		else if(collideWall == -2)
		{
			ball -> setVelocity(-ball->getVelocity()->getX(),ball->getVelocity()->getY());
			colFlag = true;
		}

		collideWall = checkCollision(&player[USER], wall[i]);
		if(collideWall == -1)
		{
			player[USER].setVelocity(player[USER].getVelocity()->getX(),-1*player[USER].getVelocity()->getY());
			colFlag = true;
		}
		else if(collideWall == -2)
		{
			player[USER].setVelocity(-1*player[USER].getVelocity()->getX(),player[USER].getVelocity()->getY());
			colFlag = true;
		}

		collideWall = checkCollision(&player[COMPUTER], wall[i]);
		if(collideWall == -1)
		{
			player[COMPUTER].setVelocity(player[COMPUTER].getVelocity()->getX(), -1* player[COMPUTER].getVelocity()->getY());
			colFlag = true;
		}
		else if(collideWall == -2)
		{
			player[COMPUTER].setVelocity(-1*player[COMPUTER].getVelocity()->getX(),player[COMPUTER].getVelocity()->getY());
			colFlag = true;
		}		
	}

/*	if (colFlag){
		bool success = false;
		success = changePositions(eball, eplayer);
		return success;
	}*/
	changePositions(eball, eplayer);
	player[USER].getHand() -> setAngle(player[USER].getHand() -> getAngle() + player[USER].getHand() -> getOmega());
	player[COMPUTER].getHand() -> setAngle(player[COMPUTER].getHand() -> getAngle() + player[COMPUTER].getHand() -> getOmega());
	return 0;
}

int changePositions(entity* eball, entity** eplayer)
{
	eball -> setX(eball -> getX() + eball->getVelocity()->getX());
	eball -> setY(eball -> getY() + eball->getVelocity()->getY());
	eplayer[USER] -> setX(eplayer[USER] -> getX() + eplayer[USER]->getVelocity()->getX());
	eplayer[USER] -> setY(eplayer[USER] -> getY() + eplayer[USER]->getVelocity()->getY());
	eplayer[COMPUTER]->setX(eplayer[COMPUTER]->getX()+eplayer[COMPUTER]->getVelocity() -> getX());
	eplayer[COMPUTER]->setY(eplayer[COMPUTER]->getY()+eplayer[COMPUTER]->getVelocity() -> getY());
	return 1;
}

int collisionReact( entity* A, entity* B, int m1, int m2, float e){
	mVector dir;
	dir.setX(B->getX()-A->getX());
	dir.setY(B->getY()-A->getY());
	float vNormalAf,vNormalAi,vNormalBf,vNormalBi;
	vNormalAi = A->getVelocity()->dot(dir);
	vNormalBi = B->getVelocity()->dot(dir);

	if(vNormalBi>vNormalAi){
		return 0;
	}
	vNormalBf = (m1*vNormalAi*(1+e) + (m2-m1*e)*vNormalBi) / (m1+m2);
	vNormalAf = (m2*vNormalBi*(1+e) + (m1-m2*e)*vNormalAi) / (m1+m2);

	mVector dirP;
	dirP.setX(dir.getY());
	dirP.setY(-1*dir.getX());

	float vlaf = A->getVelocity()->dot(dirP);
	float vlbf = B->getVelocity()->dot(dirP);

	float vax = (vNormalAf*(dir.getX()/(dir.getMag()*dir.getMag()))) + vlaf*(dirP.getX()/(dirP.getMag()*dirP.getMag()));
	float vay = (vNormalAf*(dir.getY()/(dir.getMag()*dir.getMag()))) + vlaf*(dirP.getY()/(dirP.getMag()*dirP.getMag()));
	
	float vbx = (vNormalBf*(dir.getX()/(dir.getMag()*dir.getMag()))) + vlbf*(dirP.getX()/(dirP.getMag()*dirP.getMag()));
	float vby = (vNormalBf*(dir.getY()/(dir.getMag()*dir.getMag()))) + vlbf*(dirP.getY()/(dirP.getMag()*dirP.getMag()));
	
	A->setVelocity(vax,vay);
	B->setVelocity(vbx,vby);
	return 0;
}
int handShot( player* player, ball* ball, float e, int r){
	float angle = (player->getHand()->getAngle())*PI/180;
	int vx = ball->getVelocity()->getX() - player->getVelocity()->getX();
	int vy = ball->getVelocity()->getY() - player->getVelocity()->getY();
	int omega = player->getHand()->getOmega();
	int v1x = (vx*(sin(angle))*(sin(angle))) - (cos(angle)*((e*vx*cos(angle))-(e*vy*sin(angle)) + omega*r*(1+e))) - vy*cos(angle)*sin(angle);
	int v1y = (vy*(cos(angle))*(cos(angle))) - (sin(angle)*((e*vx*cos(angle))-(e*vy*sin(angle)) + omega*r*(1+e))) - vx*cos(angle)*sin(angle);
	int u1x = v1x + player->getVelocity()->getX();
	int u1y = v1y + player->getVelocity()->getY();
	ball->setVelocity(u1x, u1y);	
	return 1;
}
