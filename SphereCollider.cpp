#include "SphereCollider.h"
#include "AABBCollider.h"

SphereCollider::SphereCollider(Transform* transform, float radius) : Collider(transform)
{
	m_radius = radius;
}

bool SphereCollider::CollidesWith(Collider& other)
{
	return other.CollidesWith(*this);
}

bool SphereCollider::CollidesWith(SphereCollider& other)
{
	float distanceSq = (other.GetPosition() - GetPosition()).MagnitudeSq();	//find the squared dsitance between the two colliders
	float radiiSq = (other.GetRadius() + m_radius);	//Get the sum of the two radii,
	radiiSq *= radiiSq;								//and square it.
	bool collided = distanceSq <= radiiSq;
	return collided;	//So long as the distance squared between the two is less than the radii sum squared, the two are touching!
}

bool SphereCollider::CollidesWith(AABBCollider& other)
{
	float distanceSq = DistanceSq(GetPosition(), other);
	return distanceSq <= GetRadiusSq();
}
