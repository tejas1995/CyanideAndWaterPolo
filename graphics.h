#ifndef graphics_h
#define graphics_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <stdio.h>

bool init();

class LTexture
{
public:
	//Constructor
	LTexture();
	//Destructor
	~LTexture();

	//Load an image from the specified path
	bool loadFromFile(std::string path);

	//Deallocation
	void free();

	//Render at (x,y)
	void render(int x, int y, double angle=0.0, int center_x = -1, int center_y = -1);

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