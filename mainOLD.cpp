//Some Testing SDL code for Neykron
//NOTE: you will need the SDL2 development files and you will need to include them into visual c++ to compile this
//Not sure if this will compile on visual c++, so if not send me the compile error and i'll see if i can help (since i dont use visual c++)

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include "LTexture.h"
#include <string>


//Running time
float runFor = 2.0; //run for 2 seconds
float runUntil = 0.0; //this counts up to runFor and then the program exits

float sampleRate = 48000.0;
float phase = 0.0;
float frequency = 840.0;

//Some premade sound generators
float nextSampleSquare()
{
	phase += (1.0 / sampleRate) * frequency;
	if (phase > 1.0) phase -= 1.0;
	return phase > 0.5 ? 1.0 : -1.0;
}

float nextSamplePulse()
{
	phase += (1.0 / sampleRate) * frequency;
	if (phase > 1.0) phase -= 1.0;
	return phase > 0.25 ? 1.0 : -1.0;
}

float nextSampleSine()
{
	phase += (M_PI * 2.0 / sampleRate) * frequency;
	if (phase > M_PI * 2.0) phase -= M_PI * 2.0;
	return sinf(phase);
}

float nextSampleSaw()
{
	phase += (1.0 / sampleRate) * frequency;
	if (phase > 1.0) phase -= 1.0;
	return phase;
}

//Now, the square and saw generators sound pretty crappy (they have aliasing)
//  Let's generate a square and saw wave with sine waves (harmonics)

float nextSampleSquareHarmonics()
{
	phase += (M_PI * 2.0 / sampleRate) * frequency;
	if (phase > M_PI * 2.0) phase -= M_PI * 2.0;

	int harmonic = 1;
	float total = 0.0; //The sound

	//Generate harmonics of base frequency (of sine waves) until nyquist frequency is hit
	while (1) //while 1 means loop forever until break; is hit
	{
		if (frequency * (float) harmonic > sampleRate / 2.0) //if the base frequency multiplied by the harmonic is over half the sampling rate (nyquist) stop generating harmonics
			break;

		total += sinf(phase * (float) harmonic) / (float) harmonic / 2.0;

		harmonic += 2; //square waves only use odd harmonics
	}

	return total;
}

float nextSampleSawHarmonics()
{
	phase += (M_PI * 2.0 / sampleRate) * frequency;
	if (phase > M_PI * 2.0) phase -= M_PI * 2.0;

	int harmonic = 1;
	float total = 0.0; //The sound

	//Generate harmonics of base frequency (of sine waves) until nyquist frequency is hit
	while (1) //while 1 means loop forever until break; is hit
	{
		if (frequency * (float) harmonic > sampleRate / 2.0) //if the base frequency multiplied by the harmonic is over half the sampling rate (nyquist) stop generating harmonics
			break;

		total += sinf(phase * (float) harmonic) / (float) harmonic / 2.0;

		harmonic += 1; //saw waves use all integer harmonics
	}

	return total;
}

void SDLAudioCallback(void *udata, Uint8 *stream, int len)
{
	unsigned int i;
	float *str = (float *) stream;

	for (i = 0; i < len / sizeof(float) / 2; i++)
	{
		//The code here gets called for each sample, you can replace the function generator below
		float smp = nextSampleSine(); //Change this function name to generate a different sound or create your own
		str[i * 2]     = smp;
		str[i * 2 + 1] = smp;
		//frequency += frequency * 0.00001; //remove the two '//' at the beginning of this line to activate frequency sweep
		runUntil += 1.0 / sampleRate;
	}
}





/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, SDL_ttf, standard IO, math, and strings

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;



//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font *gFontTiny = NULL;
TTF_Font *gFont = NULL;
TTF_Font *gFontBig = NULL;

//Rendered texture
// LTexture gTextTexture;

// LTexture gTexts[10];


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else {
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "barChartRacer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL ) {
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL ) {
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else {
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

				 //Initialize SDL_ttf
				if( TTF_Init() == -1 ) {
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		printf("Couldn't initialize SDL: \n%s\n", SDL_GetError());
		return 1;
	}

	return success;
}

SDL_Color textColor = { 0, 0, 0 };

bool loadMedia() {
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont( "16_true_type_fonts/DejaVuSans.ttf", 20 );
	if( gFont == NULL ) {
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
//	return success;

    gFontBig = TTF_OpenFont( "16_true_type_fonts/DejaVuSans.ttf", 40 );
    if( gFontBig == NULL ) {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

    gFontTiny = TTF_OpenFont( "16_true_type_fonts/DejaVuSans.ttf", 10 );
    if( gFontTiny == NULL ) {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

    return success;


}

void close() {

	//Free global font
	TTF_CloseFont( gFont );
    gFontTiny = NULL;
	gFont = NULL;
    gFontBig = NULL;

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int colorTable[3][3] = { { 0,0,255},  { 0,255,0}, { 255, 0,0}};





// drawRect:
//    renderer: pointer to an SDL_Renderer object
//    x, y, width, height: position and dimensions of the rectangle
//    color: an SDL_color structure that contains r,g,b,and a values
//    txt: the text that will be printed (right justified) inside the rectangle

void drawRect(SDL_Renderer* renderer, int x, int y, int width, int height, const SDL_Color& color, std::string txt){
	SDL_Rect fillRect = { x, y, width, height };
	SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a );		
	SDL_RenderFillRect( gRenderer, &fillRect );
	LTexture gText(renderer);
	gText.loadFromRenderedText( txt, textColor, gFont );
	gText.render( width - gText.getWidth() , y );
}

void drawText(SDL_Renderer* renderer, int x, int y,
        const SDL_Color& color, std::string txt, TTF_Font *font , int justify = 0){
    LTexture gText(renderer);
    gText.loadFromRenderedText( txt, textColor, font );
    if (justify == 1) x = x - gText.getWidth()/2;
    else if (justify == 2) x = - gText.getWidth();
    gText.render( x, y );
}

void drawXAxis(SDL_Renderer* renderer, int x, int y, int width, int height, int maxX, int tickSeparation) {
    SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
    int i = 0;
    int delta = tickSeparation *   width / static_cast<float>(maxX);
    int label = 0;
    int y2 = y + height;
    int tickHeight = .05 * height;
    while (i <= width) {
        SDL_RenderDrawLine(renderer, x + i, y + tickHeight, x + i, y + height );



        drawText(renderer,x + i, y,
                SDL_Color {.r= 0, .g = 0x0, .b = 0, .a = 0xff } ,
                std::to_string(label), gFontTiny, 1);
        i += delta;
        label = label + tickSeparation;
    }
//    SDL_RenderDrawLine(renderer, x, y + height, x+width, y + height );

}

int gGraphX, gGraphWidth, gGraphY, gGraphHeight, gBarHeight, gBarSpace;

int main( int argc, char* args[] ) {
	std::vector < std::vector < std::pair<int,std::string> > > data;
	int idx = 0;

	std::vector < std::pair<int,std::string> > tmp;
	tmp.push_back(std::pair<int,std::string> (90, "PR"));
	tmp.push_back(std::pair<int,std::string> (70, "Cuba"));

	data.push_back(tmp);

	tmp.clear();
	tmp.push_back(std::pair<int,std::string> (80, "PR"));
	tmp.push_back(std::pair<int,std::string> (75, "Cuba"));
	data.push_back(tmp);

	tmp.clear();
	tmp.push_back(std::pair<int,std::string> (95, "Cuba"));
	tmp.push_back(std::pair<int,std::string> (60, "PR"));
	data.push_back(tmp);


	gGraphX = SCREEN_WIDTH * .01;
	gGraphWidth = SCREEN_WIDTH * .98;

	gGraphY = SCREEN_HEIGHT * .2;
    gGraphHeight = SCREEN_HEIGHT * .8;

    gBarHeight = (gGraphHeight / 50) * 4;
    gBarSpace =  (gGraphHeight / 50);



	SDL_AudioSpec audioSpec;

	audioSpec.freq = (int) sampleRate;
	audioSpec.format = AUDIO_F32SYS;
	audioSpec.channels = 2;
	audioSpec.samples = 8192;
	audioSpec.callback = SDLAudioCallback;

	if (SDL_OpenAudio(&audioSpec, 0) < 0)
	{
		printf("Couldn't open audio with SDL: \n%s\n", SDL_GetError());
		return 1;
	}

	SDL_PauseAudio(0);


	//Start up SDL and create window
	if( !init() ) {
		printf( "Failed to initialize!\n" );
	}
	else {
		//Load media
		if( !loadMedia() ) {
			printf( "Failed to load media!\n" );
		}
		else {	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit ) {
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 ) {
					//User requests quit
					if( e.type == SDL_QUIT ) {
						quit = true;
					}

					else if (e.type == SDL_KEYDOWN ) {
						if ( e.key.keysym.sym== SDLK_q)  quit = true;
						if ( e.key.keysym.sym== SDLK_d) {
							idx = (idx + 1) % data.size();
						}
						if ( e.key.keysym.sym== SDLK_p) {
							frequency = frequency * .9;
							std::cout << frequency << std::endl;
						}
					}

				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );


				int y = 0;

				for (auto e: data[idx]) {
					drawRect(gRenderer, gGraphX, gGraphY + y, static_cast<int>(gGraphWidth* (e.first/100.)), gBarHeight,
					        SDL_Color {.r= 0, .g = 0xff, .b = 0, .a = 0xff }  ,e.second);
					y = y + gBarHeight + gBarSpace;
					drawText(gRenderer, 0, 300, SDL_Color {.r= 0, .g = 0x0, .b = 0, .a = 0xff } , std::to_string(idx), gFontBig);
				}
                drawXAxis(gRenderer, 50,50,400,600, 800,200);

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0; 
}



int mainzzz()
{



	SDL_Event e;
	while (runUntil < runFor) {
						while( SDL_PollEvent( &e ) != 0 ) {
					//User requests quit


					if (e.type == SDL_KEYDOWN ) {
						
						if ( e.key.keysym.sym== SDLK_d) {
							frequency = frequency * .90;
							std::cout << "freq" << frequency << std::endl;
							;

						}
				}
				}
				}	

	SDL_CloseAudio();
	SDL_Quit();
	return 0;
}
