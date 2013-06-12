#ifndef ENTITY_HPP
#define ENTITY_HPP

class CEntity
{
public:
	CEntity();
	~CEntity();

	virtual void Render() = 0;
	virtual void Update() = 0;

	std::vector<CEntity*> Children;

protected:
	vec2f	pos;		
	vec2f	scale;		
	float	rot;	

	void RenderChildren();
	void UpdateChildren();
};

class CScene : public CEntity
{
public:
	virtual void Render();
	virtual void Update();

private:

};

class CPolygon : public CEntity
{
public:
	CPolygon();

	virtual void Render();
	virtual void Update();

protected:
	void		*PolygonData;	//Pointer to the data needed to render the poly
	unsigned int	 PolygonDataCount;	//Amount of data units (float, int, etc)
	GLenum           PolygonDataType;	//Data type used to store polygon vertex data (GL_SHORT, GL_INT, GL_FLOAT, or GL_DOUBLE)
	GLenum		 PolygonType;	//OpenGL polygon type, such as GL_QUADS
	
};

#endif
