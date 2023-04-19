#include "SphereCollider.h"
#include "AABBCollider.h"
#include "PlaneCollider.h"


PlaneCollider::PlaneCollider(Transform* transform, Vector3 normal, float distance) : Collider(transform)
{
    m_normal = normal.Normalized();
    m_distance = distance;
}

PlaneCollider::PlaneCollider(Transform* transform, Vector3 normal, Vector3 point) : Collider(transform)
{
    m_normal = normal.Normalized();
    m_distance = CalculateDistanceFromPoint(point);
}

bool PlaneCollider::CollidesWith(Collider& other)
{
    return other.CollidesWith(*this);
}

bool PlaneCollider::CollidesWith(AABBCollider& other)
{
    return false;
}

bool PlaneCollider::CollidesWith(SphereCollider& other)
{
    //Find the distance between the centre of the sphere and the plane with (s.c dot n) and subtracting m_distance
    float distance = (other.GetPosition() * m_normal) - m_distance;
    //if this distance is within +- a radius' distance from the plane, the two intersect
    return abs(distance) <= other.GetRadius();
}

bool PlaneCollider::CollidesWith(PlaneCollider& other)
{
    //Find the cross product between the two normals. If it's not the zero vector, they are not parralel and will collide
    return (m_normal ^ other.GetNormal()) != Vector3();
}

float PlaneCollider::CalculateDistanceFromPoint(Vector3 point) const
{
    return m_normal * point;
}
