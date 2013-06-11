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

private:
	SDL_Surface* display;
	
};

extern CRenderer Renderer;

#endif
