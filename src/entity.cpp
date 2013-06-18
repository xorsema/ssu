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
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <iostream>
#include <Box2D/Box2D.h>

#include "types.hpp"
#include "timer.hpp"
#include "entity.hpp"
#include "renderer.hpp"

float QuadData[8] = {
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

	type = TYPE_DEFAULT;
}

CEntity::~CEntity()
{
	//Loop through the children of the entity and free them
	for(std::vector<CEntity*>::iterator it = children.begin(); it != children.end(); it++)
	{
		delete *it;
	}
}

//This should be called each from, subclasses will override the Update and Render functions to change behavior
void CEntity::Frame()
{
	Update();
	Render();
	
	for(std::vector<CEntity*>::iterator it = children.begin(); it != children.end(); it++)
	{
		(*it)->Update();
		(*it)->Render();
	}
}

//Attach a child, children will be updated and rendered along with the parent
void CEntity::AttachChild(CEntity* e)
{
	children.push_back(e);
	e->OnAttach(this);
}

//Set up the scene
void CScene::Render()
{
	//Move the projection matrix to the correct location + half width/height so it will be changed from the center
	glTranslatef(pos.x + (renderer.GetWidth() / 2.0), pos.y + (renderer.GetHeight() / 2.0), 0.0f);
	
	//Scale it, effectively zooming
	glScalef(scale.x, scale.y, 1.0f);

	//Rotate it
	glRotatef(rot, 0.0f, 0.0f, 0.0f);

	//Move it back to the "correct" position
	glTranslatef(-renderer.GetWidth() / 2.0, -renderer.GetHeight() / 2.0, 0.0f);

	//Save the projection matrix for gluUnProject
	glGetDoublev(GL_PROJECTION_MATRIX, pm);

	//Switch to the modelview matrix
	glMatrixMode(GL_MODELVIEW);

	//Reset its position
	glLoadIdentity();

	//Save the modelview matrix for gluUnProject
	glGetDoublev(GL_MODELVIEW_MATRIX, mvm);
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

//Use gluUnProject to get the world coordinates
void CScene::ScreenToWorld(float inx, float iny, float *outx, float *outy)
{
	GLdouble	dox, doy, doz;
	GLint		vp[4];	//viewport

	glGetIntegerv(GL_VIEWPORT, vp);
	
	gluUnProject(inx, iny, 0.0f, mvm, pm, vp, &dox, &doy, &doz);

	*outx = (float)dox;
	*outy = (float)doy;
}

CPhysScene::CPhysScene() :
	world(b2Vec2(0.0f, -10.0f))
{
	pixelsPerMeter = 50.0f;
}

void CPhysScene::Update()
{
	StepWorld();
}

void CPhysScene::StepWorld()
{
	world.Step(1.0f / 60.0f, 6, 2);
}

b2Body *CPhysScene::CreateBody(b2BodyDef *bdef)
{
	return world.CreateBody(bdef);
}

CPolygon::CPolygon()
{
	polygonData	 = NULL;
	polygonDataCount = 0;
	polygonDataType	 = 0;
	polygonType	 = 0;
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
	glVertexPointer(2, polygonDataType, 0, polygonData);

	//Draw our polygon
	glDrawArrays(polygonType, 0, polygonDataCount / 2);

	//Disable this because we're done
	glDisableClientState(GL_VERTEX_ARRAY);
	
	//Restore the saved matrix
	glPopMatrix();
}

CPhysicsPolygon::CPhysicsPolygon()
{
	body = NULL;
}

void CPhysicsPolygon::OnAttach(CEntity* e)
{
	parent = static_cast<CPhysScene*>(e);

	if(body == NULL)
	{
		CreateBody();
		body->SetUserData(this);
	}
}

//Use the body's coordinates to position our polygon
void CPhysicsPolygon::Update()
{
	pos.x = parent->MetersToPixels(body->GetPosition().x);
	pos.y = parent->MetersToPixels(body->GetPosition().y);

	//We have to convert from radians to degrees
	rot = body->GetAngle() * (180/b2_pi);
}

CPhysRect::CPhysRect()
{
	scale.x = scale.y = 1;
	pos.x = pos.y = 0;
}

//Specify x/y and w/h in pixels
CPhysRect::CPhysRect(float x, float y, float w, float h)
{
	polygonData	 = QuadData;
	polygonDataCount = sizeof(QuadData);
	polygonDataType	 = GL_FLOAT;
	polygonType	 = GL_QUADS;
	scale.x		 = w;
	scale.y		 = h;
	pos.x		 = x;
	pos.y		 = y;
}
