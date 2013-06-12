#include <Box2D/Box2D.h>

#include "timer.hpp"
#include "physics.hpp"

CPhysics physics;

CPhysics::CPhysics() :
	gravity(0.0f, GRAVITY), world(gravity)
{
}

void CPhysics::StepWorld()
{
	world.Step(1.0/60.0, 6, 2);
}

