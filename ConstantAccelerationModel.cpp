#include "ConstantAccelerationModel.h"

ConstantAccelerationModel::ConstantAccelerationModel(Transform* transform, float mass, float restitution) :PhysicsModel(transform, mass, restitution)
{

}

void ConstantAccelerationModel::Update(float dt)
{
	CalculateVelocity(dt);
	CalculateDisplacement(dt);
}