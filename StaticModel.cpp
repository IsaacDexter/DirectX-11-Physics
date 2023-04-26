#include "StaticModel.h"

StaticModel::StaticModel(Transform* transform, float mass = 1.0f, float restitution = 0.5f) :PhysicsModel(transform, mass, restitution)
{
}

void StaticModel::Update(float dt)
{
	
}

void StaticModel::ApplyImpulse(Vector3 impulse)
{
}
