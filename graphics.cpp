#include "graphics.h"
#include "structures.h"
#include "enums.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* mWindow = NULL;
SDL_Renderer* gRenderer = NULL;
LTexture backTexture;
water* waterLocal;
goal* goalLocal;

LTexture::LTexture()
{
	this->mTexture = NULL;
	this->mWidth = 0;
	this->mHeight = 0;
}

LTexture::~LTexture()
{
	this->free();
}

void LTexture::free()
{
	if(this->mTexture!= NULL)
	{
		SDL_DestroyTexture(this->mTexture);
		this->mTexture = NULL;
		this->mWidth = 0;
		this->mHeight = 0;
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
			this->mWidth = loadedSurface->w;
			this->mHeight = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}
	this->mTexture = newTexture;
	return ((this->mTexture)!=NULL);
}

void LTexture::render(int x,int y, double angle, int center_x,int center_y)
{
	SDL_Point center = {center_x,center_y};
	SDL_Rect renderQuad = {x ,y , this->mWidth, this->mHeight};
	if(center_x==-1||center_y == -1)
	{
		center.x = x + (this->mWidth)/2;
		center.y = y + (this->mHeight)/2;
		SDL_RenderCopyEx(gRenderer, (this->mTexture), NULL, &renderQuad, angle, &center, SDL_FLIP_NONE);
	}
	else
	{
		SDL_RenderCopyEx(gRenderer, (this->mTexture), NULL, &renderQuad, angle, &center, SDL_FLIP_NONE);
	}
	
}

int LTexture::getWidth()
{
	return (this->mWidth);
}

int LTexture::getHeight()
{
	return (this->mHeight);
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
	return true;
}

bool loadMedia(player *Player, goal *Goal, ball *Ball, water* Water)
{
	if   
    (!(Player[ USER ].getTexture()->loadFromFile("img/player.png")&&
        Player[ COMPUTER ].getTexture()->loadFromFile("img/player.png")&&
        Goal[ USER ].getTexture()->loadFromFile("img/goal.png")&&
        Goal[ COMPUTER ].getTexture()->loadFromFile("img/goal.png")&&
        Ball->getTexture()->loadFromFile("img/ball.png")&&
        Water->getTexture()->loadFromFile("img/water.png")
        //&& backTexture.loadFromFile("img/background.png")
        ))
		return false;
		goalLocal = Goal;
		waterLocal = Water;
        frameRender(Player, Ball);
        return true;
}

void frameRender(player *Player, ball *Ball)
{
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
	//backTexture.loadFromFile("img/background.png")
	waterLocal->getTexture()->render(0,0);
	Player[USER].getTexture()->render(Player[USER].getX(), Player[USER].getY(), Player[USER].getAngle());
	Player[COMPUTER].getTexture()->render(Player[COMPUTER].getX(), Player[COMPUTER].getY(), Player[COMPUTER].getAngle());
	goalLocal[USER].getTexture()->render(goalLocal[USER].getX(), goalLocal[COMPUTER].getY());
	goalLocal[COMPUTER].getTexture()->render(20,340);
	Ball->getTexture()->render(Ball->getX(),Ball->getY());
	SDL_RenderPresent(gRenderer);
}

void closeObjectTextures(player *Player, ball *Ball)
{
	//backTexture.free();
    Player[ USER ].getTexture()->free();
    Player[ COMPUTER ].getTexture()->free();
    goalLocal[ USER ].getTexture()->free();
    goalLocal[ COMPUTER ].getTexture()->free();
    Ball->getTexture()->free();
    waterLocal->getTexture()->free();
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(mWindow);
    
    mWindow = NULL;
    gRenderer = NULL;

    IMG_Quit();
    SDL_Quit();
}











/*LTexture face;
LTexture bground;

void close()
{
	face.free();
	bground.free();

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(mWindow);

	mWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}

bool loadMedia()
{
	if(!(face.loadFromFile("img/obama_2.png")))
	{
		return false;
	}
	else if(!(bground.loadFromFile("img/bground.png")))
	{
		return false;
	}
	return true;
}*/
int main(int argc, char const *argv[])
{
	/*int degrees =0;
	player Players[2];
	ball Ball;
	goal Goal[2];
	water Water;

	if(!init())
	{
		printf("Error1.\n");
	}
		else
		{
			if(!loadMedia(Players, Goal, &Ball, &Water))
			{
				printf("Error2.\n");
			}
			else
			{
				bool quit = false;
				SDL_Event e;
				SDL_RenderPresent(gRenderer);
				while(!quit)
				{
					while(SDL_PollEvent(&e)!=0)
					{
						if(e.type == SDL_QUIT)
						{
							quit = true;
						}
						/*else if(e.type == SDL_KEYDOWN)
						{
							SDL_RenderClear(gRenderer);
							switch(e.key.keysym.sym)
							{
								case SDLK_LEFT:{
									degrees-=10;
									face.render(0,0);
									break;
								}
								case SDLK_RIGHT:{
									degrees+=10;
									face.render(0,0,degrees,300,100);
								}
							}
							SDL_RenderPresent(gRenderer);
						}
					}
									
				}
				//close();
			}
		}*/
	return 0;	
	}

