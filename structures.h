#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "graphics.h"
#include "math.h"

class mVector{
	private:

		float x;
		float y;
		
	public:
		
		float getX();
		float getY();
		bool setX(float);
		bool setY(float);
		float getSlope();
		float getMag();
		float dot(mVector);
};

class entity{
	private:

		int x, y;
		float omega;
		int angle;
		mVector velocity;
		LTexture texture;

	public:
		
		LTexture* getTexture();
		int getX();
		int getY();
		float getOmega();
		int getAngle();
		bool setAngle(int);
		bool setOmega(float);
		bool setX(int);
		bool setY(int);
		mVector* getVelocity();
		bool setVelocity(mVector);
		bool setVelocity(float,float);
};

class ball: public entity{
	private:
		int radius;
		
	public:
		ball();
		int getRadius();
		~ball();
};

class hand: public entity{
	private:
		int wastedMemoryYay;
		int width;
		int height;
	public:
		int getWidth();
		int getHeight();
};
class player: public entity{
	private:
		float maxJumpVelocity;
		float maxWadeVelocity;
		float maxSwimVelocity;
		int mode;
		hand haath;
		int radius;
	public:
		bool setAttributes(float , float , float);
		float getMaxJumpVelocity();
		float getMaxWadeVelocity();
		float getMaxSwimVelocity();
		int getMode();
		int getRadius();
		hand* getHand();
		bool setMode(int);
};




class water{
	private:
		int depth;
		LTexture texture;
		SDL_Rect topSurface, bottomSurface;
	public:
		int getDepth();
		bool setDepth(int);
		LTexture* getTexture();
};

class goal{
	private:
		int x, y;
		SDL_Rect backNet, topNet, blankSpace;
		LTexture texture;
	public:
		LTexture* getTexture();
		bool setX(int);
		bool setY(int);
		int getX();
		int getY();
		bool defineTopNet(int, int, int, int);
		bool defineBackNet(int, int, int, int);
		bool defineBlankSpace(int, int, int, int);
		SDL_Rect getBackNet();
		SDL_Rect getTopNet();
		SDL_Rect getBlankSpace();

};

bool loadMedia(player *Player, goal *Goal, ball *Ball, water* Water);
void frameRender(player *Player, ball *Ball);
void closeObjectTextures(player *Player, ball *Ball);
#endif
