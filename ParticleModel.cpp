#include "ParticleModel.h"

ParticleModel::ParticleModel(Transform* transform, float mass, float restitution):PhysicsModel(transform, mass, restitution)
{
	
}

void ParticleModel::Update(float dt)
{
	PhysicsModel::Update(dt);
}

void ParticleModel::AddRelativeForce(Vector3 force, Vector3 point)
{
	AddForce(force);
}
