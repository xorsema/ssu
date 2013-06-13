#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <Box2D/Box2D.h>

#include "types.hpp"
#include "physics.hpp"
#include "entity.hpp"
#include "renderer.hpp"

float QuadData[] =
	{
		-.5f, -.5f,
		.5f, -.5f,
		.5f, .5f,
		-.5f, .5f
	};


CEntity::CEntity()
{
	pos.x	= pos.y = 0.0f;
	scale.x = scale.y = 1.0f;
	rot	= 0.0f;
}

CEntity::~CEntity()
{
	//Loop through the children of the entity and free them
	for(std::vector<CEntity*>::iterator it = Children.begin(); it != Children.end(); it++)
	{
		delete *it;
	}
}

//Render the entity's children and the children's children
void CEntity::RenderChildren()
{
	for(std::vector<CEntity*>::iterator it = Children.begin(); it != Children.end(); it++)
	{
		(*it)->Render();
		(*it)->RenderChildren();
	}
}

//Do the same but update them
void CEntity::UpdateChildren()
{
	for(std::vector<CEntity*>::iterator it = Children.begin(); it != Children.end(); it++)
	{
		(*it)->Update();
		(*it)->UpdateChildren();
	}
}

//Set up the scene
void CScene::Render()
{
	//Move the projection matrix
	glTranslatef(pos.x, pos.y, 0.0f);
	
	//Scale it, effectively zooming
	glScalef(scale.x, scale.y, 0.0f);

	//Rotate it
	glRotatef(rot, 0.0f, 0.0f, 0.0f);

	//Switch to the modelview matrix
	glMatrixMode(GL_MODELVIEW);

	//Reset its position
	glLoadIdentity();

	RenderChildren();
}

void CScene::Update()
{
	UpdateChildren();
}

//Change the offset, or where the "camera" is
void CScene::Offset(float x, float y)
{
	pos.x += x;
	pos.y += y;
}

//Set the zoom, or the scale for both the x and y axis
void CScene::Zoom(float z)
{
	scale.x += z;
	scale.y += z;
}

CPolygon::CPolygon()
{
}

//Render our polygon
void CPolygon::Render()
{
	//Save the matrix
	glPushMatrix();
	
	//Set the position
	glTranslatef(pos.x, pos.y, 0.0f);

	//Set the scale (size)
	glScalef(scale.x, scale.y, 0.0f);

	//Set the rotation (rotate about the poly's origin)
	glRotatef(rot, 0.0f, 0.0f, 1.0f);

	//Enable the vertex coord array
	glEnableClientState(GL_VERTEX_ARRAY);

	//Specify our vertex data
	glVertexPointer(2, PolygonDataType, 0, PolygonData);

	//Draw our polygon
	glDrawArrays(PolygonType, 0, PolygonDataCount / 2);

	//Disable this because we're done
	glDisableClientState(GL_VERTEX_ARRAY);
	
	//Restore the saved matrix
	glPopMatrix();
}

void CPolygon::Update()
{
}

//Use the body's coordinates to position our polygon
void CPhysicsPolygon::Update()
{
	pos.x = physics.MetersToPixels(Body->GetPosition().x);
	pos.y = physics.MetersToPixels(Body->GetPosition().y);

	//We have to convert from radians to degrees
	rot = Body->GetAngle() * (180/b2_pi);
}

b2Body *CPhysicsPolygon::CreateBody(b2BodyDef *bdef)
{
	return physics.world.CreateBody(bdef);
}

CPhysRect::CPhysRect(float x, float y, float w, float h)
{
	PolygonData	 = QuadData;
	PolygonDataCount = sizeof(QuadData);
	PolygonDataType	 = GL_FLOAT;
	PolygonType	 = GL_QUADS;
	scale.x		 = physics.MetersToPixels(w);
	scale.y		 = physics.MetersToPixels(h);
	pos.x		 = physics.MetersToPixels(x);
	pos.y		 = physics.MetersToPixels(y);
}

CStackableRect::CStackableRect(float x, float y) :
	CPhysRect(x, y, 1, 1)
{
	SetUpBody();
}

void CStackableRect::SetUpBody()
{
	b2BodyDef	bdef;
	b2FixtureDef	fdef;
	b2PolygonShape	shape;

	float x, y;
	float w, h;

	x = physics.PixelsToMeters(pos.x);
	y = physics.PixelsToMeters(pos.y);
	w = physics.PixelsToMeters(scale.x);
	h = physics.PixelsToMeters(scale.y);

	bdef.type = b2_dynamicBody;
	bdef.position.Set(x, y);
	Body = CreateBody(&bdef);

	shape.SetAsBox(w / 2.0, h / 2.0);
	fdef.shape    = &shape;
	fdef.density  = 1.0f;
	fdef.friction = 0.3f;
	
	Body->CreateFixture(&fdef);
}

CGroundRect::CGroundRect() :
	CPhysRect(10, 3, 20, 0.1)
{
	b2BodyDef	bdef;
	b2PolygonShape	shape;

	float x, y;
	float w, h;

	x = physics.PixelsToMeters(pos.x);
	y = physics.PixelsToMeters(pos.y);
	w = physics.PixelsToMeters(scale.x);
	h = physics.PixelsToMeters(scale.y);

	bdef.type = b2_staticBody;
	bdef.position.Set(x, y);
	Body = CreateBody(&bdef);

	shape.SetAsBox(w / 2.0, h / 2.0);
	Body->CreateFixture(&shape, 0.0f);
}
