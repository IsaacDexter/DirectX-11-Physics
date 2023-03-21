#include "PhysicsModel.h"

PhysicsModel::PhysicsModel(Transform* transform, float mass = 1.0f)
{
	m_transform = transform;
	m_mass = mass;
	CalculateWeight();
}

void PhysicsModel::Update(float dt)
{
	//Apply forces; calculate acc; calculate vel and dis; update position; clear acc and forces

	ApplyGravity();

	CalculateAcceleration();
	CalculateVelocity(dt);
	CalculateDisplacement(dt);


	ClearForceAndAcceleration();
}

void PhysicsModel::CalculateAcceleration()
{
	m_acceleration += m_netforce / m_mass;	//a = f / m
}

void PhysicsModel::CalculateVelocity(float dt)
{
	m_velocity += m_acceleration * dt;					//dv = a * dt
}

void PhysicsModel::CalculateDisplacement(float dt)
{
	Vector3 position = m_transform->GetPosition();		//Get current position
	Vector3 displacement = m_velocity * dt;				//ds = v * dt
	m_transform->SetPosition(position + displacement);	//Sets current position
}

void PhysicsModel::ClearForceAndAcceleration()
{
	m_netforce = Vector3();	//Reset force after movement as it is recalculated every frame
	m_acceleration = Vector3();	//Reset acceleration as it is recalculated every frame
}

void PhysicsModel::CalculateWeight()
{
	m_weight.y = m_mass * m_gravityAcceleration;
}

void PhysicsModel::ApplyGravity()
{
	if (m_enableGravity)
	{
		AddForce(m_weight);
	}
}
