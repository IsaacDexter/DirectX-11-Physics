#ifndef _PHYSICS_MODEL_H
#define _PHYSICS_MODEL_H

#include "PhysicsModel.h"

class RigidBodyModel:
	public PhysicsModel
{
public:
	RigidBodyModel(Transform* transform, float mass = 1.0f, float restitution = 0.5f);
	virtual void Update(float dt);

	/// <summary>For use in rigidbody rotations. Applies a force to a specific global point</summary>
	/// <param name="force">The force impulse, in N</param>
	/// <param name="point">The point to apply the impulse at, in global space</param>
	virtual void AddRelativeForce(Vector3 force, Vector3 point) override;

#pragma region GettersAndSetters

	/// <param name="orientation">The new orientation as a quaternion</param>
	virtual void SetOrientation(Quaternion orientation) { m_transform->SetOrientation(orientation); };
	/// <returns>The orientation as a quaternion</returns>
	Quaternion GetOrientation() const { return m_transform->GetOrientation(); };
	/// <param name="position">The change in orientation</param>
	void Rotate(Vector3 rotation) { SetOrientation(GetOrientation() + GetOrientation() * rotation); };
	//void Rotate(Vector3 rotation) { SetOrientation(GetOrientation() * MakeQFromEulerAngles(rotation.x, rotation.y, rotation.z)); };

	/// <param name="inertiaTensor"><para>The inertia tensor in three dimensions is a 3 × 3 matrix that is characteristic of a rigid body</para>
	/// <para>Each rigid body will have its own inertia tensor, just like it has its own mass</para>
	/// <para>Tensor is another way of saying matrix(or vector) – multidimensional number</para>
	/// <para>Could also be called a 'Mass Matrix'</para></param>
	void SetIntertiaTensor(XMFLOAT3X3 inertiaTensor) { m_inertiaTensor = inertiaTensor; };
	/// <returns><para>The inertia tensor in three dimensions is a 3 × 3 matrix that is characteristic of a rigid body</para>
	/// <para>Each rigid body will have its own inertia tensor, just like it has its own mass</para>
	/// <para>Tensor is another way of saying matrix(or vector) – multidimensional number</para>
	/// <para>Could also be called a 'Mass Matrix'</para></returns>
	XMFLOAT3X3 GetIntertiaTensor() const { return m_inertiaTensor; };

	XMFLOAT3X3 GetCubeInertiaTensor(Vector3 halfExtents);
	XMFLOAT3X3 GetSphereInertiaTensor(float radius);

	/// <param name="torque"><para>Torque can be thought of as a twisting force</para>
	/// <para>A car with more torque can apply a more turning force to the wheels</para>
	/// <para>Torque spins wheels, forward motion is a secondary effect caused by friction</para>
	/// <para>Every force applied to an object will generate a corresponding torque</para></param>
	void SetTorque(Vector3 torque) { m_torque = torque; };
	/// <returns><para>Torque can be thought of as a twisting force</para>
	/// <para>A car with more torque can apply a more turning force to the wheels</para>
	/// <para>Torque spins wheels, forward motion is a secondary effect caused by friction</para>
	/// <para>Every force applied to an object will generate a corresponding torque</para></returns>
	Vector3 GetTorque() const { return m_torque; };

	void SetAngularVelocity(Vector3 angularVelocity) { m_angularVelocity = angularVelocity; };
	Vector3 GetAngularVelocity() const { return m_angularVelocity; };

	void SetAngularAcceleration(Vector3 angularAcceleration) { m_angularAcceleration = angularAcceleration; };
	Vector3 GetAngularAcceleration() const { return m_angularAcceleration; };

#pragma endregion

protected:
#pragma region Calculations

	void CalculateAngularAcceleration();
	void CalculateAngularVelocity(float dt);
	/// <summary>Calculates the change in orientation and applies it.</summary>
	void CalculateRotation(float dt);

	void ClearTorqueAndAngularAcceleration();

#pragma endregion

#pragma region FieldForces

	/// <summary>To stand in for angular drag, use angular damping to stop the cube rotating forever</summary>
	void ApplyAngularDamping(float dt);

#pragma endregion

#pragma region Rotations

	/// <summary><para>Torque can be thought of as a twisting force</para>
	/// <para>A car with more torque can apply a more turning force to the wheels</para>
	/// <para>Torque spins wheels, forward motion is a secondary effect caused by friction</para>
	/// <para>Every force applied to an object will generate a corresponding torque</para></summary>
	Vector3 m_torque;
	/// <summary><para>The inertia tensor in three dimensions is a 3 × 3 matrix that is characteristic of a rigid body</para>
	/// <para>Each rigid body will have its own inertia tensor, just like it has its own mass</para>
	/// <para>Tensor is another way of saying matrix(or vector) – multidimensional number</para>
	/// <para>Could also be called a 'Mass Matrix'</para></summary>
	XMFLOAT3X3 m_inertiaTensor;
	/// <summary>Rigidbodies use angular velocity and acceleration</summary>
	Vector3 m_angularVelocity;
	Vector3 m_angularAcceleration;

#pragma endregion
};

#endif // !_PHYSICS_MODEL_H