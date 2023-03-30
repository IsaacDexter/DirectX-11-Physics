#ifndef _SPHERE_COLLIDER_H
#define _SPHERE_COLLIDER_H

#include "Collider.h"

class SphereCollider :
	public Collider
{
public:
	/// <summary>A sphere collided only contains collision code for other sphere colliders, as more complex colliders handle sphere colliders differently</summary>
	/// <param name="transform">The transform component used for position and orientation</param>
	/// <param name="radius">The radius, in m, of the sphere modelled</param>
	SphereCollider(Transform* transform, float radius);

	/// <summary>Simply get the other collider to collide with this, as each handles sphere differently</summary>
	/// <param name="other">The object this object is colliding with</param>
	/// <returns>Whether or not the objects have collided, true for a collision</returns>
	virtual bool CollidesWith(Collider& other) override;
	/// <summary>Finds the squared distance between this and the other sphere, and compare it to the squared sum between the two radii</summary>
	/// <param name="other">The object this object is colliding with</param>
	/// <returns>Whether or not the objects have collided, true for a collision</returns>
	virtual bool CollidesWith(SphereCollider& other) override;

	/// <returns>The radius, in m, of the sphere modelled</returns>
	float GetRadius() const { return m_radius; };
protected:
	float m_radius = 1.0f;
};

#endif // !_SPHERE_COLLIDER_H