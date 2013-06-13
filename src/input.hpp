#ifndef INPUT_HPP
#define INPUT_HPP

class CInput
{
public:
	CInput();
	~CInput();
	
	bool IsKeyDown(SDLKey);
	bool IsButtonDown(int);
	float GetMouseX();
	float GetMouseY();
	bool QuitStatus();
	void HandleInput();

private:
	void UpdateKeyStatus(SDL_Event&);	
	void UpdateMouseStatus(SDL_Event&);

	bool	KeyStatus[SDLK_LAST - SDLK_FIRST];	// true == pressed | false == released
	bool	MouseStatus[SDL_BUTTON_X2 + 1];
	vec2f	MousePos;	//The mouse's position this frame
	vec2f	MouseOffset;	//The relative movement of the mouse this frame, if any
	bool	Quit;		//whether the quit event has been received
};

extern CInput input;

#endif
