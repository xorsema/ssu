#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#define PIXELSPERMETER 50
#define GRAVITY -10.0f

class CPhysics
{
public:
	CPhysics();

	void StepWorld();

	b2World world;

private:
	b2Vec2 gravity;
};

extern CPhysics physics;

#endif
