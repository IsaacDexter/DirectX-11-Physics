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

	//Cache this' attributes
	Vector3 thisPosition = GetPosition();
	float thisRadius = GetRadius();

	//Cache other's attributes
	Vector3 otherPosition = other.GetPosition();
	float otherRadius = other.GetRadius();



	//find the vector between this and the other sphere
	Vector3 distance = otherPosition - thisPosition;
	float distanceSq = distance.MagnitudeSq();
	//find the squared sum of magnitudes
	float radii = otherRadius + thisRadius;
	float radiiSq = radii * radii;

	//if the distance (squared) is less than the sum of the radii (squared), theres been a collision
	if (distanceSq >= 0.0f && distanceSq <= radiiSq)
	{
		collision.collided = true;
		//find the contact between the two
		//The normal will be the distance, normalized
		Vector3 normal = distance.Normalized();
		//The point of contact will be the midpoint between the two
		Vector3 point = thisPosition + (distance * 0.5f);
		//The penetration is the sum of the radii - the magnitude of the collision
		float penetration = radii - distance.Magnitude();

		//write this data back
		collision.contacts.push_back(Contact(normal, point, penetration));
	}
	return collision;
}

Collision SphereCollider::CollidesWith(AABBCollider& other)
{
	return other.CollidesWith(*this);
}

Collision SphereCollider::CollidesWith(PlaneCollider& other)
{
	Collision collision;

	//Cache sphere attributes
	Vector3 spherePosition = GetPosition();
	float sphereRadius = GetRadius();

	//Cache plane attributes
	Vector3 planeNormal = other.GetNormal();
	float planeDistance = other.GetDistance();


	//Find the distance between the centre of the sphere and the plane with (s.c dot n) and subtracting m_distance
	float distance = (spherePosition * planeNormal) - planeDistance;
	//if this distance is within +- a radius' distance from the plane, the two intersect
	if (abs(distance) <= sphereRadius)
	{
		//if theres been a collision,
		collision.collided = true;

		//Calculate contact points:
		Vector3 point = spherePosition - planeNormal * (distance + sphereRadius);
		//the plane only has one face so the normal will always be the plane's
		Vector3 normal = planeNormal;
		//The sphere has definitely intersected, so the distance from the plane will be the amount intersected, as its distance from the other side.
		float penetration = -distance;
		//Find the point of collision along the line of the normal from the position
		//write this contact to the collision
		collision.contacts.push_back(Contact(point, normal, penetration));
	}
	return collision;
}
