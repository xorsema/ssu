#ifndef TEXT_HPP
#define TEXT_HPP

//Simple class just to handle fonts with SDL_ttf
class CFont
{
public:
	CFont();
	~CFont();
	void Init();
	void OpenFont(const char*, unsigned int);
	void SetSize(unsigned int);
	TTF_Font *GetFont() { return font; }

private:
	TTF_Font	*font;
	std::string	 fontPath;
};

//Actual text drawing class
class CText : public CPolygon
{
public:
	CText();
	CText(const char*, CFont*);

	void SetSize(unsigned int);
	void SetText(const char*);

private:
	void TextToTexture(const char *text);

	CFont		*font;
	CTexture	*texture;
	std::string	 textString;
	unsigned int	 textWidth;	//Width and height (in pixels)
	unsigned int	 textHeight;
	unsigned int	 textSize;	//Size in points
	color3		 textColor;
};

#endif
