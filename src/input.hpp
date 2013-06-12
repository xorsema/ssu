#ifndef INPUT_HPP
#define INPUT_HPP

class CInput
{
public:
	CInput();
	~CInput();
	
	bool IsKeyDown(SDLKey);
	void HandleInput();

private:
	void UpdateKeyStatus(SDL_Event);	
	
	bool KeyStatus[SDLK_LAST - SDLK_FIRST]; // true == pressed | false == released
	bool Quit; //whether the quit event has been received
};

extern CInput input;

#endif
