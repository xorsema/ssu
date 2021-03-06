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
#include <cstring>

#include "types.hpp"
#include "input.hpp"

CInput input;

CInput::CInput()
{
	mouseListener	 = NULL;
	keyboardListener = NULL;
}

void CInput::SetKeyboardListener(CKeyboardListener *kl)
{
	keyboardListener = kl;
}

void CInput::SetMouseListener(CMouseListener *ml)
{
	mouseListener = ml;
}

//Update our arrays of bools depending whether the status of the key(s)
void CInput::UpdateKeyStatus(SDL_Event& event)
{
	if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
	{
		keyStatus[event.key.keysym.sym] = event.key.state;
	}
}

void CInput::UpdateMouseStatus(SDL_Event& event)
{
	if(event.type == SDL_MOUSEMOTION)
	{
		mousePos.x    = event.motion.x;
		mousePos.y    = event.motion.y;
		mouseOffset.x = event.motion.xrel;
		mouseOffset.y = event.motion.yrel;
	}
	else if(event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
	{
		mouseStatus[event.button.button] = event.button.state;
		mousePos.x			 = event.button.x;
		mousePos.y			 = event.button.y;
	}
}

void CInput::UpdateListeners(SDL_Event& event)
{
	//Call mouse listener functions
	if(mouseListener != NULL)
	{
		switch(event.type)
		{
		case SDL_MOUSEMOTION:
			mouseListener->MouseMove(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouseListener->MouseDown(event.button.x, event.button.y, event.button.button);
			break;

		case SDL_MOUSEBUTTONUP:
			mouseListener->MouseUp(event.button.x, event.button.y, event.button.button);
			break;
		}
	}

	//Call keyboard listener functions
	if(keyboardListener != NULL)
	{
		switch(event.type)
		{
		case SDL_KEYDOWN:
			keyboardListener->KeyDown(event.key.keysym.sym);
			break;

		case SDL_KEYUP:
			keyboardListener->KeyUp(event.key.keysym.sym);
			break;
		}
	}
}

//Check the array for a key and return if it's being pressed or not
bool CInput::IsKeyDown(SDLKey sym)
{
	return (keyStatus[sym] == SDL_PRESSED);
}

bool CInput::IsButtonDown(int b)
{
	return (mouseStatus[b] == SDL_PRESSED);
}

float CInput::GetMouseX()
{
	return mousePos.x;
}

float CInput::GetMouseY()
{
	return mousePos.y;
}

//Take care of the input each frame
void CInput::HandleInput()
{
	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		if(event.type == SDL_QUIT)
			quit = true;

		//Call apropriate keyboard and mouse listener functions
		UpdateListeners(event);

		UpdateKeyStatus(event);
		UpdateMouseStatus(event);
	}
}

bool CInput::QuitStatus()
{
	return quit;
}
