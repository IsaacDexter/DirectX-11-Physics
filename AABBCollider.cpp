#include "AABBCollider.h"
#include "SphereCollider.h"

AABBCollider::AABBCollider(Transform* transform, Vector3 halfExtents) : Collider(transform)
{
	m_halfExtents = halfExtents;
	m_centre = Vector3(0.0f, 0.0f, 0.0f);
}

AABBCollider::AABBCollider(Transform* transform, float rx, float ry, float rz) : Collider(transform)
{
	m_halfExtents = Vector3(rx, ry, rz);
	m_centre = Vector3(0.0f, 0.0f, 0.0f);
}

AABBCollider::AABBCollider(Transform* transform, Vector3 centre, Vector3 halfExtents) : Collider(transform)
{
	m_halfExtents = halfExtents;
	m_centre = centre;
}

AABBCollider::AABBCollider(Transform* transform, float dx, float dy, float dz, float rx, float ry, float rz) : Collider(transform)
{
	m_halfExtents = Vector3(rx, ry, rz);
	m_centre = Vector3(dx, dy, dz);
}

bool AABBCollider::CollidesWith(Collider& other)
{
	//return false;
	return other.CollidesWith(*this);
}

bool AABBCollider::CollidesWith(AABBCollider& other)
{
	if (abs(GetCentre().x - other.GetCentre().x) > (m_halfExtents.x + other.GetHalfExtents().x))	//If we aren't not overlapping on the x
	{
		return false;
	}
	if (abs(GetCentre().y - other.GetCentre().y) > (m_halfExtents.y + other.GetHalfExtents().y))	//or y
	{
		return false;
	}
	if (abs(GetCentre().z - other.GetCentre().z) > (m_halfExtents.z + other.GetHalfExtents().z))	//or z
	{
		return false;
	}
	return true;	//there's been an overlap
}

bool AABBCollider::CollidesWith(SphereCollider& other)
{
	float distanceSq = DistanceSq(other.GetPosition(), *this);
	return distanceSq <= other.GetRadiusSq();
}

bool AABBCollider::CollidesWith(SphereCollider& other, Vector3& out)
{
	//Find the closest point on this box to the centre of the sphere
	out = ClosestPoint(other.GetPosition(), *this);

	//The sphere intersects with this box if the squared distance from the sphere's centre to the closest point is less than the squared sphere radius
	float distanceSq = (out - other.GetPosition()).MagnitudeSq();
	return distanceSq <= other.GetRadiusSq();
	
}
