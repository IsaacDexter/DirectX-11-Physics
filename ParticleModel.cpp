#include "ParticleModel.h"

ParticleModel::ParticleModel(Transform* transform, float mass = 1.0f) : PhysicsModel(transform, mass)
{
	m_maxLifetime = -1.0f;
}

ParticleModel::ParticleModel(Transform* transform, float mass, float lifetime, Vector3 perturbation) : PhysicsModel(transform, mass)
{
	m_maxLifetime = lifetime;
	AddForce(perturbation);
}

void ParticleModel::Update(float dt)
{
	m_lifetime += dt;
	if (m_lifetime > m_maxLifetime)
	{
		Expire();
	}
	//Apply forces; calculate acc; calculate vel and dis; update position; clear acc and forces

	ApplyGravity();
	ApplyDrag();
	ApplyFriction();

	CalculateAcceleration();
	CalculateVelocity(dt);
	CalculateDisplacement(dt);


	ClearForceAndAcceleration();
}

void ParticleModel::Expire()
{
	delete this;
}
