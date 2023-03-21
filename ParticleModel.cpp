#include "ParticleModel.h"

ParticleModel::ParticleModel(Transform* transform, float mass = 1.0f):PhysicsModel(transform, mass)
{
	
}

void ParticleModel::Update(float dt)
{
	ApplyGravity();
	ApplyDrag();

	CalculateAcceleration();
	CalculateVelocity(dt);
	CalculateDisplacement(dt);

	ClearForceAndAcceleration();
}
