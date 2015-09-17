#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* mWindow = NULL;
SDL_Renderer* gRenderer = NULL;

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
	void render(int x, int y);

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

