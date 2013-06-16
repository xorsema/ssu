#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <GL/gl.h>
#include <Box2D/Box2D.h>

#include <iostream>
#include <string>
#include <vector>

#include "types.hpp"
#include "entity.hpp"
#include "texture.hpp"
#include "text.hpp"

CFont::CFont()
{
	font = NULL;
	Init();
}

CFont::~CFont()
{
	if(font != NULL)
		TTF_CloseFont(font);
}

void CFont::Init()
{
	if(!TTF_WasInit() && TTF_Init() == -1)
	{
		std::cout << "SDL_ttf Error:" << TTF_GetError() << std::endl;	
	}	
}

void CFont::OpenFont(const char *path, int size)
{
	if(!(font = TTF_OpenFont(path, size)))
	{
		std::cout << "TTF_OpenFont Error:" << TTF_GetError() << std::endl;	
	}
}
