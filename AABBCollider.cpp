#include "AABBCollider.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"

#pragma region Constructors

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

#pragma endregion

#pragma region CollidesWiths

Collision AABBCollider::CollidesWith(Collider& other)
{
	//pass to the other collider;
	return other.CollidesWith(*this);
}

Collision AABBCollider::CollidesWith(AABBCollider& other)
{
	Collision collision;

	//Cache this' attributes
	Vector3 thisCentre = GetCentre();
	Vector3 thisHalfExtents = GetHalfExtents();

	//Cache other's attributes
	Vector3 otherCentre = other.GetCentre();
	Vector3 otherHalfExtents = other.GetHalfExtents();
	std::array<Vector3, 8> otherVertices = other.GetVertices();


	//Run early out separating axes test by testing to see if they are not overlapping on either the x, y or z
	if (abs(thisCentre.x - otherCentre.x) > (thisHalfExtents.x + otherHalfExtents.x))	//If we aren't not overlapping on the x
	{
		return collision;
	}
	if (abs(thisCentre.y - otherCentre.y) > (thisHalfExtents.y + otherHalfExtents.y))	//or y
	{
		return collision;
	}
	if (abs(thisCentre.z - otherCentre.z) > (thisHalfExtents.z + otherHalfExtents.z))	//or z
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
	for (Vector3 point : otherVertices)
	{
		//Transform the vertex into local space
		Vector3 relativeVertex = point - thisCentre;
		
		Vector3 normal;
		//Check each axis, looking gor the axis with the least penetration
		// 
		//x:
		float penetration = thisHalfExtents.x - abs(relativeVertex.x);
		if (penetration < 0)
		{
			continue;
		}
		//get the normal, the latter part makes sure the normal is pointing in the direction towards the point
		normal = GetXAxis() * ((relativeVertex.x < 0) ? -1 : 1);

		//y:
		float depth = thisHalfExtents.y - abs(relativeVertex.y);
		if (depth < 0)
		{
			continue;
		}
		//if this depth is less than the minimum depth
		else if (depth < penetration && depth != 0.0f)
		{
			//update it, and the normal
			penetration = depth;
			normal = GetYAxis() * ((relativeVertex.y < 0) ? -1 : 1);
		}
		
		//z
		depth = thisHalfExtents.z - abs(relativeVertex.z);
		if (depth < 0)
		{
			continue;
		}
		//if this depth is less than the minimum depth
		else if (depth < penetration && depth != 0.0f)
		{
			//update it, and the normal
			penetration = depth;
			normal = GetZAxis() * ((relativeVertex.z < 0) ? -1 : 1);
		}
		//write the contact
		collision.contacts.push_back(Contact(point, normal, penetration));
	}

	return collision;
}

Collision AABBCollider::CollidesWith(SphereCollider& other)
{
	Collision collision;

	//Cache box's values
	Vector3 boxCentre = GetCentre();
	Vector3 boxHalfExtents = GetHalfExtents();
	
	//Cache sphere attributes
	Vector3 spherePosition = other.GetPosition();
	float sphereRadius = other.GetRadius();
	float sphereRadiusSq = other.GetRadiusSq();

	//convert the centre of the sphere into the box's local space
	Vector3 relativeCentreOther = spherePosition - boxCentre;


	//perform an early-out seperating axis test to find an axis where the two objects are not colliding
	if (abs(relativeCentreOther.x) - sphereRadius > boxHalfExtents.x)
	{
		return collision;
	}
	if (abs(relativeCentreOther.y) - sphereRadius > boxHalfExtents.y)
	{
		return collision;
	}
	if (abs(relativeCentreOther.z) - sphereRadius > boxHalfExtents.z)
	{
		return collision;
	}

	//if we haven't early outed, there may still be a collision

	
	//Find the distance between this box and the centre of the sphere
	float distanceSq= DistanceSq(spherePosition);

	//if its less than the radius, theres been a collision
	if (distanceSq <= sphereRadiusSq)
	{
		//There has been a collision
		collision.collided = true;
		//determine contacts here:

		//Find the closest point on this box to the centre of the sphere
		Vector3 point = ClosestPoint(spherePosition);
		//Find the line between the centre and the closest point to find the collision normal
		Vector3 normal = -(boxCentre - point).Normalized();
		//Fidn the penetration with the full calculation,  not just the sqrt one
		float penetration = sphereRadius - sqrt(distanceSq);

		collision.contacts.push_back(Contact(point, normal, penetration));
	}
	return collision;
}

Collision AABBCollider::CollidesWith(PlaneCollider& other)
{
	Collision collision;

	//Cache aabb Attributes
	Vector3 aabbHalfExtents = GetHalfExtents();
	Vector3 aabbCentre = GetCentre();
	std::array<Vector3, 8> aabbVertices = GetVertices();

	//Cache Plane Attributes
	Vector3 planeNormal = other.GetNormal();
	float planeDistance = other.GetDistance();

	//Calculate the projection interval radius of the box onto a line parallel to the normal that goes through the box's centre
	//Line L = box's centre + t * plane's normal, where the box's centre projects onto L when t = 0, and L || plane's normal.
	//L = b.c + t * p.n
	//Projection interval radius
	//dot product of the box's half extents and the absolute of the plane's normal
	float projectionIntervalRadius = aabbHalfExtents * abs(planeNormal);
	//Distance between box's centre and plane
	//dot product of box's centre and planes normal - plane's distance from origin
	float centreDistance = (aabbCentre * planeNormal) - planeDistance;
	//If the distance, s, falls within the projection interval radius, r, there's been a collision
	if (abs(centreDistance) <= projectionIntervalRadius)
	{
		//if theres been a collision,
		collision.collided = true;

		//Calculate contact points:
		//We can find the set of contacts by simply checking each vertex of the box one ny one and generating a contact if it lies below the plane
		//This check for each vertex is similar to the one made in the sphere plane detector: d = p dot l - t,
		//But as the the vertices have no radii, simply check if the sign of d is positive or negative. A collision has occured if p dot l < l
		
		//for each vertex 
		std::array<Vector3, 8>::iterator it;
		for (it = aabbVertices.begin(); it != aabbVertices.end(); ++it)
		{
			//calculate the distance of this vertex from the plane
			float distance = ((*it) * planeNormal);
			//if it's greater than the plane's distance, this vertex hasn't collided, skip it
			if (distance > planeDistance)
			{
				continue;
			}

			//The contact point, halfway between the vertex and the plane, can be found by:
			//plane's normal * (speraration distance)/2 + vertex's location
			Vector3 point = planeNormal * (distance - planeDistance) + (*it);
			//The normal is the plane's normal, always
			Vector3 normal = planeNormal;
			//penetration is how the plane's distance - the distance to the vertex
			float penetration = planeDistance - distance;

			//write this vertex
			collision.contacts.push_back(Contact(point, normal, penetration));
		}
	}

	return collision;
}

#pragma endregion

#pragma region GSetters

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

#pragma endregion

#pragma region Calculations

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

#pragma endregion