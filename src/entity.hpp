#ifndef ENTITY_HPP
#define ENTITY_HPP

//Base entity class
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

//CScene class handles other entities
class CScene : public CEntity
{
public:
	virtual void Render();
	virtual void Update();

	void Offset(float, float);
	void Zoom(float);
	void ScreenToWorld(float, float, float*, float*);
private:
	GLdouble	pm[16]; //projection matrix
	GLdouble	mvm[16];//modelview matrix
};

//Base polygonal entity class
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

//Polygonal entity class with a physical body
class CPhysicsPolygon : public CPolygon
{
public:
	void Update();

protected:
	b2Body	*CreateBody(b2BodyDef*);
	b2Body	*Body;	
};

//Rectangular version of a physical polygon
class CPhysRect : public CPhysicsPolygon
{
public:
	CPhysRect(float, float, float, float);
};

//Rectangle that is physical and is meant to be stacked
class CStackableRect : public CPhysRect
{
public:
	CStackableRect(float, float);
private:
	void SetUpBody();
};

class CGroundRect : public CPhysRect
{
public:
	CGroundRect();
};

#endif
