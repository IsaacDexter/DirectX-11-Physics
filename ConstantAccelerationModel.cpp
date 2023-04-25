#include "ConstantAccelerationModel.h"

ConstantAccelerationModel::ConstantAccelerationModel(Transform* transform, float mass = 1.0f, float restitution = 0.5f) :PhysicsModel(transform, mass, restitution)
{

}

void ConstantAccelerationModel::Update(float dt)
{
	CalculateVelocity(dt);
	CalculateDisplacement(dt);
}