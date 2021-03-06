/*
  Copyright (c) 2013 Max Rose

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.
  
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
  
   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
*/
#ifndef ENTITY_HPP
#define ENTITY_HPP

enum {
	TYPE_DEFAULT,
	TYPE_STACKABLE,
	TYPE_GROUND
};

//Base entity class
class CEntity
{
public:
	CEntity();
	~CEntity();

	void		Frame();
	virtual void	AttachChild(CEntity*);
	virtual void OnAttach(CEntity*) {}	//Called when the entity is attached to a parent
	int	GetType() { return type; }

protected:
	vec2f			pos;		
	vec2f			scale;		
	float			rot;	
	int			type;
	std::vector<CEntity*>	children;

private:
	virtual void Render() {} //These functions will be called in Frame()
	virtual void Update() {}
	virtual void EndFrame() {} //Called after children are handled
};


//Basically a pass through to separate things
class CLayer : public CEntity
{
public:
	CLayer(CEntity*);

	void AttachChild(CEntity*);
	
private:
	CEntity *parent;
};

//CScene class handles other entities and the "camera"
class CScene : public CEntity
{
public:
	void Offset(float, float);
	void Zoom(float);
	void ScreenToWorld(float, float, float*, float*);

private:
	GLdouble	pm[16]; //projection matrix
	GLdouble	mvm[16];//modelview matrix

	virtual void Render();
	virtual void Update() {}
	virtual void EndFrame();
};

//A scene with a Box2D world
class CPhysScene : public CScene
{
public:
	CPhysScene();
	b2Body *CreateBody(b2BodyDef*);
	float PixelsToMeters(float in) { return in / pixelsPerMeter; }
	float MetersToPixels(float in) { return in * pixelsPerMeter; }
	
protected:
	void StepWorld();
	b2World world;

private:
	float	pixelsPerMeter;

	virtual void Update();
};

//Base polygonal entity class
class CPolygon : public CEntity
{
public:
	CPolygon();

protected:
	void		*polygonData;	//Pointer to the data needed to render the poly
	unsigned int	 polygonDataCount;	//Amount of data units (float, int, etc)
	GLenum           polygonDataType;	//Data type used to store polygon vertex data (GL_SHORT, GL_INT, GL_FLOAT, or GL_DOUBLE)
	GLenum		 polygonType;	//OpenGL polygon type, such as GL_QUADS	

private:
	void Render();
	virtual void RenderPolygon();
	virtual void Update() {}
};

class CTexturedPolygon : public CPolygon
{
protected:
	CTexture	*texture;
	void		*texCoordData;
	GLenum		 texCoordDataType;

private:
	virtual void RenderPolygon();
};

//Polygonal entity class with a physical body, must be used with CPhysScene or a subclass
class CPhysicsPolygon : public CPolygon
{
public:
	CPhysicsPolygon();
	void OnAttach(CEntity*);
	b2Body *GetBody() { return body; }

protected:
	b2Body		*body;	//Box2D body of the poly
	CPhysScene	*parent;//The parent scene, which holds the world

private:
	void Update();
	virtual void CreateBody() {} //called after the polygon is attached, if body == NULL
};

//Rectangular version of a physical polygon
class CPhysRect : public CPhysicsPolygon
{
public:
	CPhysRect();
	CPhysRect(float, float, float, float);
};

extern float QuadData[8];

#endif
