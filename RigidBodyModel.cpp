#include "RigidBodyModel.h"

RigidBodyModel::RigidBodyModel(Transform* transform, float mass = 1.0f) :PhysicsModel(transform, mass)
{
}

void RigidBodyModel::Update(float dt)
{
	PhysicsModel::Update(dt);
}
