#include "PhysicsModel.h"

#include <string>

PhysicsModel::PhysicsModel(Transform* transform, float mass, float restitution)
{
	m_transform = transform;
	m_collider = nullptr;
	m_mass = mass;
	m_restitution = restitution;
	CalculateWeight();

	m_referenceArea *= m_transform->GetScale();

	m_velocity = Vector3();
}

PhysicsModel::~PhysicsModel()
{
	delete m_collider;
	m_collider = nullptr;
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
	m_acceleration.Truncate();	//Stop the acceleration from wasting processing on insignificant values
}

void PhysicsModel::CalculateVelocity(float dt)
{
	m_velocity += m_acceleration * dt;					//dv = a * dt
	m_velocity.Truncate();	//stops the velocity from wasting processing on insignificant values
}

void PhysicsModel::CalculateDisplacement(float dt)
{
	Vector3 displacement = m_velocity * dt;				//ds = v * dt
	Displace(displacement);	//Sets current position
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



void PhysicsModel::ApplyGravity()
{
	if (m_enableGravity)	//If gravity is being applied, apply weight downwards
	{
		AddForce(m_weight);	
	}
}

void PhysicsModel::ApplyDrag()
{
	if (m_fluidDensity > 0.0f && m_velocity.MagnitudeSq() > 0.0f)	//if we are in a fluid that applies drag (i.e. this is set to 0 for objects that feel no air resistance.)
	{
		Vector3 direction = m_velocity.Normalized();
		//Calculate how much area is being moved against it, remembering that area is a vector3, simply get the dot product of the direction and the areas to get the combined area for the direction being moved in
		float referenceArea = abs(direction) * m_referenceArea;

		m_dragForceMagnitude = 0.5f * m_fluidDensity * m_dragCoefficient * referenceArea * m_velocity.MagnitudeSq();	//|Fd| = 0.5 * rho * Cd * A * |V|^2
		Vector3 dragForce = direction * -1;	//Find the direction against movement
		dragForce *= m_dragForceMagnitude;	//multiply this direction by the dragnitude (drag magnitude lol)
		AddForce(dragForce);
	}
}

void PhysicsModel::ApplyFriction()
{
	if (m_velocity.MagnitudeSq() > 0.0f) //if we are in contact with a surface, and are moving, apply dynamic friction.
	{
		//float normalForceMagnitude = abs(fmin(m_netforce.y, 0.0f));	//Get the downwards net force, and then get its normal by absoluting it. 
		float normalForceMagnitude = fabs(fmin(m_weight.y, 0.0f));	//until collision is done, simply use the weight for now.
		float frictionForceMagnitude = fmin(STEEL_STEEL_DYNAMIC_FRICTION_COEFFICIENT * normalForceMagnitude, m_velocity.Magnitude());	//Get the strength of the frictional force
		Vector3 frictionForce = m_velocity.Normalized() * -1.0f;	//Get the direction against the movement
		frictionForce *= frictionForceMagnitude;	//Apply the strength of the frictional force
		AddForce(frictionForce);
	}
}

void PhysicsModel::ApplyImpulse(Vector3 impulse)
{
	m_velocity += impulse;
}

void PhysicsModel::AddRelativeForce(Vector3 force, Vector3 point)
{
	AddForce(force);
}

void PhysicsModel::AddRelativeForceLocal(Vector3 force, Vector3 point)
{
	//Move the relative point into global space
	point += GetPosition();
	AddRelativeForce(force, point);
}

