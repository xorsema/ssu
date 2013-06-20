#ifndef TEXTURE_HPP
#define TEXTURE_HPP

class CTexture
{
public:
	CTexture();
	CTexture(SDL_Surface*);
	~CTexture();

	void		FromSurface(SDL_Surface*);
	virtual void	Load();

	unsigned int GetName() { return name; }
	GLenum GetFormat() { return glFormat; }
	GLenum GetInternalFormat() { return glInternalFormat; }
	GLenum GetType() { return glType; }
	unsigned int GetWidth() { return width; }
	unsigned int GetHeight() { return height; }
	
protected:
	virtual void Init();

	unsigned char	*pixels;
	unsigned int	 bytesPerPixel;
	unsigned int	 width, height;
	GLenum		 glFormat;
	GLenum		 glInternalFormat;
	GLenum		 glType;
	unsigned int	 name;
};

#endif
