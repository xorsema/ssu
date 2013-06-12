#ifndef ENTITY_HPP
#define ENTITY_HPP

class CEntity
{
public:
	CEntity();

	virtual void Render() = 0;
	virtual void Update() = 0;

protected:
	vec2f	pos;		
	vec2f	scale;		
	vec2f	rot;		
	color4	color;           
};

class CPolygon : public CEntity
{
public:
	CPolygon();
	~CPolygon();

	virtual void Render();
	virtual void Update();

protected:
	void		*PolygonData;	//Pointer to the data needed to render the poly
	unsigned int	 PolygonDataBytes;	//Size of data in bytes
	GLenum		 PolygonType; //OpenGL polygon type, such as GL_QUADS
	
};

#endif
