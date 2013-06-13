#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <Box2D/Box2D.h>

#include <iostream>
#include <vector>

#include "types.hpp"
#include "renderer.hpp"
#include "input.hpp"
#include "entity.hpp"
#include "physics.hpp"

CScene scene;

void do_controls()
{	
	if(input.IsKeyDown(SDLK_i))
	{
		scene.Zoom(.01f);
	} 
	if(input.IsKeyDown(SDLK_o))
	{
		scene.Zoom(-.01f);
	}
	
	if(input.IsKeyDown(SDLK_LEFT))
	{
		scene.Offset(5.0f, 0.0f);
	}
	
	if(input.IsKeyDown(SDLK_RIGHT))
	{
		scene.Offset(-5.0f, 0.0f);
	}
	
	if(input.IsKeyDown(SDLK_UP))
	{
		scene.Offset(0.0f, -5.0f);
	}
	
	if(input.IsKeyDown(SDLK_DOWN))
	{
		scene.Offset(0.0f, 5.0f);
	}

	
}

int main(int argc, char **argv)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return 1;

	if(renderer.Init(1024, 768) == false)
		return 1;

	scene.Children.push_back(new CGroundRect());

	//Main game loop
	for(;;)
	{
		//Take care of input
		input.HandleInput();	

		//Exit the loop if we receive an SDL_QUIT event
		if(input.QuitStatus() == true)
			break;

		if(input.IsKeyDown(SDLK_ESCAPE))
		{
			break;
		}

		do_controls();

		
		//Step the physics world
		physics.StepWorld();

		//Update our scene, this also will update its children
		scene.Update();

		//Set up our frame for entities to be rendered
		renderer.BeginFrame();
		
		//Render the scene
		scene.Render();

		if(input.IsButtonDown(SDL_BUTTON_LEFT))
		{
			float x, y;
			
			scene.ScreenToWorld(input.GetMouseX(), renderer.GetHeight() - input.GetMouseY(), &x, &y);

			x = physics.PixelsToMeters(x);
			y = physics.PixelsToMeters(y);
			
			scene.Children.push_back(new CStackableRect(x, y));
		}

		//After we're finished drawing, end the frame, swapping the buffers
		renderer.EndFrame();
	}

	SDL_Quit();

	return 0;
}
