#include "RigidBodyModel.h"

RigidBodyModel::RigidBodyModel(Transform* transform, float mass = 1.0f, float restitution = 0.5f) :PhysicsModel(transform, mass, restitution)
{
}

void RigidBodyModel::Update(float dt)
{
	PhysicsModel::Update(dt);
}
