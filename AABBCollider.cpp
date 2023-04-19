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
	float distanceSq = DistanceSq(other.GetPosition());
	return distanceSq <= other.GetRadiusSq();
}

bool AABBCollider::CollidesWith(SphereCollider& other, Vector3& out)
{
	//Find the closest point on this box to the centre of the sphere
	out = ClosestPoint(other.GetPosition());

	//The sphere intersects with this box if the squared distance from the sphere's centre to the closest point is less than the squared sphere radius
	float distanceSq = (out - other.GetPosition()).MagnitudeSq();
	return distanceSq <= other.GetRadiusSq();
}

bool AABBCollider::CollidesWith(PlaneCollider& other)
{
	return false;
}

float AABBCollider::DistanceSq(Vector3 p)
{
	float distanceSq = 0.0f;

	//For each axis, count any excess distance outside box extents
	//x:
	if (p.x < GetMin().x)	//if the point is below the minimum of this axis,
	{
		distanceSq += (GetMin().x - p.x); //add the distance, 
		distanceSq *= distanceSq;			//squared
	}
	if (p.x > GetMax().x)	//if the point is above the maxinum of this axis,
	{
		distanceSq += (p.x - GetMax().x);	//add the distance,
		distanceSq *= distanceSq;			//squared
	}
	//y:
	if (p.y < GetMin().y)	//if the point is below the minimum of this axis,
	{
		distanceSq += (GetMin().y - p.y); //add the distance, 
		distanceSq *= distanceSq;			//squared
	}
	if (p.y > GetMax().y)	//if the point is above the maxinum of this axis,
	{
		distanceSq += (p.y - GetMax().y);	//add the distance,
		distanceSq *= distanceSq;			//squared
	}
	//z:
	if (p.z < GetMin().z)	//if the point is below the minimum of this axis,
	{
		distanceSq += (GetMin().z - p.z); //add the distance, 
		distanceSq *= distanceSq;			//squared
	}
	if (p.z > GetMax().z)	//if the point is above the maxinum of this axis,
	{
		distanceSq += (p.z - GetMax().z);	//add the distance,
		distanceSq *= distanceSq;			//squared
	}

	return distanceSq;
}

float AABBCollider::Distance(Vector3 point)
{
	return sqrt(DistanceSq(point));
}

Vector3 AABBCollider::ClosestPoint(Vector3 point)
{
	Vector3 closestPoint = point;
	//For each axis, if the point is outside the box, clamp it to the box, otherwise keep it as is
	//x:
	closestPoint.Clamp(GetMin(), GetMax());
	return closestPoint;
}

//Vector3 Collider::ClosestPoint(Vector3 p, AABBCollider b)
//{
//	Vector3 closestPoint = p;
//	//For each axis, if the point is outside the box, clamp it to the box, otherwise keep it as is
//	//x:
//	if (p.x < b.GetMin().x)	//if the point is below the minimum of this axis,
//	{
//		closestPoint.x = b.GetMin().x;	//The closest point's value in this axis is the minimum
//	}
//	if (p.x > b.GetMax().x)	//if the point is above the maxinum of this axis,
//	{
//		closestPoint.x = b.GetMax().x;
//	}
//	//y:
//	if (p.y < b.GetMin().y)	//if the point is below the minimum of this axis,
//	{
//		closestPoint.y = b.GetMin().y;	//The closest point's value in this axis is the minimum
//	}
//	if (p.y > b.GetMax().y)	//if the point is above the maxinum of this axis,
//	{
//		closestPoint.y = b.GetMax().y;
//	}
//	//z:
//	if (p.z < b.GetMin().z)	//if the point is below the minimum of this axis,
//	{
//		closestPoint.z = b.GetMin().z;	//The closest point's value in this axis is the minimum
//	}
//	if (p.z > b.GetMax().z)	//if the point is above the maxinum of this axis,
//	{
//		closestPoint.z = b.GetMax().z;
//	}
//}

//float Collider::DistanceSq(Vector3 point, AABBCollider aabb)
//{
//	float distanceSq = 0.0f;
//	const float p[3] = { point.x, point.y, point.z };
//	const float bMin[3] = { aabb.GetMin().x, aabb.GetMin().y, aabb.GetMin().z };
//	const float bMax[3] = { aabb.GetMax().x, aabb.GetMax().y, aabb.GetMax().z };
//
//	//For each axis, count any excess distance outside box extents
//	for (unsigned int i = 0; i < 3; i++)
//	{
//		if (p[i] < bMin[i])	//if the point is below the minimum of this axis,
//		{
//			distanceSq += (bMin[i] - p[i]) * (bMin[i] - p[i]);	//add the distance, squared
//		}
//		if (p[i] < bMax[i])	//if the point is above the maxinum of this axis,
//		{
//			distanceSq += (p[i] - bMax[i]) * (p[i] - bMax[i]);	//add the distance, squared
//		}
//	}
//
//	return distanceSq;
//}