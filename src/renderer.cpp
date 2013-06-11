#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "renderer.hpp"

CRenderer Renderer;

CRenderer::CRenderer()
{

}

CRenderer::~CRenderer()
{

}

//Set the display size returning true if it's valid, otherwise false
bool CRenderer::SetDisplaySize(int w, int h)
{
	display = SDL_SetVideoMode(w, h, 32, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL);
	return (display != NULL);
}

//Initialize OpenGL, assumes SDL has already been initialized, and the display mode set
void CRenderer::GLInit(void)
{
	//Set the clear color and clear the screen for the first time
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	//Set the viewport to the size of the window
	glViewport(0, 0, display->w, display->h);

	//Set the blending function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//Set the pixel unpack to 1 byte for now
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Disable depth testing because we're in 2D
	glDisable(GL_DEPTH_TEST);
}

//Set up the rendering context for this frame
void CRenderer::BeginFrame(void)
{
	//Switch to the projection matrix
	glMatrixMode(GL_PROJECTION);

	//Reset its position
	glLoadIdentity();

	//Clear the screen
	glClear(GL_COLOR_BUFFER_BIT);

	//Set the projection matrix to orthographic rather than perspective
	//This function sets the near to -1 and the far to 1
	//We also make the coordinates on the y axis start from the bottom rather than the top
	gluOrtho2D(0, display->w, 0, display->h);
}

//End the frame by swapping the buffers to display the changes
void CRenderer::EndFrame(void)
{
	SDL_GL_SwapBuffers();
}

//Init the renderer
bool CRenderer::Init(int w, int h)
{
	if(!SetDisplaySize(w, h))
		return false;

	GLInit();

	return true;
}
