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
	
	//Spawn the ground
	ground = new CGroundRect(MetersToPixels(0), MetersToPixels(0), MetersToPixels(renderer.GetWidth()), MetersToPixels(GROUNDHEIGHT));
	AttachChild(ground);
}

void CGameScene::SpawnStackableRect(float x, float y)
{
	AttachChild(new CStackableRect(MetersToPixels(x), MetersToPixels(y), MetersToPixels(STACKABLERECTSIZE), MetersToPixels(STACKABLERECTSIZE)));
}

void CGameScene::DoControls()
{
	//Zoom in when pressing i
	if(input.IsKeyDown(SDLK_i))
	{
		Zoom(zoomSpeed);
	}
	
	//Zoom out when pressing o
	if(input.IsKeyDown(SDLK_o))
	{
		Zoom(-zoomSpeed);
	}
}

void CGameScene::Update()
{
	//Handle controls (clicking/dragging + keyboard)
	DoControls();

	//Step the world
	StepWorld();
}
