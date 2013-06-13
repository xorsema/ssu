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
