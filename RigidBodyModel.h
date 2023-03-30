#ifndef _PHYSICS_MODEL_H
#define _PHYSICS_MODEL_H

#include "PhysicsModel.h"

class RigidBodyModel:
	public PhysicsModel
{
public:
	RigidBodyModel(Transform* transform, float mass);
	virtual void Update(float dt);
};

#endif // !_PHYSICS_MODEL_H