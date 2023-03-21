#include "ConstantVelocityModel.h"

ConstantVelocityModel::ConstantVelocityModel(Transform* transform, float mass = 1.0f) :PhysicsModel(transform, mass)
{
}

void ConstantVelocityModel::Update(float dt)
{
	CalculateDisplacement(dt);
}
