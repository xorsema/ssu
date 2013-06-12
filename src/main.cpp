#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <vector>

#include "types.hpp"
#include "renderer.hpp"
#include "input.hpp"
#include "entity.hpp"

CScene scene;

int main(int argc, char **argv)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return 1;

	if(renderer.Init(1024, 768) == false)
		return 1;

	//Main game loop
	for(;;)
	{
		//Take care of input
		input.HandleInput();
		

		//Exit the loop if we receive an SDL_QUIT event
		if(input.QuitStatus() == true)
			break;

		//Update our scene, this also will update its children
		scene.Update();

		//Set up our frame for entities to be rendered
		renderer.BeginFrame();

		//Render the scene
		scene.Render();

		//After we're finished drawing, end the frame, swapping the buffers
		renderer.EndFrame();
	}

	SDL_Quit();

	return 0;
}
