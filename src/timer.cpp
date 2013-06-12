#include <SDL/SDL.h>

#include "timer.hpp"

//Create a timer for t milliseconds
CTimer::CTimer(float t)
{
	Amount = t;
}

//Start the timer by setting the start time
void CTimer::Start()
{
	StartTime = SDL_GetTicks();
}

//Reset the timer
void CTimer::Reset()
{
	StartTime = SDL_GetTicks();
}

//Reset the timer and set a different wait time
void CTimer::Reset(float t)
{
	StartTime = SDL_GetTicks();
	Amount = t;
}

//If the timer has elapsed, reset it, and return true, otherwise return false
bool CTimer::Elapsed()
{
	if(SDL_GetTicks() - StartTime >= Amount)
	{
		StartTime = SDL_GetTicks();
		return true;
	}

	return false;
}
