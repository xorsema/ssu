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
#ifndef GAME_HPP
#define GAME_HPP

#define STACKABLERECTSIZE 1.0f
#define GROUNDHEIGHT 1.0f

//Rectangle that is physical and is meant to be stacked
class CStackableRect : public CPhysRect
{
public:
	CStackableRect();
	CStackableRect(float, float, float, float);

private:
	void CreateBody();
};

//Rectangle used as the "ground"
class CGroundRect : public CPhysRect
{
public:
	CGroundRect();
	CGroundRect(float, float, float, float);

private:
	void CreateBody();
};

//AABB query callback that determines if any stackables are at a point (if any)
class CStackableQuery : public b2QueryCallback
{
public:
	CStackableQuery(const b2Vec2&);

	bool ReportFixture(b2Fixture*);

	b2Fixture	*fixture;
	b2Vec2		 point;
};

//The scene for the main game
class CGameScene : public CPhysScene, public CMouseListener
{
public:
	CGameScene();

	void MouseUp(float, float, unsigned char);
	void MouseDown(float, float, unsigned char);
	void MouseMove(float, float);

private:
	virtual void	Update();
	void		SpawnStackableRect(float, float);

	float		 zoomSpeed;
	CGroundRect	*ground;
	b2MouseJoint	*mouseJoint;
	b2Body		*mouseJointTarget;
};

#endif
