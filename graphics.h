#ifndef graphics_h
#define graphics_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <stdio.h>
#include <sstream>
#include <iomanip>
#include "enums.h"

bool init();
bool init_render();
unsigned int setTime(Uint32 currTime, Uint32 startTime);
void scoreUpdate(int []);
class LTexture
{
public:
	//Constructor
	LTexture();
	//Destructor
	~LTexture();

	//Load an image from the specified path
	bool loadFromFile(std::string path);

	//Create an image from a string
	bool loadFromRenderedText(std::string text, SDL_Color textColor);
	
	//Deallocation
	void free();

	//Render at (x,y)
	void render(int x, int y, double angle=0.0, int center_x = -1000, int center_y = -1000);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set opacity
	void setAlpha(Uint8 Alpha);

	//Get Dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware structure
	SDL_Texture* mTexture;

	//Image Dimensions
	int mWidth;
	int mHeight;
};

class LButton
{
public:
	//Constructor and Destructor
	LButton();
	~LButton();

	//Setting button position
	void setPosition(int x, int y);

	//Handle mouse events
	void handleEvent(SDL_Event* e);

	//Render button sprite
	void render();

private:
	//Position
	SDL_Point mPosition;

	//Currently used global sprite
	ButtonSprite mCurrentSprite;
};
#endif