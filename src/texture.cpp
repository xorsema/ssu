#include <SDL/SDL.h>
#include <GL/gl.h>

#include <iostream>

#include "types.hpp"
#include "texture.hpp"

CTexture::CTexture()
{
	Init();
}

CTexture::CTexture(SDL_Surface *surface)
{
	Init();
	FromSurface(surface);
}

CTexture::~CTexture()
{
	if(pixels != NULL)
		delete pixels;

	if(name != 0)
		glDeleteTextures(1, &name);
}

void CTexture::Init()
{
	pixels		 = NULL;
	bytesPerPixel	 = 0;
	width		 = 0;
	height		 = 0;
	name		 = 0;
	glFormat	 = GL_RGB;
	glInternalFormat = GL_RGB;
	glType		 = GL_UNSIGNED_BYTE;
}

//Upload the texture to the GPU and get a name
void CTexture::Load()
{
	glGenTextures(1, &name);
	glBindTexture(GL_TEXTURE_2D, name);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, glFormat, glType, pixels);
}

//Turn an SDL surface into an OpenGL compatible texture
void CTexture::FromSurface(SDL_Surface *surface)
{
	SDL_PixelFormat *fmt;
	unsigned char	*result;

	fmt    = surface->format;//shortcut
	width  = surface->w;//get the width and height from the surface
	height = surface->h;
	
	SDL_LockSurface(surface);

	//If it's done by a palette, we expand it into something OpenGL can handle better
	if(fmt->BytesPerPixel == 1)
	{
		color3 *data;
		data = new color3[width * height];//we'll make it into a 3 component texture
		
		for(int i = 0; i < width * height; i++)
		{
			unsigned char	 index;
			unsigned char	*p;
			SDL_Color	*color;

			p     = (unsigned char*)surface->pixels;
			index = p[i];	//the index into the colors array/palette
			color = &fmt->palette->colors[index];	//Get the color with the index
			
			//Copy over the values into our newly allocated array
			data[i][0] = color->r;
			data[i][1] = color->g;
			data[i][2] = color->b;
		}

		bytesPerPixel	 = 3;	//We made it into a color3 array
		result		 = (unsigned char*)data;
		glFormat	 = GL_RGB;
		glInternalFormat = GL_RGB;
		glType		 = GL_UNSIGNED_BYTE;
	}

	//If its some combination of red, green and blue components, make sure its RGB and not BGR or something
	if(fmt->BytesPerPixel == 3)
	{
		color3 *data;
		data = new color3[width * height];

		for(int i = 0; i < width * height; i++)
		{
			unsigned int	 pixel, temp;
			unsigned int	*p;
			
			p	   = (unsigned int*)surface->pixels;
			pixel	   = p[i];

			temp	   = pixel & fmt->Rmask;
			temp	   = temp >> fmt->Rshift;
			temp	   = temp << fmt->Rloss;
			data[i][0] = (unsigned char)temp;//red

			temp	   = pixel & fmt->Gmask;
			temp	   = temp >> fmt->Gshift;
			temp	   = temp << fmt->Gloss;
			data[i][1] = (unsigned char)temp;//green

			temp	   = pixel & fmt->Bmask;
			temp	   = temp >> fmt->Bshift;
			temp	   = temp << fmt->Bloss;
			data[i][2] = (unsigned char)temp;//blue
		}

		bytesPerPixel	 = 3;
		result		 = (unsigned char*)data;
		glFormat	 = GL_RGB;
		glInternalFormat = GL_RGB;
		glType		 = GL_UNSIGNED_BYTE;
	}

	//Same as above, but with an alpha channel
	if(fmt->BytesPerPixel == 4)
	{
		color4 *data;
		data = new color4[width * height];

		for(int i = 0; i < width * height; i++)
		{
			unsigned int pixel, temp;
			unsigned int *p;
			
			p	   = (unsigned int*)surface->pixels;
			pixel	   = p[i];

			temp	   = pixel & fmt->Rmask;
			temp	   = temp >> fmt->Rshift;
			temp	   = temp << fmt->Rloss;
			data[i][0] = (unsigned char)temp;//red

			temp	   = pixel & fmt->Gmask;
			temp	   = temp >> fmt->Gshift;
			temp	   = temp << fmt->Gloss;
			data[i][1] = (unsigned char)temp;//green

			temp	   = pixel & fmt->Bmask;
			temp	   = temp >> fmt->Bshift;
			temp	   = temp << fmt->Bloss;
			data[i][2] = (unsigned char)temp;//blue

			temp	   = pixel & fmt->Amask;
			temp	   = temp >> fmt->Ashift;
			temp	   = temp << fmt->Aloss;
			data[i][3] = (unsigned char)temp;//alpha
		}

		bytesPerPixel	 = 4;
		result		 = (unsigned char*)data;
		glFormat	 = GL_RGBA;
		glInternalFormat = GL_RGBA;
		glType		 = GL_UNSIGNED_BYTE;
	}

	pixels = result;//We're finished, so grab the result

	SDL_UnlockSurface(surface);//Done messing with the surface's pointers

	/*SDL_Surface *temp;
	unsigned int size;

	temp = SDL_CreateRGBSurface(SDL_SRCALPHA | SDL_SWSURFACE, surface->w, surface->h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	SDL_BlitSurface(surface, 0, temp, 0);
	
	size = surface->w * surface->h * surface->format->BytesPerPixel;

	pixels = new unsigned char[size];
	SDL_LockSurface(temp);
	memcpy(pixels, temp->pixels, size);
	SDL_UnlockSurface(temp);

	glFormat	 = GL_RGBA;
	glInternalFormat = GL_RGBA;
	glType		 = GL_UNSIGNED_BYTE;
	bytesPerPixel	 = temp->format->BytesPerPixel;
	width		 = temp->w;
	height		 = temp->h;*/
}
