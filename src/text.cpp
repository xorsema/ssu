#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <GL/gl.h>
#include <box2d/box2d.h>

#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#include "types.hpp"
#include "texture.hpp"
#include "entity.hpp"
#include "text.hpp"

float QuadTextureData[8] = {
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f
};

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

//Open a font by creating a font object with SDL_ttf
void CFont::OpenFont(const char *path, unsigned int size)
{
	if(!(font = TTF_OpenFont(path, size)))
	{
		std::cout << "TTF_OpenFont Error:" << TTF_GetError() << std::endl;	
	}
	else 
	{
		fontPath = std::string(path);
	}
}

//Reopen the font in a different size
void CFont::SetSize(unsigned int size)
{
	if(font == NULL)
		return;//Just return if we haven't opened a font yet

	TTF_CloseFont(font);
	OpenFont(fontPath.c_str(), size);
}

void CText::Init()
{
	polygonData	 = QuadData;
	polygonDataCount = sizeof(QuadData) / sizeof(float);
	polygonDataType	 = GL_FLOAT;
	polygonType	 = GL_QUADS;
	texCoordData	 = QuadTextureData;
       	texCoordDataType = GL_FLOAT;
	font		 = NULL;
	texture		 = NULL;
	memset(textColor, 255, 3);
}

CText::CText()
{
	Init();
}

CText::CText(CFont *f)
{
	Init();
	font = f;
}

CText::CText(const char *text, CFont *f)
{
	Init();
	font = f;
	SetText(text);
}

void CText::SetSize(unsigned int size)
{
	if(size != textSize)
	{
		textSize = size;//Set the new size
		font->SetSize(textSize);//Set it in the font
		TextToTexture(textString.c_str());//Re-render it
	}
}

void CText::SetPosition(float x, float y)
{
	pos.x = x;
	pos.y = y;
}

//If the text is the same, do nothing, otherwise set the text to the new string
void CText::SetText(const char *text)
{
	if(std::string(text) == textString)
		return;

	TextToTexture(text);
}

void CText::SetColor(unsigned char r, unsigned char g, unsigned char b)
{
	textColor[0] = r;
	textColor[1] = g;
	textColor[2] = b;
}

//Replaces the current texture (freeing it) with a new one
void CText::TextToTexture(const char *text)
{
	SDL_Surface	*surface;
	SDL_Color	color = { textColor[0], textColor[1], textColor[2] };
	CTexture	*newTexture;

	textString = std::string(text);//Save the string
	
	surface	   = TTF_RenderText_Blended(font->GetFont(), textString.c_str(), color);	//Render it to a surface
	textWidth  = surface->w;//Get the width and height of the rendered string
	textHeight = surface->h;
	scale.x	   = textWidth;
	scale.y	   = textHeight;

	newTexture = new CTexture(surface);//Turn it into a texture object

	if(texture != NULL)
		delete texture;//Get rid of the old one if it exists

	texture = newTexture;//Set the member to the new one
	texture->Load();
}
