#include <SDL/SDL.h>

#include "input.hpp"

CInput input;

CInput::CInput()
{
}

CInput::~CInput()
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

}

//Check the array for a key and return if it's being pressed or not
bool CInput::IsKeyDown(SDLKey sym)
{
	return KeyStatus[sym];
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
	}
}

bool CInput::QuitStatus()
{
	return Quit;
}
