#include <SDL/SDL.h>
#include <GL/gl.h>
#include <vector>
#include <Box2D/Box2D.h>

#include "types.hpp"
#include "entity.hpp"

CEntity::CEntity()
{
	pos.x	= pos.y = 0.0f;
	scale.x = scale.y = 0.0f;
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
	glRotatef(0.0f, 0.0f, 0.0f, rot);

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

//Set the offset, or where the "camera" is
void CScene::Offset(float x, float y)
{
	pos.x = x;
	pos.y = y;
}

//Set the zoom, or the scale for both the x and y axis
void CScene::Zoom(float z)
{
	scale.x = scale.y = z;
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
	glRotatef(0.0f, 0.0f, 0.0f, rot);

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
