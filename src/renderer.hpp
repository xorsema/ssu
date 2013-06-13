#ifndef RENDERER_HPP
#define RENDERER_HPP

class CRenderer
{
public:
	CRenderer();
	~CRenderer();
	
	bool Init(int, int);
	void GLInit(void);
	bool SetDisplaySize(int, int);
	void BeginFrame(void);
	void EndFrame(void);

	float GetWidth();
	float GetHeight();

private:
	SDL_Surface* display;
	
};

extern CRenderer renderer;

#endif
