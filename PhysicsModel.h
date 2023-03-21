#ifndef PHYSICS_MODEL_H
#define PHYSICS_MODEL_H

#include "Transform.h"

class PhysicsModel
{
public:
	PhysicsModel(Transform* transform);
	virtual void Update(float dt) = 0;

	/// <param name="velocity">The objects velocity in m/s</param>
	virtual void SetVelocity(Vector3 velocity) { m_velocity = velocity; }
	/// <param name="x">The objects velocity in the x direction in m/s</param>
	/// <param name="y">The objects velocity in the y direction in m/s</param>
	/// <param name="z">The objects velocity in the z direction in m/s</param>
	virtual void SetVelocity(float x, float y, float z) { m_velocity.x = x; m_velocity.y = y; m_velocity.z = z; }
	/// <returns>The objects velocity in m/s</returns>
	virtual Vector3 GetVelocity() const { return m_velocity; }
	
	/// <param name="velocity">The objects acceleration in m/s^2</param>
	virtual void SetAcceleration(Vector3 acceleration) { m_acceleration = acceleration; }
	/// <param name="x">The objects acceleration in the x direction in m/s^2</param>
	/// <param name="y">The objects acceleration in the y direction in m/s^2</param>
	/// <param name="z">The objects acceleration in the z direction in m/s^2</param>
	virtual void SetAcceleration(float x, float y, float z) { m_acceleration.x = x; m_acceleration.y = y; m_acceleration.z = z; }
	/// <returns>The objects acceleration in m/s^2</returns>
	virtual Vector3 GetAcceleration() const { return m_acceleration; }

protected:
	Transform* m_transform;
	/// <summary>The objects velocity in m/s</summary>
	Vector3 m_velocity;
	/// <summary>The object's acceleration in m/s^2</summary>
	Vector3 m_acceleration;
};

#endif // !PHYSICS_MODEL_H