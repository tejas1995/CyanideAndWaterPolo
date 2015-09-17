#ifndef STRUCTURES_H
#define STRUCTURES_H
#include "graphics.h"
#include "math.h"

class mVector{
	private:

		int x;
		int y;
		
	public:
		
		int getX();
		int getY();
		bool setX(int);
		bool setY(int);
		int getSlope();
		int getMag();
};

class entity{
	private:

		int x, y;
		int omega;
		int angle;
		mVector velocity;
		LTexture texture;

	public:
		
		LTexture* getTexture();
		int getX();
		int getY();
		int getOmega();
		bool setOmega(int);
		bool setX(int);
		bool setY(int);
		mVector getVelocity();
		bool setVelocity(mVector);
		bool setVelocity(int,int);
};

class ball: public entity{
	private:
		int radius;
		
	public:
		ball();
		int getRadius();
		~ball();
};

class player: public entity{
	private:
		int maxJumpHeight;
		int maxWadeVelocity;
		int maxSwimVelocity;
		int mode;
		hand haath;
		int radius;
	public:
		int getRadius();
};

class hand: public entity{
	private:
		int wastedMemoryYay;
		int width;
		int height;
	public:
		int getWidth();
		int getheight();

};

class water{
	private:
		LTexture texture;
	public:
		Ltexture* getTexture();
};

class goal{
	private:
		LTexture texture;
	public:
		Ltexture* getTexture();
};


#endif
