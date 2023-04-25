#include "ParticleModel.h"

ParticleModel::ParticleModel(Transform* transform, float mass, float restitution):PhysicsModel(transform, mass, restitution)
{
	
}

void ParticleModel::Update(float dt)
{
	PhysicsModel::Update(dt);
}
