#include "graphics.h"

LTexture::LTexture()
{
	this-> mTexture = NULL;
	this-> mWidth = 0;
	this-> mHeight = 0;
}

LTexture::~LTexture()
{
	this->free();
}

void LTexture::free()
{
	if(this->mTexture!= NULL)
	{
		SDL_DestroyTexture(this-> mTexture);
		this-> mTexture = NULL;
		this-> mWidth = 0;
		this-> mHeight = 0;
	}
}

bool LTexture::loadFromFile(std::string path)
{
	free();
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if(loadedSurface == NULL)
	{
		printf("Unable to load image from %s. SDL Error: %s\n", path.c_str(), SDL_GetError());
	}
	else
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if(newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error : %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			this-> mWidth = loadedSurface->w;
			this-> mHeight = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}
	this-> mTexture = newTexture;
	return ((this->mTexture)!=NULL);
}

void LTexture::render(int x,int y)
{
	SDL_Rect renderQuad = {x ,y , mWidth, mHeight};
	SDL_RenderCopy(gRenderer, (this-> mTexture), NULL, &renderQuad);
}

int LTexture::getWidth()
{
	return (this-> mWidth);
}

int LTexture::getHeight()
{
	return (this-> mHeight);
}

bool init()
{
	mWindow = SDL_CreateWindow("waterpolo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(mWindow == NULL)
	{
		printf("Window creation failed. SDL Error:%s\n", SDL_GetError());
		return false;
	}
	else
	{
		gRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
		if (gRenderer == NULL)
		{
			printf("Renderer could not be created%s\n", SDL_GetError());
			return false;
		}
		else
		{
			int imgFlags = IMG_INIT_PNG;
			if(!(IMG_Init(imgFlags) & imgFlags))
			{
				printf("SDL_image could not be initialized! SDL Error:%s\n", SDL_GetError());
				return false;
			}
		}
	}
}

bool loadMedia()
{
		
}

int main(int argc, char const *argv[])
{
	return 0;
}