#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#define PIXELSPERMETER 50.0f
#define GRAVITY -10.0f

class CPhysics
{
public:
	CPhysics();

	void StepWorld();
	b2PolygonShape *PolyShapeFromVerts(vec2f*, unsigned int);
	float PixelsToMeters(float);
	float MetersToPixels(float);

	b2World world;
};

extern CPhysics physics;

#endif
