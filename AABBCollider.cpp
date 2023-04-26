#include "AABBCollider.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "Debug.h"

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

Collision AABBCollider::CollidesWith(Collider& other)
{
	//pass to the other collider;
	return other.CollidesWith(*this);
}

Collision AABBCollider::CollidesWith(AABBCollider& other)
{
	Collision collision;
	if (abs(GetCentre().x - other.GetCentre().x) > (m_halfExtents.x + other.GetHalfExtents().x))	//If we aren't not overlapping on the x
	{
		return collision;
	}
	if (abs(GetCentre().y - other.GetCentre().y) > (m_halfExtents.y + other.GetHalfExtents().y))	//or y
	{
		return collision;
	}
	if (abs(GetCentre().z - other.GetCentre().z) > (m_halfExtents.z + other.GetHalfExtents().z))	//or z
	{
		return collision;
	}
	//there's been an overlap
	collision.collided = true;
	//calculate contacts here:
	//point-point and point-edge contacts as a mere 60th of a second later, a better version of the collision will come about
	// 1. Perform seperating axes theorem to test for an earlier out
	// 2. perform the full collision detection and contact resolution to get a single detected contact between the two boxes
	// 3. combine the new contact previously detected contacts between the two boxes to form a complete set of contacts

	//for each vertex on the other cube
	for (Vector3 vertex : other.GetVertices())
	{
		//Transform the vertex into local space
		Vector3 relativeVertex = vertex - GetCentre();
		
		Vector3 normal;
		//Check each axis, looking gor the axis with the least penetration
		// 
		//x:
		float minDepth = m_halfExtents.x - abs(relativeVertex.x);
		if (minDepth < 0)
		{
			continue;
		}
		//get the normal, the latter part makes sure the normal is pointing in the direction towards the point
		normal = GetXAxis() * ((relativeVertex.x < 0) ? -1 : 1);

		//y:
		float depth = m_halfExtents.y - abs(relativeVertex.y);
		if (depth < 0)
		{
			continue;
		}
		//if this depth is less than the minimum depth
		else if (depth < minDepth && depth != 0.0f)
		{
			//update it, and the normal
			minDepth = depth;
			normal = GetYAxis() * ((relativeVertex.y < 0) ? -1 : 1);
		}
		
		//z
		depth = m_halfExtents.z - abs(relativeVertex.z);
		if (depth < 0)
		{
			continue;
		}
		//if this depth is less than the minimum depth
		else if (depth < minDepth && depth != 0.0f)
		{
			//update it, and the normal
			minDepth = depth;
			normal = GetZAxis() * ((relativeVertex.z < 0) ? -1 : 1);
		}

		//write the contact
		Contact* contact = new Contact();
		contact->normal = normal;
		contact->point = vertex;
		contact->penetration = minDepth;

		DebugPrintF("contact.point = (%f, %f, %f)\n", contact->point.x, contact->point.y, contact->point.z);
		DebugPrintF("contact.normal = (%f, %f, %f)\n", contact->normal.x, contact->normal.y, contact->normal.z);
		DebugPrintF("contact.penetration = %f\n", contact->penetration);

		collision.contacts.push_back(contact);
	}

	//return
	return collision;
}

Collision AABBCollider::CollidesWith(SphereCollider& other)
{
	Collision collision;

	//convert the centre of the sphere into the box's local space
	Vector3 relativeCentreOther = other.GetPosition() - GetCentre();
	//perform seperating axis test to find an axis where the two objects are not colliding

	//DebugPrintF("sphere's relative centre = (%f, %f, %f)\n", relativeCentreOther.x, relativeCentreOther.y, relativeCentreOther.z);

	if (abs(relativeCentreOther.x) - other.GetRadius() > m_halfExtents.x)
	{
		return collision;
	}
	if (abs(relativeCentreOther.y) - other.GetRadius() > m_halfExtents.y)
	{
		return collision;
	}
	if (abs(relativeCentreOther.z) - other.GetRadius() > m_halfExtents.z)
	{
		return collision;
	}

	//if we haven't early outed, there may still be a collision

	
	//Find the distance between this box and the centre of the sphere
	float distanceSq= DistanceSq(other.GetPosition());

	if (distanceSq <= other.GetRadiusSq())
	{
		//There has been a collision
		collision.collided = true;
		//determine contacts here:
		Contact* contact = new Contact();

		//Find the closest point on this box to the centre of the sphere
		Vector3 closestPoint = ClosestPoint(other.GetPosition());
		contact->point = closestPoint;
		//Find the line between the centre and the closest point to find the collision normal
		contact->normal = -(GetCentre() - closestPoint).Normalized();
		//Fidn the penetration with the full calculation,  not just the sqrt one
		contact->penetration = other.GetRadius() - sqrt(distanceSq);
		
		//DebugPrintF("contact.point = (%f, %f, %f)\n", contact->point.x, contact->point.y, contact->point.z);
		//DebugPrintF("contact.normal = (%f, %f, %f)\n", contact->normal.x, contact->normal.y, contact->normal.z);
		//DebugPrintF("contact.penetration = %f\n", contact->penetration);

		collision.contacts.push_back(contact);
	}
	return collision;
}

Collision AABBCollider::CollidesWith(SphereCollider& other, Vector3& out)
{
	//Find the closest point on this box to the centre of the sphere
	out = ClosestPoint(other.GetPosition());

	//The sphere intersects with this box if the squared distance from the sphere's centre to the closest point is less than the squared sphere radius
	float distanceSq = (out - other.GetPosition()).MagnitudeSq();
	return Collision(distanceSq <= other.GetRadiusSq());
}

Collision AABBCollider::CollidesWith(PlaneCollider& other)
{
	return other.CollidesWith(*this);
}

std::array<Vector3, 8> AABBCollider::GetVertices() const
{
	std::array<Vector3, 8> vertices = std::array<Vector3, 8>();
	//Write all 8 vertices of the shape to an array
	vertices = {
		Vector3(-m_halfExtents.x, -m_halfExtents.y, -m_halfExtents.z),
		Vector3(-m_halfExtents.x, -m_halfExtents.y, +m_halfExtents.z),
		Vector3(-m_halfExtents.x, +m_halfExtents.y, -m_halfExtents.z),
		Vector3(-m_halfExtents.x, +m_halfExtents.y, +m_halfExtents.z),
		Vector3(+m_halfExtents.x, -m_halfExtents.y, -m_halfExtents.z),
		Vector3(+m_halfExtents.x, -m_halfExtents.y, +m_halfExtents.z),
		Vector3(+m_halfExtents.x, +m_halfExtents.y, -m_halfExtents.z),
		Vector3(+m_halfExtents.x, +m_halfExtents.y, +m_halfExtents.z)
	};
	//Apply the offset of the shape to give the world position. For an OOBB, this would be a transform matrix
	Vector3 offset = m_centre + GetPosition();
	for (unsigned int i = 0; i < 8; i++)
	{
		vertices[i] += offset;
	}
	return vertices;
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
	//For each axis, count any excess distance outside box extents
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

float AABBCollider::TransformToAxis(const Vector3& axis)	//Code has been noticably simplified as we're using AABB instead of OOBB
{
	return	m_halfExtents.x * abs(axis * GetXAxis()) +		//project onto x
			m_halfExtents.y * abs(axis * GetYAxis()) +	//project onto y
			m_halfExtents.z * abs(axis * GetZAxis());	//project onto z

}

bool AABBCollider::overlapOnAxis(AABBCollider& other, const Vector3& axis)
{
	//project the half size onto axis
	float project = TransformToAxis(axis);
	//project the other's half size onto axis
	float projectOther = other.TransformToAxis(axis);

	//find the vector between the two centres
	Vector3 toCentre = other.GetCentre() - GetCentre();
	//and project that vector onto the axis
	float distance = abs(toCentre * axis);

	//Check for overlap
	return (distance < project + projectOther);
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