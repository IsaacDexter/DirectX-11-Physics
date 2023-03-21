#include "ParticleModel.h"

ParticleModel::ParticleModel(Transform* transform):PhysicsModel(transform)
{
	
}

void ParticleModel::Update(float dt)
{
	Vector3 position = m_transform->GetPosition();		//Get current position
	m_velocity += m_acceleration * dt;					//dv = a * dt
	Vector3 displacement = m_velocity * dt;				//ds = v * dt
	m_transform->SetPosition(position + displacement);	//Sets current position
}
