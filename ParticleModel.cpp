#include "ParticleModel.h"

ParticleModel::ParticleModel(Transform* transform, float mass = 1.0f, float restitution = 0.5f):PhysicsModel(transform, mass, restitution)
{
	
}

void ParticleModel::Update(float dt)
{
	PhysicsModel::Update(dt);
}
