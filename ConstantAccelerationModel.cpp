#include "ConstantAccelerationModel.h"

ConstantAccelerationModel::ConstantAccelerationModel(Transform* transform, float mass = 1.0f) :PhysicsModel(transform, mass)
{

}

void ConstantAccelerationModel::Update(float dt)
{
	CalculateVelocity(dt);
	CalculateDisplacement(dt);
}