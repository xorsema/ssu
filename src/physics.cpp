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
#include <Box2D/Box2D.h>

#include "types.hpp"
#include "timer.hpp"
#include "physics.hpp"

CPhysics physics;

CPhysics::CPhysics() :
	world(b2Vec2(0.0f, GRAVITY))
{
}

void CPhysics::StepWorld()
{
	world.Step(1.0f / 60.0f, 6, 2);
}

float CPhysics::PixelsToMeters(float in)
{
	return in / PIXELSPERMETER;
}

float CPhysics::MetersToPixels(float in)
{
	return in * PIXELSPERMETER;
}

//Takes count vertices and turns them into a b2PolygonShape
b2PolygonShape *PolyShapeFromVerts(vec2f* verts, unsigned int count)
{
	b2PolygonShape	*result;
	b2Vec2		*vecs;
	vec2f		*in;
	
	result = new b2PolygonShape;
	vecs   = new b2Vec2[count];

	for(int i = 0; i < count; i++)
	{
		in = &verts[i];
		vecs[i].Set(in->x, in->y);
	}

	result->Set(vecs, count);

	delete vecs;

	return result;
}
