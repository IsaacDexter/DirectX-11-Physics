#include "ConstantVelocityModel.h"

ConstantVelocityModel::ConstantVelocityModel(Transform* transform, float mass = 1.0f, float restitution = 0.5f) :PhysicsModel(transform, mass, restitution)
{
}

void ConstantVelocityModel::Update(float dt)
{
	CalculateDisplacement(dt);
}
