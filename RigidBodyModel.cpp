#include "RigidBodyModel.h"

RigidBodyModel::RigidBodyModel(Transform* transform, float mass, float restitution) :PhysicsModel(transform, mass, restitution)
{
	m_angularVelocity = Vector3();
	m_inertiaTensor = XMFLOAT3X3();
	XMStoreFloat3x3(&m_inertiaTensor, XMMatrixIdentity());
}

void RigidBodyModel::Update(float dt)
{
	//Apply forces; calculate acc; calculate vel and dis; update position; clear acc and forces

	ApplyGravity();
	ApplyDrag();
	ApplyFriction();

	ApplyAngularDamping(dt);


	CalculateAcceleration();
	CalculateVelocity(dt);
	CalculateDisplacement(dt);

	CalculateAngularAcceleration();
	CalculateAngularVelocity(dt);
	CalculateRotation(dt);


	ClearForceAndAcceleration();
	ClearTorqueAndAngularAcceleration();
}

void RigidBodyModel::AddRelativeForce(Vector3 force, Vector3 point)
{
	AddForce(force);
	Vector3 relativePosition = point - GetPosition();
	//Torque is the cross product of the relative position and the force vector
	m_torque += relativePosition ^ force;
}

XMFLOAT3X3 RigidBodyModel::GetCubeInertiaTensor(Vector3 halfExtents)
{
	XMFLOAT3X3 inertiaTensor;
	//initialise to the identity matrix
	XMStoreFloat3x3(&inertiaTensor, XMMatrixIdentity());
	//[	(m/12)*(y^2+z^2)	0					0					]
	//[	0					(m/12)*(y^2+z^2)	0					]
	//[	0					0					(m/12)*(y^2+z^2)	]
	inertiaTensor._11 = (1.0f / 12.0f) * m_mass * (halfExtents.y * halfExtents.y + halfExtents.z * halfExtents.z);
	inertiaTensor._22 = (1.0f / 12.0f) * m_mass * (halfExtents.x * halfExtents.x + halfExtents.z * halfExtents.z);
	inertiaTensor._33 = (1.0f / 12.0f) * m_mass * (halfExtents.x * halfExtents.x + halfExtents.y * halfExtents.y);
	return inertiaTensor;
}

XMFLOAT3X3 RigidBodyModel::GetSphereInertiaTensor(float radius)
{
	XMFLOAT3X3 inertiaTensor;
	//initialise to the identity matrix
	XMStoreFloat3x3(&inertiaTensor, XMMatrixIdentity());
	//[	(2/5)mr^2	0			0			]
	//[	0			(2/5)mr^2	0			]
	//[	0			0			(2/5)mr^2	]
	inertiaTensor._11 = (2.0f / 5.0f) * m_mass * radius * radius;
	inertiaTensor._22 = (2.0f / 5.0f) * m_mass * radius * radius;
	inertiaTensor._33 = (2.0f / 5.0f) * m_mass * radius * radius;
	return inertiaTensor;
}

void RigidBodyModel::CalculateAngularAcceleration()
{
	XMVECTOR angularAccelerationVector;
	//Store the torque in a vector
	XMVECTOR torqueVector = XMVectorSet(m_torque.x, m_torque.y, m_torque.z, m_torque.z);
	//store the inverse of the inertia tensor in a matrix
	XMMATRIX inverseInertiaTensorMatrix = XMMatrixInverse(nullptr, XMLoadFloat3x3(&m_inertiaTensor));
	//angular Acceleration = inverse inertia Tensort * torque
	angularAccelerationVector = XMVector3Transform(torqueVector, inverseInertiaTensorMatrix);
	XMFLOAT3 angularAcceleration;
	XMStoreFloat3(&angularAcceleration, angularAccelerationVector);
	m_angularAcceleration = Vector3(angularAcceleration.x, angularAcceleration.y, angularAcceleration.z);
}

void RigidBodyModel::CalculateAngularVelocity(float dt)
{
	m_angularVelocity += m_angularAcceleration * dt;
	m_angularAcceleration.Truncate();	//Prevents wasting processing power on insignificant values
}

void RigidBodyModel::CalculateRotation(float dt)
{
	Vector3 rotation = m_angularVelocity * dt;

	Quaternion orientation = GetOrientation();
	orientation += orientation * m_angularVelocity * 0.5f;
	orientation /= orientation.Magnitude();	//normalise
	SetOrientation(orientation);
	/*Rotate(rotation * 0.5);*/
}

void RigidBodyModel::ClearTorqueAndAngularAcceleration()
{
	m_torque = Vector3();	//Reset torque after movement as it is recalculated every frame
	m_angularAcceleration = Vector3();	//Reset angularacceleration as it is recalculated every frame
}

void RigidBodyModel::ApplyAngularDamping(float dt)
{
	m_angularVelocity *= pow(ANGULAR_DAMPING, dt);
}
