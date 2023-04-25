#include "ConstantVelocityModel.h"

ConstantVelocityModel::ConstantVelocityModel(Transform* transform, float mass, float restitution) :PhysicsModel(transform, mass, restitution)
{
}

void ConstantVelocityModel::Update(float dt)
{
	CalculateDisplacement(dt);
}
