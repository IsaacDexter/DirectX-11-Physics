#ifndef PHYSICS_MODEL_H
#define PHYSICS_MODEL_H

#include "Transform.h"

class PhysicsModel
{
public:
	/// <param name="transform">The a pointer to the transform object, which is used to apply these forces</param>
	/// <param name="mass">The object's mass, in kg. Defaults to 1 kg</param>
	PhysicsModel(Transform* transform, float mass);

	/// <summary>Calculates the object's acceleration, velocity and displacement from the forces applied to it; Appliest the displacement; Resets acceleration and force</summary>
	/// <param name="dt">The change in time since last frame</param>
	virtual void Update(float dt) = 0;

	/// <param name="force">The force to be added, in N</param>
	void AddForce(Vector3 force) { m_netforce += force; };
	/// <param name="x">The force to be added in the x direction, in N</param>
	/// <param name="y">The force to be added in the y direction, in N</param>
	/// <param name="z">The force to be added in the z direction, in N</param>
	void AddForce(float x, float y, float z) { m_netforce += Vector3(x, y, z); };

	/// <param name="velocity">The object's velocity in m/s</param>
	void SetVelocity(Vector3 velocity) { m_velocity = velocity; }
	/// <param name="x">The object's velocity in the x direction in m/s</param>
	/// <param name="y">The object's velocity in the y direction in m/s</param>
	/// <param name="z">The object's velocity in the z direction in m/s</param>
	void SetVelocity(float x, float y, float z) { m_velocity.x = x; m_velocity.y = y; m_velocity.z = z; }
	/// <returns>The object's velocity in m/s</returns>
	Vector3 GetVelocity() const { return m_velocity; }
	
	/// <param name="velocity">The object's acceleration in m/s^2</param>
	void SetAcceleration(Vector3 acceleration) { m_acceleration = acceleration; }
	/// <param name="x">The object's acceleration in the x direction in m/s^2</param>
	/// <param name="y">The object's acceleration in the y direction in m/s^2</param>
	/// <param name="z">The object's acceleration in the z direction in m/s^2</param>
	void SetAcceleration(float x, float y, float z) { m_acceleration.x = x; m_acceleration.y = y; m_acceleration.z = z; }
	/// <returns>The object's acceleration in m/s^2</returns>
	Vector3 GetAcceleration() const { return m_acceleration; }

	/// <param name="mass">The object's mass, in kg</param>
	void SetMass(float mass) { m_mass = mass; };
	/// <returns>The object's mass, in kg</returns>
	float GetMass() const { return m_mass; };

protected:
	/// <summary>Acceleration = Force / Mass</summary>
	void CalculateAcceleration();
	/// <summary>Delta Velocity = Acceleration * Delta Time. Velocity += Delta Velocity</summary>
	/// <param name="dt">Delta Time</param>
	void CalculateVelocity(float dt);
	/// <summary>Displacement = Velocity * Delta Time. Position += Displacement</summary>
	/// <param name="dt">Delta Time</param>
	void CalculateDisplacement(float dt);
	/// <summary>Resets force and acceleration to their default Vector. Should be called at the end of the update call</summary>
	void ClearForceAndAcceleration();

	Transform* m_transform;
	/// <summary>The objects velocity in m/s</summary>
	Vector3 m_velocity;
	/// <summary>The object's acceleration in m/s^2</summary>
	Vector3 m_acceleration;
	/// <summary>The sum of all forces acting on the object, in N</summary>
	Vector3 m_netforce;
	/// <summary>The object's mass, in kg</summary>
	float m_mass;
};

#endif // !PHYSICS_MODEL_H