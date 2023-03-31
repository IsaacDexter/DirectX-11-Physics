#include "AABBCollider.h"
#include "SphereCollider.h"

AABBCollider::AABBCollider(Transform* transform, Vector3 centre, Vector3 halfExtents) : Collider(transform)
{
	m_centre = centre;
	m_halfExtents = halfExtents;
}

AABBCollider::AABBCollider(Transform* transform, float dx, float dy, float dz, float rx, float ry, float rz) : Collider(transform)
{
	m_centre = Vector3(dx, dy, dz);
	m_halfExtents = Vector3(rx, ry, rz);
}

bool AABBCollider::CollidesWith(Collider& other)
{
	return other.CollidesWith(*this);
}

bool AABBCollider::CollidesWith(AABBCollider& other)
{
	if (abs(m_centre.x - other.GetCentre().x) > (m_halfExtents.x + other.GetHalfExtents().x))	//If we aren't not overlapping on the x
	{
		return false;
	}
	if (abs(m_centre.y - other.GetCentre().y) > (m_halfExtents.y + other.GetHalfExtents().y))	//or y
	{
		return false;
	}
	if (abs(m_centre.z - other.GetCentre().z) > (m_halfExtents.z + other.GetHalfExtents().z))	//or z
	{
		return false;
	}
	return true;	//there's been an overlap
}

bool AABBCollider::CollidesWith(SphereCollider& other)
{
	float distanceSq = DistanceSq(other.GetPosition(), *this);
	return distanceSq <= other.GetRadius() * other.GetRadius();
}

bool AABBCollider::CollidesWith(SphereCollider& other, Vector3& out)
{
	return false;
}
