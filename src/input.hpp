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
#ifndef INPUT_HPP
#define INPUT_HPP

class CInput
{
public:
	CInput();
	
	bool	IsKeyDown(SDLKey);
	bool	IsButtonDown(int);
	float	GetMouseX();
	float	GetMouseY();
	bool	QuitStatus();
	void	HandleInput();

private:
	void UpdateKeyStatus(SDL_Event&);	
	void UpdateMouseStatus(SDL_Event&);

	unsigned char	keyStatus[SDLK_LAST - SDLK_FIRST];	// true == pressed | false == released
	unsigned char	mouseStatus[SDL_BUTTON_X2 + 1];
	vec2f		mousePos;	//The mouse's position this frame
	vec2f		mouseOffset;	//The relative movement of the mouse this frame, if any
	bool		quit;	//whether the quit event has been received
};

extern CInput input;

#endif
