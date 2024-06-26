/*
  Copyright (c) 2013 Max Rose

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.
  
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
  
   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
 */
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_ttf.h>
#include <box2d/box2d.h>

#include <iostream>
#include <vector>

#include "types.hpp"
#include "timer.hpp"
#include "renderer.hpp"
#include "input.hpp"
#include "texture.hpp"
#include "entity.hpp"
#include "text.hpp"
#include "game.hpp"

CGameScene *gameScene;

int main(int argc, char **argv)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return 1;

	if(renderer.Init(1024, 768) == false)
		return 1;

	gameScene = new CGameScene;

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
		
		//Set up our frame for entities to be rendered
		renderer.BeginFrame();
		
		gameScene->Frame();

		//After we're finished drawing, end the frame, swapping the buffers
		renderer.EndFrame();
	}

	SDL_Quit();

	return 0;
}
