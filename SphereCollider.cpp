#include "SphereCollider.h"
#include "AABBCollider.h"
#include "PlaneCollider.h"

SphereCollider::SphereCollider(Transform* transform, float radius) : Collider(transform)
{
	m_radius = radius;
}

Collision SphereCollider::CollidesWith(Collider& other)
{
	return other.CollidesWith(*this);
}

Collision SphereCollider::CollidesWith(SphereCollider& other)
{
	Collision collision;
	//Get the vector between this and the other sphere
	Vector3 distance = other.GetPosition() - GetPosition();
	float distanceSq = distance.MagnitudeSq();
	//find the squared sum of magnitudes
	float radii = other.GetRadius() + GetRadius();
	float radiiSq = radii * radii;
	//if the distance (squared) is less than the sum of the radii (squared), theres been a collision
	if (distanceSq >= 0.0f && distanceSq <= radiiSq)
	{
		collision.collided = true;
		//find the contact between the two
		Contact* contact = new Contact();
		//The normal will be the distance, normalized
		contact->normal = distance.Normalized();
		//The point of contact will be the midpoint between the two
		contact->point = GetPosition() + (distance * 0.5f);
		//The penetration is the sum of the radii - the magnitude of the collision
		contact->penetration = radii - distance.Magnitude();

		//write this data back
		collision.contacts.push_back(contact);
	}
	return collision;
}

Collision SphereCollider::CollidesWith(AABBCollider& other)
{
	Collision collision;
	float distanceSq = other.DistanceSq(GetPosition());
	if( distanceSq <= GetRadiusSq())
	{
		//If the two are touching,
		collision.collided = true;
		//Calculate contacts:

	}
	return collision;
}

Collision SphereCollider::CollidesWith(PlaneCollider& other)
{
	Collision collision;
	//Find the distance between the centre of the sphere and the plane with (s.c dot n) and subtracting m_distance
	float distance = (GetPosition() * other.GetNormal() ) - other.GetDistance();
	//if this distance is within +- a radius' distance from the plane, the two intersect
	if (abs(distance) <= m_radius)
	{
		//if theres been a collision,
		collision.collided = true;
		//Calculate contact points:

	}
	return collision;
}
