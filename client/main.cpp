/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <string>
#include <cmath>
#include <iostream>
#include<sstream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <chrono>
#include <ctime>
#define PORT 8080

using namespace std;
//Screen dimension constants
const int SCREEN_WIDTH = 1867;
const int SCREEN_HEIGHT = 800;
void server();
void client(int x, int y);
//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		SDL_Texture* loadFromFile( std::string path );
		
		#if defined(SDL_TTF_MAJOR_VERSION)
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

//The application time based timer
class LTimer
{
    public:
		//Initializes variables
		LTimer();

		//The various clock actions
		void start();
		void stop();
		void pause();
		void unpause();

		//Gets the timer's time
		Uint32 getTicks();

		//Checks the status of the timer
		bool isStarted();
		bool isPaused();

    private:
		//The clock time when the timer started
		Uint32 mStartTicks;

		//The ticks stored when the timer was paused
		Uint32 mPausedTicks;

		//The timer status
		bool mPaused;
		bool mStarted;
};

//The dot that will move around on the screen
class Dot
{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		//Maximum axis velocity of the dot
		static const int DOT_VEL = 2;

		//Initializes the variables
		Dot();

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e );

		//Moves the dot
		void move(int part);

		//Shows the dot on the screen
		void render();
		void render2();
		//The X and Y offsets of the dot
		int mPosX, mPosY;
    private:
		

		//The velocity of the dot
		int mVelX, mVelY;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font *gFont = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;
SDL_Texture* gHostel1 = NULL;
SDL_Texture* gHostel2 = NULL;
SDL_Texture* gHostel3 = NULL;
SDL_Texture* gHostel4 = NULL;
SDL_Texture* gHostel5 = NULL;
SDL_Texture* gHostel6 = NULL;
SDL_Texture* gHostel7 = NULL;
SDL_Texture* gHostel8 = NULL;
SDL_Texture* gHostel9 = NULL;
SDL_Texture* gHostel10 = NULL;
SDL_Texture* gHostel11 = NULL;
SDL_Texture* gHostel12 = NULL;
SDL_Texture* text_health = NULL;
SDL_Texture* text_money = NULL;
SDL_Texture* text_SCORE = NULL;
SDL_Texture* text_education = NULL;
SDL_Texture* text_hi = NULL;
SDL_Texture* text_NSO = NULL;
SDL_Texture* player1_texture = NULL;
SDL_Texture* player2_texture = NULL;
//Rendered texture
LTexture gTextTexture;

//Scene textures
LTexture gDotTexture;
LTexture gDotTexture2;

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

SDL_Texture* LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
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
	return mTexture;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();
	TTF_Font* Sans = TTF_OpenFont("Sans.ttf", 24);
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(Sans , textureText.c_str(), textColor );
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

void add_Text(int h,int n,int m,int e,int hi,int nso)
{
	// Get rid of preexisting texture
	std::stringstream ss1,ss2,ss3,ss4,ss5,ss6;
	ss1 << h;
	std::string h_str;
	ss1 >> h_str;
	ss2 << n;
	std::string n_str;
	ss2 >> n_str;
	ss3 << m;
	std::string m_str;
	ss3>> m_str;
	ss4 << e;
	std::string e_str;
	ss4>> e_str;
	ss5 << hi;
	std::string hi_str;
	ss5>> hi_str;
	ss6 << nso;
	std::string nso_str;
	ss6>> nso_str;
	std::string health = "Health POINTS =  " + h_str ;
	std::string SCORE = "SCORE = " + n_str;
	std::string money = "Money = " + m_str;
	std::string education = "Education Level = " + e_str;
	std::string happiness_index = "Happiness Index = " + hi_str;
	std::string NSO = "NSO Hours = " + nso_str;
	TTF_Font* lato = TTF_OpenFont("Lato-Black.ttf", 24);
	SDL_Color color = {255,51,153}; 
	SDL_Surface* textSurface1 = TTF_RenderText_Solid( lato, health.c_str(), color );
	SDL_Surface* textSurface2 = TTF_RenderText_Solid( lato, SCORE.c_str(), color );
	SDL_Surface* textSurface3 = TTF_RenderText_Solid( lato, money.c_str(), color );
	SDL_Surface* textSurface4 = TTF_RenderText_Solid( lato, education.c_str(), color );
	SDL_Surface* textSurface5 = TTF_RenderText_Solid( lato, happiness_index.c_str(), color );
	SDL_Surface* textSurface6 = TTF_RenderText_Solid( lato, NSO.c_str(), color );
	if( textSurface1 != NULL )
	{
		//Create texture from surface pixels
        text_health = SDL_CreateTextureFromSurface( gRenderer, textSurface1 );
		text_SCORE = SDL_CreateTextureFromSurface( gRenderer, textSurface2 );
		text_money = SDL_CreateTextureFromSurface( gRenderer, textSurface3 );
		text_education = SDL_CreateTextureFromSurface( gRenderer, textSurface4 );
		text_hi = SDL_CreateTextureFromSurface( gRenderer, textSurface5 );
		text_NSO = SDL_CreateTextureFromSurface( gRenderer, textSurface6 );
		//Get rid of old surfac
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
		SDL_FreeSurface( textSurface1 );
		SDL_FreeSurface( textSurface2 );
		SDL_FreeSurface( textSurface3 );
		SDL_FreeSurface( textSurface4 );
		SDL_FreeSurface( textSurface5 );
		SDL_FreeSurface( textSurface6 );
		TTF_CloseFont( lato );
}

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


Dot::Dot()
{
    //Initialize the offsets
    mPosX = 1430;
    mPosY = 410;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

void Dot::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= DOT_VEL; break;
            case SDLK_DOWN: mVelY += DOT_VEL; break;
            case SDLK_LEFT: mVelX -= DOT_VEL; break;
            case SDLK_RIGHT: mVelX += DOT_VEL; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += DOT_VEL; break;
            case SDLK_DOWN: mVelY -= DOT_VEL; break;
            case SDLK_LEFT: mVelX += DOT_VEL; break;
            case SDLK_RIGHT: mVelX -= DOT_VEL; break;
        }
    }
}

void Dot::move(int part)
{
    //Move the dot left or right
    mPosX += mVelX;

    //If the dot went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > SCREEN_WIDTH ) )
    {
        //Move back
        mPosX -= mVelX;
    }
    //Move the dot up or down
    mPosY += mVelY;
    //If the dot went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > SCREEN_HEIGHT ) )
    {
        //Move back
        mPosY -= mVelY;
    }
	// int HorV;
	// if( (mPosX>178) && (mPosX <188) ) {
	// 		HorV = 0;
	// 	}
	// if( (mPosY >112) && (mPosY <125) && ( (mPosX <=178) || (mPosX >=188) ) ) {
	// 		HorV = 1;
	// 	}
		
	if(part == 0 ) {
		// Jwala, Ara, Kara, Nilgiri 
		if ( (mPosX <183) and ((mPosY<140) or (170<mPosY and mPosY<300) or (337<mPosY and mPosY<460) or (495<mPosY and mPosY<625) or (660<mPosY)) ) {
			mPosX -= mVelX;			
		}
		if ( (mPosX <183) and not((mPosY<140) or (170<mPosY and mPosY<300) or (337<mPosY and mPosY<460) or (495<mPosY and mPosY<625) or (660<mPosY)) ) {
			mPosY -= mVelY;
		}
		if ( (mPosX > 198 and mPosX<220) and ((mPosY<140) or (170<mPosY and mPosY<300) or (327<mPosY and mPosY<340) or (360<mPosY and mPosY<450) or (480<mPosY and mPosY<595) or (620<mPosY and mPosY<780)) ) {
			mPosX -= mVelX;
		}
		if ((mPosX<370 and mPosX>=350) and ((mPosY<140) or (170<mPosY and mPosY<300) or (320<mPosY and mPosY<340) or (360<mPosY and mPosY<450) or (480<mPosY and mPosY<595) or (620<mPosY and mPosY<675) or (690<mPosY and mPosY<780)) ) {
			mPosX -= mVelX;	
		}
		if ((mPosX>210 and mPosX<370)) {
			if (mPosX>235 and mPosX<260 and mPosY<=170) {
				if (mPosY<140) {
					mPosX -= mVelX;
				}
				if (mPosY<85) {
					mPosY -= mVelY;
				}
			}
			else {
				mPosY -= mVelY;
			}
		}
		if (mPosX>250 and mPosX<280 and mPosY>=300 and mPosY<=320) {
			mPosX -= mVelX;
		}
		if (mPosX>380 and mPosX<415 and ((mPosY<140) or (170<mPosY and mPosY<200) or (220<mPosY and mPosY<370)  or (390<mPosY and mPosY<450) or (480<mPosY and mPosY<780))) {
			mPosX -= mVelX;
		}	
		if (mPosX>=370 and mPosX<=390) {
			if ((mPosY<55 or mPosY>690)) {
				mPosY -= mVelY;
			}
		}
		if (mPosX>380 and mPosX<555 and mPosY>=140 and mPosY<=170) {
			mPosY -= mVelY;
		}
		if (mPosX>380 and mPosX<=420 and mPosY>=200 and mPosY<=220) {
			mPosY -= mVelY;
		}
		if (mPosX>420 and mPosX<=435) {
			if (mPosY>200 and mPosY<245) {
				mPosX -= mVelX;
			}
			else {
				mPosY -= mVelY;
			}
		}
		if (mPosX>380 and mPosX<=490 and mPosY>=370 and mPosY<=390) {
			mPosY -= mVelY;
		}
		if (mPosX>490 and mPosX<505) {
			if (mPosY>=275 and mPosY<=385) {
				mPosX -= mVelX;
			}
			else {
				mPosY -= mVelY;
			}
		}
		if (mPosX>380 and mPosX<=620 and mPosY>=450 and mPosY<=480) {
			mPosY -= mVelY;
		}
		if (mPosX>=555 and mPosX<580) {
			if ((mPosY>=65 and mPosY<140) or (mPosY>170 and mPosY<=335)) {
				mPosX -= mVelX;
			}
			else if (mPosY<65 or mPosY>355){
				mPosY -= mVelY;
			}
		}
		if (mPosX<=555 and mPosX>540 and mPosY>335 and mPosY<355) {
			mPosX -= mVelX;
		}
		if (mPosX>=580 and mPosX<620 and mPosY>335 and mPosY<355) {
			mPosY -= mVelY;
		}
		if (mPosX>=620 and mPosX<645 and ((mPosY>335 and mPosY<450) or (mPosY>480 and mPosY<705) or (mPosY>725 and mPosY<780))) {
			mPosX -= mVelX;
		}
		if (mPosX>=620 and mPosX<645 and mPosY<335) {
			mPosY -= mVelY;
		}
		if (mPosX>645 and mPosX<665 and mPosY>=780) {
			mPosX -= mVelX;
		}
		if (((mPosX>645 and mPosX<=1615) or (mPosX>1635)) and mPosY>=705 and mPosY<=725) {
			mPosY -= mVelY;
		}
		if ((mPosX>1615 and mPosX<=1635) and mPosY>725) {
			mPosY -= mVelY;
		}
		if (mPosY>=140 and mPosY<=170 and (mPosX>=580 and mPosX<715)) {
			mPosY -= mVelY;
		}
		if (mPosX>715 and mPosX<735 ) {
			if ( mPosY<140 or (mPosY>285 and mPosY<320)) {
				mPosY -= mVelY;
			}
			else if (mPosY<265) {
				mPosX -= mVelX;
			}
			else if (mPosY>265 and mPosY<320 and mPosX<=715) {
				mPosX -= mVelX;
			}
		}
		if (mPosX>=735 and mPosX<795 and mPosY>265 and mPosY<285) {
			mPosY -= mVelY;
		}
		if (mPosX>=795 and mPosX<820) {
			if (mPosY<265 or (mPosY>285 and mPosY<355)) {
				mPosX -= mVelX;
			}
			else if (mPosY>370 and mPosY<390) {
				mPosY -= mVelY;
			}
			else {
				if (mPosX>815 and mPosX<840 and mPosY>=355 and mPosY<370) {
					mPosX -= mVelX;
				}
			}
		}
		if (mPosX>=750 and mPosX<820 and mPosY>=355 and mPosY<=370) {
			if (mPosX>819) {
				mPosX -= mVelX;
			}
			else {
				mPosY -= mVelY;
			}
		}
		if (mPosX<750 and mPosX>730 and mPosY>=355 and mPosY<450) {
			mPosX -= mVelX;
		}
		if (mPosY>=440 and mPosY<=480 and mPosX>=645) {
			if (mPosX>1450) {
				mPosX -= mVelX;
			}
			else if (mPosX<735 or (mPosX>755 and mPosX<1285) or (mPosX>1305 and mPosX<1430)) {
				mPosY -= mVelY;
			}
			else if (mPosY>=480) {
				mPosY -= mVelY;
			}
		}
		if ( mPosX>=1285 and mPosX<=1305) {
			if (mPosY<270 or (mPosY>480 and mPosY<520)) {
				mPosY -= mVelY;
			}
			else if (mPosY<450 and mPosY>=270) {
				mPosX -= mVelX;
			}
		}
		if (mPosX>=1430 and mPosX<=1450) {
			if (mPosY<80 or (mPosY>480 and mPosY<520) ) {
				mPosY -= mVelY;
			}
			else if ((mPosY>100 and mPosY<210) or (mPosY>225 and mPosY<235) or (mPosY>250 and mPosY<450)) {
				mPosX -= mVelX;
			}
		}
		if (mPosY>=235 and mPosY<=250) {
			if (mPosX>1400 and mPosY<1430) {
				mPosX -= mVelX;
			}
		}
		if (mPosY>=210 and mPosY<=225) {
			if ((mPosX>1450 and mPosX<1480) or (mPosX<1320 and mPosX>1290)) {
				mPosX -= mVelX;
			}
		}
		if (mPosY>=80 and mPosY<=100) {
			if ((mPosX>1450 and mPosX<1480 )or (mPosX<1320 and mPosX>1290)) {
				mPosX -= mVelX;
			}
		}
		if (((mPosY>=80 and mPosY<=100) or (mPosY>=210 and mPosY<=225)) and (mPosX<=1430 and mPosX>=1320)) {
			mPosY -= mVelY;
		}
		if (mPosY>230 and mPosY<250) {
			if ((mPosX>1450 and mPosX<=1525) or (mPosX>=1540 and mPosX<=1620) or (mPosX>1640)) {
				mPosY -= mVelY;
			}
		}
		if (mPosX>1525 and mPosX<1540) {
			if (mPosY<230 or (mPosY>560 and mPosY<590)) {
				mPosY -= mVelY;
			}
			else if (mPosY>250 and mPosY<=560) {
				mPosX -= mVelX;
			}
		}
		if (mPosX>1620 and mPosX<=1640) {
			if (mPosY<110) {
				mPosY -= mVelY;
			}
			else if ((mPosY>135 and mPosY<230) or (mPosY>250 and mPosY<705)) {
				mPosX -= mVelX;
			}
		}
		if (mPosY>=110 and mPosY<=135) {
			if (mPosX>1640 and mPosX<1705) {
				mPosY -= mVelY;
			}
			if (mPosX>1725) {
				mPosX -= mVelX;
			}
		}
		if (mPosX>=1705 and mPosX<=1725) {
			if (mPosY>135 and mPosY<160) {
				mPosY -= mVelY;
			}
			else if (mPosY<110) {
				mPosX -= mVelX;
			}
		}
	}
	
}

void Dot::render()
{
    //Show the dot
	gDotTexture.render( mPosX, mPosY );
}
void Dot::render2()
{
    //Show the dot
	gDotTexture2.render( mPosX, mPosY );
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
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				//Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}
		SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}
void load_img() {
	gTexture = loadTexture( "Finalmap.png" );
	gHostel1 = loadTexture( "hostel1.png" );
	gHostel2 = gHostel1;
	gHostel3 = gHostel1;
	gHostel4 = gHostel1;
	gHostel5 = gHostel1;
	gHostel6 = gHostel1;
	gHostel7 = gHostel1;
	gHostel8 = gHostel1;
	gHostel9 = gHostel1;
	gHostel10 = gHostel1;
	gHostel11 = gHostel1;
	gHostel12 = gHostel1;
}
void null_images() {
	gHostel1 = NULL;
	gHostel2 = NULL;
	gHostel3 = NULL;
	gHostel4 = NULL;
	gHostel5 = NULL;
	gHostel6 = NULL;
	gHostel7 = NULL;
	gHostel8 = NULL;
	gHostel9 = NULL;
	gHostel10 = NULL;
	gHostel11 = NULL;
	gHostel12 = NULL;

}
bool loadMedia()
{
	//Loading success flag
	bool success = true;
	//Load dot texture
	player1_texture= gDotTexture.loadFromFile( "p1.png" );
	player2_texture=gDotTexture2.loadFromFile( "p2.png" );
	if( (player1_texture == NULL) || (player2_texture ==NULL)) {
		printf("Eror loading images of players");
		success = false;
	}
	return success;
}

void close()
{
	//Free loaded images
	gTextTexture.free();

	//Free global font
	gFont = NULL;
	//Free loaded images
	gDotTexture.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	SDL_DestroyTexture(text_health);
	SDL_DestroyTexture(text_SCORE);
	SDL_DestroyTexture(text_money);
	SDL_DestroyTexture(text_NSO);
	SDL_DestroyTexture(text_education);
	SDL_DestroyTexture(text_hi);
	// SDL_DestroyTexture(print_message);
	SDL_DestroyTexture(gHostel1);
	SDL_DestroyTexture(gHostel2);
	SDL_DestroyTexture(gHostel3);
	SDL_DestroyTexture(gHostel4);
	SDL_DestroyTexture(gHostel5);
	SDL_DestroyTexture(gHostel6);
	SDL_DestroyTexture(gHostel7);
	SDL_DestroyTexture(gHostel8);
	SDL_DestroyTexture(gHostel9);
	SDL_DestroyTexture(gHostel10);
	SDL_DestroyTexture(gHostel11);
	SDL_DestroyTexture(gHostel12);
	SDL_DestroyTexture(player1_texture);
	SDL_DestroyTexture(player2_texture);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
float health=1000;
int money=1000;
float education=50;
int score=0;
float happiness_index=50;
float NSO=0;
bool isabletoplay=false;


int main( )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{	
		load_img();
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

			//The dot that will be moving around on the screen
			Dot dot;

	// socketing
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	// char* hello = "Hello from client";
	char buffer[32] = { 0 };
	char* IPV4 = "127.0.0.1";	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, IPV4, &serv_addr.sin_addr)
		<= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
	send(sock, "Hello from client", strlen("Hello from client"), 0);
	printf("Hello message sent\n");
	valread = read(sock, buffer, 1024);
	printf("%s\n", buffer);

			int part=0 ;
			//While application is running
			while( !quit )
			{
				//Handle events on queue
				// if (part==0 and health > 0) {
				// 	health-=0.003;
				// }
				
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else if( e.type == SDL_KEYDOWN )
					{
						//Select surfaces based on key press
						switch( e.key.keysym.sym )
						{
							
							case SDLK_r:
							if (part==0) {
							if( ( (dot.mPosX < 208) && (dot.mPosX > 168) ) && ( (dot.mPosY > 40) && (dot.mPosY < 90) ) ) {
							part = 1;
							gTexture = loadTexture( "HOSTEL_inside.png" );
							null_images();
							}
							else if( ( (dot.mPosX < 208) && (dot.mPosX > 168) ) && ( (dot.mPosY > 190) && (dot.mPosY < 270) ) ) {
							part = 2;
							gTexture = loadTexture( "HOSTEL_inside.png" );
							null_images();
							}
							else if( ( (dot.mPosX < 208) && (dot.mPosX > 168) ) && ( (dot.mPosY > 340) && (dot.mPosY < 460) ) ) {
							part = 3;
							gTexture = loadTexture( "HOSTEL_inside.png" );
							null_images();
							}
							else if( ( (dot.mPosX < 208) && (dot.mPosX > 168) ) && ( (dot.mPosY > 490) && (dot.mPosY < 650) ) ) {
							part = 4;
							gTexture = loadTexture( "HOSTEL_inside.png" );
							null_images();
							}
							else if( ( (dot.mPosX < 208) && (dot.mPosX > 168) ) && ( (dot.mPosY > 680) && (dot.mPosY < 780) ) ) {
							part = 5;
							gTexture = loadTexture( "HOSTEL_inside.png" );
							null_images();
							}
							else if( ( (dot.mPosX < 350) && (dot.mPosX > 250) ) && ( (dot.mPosY > 35) && (dot.mPosY < 180) ) ) {
							part = 6;
							gTexture = loadTexture( "HOSTEL_inside.png" );
							null_images();
							}
							else if( ( (dot.mPosX < 540) && (dot.mPosX > 420) ) && ( (dot.mPosY > 20) && (dot.mPosY < 180) ) ) {
							part = 7;
							gTexture = loadTexture( "HOSTEL_inside.png" );
							null_images();
							}
							else if( ( (dot.mPosX < 440) && (dot.mPosX > 360) ) && ( (dot.mPosY > 200) && (dot.mPosY < 320) ) ) {
							part = 8;
							gTexture = loadTexture( "HOSTEL_inside.png" );
							null_images();
							}
							else if( ( (dot.mPosX < 540) && (dot.mPosX > 480) ) && ( (dot.mPosY > 350) && (dot.mPosY < 480) ) ) {
							part = 9;
							gTexture = loadTexture( "HOSTEL_inside.png" );
							null_images();
							}
							else if( ( (dot.mPosX < 710) && (dot.mPosX > 620) ) && ( (dot.mPosY > 130) && (dot.mPosY < 180) ) ) {
							part = 10;
							gTexture = loadTexture( "HOSTEL_inside.png" );
							null_images();
							}
							else if( ( (dot.mPosX < 820) && (dot.mPosX > 780) ) && ( (dot.mPosY > 50) && (dot.mPosY < 200) ) ) {
							part = 11;
							gTexture = loadTexture( "HOSTEL_inside.png" );
							null_images();
							}
							else if( ( (dot.mPosX < 1750) && (dot.mPosX > 1700) ) && ( (dot.mPosY > 20) && (dot.mPosY < 100) ) ) {
							part = 12;
							gTexture = loadTexture( "HOSTEL_inside.png" );
							null_images();
							}
							else if( ( (dot.mPosX < 220) && (dot.mPosX > 170) ) && ( (dot.mPosY > 210) && (dot.mPosY < 290) ) ) {
							part = 13;
							gTexture = loadTexture( "FOOD_inside.png" );
							null_images();
							}
							else if( ( (dot.mPosX < 650) && (dot.mPosX > 570) ) && ( (dot.mPosY > 360) && (dot.mPosY < 470) ) ) {
							part = 14;
							gTexture = loadTexture( "FOOD_inside.png" );
							null_images();
							}
							else if( ( (dot.mPosX < 780) && (dot.mPosX > 735) ) && ( (dot.mPosY > 250) && (dot.mPosY < 290) ) ) {
							part = 15;
							gTexture = loadTexture( "FOOD_inside.png" );
							null_images();
							}
							else if( ( (dot.mPosX < 1370) && (dot.mPosX > 1310) ) && ( (dot.mPosY > 70) && (dot.mPosY < 110) ) ) {
							part = 16;
							gTexture = loadTexture( "SHIRU_inside.png" );
							null_images();
							}
							else if( ( (dot.mPosX < 940) && (dot.mPosX > 840) ) && ( (dot.mPosY > 450) && (dot.mPosY < 490) ) ) {
							part = 17;
							gTexture = loadTexture( "HOSPITAL_inside.png" );
							null_images();
							}
							else if( ( (dot.mPosX < 1310) && (dot.mPosX > 1280) ) && ( (dot.mPosY > 270) && (dot.mPosY < 420) ) ) {
							part = 18;
							gTexture = loadTexture( "LIBRARY_inside.png" );
							null_images();
							}
							else if( ( (dot.mPosX < 1560) && (dot.mPosX > 1510) ) && ( (dot.mPosY > 520) && (dot.mPosY < 580) ) ) {
							part = 19;
							gTexture = loadTexture( "LHC_inside.png" );
							null_images();
							}
							else if( ( (dot.mPosX < 1060) && (dot.mPosX > 1000) ) && ( (dot.mPosY > 450) && (dot.mPosY < 490) ) ) {
							part = 20;
							gTexture = loadTexture( "CRICKET_inside.png" );
							null_images();
							}
							else if( ( (dot.mPosX < 1380) && (dot.mPosX > 1320) ) && ( (dot.mPosY > 450) && (dot.mPosY < 490) ) ) {
							part = 21;
							gTexture = loadTexture( "HOCKEY_inside.png" );
							null_images();
							}
							else if( ( (dot.mPosX < 330) && (dot.mPosX > 230) ) && ( (dot.mPosY > 440) && (dot.mPosY < 490) ) ) {
							part = 22;
							gTexture = loadTexture( "TENNIS_inside.png" );
							null_images();
							}
							else if( ( (dot.mPosX < 650) && (dot.mPosX > 580) ) && ( (dot.mPosY > 570) && (dot.mPosY < 660) ) ) {
							part = 23;
							gTexture = loadTexture( "SAC_inside.png" );
							null_images();
							}
							else if( ( (dot.mPosX < 470) && (dot.mPosX > 420) ) && ( (dot.mPosY > 440) && (dot.mPosY < 490) ) ) {
							part = 24;
							gTexture = loadTexture( "ATM_inside.png" );
							null_images();
							}
							}
							break;
							
							case SDLK_e:
							load_img();
							part =0;
							if (health>1000) {
								health=1000;
							}
							if (education>100) {
								education=100;
							}
							if (happiness_index>100) {
								happiness_index=100;
							}
							if (NSO>100) {
								NSO=100;
							}
							break;
							case SDLK_UP:
							if (part==0 and health >0) {
							health-=1;
							}
							break;
							case SDLK_DOWN:
							if (part==0 and health >0) {
							health-=1;
							}
							break;
							case SDLK_RIGHT:
							if (part==0 and health >0) {
							health-=1;
							}
							break;
							case SDLK_LEFT:
							if (part==0 and health >0) {
							health-=1;
							}
							break;
							case SDLK_h:
							if (part==17) {
					health+=1;
				}
				if ((part==20 or part==21 or part==22) and isabletoplay==true) {
					NSO+=1;
				}
				if (part==23) {
					happiness_index+=1;
				}
				if (part==18 or part==19) {
					education+=1;
				}
							break;
							default:
							
							break;
							
							case SDLK_s:
							if (part>=1 and part<=12) {
								health+=0;
								money+=0;
								education+=0;
								happiness_index+=5;
								score+=-200;
							}
							break;
							
							case SDLK_m:
							if (part>=1 and part<=12 and money>=100) {
								health+=5;
								money+=-100;
								education+=0;
								happiness_index+=0;
								score+=0;
							}
							break;
							
							case SDLK_l:
							if (part>=1 and part<=12) {
								health+=0;
								money+=0;
								education+=1;
								happiness_index+=-1;
								score+=100;
							}
							break;
							
							case SDLK_p:
							if (part>=1 and part<=12) {
								health+=-5;
								money+=0;
								education+=0;
								happiness_index+=5;
								score+=-100;
							}
							break;
							
							case SDLK_g:
							if (part>=1 and part<=12) {
								isabletoplay=true;
							}
							break;
							
							case SDLK_b:
							if (part>=13 and part<=15 and money>=100) {
								health+=-5;
								money+=-100;
								education+=0;
								happiness_index+=5;
								score+=100;
							}
							break;
							
							case SDLK_i:
							if (part>=13 and part<=15 and money>=50) {
								health+=-2;
								money+=-50;
								education+=0;
								happiness_index+=5;
								score+=100;
							}
							break;
							
							case SDLK_j:
							if (part>=13 and part<=15 and money>=150) {
								health+=5;
								money+=-150;
								education+=0;
								happiness_index+=5;
								score+=100;
							}
							break;
							
							case SDLK_d:
							if (part==16) {
								health+=1;
								money+=0;
								education+=0;
								happiness_index+=5;
								score+=0;
							}
							break;
							
							case SDLK_1:
							if (part==24 and score>=200) {
								health+=0;
								money+=100;
								education+=0;
								happiness_index+=0;
								score+=-200;
							}
							break;
							
							case SDLK_2:
							if (part==24 and score>=400) {
								health+=0;
								money+=200;
								education+=0;
								happiness_index+=0;
								score+=-400;
							}
							break;
							
							case SDLK_3:
							if (part==24 and score>=600) {
								health+=0;
								money+=300;
								education+=0;
								happiness_index+=0;
								score+=-600;
							}
							break;
							
							case SDLK_4:
							if (part==24 and score>=800) {
								health+=0;
								money+=400;
								education+=0;
								happiness_index+=0;
								score+=-800;
							}
							break;
							
							case SDLK_5:
							if (part==24 and score>=1000) {
								health+=0;
								money+=500;
								education+=0;
								happiness_index+=0;
								score+=-1000;
							}
							break;
						}
					}
					
					//Handle input for the dot
					dot.handleEvent( e );
				}

				//Move the dot
				dot.move(part);

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );
				//Render texture to screen
				SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );
				SDL_Rect Hostel1_src;
				SDL_Rect Hostel1_dst;
				Hostel1_src.x = 0;
				Hostel1_src.y = 0;
				Hostel1_src.w = 200;
				Hostel1_src.h = 200;
				Hostel1_dst.x  = 0;
				Hostel1_dst.y  = 0;
				Hostel1_dst.w  = 200;
				Hostel1_dst.h  = 200;
				SDL_Rect Hostel2_src;
				SDL_Rect Hostel2_dst;
				Hostel2_src.x = 0;
				Hostel2_src.y = 0;
				Hostel2_src.w = 200;
				Hostel2_src.h = 200;
				Hostel2_dst.x  = 0;
				Hostel2_dst.y  = 150;
				Hostel2_dst.w  = 200;
				Hostel2_dst.h  = 200;
				SDL_Rect Hostel3_src;
				SDL_Rect Hostel3_dst;
				Hostel3_src.x = 0;
				Hostel3_src.y = 0;
				Hostel3_src.w = 200;
				Hostel3_src.h = 200;
				Hostel3_dst.x  = 0;
				Hostel3_dst.y  = 320;
				Hostel3_dst.w  = 200;
				Hostel3_dst.h  = 200;
				SDL_Rect Hostel4_src;
				SDL_Rect Hostel4_dst;
				Hostel4_src.x = 0;
				Hostel4_src.y = 0;
				Hostel4_src.w = 200;
				Hostel4_src.h = 200;
				Hostel4_dst.x  = 0;
				Hostel4_dst.y  = 480;
				Hostel4_dst.w  = 200;
				Hostel4_dst.h  = 200;
				SDL_Rect Hostel5_src;
				SDL_Rect Hostel5_dst;
				Hostel5_src.x = 0;
				Hostel5_src.y = 0;
				Hostel5_src.w = 200;
				Hostel5_src.h = 200;
				Hostel5_dst.x  = 380;
				Hostel5_dst.y  = 0;
				Hostel5_dst.w  = 200;
				Hostel5_dst.h  = 200;
				SDL_Rect Hostel6_src;
				SDL_Rect Hostel6_dst;
				Hostel6_src.x = 0;
				Hostel6_src.y = 0;
				Hostel6_src.w = 200;
				Hostel6_src.h = 200;
				Hostel6_dst.x  = 610;
				Hostel6_dst.y  = 0;
				Hostel6_dst.w  = 200;
				Hostel6_dst.h  = 200;
				SDL_Rect Hostel7_src;
				SDL_Rect Hostel7_dst;
				Hostel7_src.x = 0;
				Hostel7_src.y = 0;
				Hostel7_src.w = 200;
				Hostel7_src.h = 200;
				Hostel7_dst.x  = 790;
				Hostel7_dst.y  = 10;
				Hostel7_dst.w  = 200;
				Hostel7_dst.h  = 200;
				SDL_Rect Hostel8_src;
				SDL_Rect Hostel8_dst;
				Hostel8_src.x = 0;
				Hostel8_src.y = 0;
				Hostel8_src.w = 200;
				Hostel8_src.h = 200;
				Hostel8_dst.x  = 1720;
				Hostel8_dst.y  = 0;
				Hostel8_dst.w  = 200;
				Hostel8_dst.h  = 200;
				SDL_Rect Hostel9_src;
				SDL_Rect Hostel9_dst;
				Hostel9_src.x = 0;
				Hostel9_src.y = 0;
				Hostel9_src.w = 200;
				Hostel9_src.h = 200;
				Hostel9_dst.x  = 270;
				Hostel9_dst.y  = 30;
				Hostel9_dst.w  = 130;
				Hostel9_dst.h  = 130;
				SDL_Rect Hostel10_src;
				SDL_Rect Hostel10_dst;
				Hostel10_src.x = 0;
				Hostel10_src.y = 0;
				Hostel10_src.w = 200;
				Hostel10_src.h = 200;
				Hostel10_dst.x  = 380;
				Hostel10_dst.y  = 260;
				Hostel10_dst.w  = 150;
				Hostel10_dst.h  = 150;
				SDL_Rect Hostel11_src;
				SDL_Rect Hostel11_dst;
				Hostel11_src.x = 0;
				Hostel11_src.y = 0;
				Hostel11_src.w = 200;
				Hostel11_src.h = 200;
				Hostel11_dst.x  = 480;
				Hostel11_dst.y  = 380;
				Hostel11_dst.w  = 100;
				Hostel11_dst.h  = 100;
				SDL_Rect Hostel12_src;
				SDL_Rect Hostel12_dst;
				Hostel12_src.x = 0;
				Hostel12_src.y = 0;
				Hostel12_src.w = 200;
				Hostel12_src.h = 200;
				Hostel12_dst.x  = 200;
				Hostel12_dst.y  = 650;
				Hostel12_dst.w  = 130;
				Hostel12_dst.h  = 130;
				
				SDL_Rect message_rect_src,health_dst,money_dst,SCORE_dst,edu_dst,hi_dst,nso_dst;
				health_dst.x = 1700;
				health_dst.y = 250;
				health_dst.w = 150;
				health_dst.h = 35;
				money_dst = {1700,300,150,35};
				SCORE_dst = {1700,350,150,35};
				edu_dst = {1700,400,150,35};
				hi_dst = {1700,450,150,35};
				nso_dst= {1700,500,150,35};
				// message_rect_src.x = 0;
				// message_rect_src.y = 0;
				// message_rect_src.w = 100;
				// message_rect_src.h = 100;
				add_Text(int(health),score,money,int(education),int(happiness_index),int(NSO));
				SDL_RenderCopy( gRenderer, gHostel1, &Hostel1_src, &Hostel1_dst );
				SDL_RenderCopy( gRenderer, gHostel2, &Hostel2_src, &Hostel2_dst );
				SDL_RenderCopy( gRenderer, gHostel2, &Hostel3_src, &Hostel3_dst );
				SDL_RenderCopy( gRenderer, gHostel2, &Hostel4_src, &Hostel4_dst );
				SDL_RenderCopy( gRenderer, gHostel2, &Hostel5_src, &Hostel5_dst );
				SDL_RenderCopy( gRenderer, gHostel2, &Hostel6_src, &Hostel6_dst );
				SDL_RenderCopy( gRenderer, gHostel2, &Hostel7_src, &Hostel7_dst );
				SDL_RenderCopy( gRenderer, gHostel2, &Hostel8_src, &Hostel8_dst );
				SDL_RenderCopy( gRenderer, gHostel2, &Hostel9_src, &Hostel9_dst );
				SDL_RenderCopy( gRenderer, gHostel2, &Hostel10_src, &Hostel10_dst );
				SDL_RenderCopy( gRenderer, gHostel2, &Hostel11_src, &Hostel11_dst );
				SDL_RenderCopy( gRenderer, gHostel2, &Hostel12_src, &Hostel12_dst );
				SDL_RenderCopy(gRenderer,text_health,NULL,&health_dst);
				SDL_RenderCopy(gRenderer,text_SCORE,NULL,&money_dst);
				SDL_RenderCopy(gRenderer,text_money,NULL,&SCORE_dst);
				SDL_RenderCopy(gRenderer,text_education,NULL,&edu_dst);
				SDL_RenderCopy(gRenderer,text_hi,NULL,&hi_dst);
				SDL_RenderCopy(gRenderer,text_NSO,NULL,&nso_dst);
				//Render objects
				dot.render();

				// socketing
				string s= to_string(dot.mPosX)+" "+to_string(dot.mPosY)+" ";
				// cout<<s<<"\n";
				const char* c= s.c_str();
				send(sock, c , strlen(c), 0);
				//
				valread = read(sock, buffer, 1024);
				string word ="";
				int arr[10];
				std::istringstream iss(buffer);
				for(int i=0;i<2; i++) {
					iss>>arr[i];
				}
				int x= arr[0];
				int y= arr[1];
				// cout<<x<<"\n"<<y<<"\n";			
				Dot dot2;
				dot2.mPosX = x;
				dot2.mPosY = y;
				dot2.handleEvent( e );
				dot2.move(part);
				dot2.render2();

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}