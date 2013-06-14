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
#ifndef RENDERER_HPP
#define RENDERER_HPP

class CRenderer
{
public:
	CRenderer();

	bool	Init(int, int);
	void	GLInit(void);
	bool	SetDisplaySize(int, int);
	void	BeginFrame(void);
	void	EndFrame(void);

	float	GetWidth();
	float	GetHeight();
	int	GetFps();

private:
	SDL_Surface*	display;
	float		framesPerSecond;
	int		frameCount;
	CTimer		frameTimer;
	
};

extern CRenderer renderer;

#endif
