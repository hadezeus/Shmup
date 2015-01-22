/*This source code copyrighted by Lazy Foo' Productions (2004-2014)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <array>
#include "SDL2_gfxPrimitives.h"
//#include <SDL_image.h>
//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const double PI = 3.1415926535897932384626433832795;
SDL_Renderer* renderer = nullptr;

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		#ifdef _SDL_TTF_H
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );

		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

//The Bullet that will move around on the screen

//The FIGHTER that will move around on the screen
class FIGHTER
{
    public:
		//The dimensions of the FIGHTER
		static const int FIGHTER_WIDTH = 20;
		static const int FIGHTER_HEIGHT = 20;

		//Maximum axis velocity of the FIGHTER
		static const int FIGHTER_VEL = 10;

		//Initializes the variables
		FIGHTER();

		//Takes key presses and adjusts the FIGHTER's velocity
		void handleEvent( SDL_Event& e );

		//Moves the FIGHTER and checks collision
		void move();

		//Shows the FIGHTER on the screen
		void render();

    private:
		//The X and Y offsets of the FIGHTER
		int mPosX, mPosY;

		//The velocity of the FIGHTER
		int mVelX, mVelY;

		//FIGHTER's collision box
		SDL_Rect mCollider;
};

class Bullet
{
    public:

		static const int Bullet_RADIUS = 10;


		//Initializes the variables
		Bullet(int x, int y, int speed, float angle, FIGHTER fight);


		//Moves the Bullet and checks collision
		void move();

		//Shows the FIGHTER on the screen
		void render();

    private:
		//The X and Y offsets of the FIGHTER
		int mPosX, mPosY;

		//The velocity of the FIGHTER
		int mVelX, mVelY;

		//FIGHTER's collision box
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Box collision detector
void checkCollision( Bullet Bul[] );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gFIGHTERTexture;
LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
	//SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}


	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

FIGHTER::FIGHTER()
{
    //Initialize the offsets
    mPosX = SCREEN_WIDTH/2;
    mPosY = SCREEN_HEIGHT*3/4;

	//Set collision box dimension
	mCollider.w = FIGHTER_WIDTH;
	mCollider.h = FIGHTER_HEIGHT;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

void FIGHTER::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= FIGHTER_VEL; break;
            case SDLK_DOWN: mVelY += FIGHTER_VEL; break;
            case SDLK_LEFT: mVelX -= FIGHTER_VEL; break;
            case SDLK_RIGHT: mVelX += FIGHTER_VEL; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += FIGHTER_VEL; break;
            case SDLK_DOWN: mVelY -= FIGHTER_VEL; break;
            case SDLK_LEFT: mVelX += FIGHTER_VEL; break;
            case SDLK_RIGHT: mVelX -= FIGHTER_VEL; break;
        }
    }
}

void FIGHTER::move()
{
    //Move the FIGHTER left or right
    mPosX += mVelX;
	mCollider.x = mPosX;

    //If the FIGHTER collided or went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + FIGHTER_WIDTH > SCREEN_WIDTH ) )
    {
        //Move back
        mPosX -= mVelX;
		mCollider.x = mPosX;
    }

    //Move the FIGHTER up or down
    mPosY += mVelY;
	mCollider.y = mPosY;

    //If the FIGHTER collided or went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + FIGHTER_HEIGHT > SCREEN_HEIGHT )  )
    {
        //Move back
        mPosY -= mVelY;
		mCollider.y = mPosY;
    }
}

void FIGHTER::render()
{
    //Show the FIGHTER
	gFIGHTERTexture.render( mPosX, mPosY );
}

Bullet::Bullet(int x, int y, int speed, float angle, FIGHTER fight)
{
	mVelX = (int) speed*cos(angle);
	mVelY = (int) speed*sin(angle);
	mPosX = x;
	mPosY = y;
}

void Bullet::move()
{
	mPosX += mVelX;
	mPosY += mVelY;

    if( ( mPosX-(Bullet_RADIUS) < 0 ) || ( mPosX + Bullet_RADIUS > SCREEN_WIDTH ) )
    {
        //Move back
        mPosX -= mVelX;
    }

    //Move the FIGHTER up or down

    //If the FIGHTER collided or went too far up or down
    if( ( mPosY-(Bullet_RADIUS) < 0 ) || ( mPosY + Bullet_RADIUS > SCREEN_HEIGHT )  )
    {
        //Move back
        mPosY -= mVelY;
    }
}

void Bullet::render()
{
	filledCircleRGBA(gRenderer, mPosX, mPosY, Bullet_RADIUS, 0x0,0x0,0xFF,0xFF);
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Last Boss", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );


			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load press texture
	if( !gFIGHTERTexture.loadFromFile( "src/FIGHTER.bmp" ) )
	{
		printf( "Failed to load FIGHTER texture!\n" );
		success = false;
	}
	return success;
}

void close()
{
	//Free loaded images
	gFIGHTERTexture.free();
	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

void checkCollision( Bullet* bul[] )
{

}

//void dissapear(Bullet* bul[], int index)
//{
//	for(int i= index; i< (bul.size()-1); i++)
//	{
//		bul[i] = bul[i+1];
//	}
//}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The FIGHTER that will be moving around on the screen
			FIGHTER fight;
			Bullet* bullet = new Bullet(100, 100, 2, 180*PI/180, fight);
			Bullet* bullet2 = new Bullet(200, 200, 2, 90*PI/180, fight);
			Bullet* bul[1000];
			for(int i = 0; i<100; i++)
			{
				bul[i] = new Bullet(100, 100, 2, 270*PI/180, fight);
			}
			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle input for the FIGHTER
					fight.handleEvent( e );
				}
				//Move the FIGHTER and check collision
				fight.move();
				bullet->move();
				bullet2->move();
				for(int i = 0; i<100; i++)
				{
					if(bul[i] != NULL)
					{
						bul[i]->move();
					}
				}
				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render wall
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
				//Render FIGHTER
				fight.render();
				bullet->render();
				bullet2->render();
				for(int i = 0; i<100; i++)
				{
					if(bul[i] != NULL)
					{
						bul[i]->render();
					}
				}
				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
