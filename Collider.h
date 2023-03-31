#ifndef _COLLIDER_H
#define _COLLIDER_H

#include "Transform.h"

class SphereCollider;
class AABBCollider;

class Collider abstract
{
protected:
	Transform* m_transform;
public:
	Collider(Transform* transform);
	
	virtual bool CollidesWith(Collider& other) = 0;
	virtual bool CollidesWith(SphereCollider& other) = 0;

	Vector3 GetPosition() const { return m_transform->GetPosition(); };

#pragma region Calculations

	/// <summary>Finds the squared distance between a point and an Axis-Aligned Bounding Box</summary>
	/// <param name="point">The point, p</param>
	/// <param name="AABB">The axis-aligned bounding box, b</param>
	/// <returns>The distance, squared, between the two.</returns>
	float DistanceSq(Vector3 point, AABBCollider AABB);

	/// <summary>Finds the squared distance between a point and an Axis-Aligned Bounding Box</summary>
	/// <param name="point">The point, p</param>
	/// <param name="AABB">The axis-aligned bounding box, b</param>
	/// <returns>The distance, between the two.</returns>
	float Distance(Vector3 point, AABBCollider AABB);

	Vector3 ClosestPoint(Vector3 point, AABBCollider AABB);

#pragma endregion

};

#endif // !_COLLIDER_H