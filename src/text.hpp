#ifndef TEXT_HPP
#define TEXT_HPP

//Simple struct just to handle fonts with SDL_ttf
struct CFont
{
	CFont();
	~CFont();
	void Init();
	void OpenFont(const char*, int);
	TTF_Font *font;
};

//Actual text drawing class
class CText : public CPolygon
{
public:
	CText();
	CText(const char*, CFont*);

	void SetSize();
	void SetText(const char*);

private:
	CFont		*font;
	std::string	 textString;
	unsigned int	 textWidth;	//Width and height (in pixels)
	unsigned int	 textHeight;
};

#endif
