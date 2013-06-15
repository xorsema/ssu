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
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <Box2D/Box2D.h>

#include <iostream>
#include <vector>

#include "types.hpp"
#include "timer.hpp"
#include "input.hpp"
#include "entity.hpp"
#include "game.hpp"
#include "renderer.hpp"

CStackableRect::CStackableRect()
{
	type = TYPE_STACKABLE;
}

CStackableRect::CStackableRect(float x, float y, float w, float h) :
	CPhysRect(x, y, w, h)
{
	type  = TYPE_STACKABLE;
}

CGroundRect::CGroundRect(float x, float y, float w, float h) :
	CPhysRect(x, y, w, h)
{
	type  = TYPE_GROUND;
}

CGroundRect::CGroundRect()
{
	type = TYPE_GROUND;
}

void CStackableRect::CreateBody()
{
	b2BodyDef	bdef;
	b2FixtureDef	fdef;
	b2PolygonShape	shape;

	float x, y;
	float w, h;

	x = parent->PixelsToMeters(pos.x);
	y = parent->PixelsToMeters(pos.y);
	w = parent->PixelsToMeters(scale.x);
	h = parent->PixelsToMeters(scale.y);

	bdef.type = b2_dynamicBody;
	bdef.position.Set(x, y);
	body = parent->CreateBody(&bdef);

	shape.SetAsBox(w / 2.0, h / 2.0);
	fdef.shape    = &shape;
	fdef.density  = 1.0f;
	fdef.friction = 0.3f;
	
	body->CreateFixture(&fdef);
}

void CGroundRect::CreateBody()
{
	b2BodyDef	bdef;
	b2PolygonShape	shape;

	float x, y;
	float w, h;

	x = parent->PixelsToMeters(pos.x);
	y = parent->PixelsToMeters(pos.y);
	w = parent->PixelsToMeters(scale.x);
	h = parent->PixelsToMeters(scale.y);

	bdef.type = b2_staticBody;
	bdef.position.Set(x, y);
	body = parent->CreateBody(&bdef);

	shape.SetAsBox(w / 2.0, h / 2.0);
	body->CreateFixture(&shape, 0.0f);
}

CGameScene::CGameScene()
{
	zoomSpeed = 0.01f;

	//Make sure this object gets passed mouse input event data
	input.SetMouseListener(this);
	
	mouseJoint = NULL;
	
	//Spawn the ground, with the width of the screen
	ground = new CGroundRect(renderer.GetWidth()/2.0, MetersToPixels(0), renderer.GetWidth(), MetersToPixels(GROUNDHEIGHT));
	AttachChild(ground);
}

//x and y are in pixels
void CGameScene::SpawnStackableRect(float x, float y)
{
	std::cout << "Spawning Stackable at X:" << PixelsToMeters(x) << " Y:" << PixelsToMeters(y) << std::endl;
	AttachChild(new CStackableRect(x, y, MetersToPixels(STACKABLERECTSIZE), MetersToPixels(STACKABLERECTSIZE)));
}

void CGameScene::MouseMove(float inx, float iny)
{
	if(mouseJoint != NULL)
	{
		float x, y;
		
		ScreenToWorld(inx, iny, &x, &y);

		mouseJoint->SetTarget(b2Vec2(PixelsToMeters(x), PixelsToMeters(renderer.GetHeight() - y)));
	}
}

void CGameScene::MouseUp(float inx, float iny, unsigned char button)
{
	if(button == SDL_BUTTON_RIGHT)
	{
		float x, y;
	
		ScreenToWorld(inx, renderer.GetHeight() - iny, &x, &y);
		
		SpawnStackableRect(x, y);
	}
	
	if(button == SDL_BUTTON_LEFT && mouseJoint != NULL)
	{
		mouseJointTarget->SetFixedRotation(false);
		world.DestroyJoint(mouseJoint);
		mouseJoint = NULL;
	}
}

void CGameScene::MouseDown(float inx, float iny, unsigned char button)
{
	//Set up the mouse joint so we can drag stuff
	if(button == SDL_BUTTON_LEFT && mouseJoint == NULL)
	{
		b2AABB aabb;
		b2Vec2 m; //mouse
		b2Vec2 hs;//half size of the mouse's aabb
		float x, y;

		ScreenToWorld(inx, iny, &x, &y);
		
		m = b2Vec2(PixelsToMeters(x), PixelsToMeters(renderer.GetHeight() - y));
		hs.Set(0.001f, 0.001f);
		aabb.upperBound = m + hs;
		aabb.lowerBound = m - hs;

		CStackableQuery query(m);
		world.QueryAABB(&query, aabb);

		if(query.fixture != NULL)
		{
			b2Body* body = query.fixture->GetBody();
			b2MouseJointDef md;
			md.bodyA = ground->GetBody();
			md.bodyB = body;
			md.target = m;
			md.maxForce = 1000.0f * body->GetMass();
			mouseJoint = (b2MouseJoint*)world.CreateJoint(&md);
			body->SetAwake(true);
			body->SetFixedRotation(true);
			mouseJointTarget = body;
		}
	}
}

void CGameScene::Update()
{
	if(input.IsKeyDown(SDLK_i))
	{
		Zoom(zoomSpeed);
	}

	if(input.IsKeyDown(SDLK_o))
	{
		Zoom(-zoomSpeed);
	}

	//Step the world
	StepWorld();
}

CStackableQuery::CStackableQuery(const b2Vec2& in)
{
	point	= in;
	fixture = NULL;
}

bool CStackableQuery::ReportFixture(b2Fixture *f)
{
	b2Body	*body;
	CEntity *entity;

	body = f->GetBody();
	entity = (CEntity*)body->GetUserData();

	if(entity->GetType() == TYPE_STACKABLE && f->TestPoint(point))
	{
		//We found a stackable's fixture, set the fixture to the one we found and return false to quit
		fixture = f;
		return false;
	}

	//Keep checking
	return true;
}
