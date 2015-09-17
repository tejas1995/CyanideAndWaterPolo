#ifndef graphics_h
#define graphics_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <stdio.h>
#include <sstream>
#include <iomanip>

bool init();
bool init_render();
void setTime(Uint32 currTime, Uint32 startTime);

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
	void render(int x, int y, double angle=0.0, int center_x = -1, int center_y = -1);

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
#endif