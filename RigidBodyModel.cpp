#include "RigidBodyModel.h"

RigidBodyModel::RigidBodyModel(Transform* transform, float mass, float restitution) :PhysicsModel(transform, mass, restitution)
{
}

void RigidBodyModel::Update(float dt)
{
	PhysicsModel::Update(dt);
}
