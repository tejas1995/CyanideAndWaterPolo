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
int mVector::dot(mVector a){
	return getX()*(a.getX()) + getY()*(a.getY());
}
ball::ball(){
	setX(0);
	setY(0);
	getVelocity()->setX(0);
	getVelocity()->setY(0);
}

ball::~ball(){
}

int entity::getX(){
	return x;
}

int entity::getY(){
	return y;
}
int entity::getAngle(){
	return angle;
}

int entity::getOmega(){
       return omega;
}

bool entity::setOmega(int o){
	omega = o;
	return true;
}

bool entity::setX(int x1){
	x = x1;
	return true;
}

bool entity::setY(int y2){
	y = y2;
	return true;
}

mVector* entity::getVelocity(){
	return &velocity;
}

bool entity::setVelocity(mVector mV){
	velocity.setX(mV.getX());
	velocity.setY(mV.getY());
	return true;
}

bool entity::setVelocity(int a, int b){
	velocity.setX(a);
	velocity.setY(b);
	return true;
}

bool entity::setAngle(int ang){
	angle = ang;
	return true;
}

LTexture* entity::getTexture(){
	return &texture;
}

int ball::getRadius(){
	return (getTexture()->getWidth())/2;
}

int player::getRadius(){
	return (getTexture()->getWidth())/2;
}

int player::getMode(){
	return mode;
}
bool player::setMode(int s){
	mode = s;
	return true;
}

bool player::setAttributes(int Jump, int Wade, int Swim)
{
	maxJumpVelocity = Jump;
	maxWadeVelocity = Wade;
	maxSwimVelocity = Swim;
	return true;
}

hand* player::getHand(){
	return &haath;
}

int player::getMaxJumpVelocity()
{
	return maxJumpVelocity;
}

int player::getMaxWadeVelocity()
{
	return maxWadeVelocity;
}

int player::getMaxSwimVelocity()
{
	return maxSwimVelocity;
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

bool water::setDepth(int d){
	depth = d;
	return true;
}
int water::getDepth(){
	return depth;
}
bool goal::setX(int x1)
{
	x = x1;
	return true;
}

bool goal::setY(int y1)
{
	y = y1;
	return true;
}

int goal::getX()
{
	return x;
}

int goal::getY()
{
	return y;
}

bool goal::defineTopNet(int start_x, int start_y, int w, int h)
{
	topNet.x = start_x;
	topNet.y = start_y;
	topNet.w = w;
	topNet.h = h;
	return true;
}

bool goal::defineBackNet(int start_x, int start_y, int w, int h)
{
	backNet.x = start_x;
	backNet.y = start_y;
	backNet.w = w;
	backNet.h = h;
	return true;
}

bool goal::defineBlankSpace(int start_x, int start_y, int w, int h)
{
	blankSpace.x = start_x;
	blankSpace.y = start_y;
	blankSpace.w = w;
	blankSpace.h = h;
	return true;
}

SDL_Rect goal::getBackNet()
{
	return backNet;
}

SDL_Rect goal::getTopNet()
{
	return topNet;
}

SDL_Rect goal::getBlankSpace()
{
	return blankSpace;
}
