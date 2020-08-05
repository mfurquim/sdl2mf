#include "game.h"

#include <stdio.h>

Game::Game()
{
	fprintf( stderr, "Game::Game()\n" );
	m_window = NULL;
	m_screenSurface = NULL;
}

Game::~Game()
{
	fprintf( stderr, "Game::~Game()\n" );
}

bool
Game::Initialize()
{
	bool initialized = true;

	// Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		fprintf( stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		initialized = false;
	}
	else
	{
		// Create window
		m_window = SDL_CreateWindow( DEFAULT_WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( m_window == NULL )
		{
			fprintf( stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			initialized = false;
		}
		else
		{
			m_screenSurface = SDL_GetWindowSurface( m_window );
		}
	}

	return initialized;
}

bool
Game::LoadAssets()
{
	bool assetsLoaded = true;

	const char* hello_world_file_path = "assets/lazyfoo/hello_world.bmp";
	const char* x_file_path = "assets/lazyfoo/x.bmp";

	// Load image
	m_helloWorld = SDL_LoadBMP( hello_world_file_path );
	if( m_helloWorld == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", hello_world_file_path, SDL_GetError() );
		assetsLoaded = false;
	}

	m_x = SDL_LoadBMP( x_file_path );
	if( m_x == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", x_file_path, SDL_GetError() );
		assetsLoaded = false;
	}

	return assetsLoaded;
}

void
Game::Display()
{
	// Get window surface
	m_screenSurface = SDL_GetWindowSurface( m_window );

	// Fill the surface white
	SDL_FillRect( m_screenSurface, NULL, SDL_MapRGB( m_screenSurface->format, 0xEC, 0xEF, 0xF4 ) );

	// Center the image on screen
	SDL_Rect helloWorldDstrect = {
		(m_screenSurface->w - m_helloWorld->w)/2,
		(m_screenSurface->h - m_helloWorld->h)/2,
		m_helloWorld->w,
		m_helloWorld->h,
	};

	SDL_Rect xDstrect = {
		(m_screenSurface->w - m_x->w)/2,
		(m_screenSurface->h - m_x->h)/2,
		m_x->w,
		m_x->h,
	};

	// Main loop flag
	bool quit = false;

	// Event handler
	SDL_Event e;

	while( !quit )
	{
		while( SDL_PollEvent( &e ) )
		{
			if( e.type == SDL_QUIT )
			{
				quit = true;
			}

			if( e.type == SDL_KEYDOWN )
			{
				if( e.key.keysym.sym == SDLK_q )
				{
					quit = true;
				}
			}
		}

		// Apply image
		SDL_BlitSurface( m_helloWorld, NULL, m_screenSurface, &helloWorldDstrect );
		SDL_BlitSurface( m_x, NULL, m_screenSurface, &xDstrect );

		// Update the surface
		SDL_UpdateWindowSurface( m_window );
	}
}

void
Game::Shutdown()
{
	fprintf( stderr, "Shutdown()\n" );
	// Deallocate surface
	SDL_FreeSurface( m_helloWorld );
	m_helloWorld = NULL;

	SDL_FreeSurface( m_x );
	m_x = NULL;

	// Destroy window
	SDL_DestroyWindow( m_window );
	m_window  = NULL;

	// Quit SDL subsystems
	SDL_Quit();
	fprintf( stderr, "Shutdown()\n" );
}
