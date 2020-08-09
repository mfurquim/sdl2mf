#pragma once

#include <SDL2/SDL.h>

#include <vector>
#include <utility>

#include "event.h"

using point = std::pair< int, int >;

class Game {

public:
	Game( int argc, char* args[] );
	~Game();

	void Loop();

	// Gets and Sets
	bool isGameOver();
	void gameOver();

	void addPoint( int, int );
	void removePoint();

	const int ScreenWidth();
	const int ScreenHeight();

	void NextPoint();
	void PrevPoint();

	void PointUp();
	void PointDown();
	void PointLeft();
	void PointRight();


	void NextNode();

	struct sPoint2D
	{
		float x;
		float y;
		float length;
	};

	struct sSpline
	{
		std::vector< sPoint2D > points;

		sPoint2D GetSplinePoint( float t )
		{
			int p0, p1, p2, p3;

			p1 = (int) t;
			p2 = (p1 + 1) % points.size();
			p3 = (p2 + 1) % points.size();
			p0 = p1 >= 1 ? p1 - 1 : points.size() - 1;

			t = t - (int) t;

			float tt = t * t;
			float ttt = tt * t;

			float q1 = -1.0f * ttt + 2.0f * tt - t;
			float q2 =  3.0f * ttt - 5.0f * tt + 2.0f;
			float q3 = -3.0f * ttt + 4.0f * tt + t;
			float q4 =  1.0f * ttt - 1.0f * tt;

			float tx = points[p0].x * q1
					 + points[p1].x * q2
					 + points[p2].x * q3
					 + points[p3].x * q4;

			float ty = points[p0].y * q1
					 + points[p1].y * q2
					 + points[p2].y * q3
					 + points[p3].y * q4;

			return { tx*0.5, ty*0.5 };
		}

		sPoint2D GetSplineGradient( float t )
		{
			int p0, p1, p2, p3;

			p1 = (int) t;
			p2 = (p1 + 1) % points.size();
			p3 = (p2 + 1) % points.size();
			p0 = p1 >= 1 ? p1 - 1 : points.size() - 1;

			t = t - (int) t;

			float tt = t * t;

			float q1 = -3.0f * tt +  4.0f * t - 1.0f;
			float q2 =  9.0f * tt - 10.0f * t;
			float q3 = -9.0f * tt +  8.0f * t + 1.0f;
			float q4 =  3.0f * tt -  2.0f * t;

			float tx = points[p0].x * q1
					 + points[p1].x * q2
					 + points[p2].x * q3
					 + points[p3].x * q4;

			float ty = points[p0].y * q1
					 + points[p1].y * q2
					 + points[p2].y * q3
					 + points[p3].y * q4;

			return { tx*0.5, ty*0.5 };
		}

		float CalculateSegmentLength( int node )
		{
			float fLength = 0.0f;
			float fStepSize = 0.005f;

			sPoint2D old_point, new_point;
			old_point = GetSplinePoint( (float)node );

			for( float t = 0; t < 1.0f; t += fStepSize )
			{
				new_point = GetSplinePoint( (float)node + t );

				float dx = new_point.x - old_point.x;
				float dy = new_point.y - old_point.y;

				fLength += sqrtf( ( dx * dx ) + ( dy * dy ) );

				old_point = new_point;
			}

			return fLength;
		}

		float GetNormalisedOffset( float p )
		{
			// Which node is the base?
			int i = 0;

			while( p > points[i].length )
			{
				p -= points[i].length;
				i++;
			}

			return (float)i + ( p / points[i].length );

		}

		float ClosestIndex( int node, int nextNode, struct sPoint2D mouse)
		{
			fprintf( stderr, "Node: %d\tNext: %d\tMouse: (%f,%f)\n", node, nextNode, mouse.x, mouse.y);
			struct sPoint2D closest = { 0.0f, 0.0f, 0.0f };
			float closestLength = 999999.9f;
			float closestIndex = 0.0f;

			if( node < nextNode )
			{
				for( float t = node; t < nextNode; t += 0.005f )
				{
					struct sPoint2D p = GetSplinePoint( t );
					
					float dx = mouse.x - p.x;
					float dy = mouse.y - p.y;

					float length = sqrtf( ( dx * dx ) + ( dy * dy ) );
					if( closestLength > length )
					{
						closestLength = length;
						closestIndex = t;
						closest = p;
					}
				}
			}
			else
			{
				for( float t = node; t < points.size() ; t += 0.005f )
				{
					struct sPoint2D p = GetSplinePoint( t );

					
					float dx = mouse.x - p.x;
					float dy = mouse.y - p.y;

					float length = sqrtf( ( dx * dx ) + ( dy * dy ) );
					if( closestLength > length )
					{
						closestLength = length;
						closestIndex = t;
						closest = p;
					}
				}
			}
			return closestIndex;
		}
		
	};


	struct sSpline path;

	int nSelectedPoint = 0;

	int node = 0;
	int nextNode = 1;
	int lap = 1;

	int track = 0;
	void ToggleTrack();

	int agent = 0;
	void ToggleAgent();

	struct sPoint2D mouse;

private:

	// Consts
	const int DEFAULT_SCREEN_WIDTH = 854;
	const int DEFAULT_SCREEN_HEIGHT = 480;
	const char* DEFAULT_WINDOW_TITLE = "SDL2MF";

	// Game's main functions
	bool initialize();
	bool loadAssets();
	void draw();
	void shutdown();

	// Variables
	bool m_quit = false;

	// The event handler
	Event m_event;
	void handlingEvents();

	// The window to be rendered to
	SDL_Window* m_window = NULL;

	SDL_Renderer *m_renderer = NULL;

	// The surface contained by the window
	SDL_Surface* m_screenSurface = NULL;

	// The image loaded and shown on the screen
	SDL_Surface* m_helloWorld = NULL;
	SDL_Surface* m_x = NULL;
};
