#include "graphics.h"
#include "structures.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600
#define BUTTON_WIDTH 300
#define BUTTON_HEIGHT 80

SDL_Window* mWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;

std::stringstream timeText;
LTexture backTexture;
LTexture textTexture;
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
	//printf("destruct\n");
	this->free();
}

void LTexture::free()
{
	//printf("freed\n");
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
	//printf("Hereload\n");
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

bool LTexture::loadFromRenderedText(std::string text, SDL_Color textColor)
{
	SDL_Surface* loadedSurface = TTF_RenderText_Solid(gFont, text.c_str(), textColor);
	if(gFont == NULL)
	{
		printf("Failed to load font. SDL Error:%s\n", SDL_GetError());
	}
	else
	{
		this->mTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if(this->mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error:%s\n", SDL_GetError());
		}
		else
		{
			this->mWidth = loadedSurface->w;
			this->mHeight = loadedSurface->h;
		}

		SDL_FreeSurface(loadedSurface);
	}
	return (this->mTexture)!=NULL;
}

void LTexture::render(int x,int y, double angle, int center_x, int center_y)
{
	SDL_Point center = {center_x,center_y};
	SDL_Rect renderQuad = {x ,y , this->mWidth, this->mHeight};
	if(center_x==-1000||center_y == -1000)
	{
		SDL_RenderCopyEx(gRenderer, (this->mTexture), NULL, &renderQuad, angle, NULL, SDL_FLIP_NONE);
//		printf("%d %d %d %d\n", x,y,center_x,center_y);
	}
	else
	{
		SDL_RenderCopyEx(gRenderer, (this->mTexture), NULL, &renderQuad, angle, &center, SDL_FLIP_NONE);
	}
	
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(this->mTexture, blending);	
}

void LTexture::setAlpha(Uint8 Alpha)
{
	SDL_SetTextureAlphaMod(this->mTexture, Alpha);
}

int LTexture::getWidth()
{
	return (this->mWidth);
}

int LTexture::getHeight()
{
	return (this->mHeight);
}

LButton::LButton()
{
	this->mPosition.x = 0;
	this->mPosition.y = 0;

	this->mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition(int x, int y)
{
	this->mPosition.x = x;
	this->mPosition.y = y;
}

void LButton::handleEvent(SDL_Event* e)
{
	bool inside = true;
	if(e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		int x,y;
		SDL_GetMouseState(&x, &y);

		if(x < (this->mPosition.x))
		{
			inside = false;
		}
		else if(x > (this->mPosition.x) + BUTTON_WIDTH)
		{
			inside = true;
		}
		else if(y < (this->mPosition.y))
		{
			inside = false;
		}
		else if(y > (this->mPosition.y) + BUTTON_HEIGHT)
		{
			inside = false;
		}
	}
	if(!inside)
	{
		this->mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
	}
	else
	{
		switch(e->type)
		{
			case SDL_MOUSEBUTTONDOWN : this->mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;break;
			case SDL_MOUSEBUTTONUP : this->mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;break;
			case SDL_MOUSEMOTION : this->mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
		}
	}
}

void LButton::render()
{

}

bool init()
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0){
		printf("SDL init error");
		return false;
	}
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
			SDL_SetRenderDrawColor(gRenderer, 0xFF,0xFF,0xFF,0xFF);
			int imgFlags = IMG_INIT_PNG;
			if(!(IMG_Init(imgFlags) & imgFlags))
			{
				printf("SDL_image could not be initialized! SDL Error:%s\n", SDL_GetError());
				return false;
			}
			if(TTF_Init() == -1)
			{
				printf("SDL_ttf could not be initialized! SDL Error:%s\n", SDL_GetError());
				return false;
			}
		}
	}
	return true;
}

bool loadMedia(player *Player, goal *Goal, ball *Ball, water* Water)
{
	gFont = TTF_OpenFont("img/gone.ttf", 28);
	if(gFont == NULL)
	{
		printf("Could not open font.\n");
		return false;
	}
	else
	{
		SDL_Color textColor = {0,0,0};
		timeText.str(" ");
		if(!textTexture.loadFromRenderedText(timeText.str().c_str(), textColor))
		{
			printf("Could not render from Text.\n");
			return false;
		}
		else
		{
			if   
   				(!(Player[ USER ].getTexture()->loadFromFile("img/cnh-happy-1.png")&&
        		Player[ COMPUTER ].getTexture()->loadFromFile("img/cnh-angry-1.png")&&
        		Goal[ USER ].getTexture()->loadFromFile("img/goal_0.png")&&
        		Goal[ COMPUTER ].getTexture()->loadFromFile("img/goal_1.png")&&
        		Ball->getTexture()->loadFromFile("img/ball.png")&&
        		Water->getTexture()->loadFromFile("img/waves.png")&& 
        		backTexture.loadFromFile("img/beach-bg.png")
        		))
			return false;	
		}
	}
	Water->getTexture()->setBlendMode(SDL_BLENDMODE_BLEND);
	Water->getTexture()->setAlpha(200);
	goalLocal = Goal;
	waterLocal = Water;
    frameRender(Player, Ball);
    return true;
}

void frameRender(player *Player, ball *Ball)
{
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
	SDL_Delay(50);
	backTexture.render(0,0);
	textTexture.render((SCREEN_WIDTH - textTexture.getWidth())/2,0);
	Player[USER].getTexture()->render(Player[USER].getX(), Player[USER].getY(), Player[USER].getAngle());
	Player[COMPUTER].getTexture()->render(Player[COMPUTER].getX(), Player[COMPUTER].getY(), Player[COMPUTER].getAngle());
	goalLocal[USER].getTexture()->render(goalLocal[USER].getX(),goalLocal[USER].getY());
	goalLocal[COMPUTER].getTexture()->render(goalLocal[COMPUTER].getX(),goalLocal[COMPUTER].getY());
	Ball->getTexture()->render(Ball->getX(),Ball->getY());
	waterLocal->getTexture()->render(0, waterLocal->getDepth()-20);
	
	//printf("pass%d %d\n", Player[USER].getX(), Player[USER].getY());
	SDL_RenderPresent(gRenderer);
}

unsigned int setTime(unsigned int currTime, unsigned int startTime)
{
	SDL_Color textColor = {0,0,0};
	unsigned int durationSecs = 120-((currTime - startTime)/1000);
	unsigned int durationMins = durationSecs/60;
	timeText.str("");
	timeText<< std::setfill('0')<< std::setw(2) << durationMins<<":"<<std::setfill('0')<< std::setw(2)<< durationSecs%60;
	textTexture.loadFromRenderedText(timeText.str().c_str(), textColor);
	timeText.str("");
	return durationSecs;
}

void closeObjectTextures(player *Player, ball *Ball)
{
	//backTexture.free();
	//printf("close called\n");
    Player[ USER ].getTexture()->free();
    Player[ COMPUTER ].getTexture()->free();
    goalLocal[ USER ].getTexture()->free();
    goalLocal[ COMPUTER ].getTexture()->free();
    Ball->getTexture()->free();
    waterLocal->getTexture()->free();
    textTexture.free();

    TTF_CloseFont(gFont);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(mWindow);
    
    gFont = NULL;
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
		}
	return 0;	
	}
*/
