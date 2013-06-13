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
#include <SDL/SDL.h>

#include "types.hpp"
#include "input.hpp"

CInput input;

CInput::CInput()
{
}

//Update our array of bools depending whether the key is down or not
void CInput::UpdateKeyStatus(SDL_Event& event)
{
	if(event.type == SDL_KEYDOWN)
	{
		KeyStatus[event.key.keysym.sym] = true;
	}
	else if(event.type == SDL_KEYUP)
	{
		KeyStatus[event.key.keysym.sym] = false;
	}
}

void CInput::UpdateMouseStatus(SDL_Event& event)
{
	if(event.type == SDL_MOUSEMOTION)
	{
		MousePos.x    = event.motion.x;
		MousePos.y    = event.motion.y;
		MouseOffset.x = event.motion.xrel;
		MouseOffset.y = event.motion.yrel;
	}
	else if(event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
	{
		MouseStatus[event.button.button] = (event.button.state == SDL_PRESSED) ? true : false;
		MousePos.x			 = event.button.x;
		MousePos.y			 = event.button.y;
	}
}

//Check the array for a key and return if it's being pressed or not
bool CInput::IsKeyDown(SDLKey sym)
{
	return KeyStatus[sym];
}

bool CInput::IsButtonDown(int b)
{
	return MouseStatus[b];
}

float CInput::GetMouseX()
{
	return MousePos.x;
}

float CInput::GetMouseY()
{
	return MousePos.y;
}

//Take care of the input each frame
void CInput::HandleInput()
{
	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		if(event.type == SDL_QUIT)
			Quit = true;

		UpdateKeyStatus(event);
		UpdateMouseStatus(event);
	}
}

bool CInput::QuitStatus()
{
	return Quit;
}
