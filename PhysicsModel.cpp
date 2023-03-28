#include "PhysicsModel.h"

PhysicsModel::PhysicsModel(Transform* transform, float mass = 1.0f)
{
	m_transform = transform;
	m_mass = mass;
	CalculateWeight();

	m_referenceArea *= m_transform->GetScale();
}

void PhysicsModel::Update(float dt)
{
	//Apply forces; calculate acc; calculate vel and dis; update position; clear acc and forces

	ApplyGravity();
	ApplyDrag();
	ApplyFriction();

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

void PhysicsModel::CalculateDragCoefficient()
{
	/// <summary>flow speed of the object relative to the fluid</summary>
	float flowSpeedSq = m_velocity.MagnitudeSq();
	if (!(m_dragForceMagnitude == 0.0f || flowSpeedSq == 0.0f))	//If the object is not moving, or there is no fluid density, don;t recalculate the drag coefficient.
	{
		m_dragCoefficient = (2 * m_dragForceMagnitude) / (m_fluidDensity * flowSpeedSq * m_referenceArea.x);	//cd = 2Fd / rho * u^2 * A
	}
}

bool PhysicsModel::IsGrounded()
{
	bool grounded = m_transform->GetPosition().y <= 1.0f;
	if (grounded)
	{
		m_netforce.y = fmax(m_netforce.y, 0.0f);
		m_acceleration.y = fmax(m_acceleration.y, 0.0f);
		m_velocity.y = fmax(m_velocity.y, 0.0f);
	}
	return grounded;
}

void PhysicsModel::ApplyGravity()
{
	if (m_enableGravity && !IsGrounded())	//If gravity is being applied, apply weight downwards
	{
		AddForce(m_weight);	
	}
}

void PhysicsModel::ApplyDrag()
{
	if (m_fluidDensity > 0.0f && m_velocity.MagnitudeSq() > 0.0f)	//if we are in a fluid that applies drag (i.e. this is set to 0 for objects that feel no air resistance.)
	{
		m_dragForceMagnitude = 0.5f * m_fluidDensity * m_dragCoefficient * m_dragCoefficient * m_referenceArea.x * m_velocity.MagnitudeSq();	//|Fd| = 0.5 * rho * Cd * A * |V|^2
		Vector3 dragForce = m_velocity.Normalized() * -1;	//Find the direction against movement
		dragForce *= m_dragForceMagnitude;	//multiply this direction by the dragnitude (drag magnitude lol)
		AddForce(dragForce);
	}
}

void PhysicsModel::ApplyFriction()
{
	if (IsGrounded())
	{
		//float normalForceMagnitude = abs(fmin(m_netforce.y, 0.0f));	//Get the downwards net force, and then get its normal by absoluting it. 
		float normalForceMagnitude = fabs(fmin(m_weight.y, 0.0f));	//until collision is done, simply use the weight for now.
		float frictionForceMagnitude = STEEL_STEEL_FRICTION_COEFFICIENT * normalForceMagnitude;	//Get the strength of the frictional force
		Vector3 frictionForce = m_velocity.Normalized() * -1.0f;	//Get the direction against the movement
		frictionForce *= frictionForceMagnitude;	//Apply the strength of the frictional force
		AddForce(frictionForce);
	}
}
