#ifndef STRUCTURES_H
#define STRUCTURES_H

enum mode{ NONE, WADE, SWIM, HIT }

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
		int getMagSq();
};
class ball{
	private:
		int x, y;
		mVector velocity;
	public:
		ball();
		~ball();
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

class player{
	private:
		int x, y;
		int maxJumpHeight;
		int maxWadeVelocity;
		int maxSwimVelocity;
		mVector velocity;
		int mode;
		int angle;
		int handAngle;
		int omega;
		int handOmega;

	public:
		player(maxJumpHeight, maxSwimVelocity, maxWadeVelocity);
		~player();
		int getX();
		int getY();
		int getAngle();
		int getHandAngle();
		int getOmega();
		int getHandOmega();
		bool setAngle(int);
		bool setHandAngle(int);
		bool setX(int);
		bool setY(int);
		mVector getVelocity();
		bool setVelocity(mVector);
		bool setVelocity(int,int);
		bool setOmega(int);
		bool setHandOmega(int);
};

#endif
