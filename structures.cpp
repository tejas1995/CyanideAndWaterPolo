#include "structures.h"

int mVector::getX(){
	return x;
}
int mVector::getY(){
	return y;
}
bool mVector::setX(int x1){
	x = x1;
	return true; 
}
bool mVector::setY(int y1){
	y=y1;
	return true;
}
int mVector::getSlope(){
	return y/x;
}
int mVector::getMag(){
	return sqrt(y*y + x*x);
}

ball::ball(){
	setX(0);
	setY(0);
	getVelocity.setX(0);
	getVelocity.setY(0);
}

ball::~ball(){
}

int entity::getX(){
	return x;
}

int entity::getY(){
	return y;
}

int entity::getOmega(){
       return omega;
}

bool entity::setOmega(int o){
	omega = o;
}

bool entity::setX(int x1){
	x = x1;
}

bool entity::setY(int y2){
	y = y2;
}

mVector entity::getVelocity(){
	return velocity;
}

bool entity::setVelocity(mVector mV){
	velocity.setX(mVector.getX());
	velocity.setY(mVector.getY());
	return true;
}

bool entity::setVelocity(int a, int b){
	velocity.setX(a);
	velocity.setY(b);
	return true;
}

LTexture entity::getTexture(){
	return &texture;
}

int ball::getRadius(){
	return (texture.getWidth())/2;
}

int player::getRadius(){
	return (texture.getWidth())/2;
}
int hand::getHeight(){
	return height;
}
int hand::getWidth(){
	return width;
}

LTexture* water::getTexture(){
	return &texture;
}

LTexture* goal::getTexture(){
	return &texture;
}
