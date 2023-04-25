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

//bool PlaneCollider::CollidesWith(AABBCollider& other)
//{
//    //box's centre
//    Vector3 c = other.GetCentre();
//    //box's extents
//    Vector3 e = other.GetHalfExtents();
//    //plane's normal
//    Vector3 n = m_normal;
//    //plane's distance from normal
//    float d = m_distance;
//    //Calculate the projection interval radius of the box onto a line parallel to the normal that goes through the box's centre
//    //Line L = box's centre + t * plane's normal, where the box's centre projects onto L when t = 0, and L || plane's normal.
//    //L = b.c + t * p.n
//    //Projection interval radius
//    //dot product of the box's half extents and the absolute of the plane's normal
//    float r = e * abs(n);
//    //Distance between box's centre and plane
//    //dot product of box's centre and planes normal - plane's distance from origin
//    float s = c * n - d;
//    //If the distance, s, falls within the projection interval radius, r, there's been a collision
//    return abs(s) <= r;
//}

bool PlaneCollider::CollidesWith(AABBCollider& other)
{
    //Calculate the projection interval radius of the box onto a line parallel to the normal that goes through the box's centre
    //Line L = box's centre + t * plane's normal, where the box's centre projects onto L when t = 0, and L || plane's normal.
    //L = b.c + t * p.n
    //Projection interval radius
    //dot product of the box's half extents and the absolute of the plane's normal
    float projectionIntervalRadius = other.GetHalfExtents() * abs(m_normal);
    //Distance between box's centre and plane
    //dot product of box's centre and planes normal - plane's distance from origin
    float centreDistance = (other.GetCentre() * m_normal) - m_distance;
    //If the distance, s, falls within the projection interval radius, r, there's been a collision
    return abs(centreDistance) <= projectionIntervalRadius;
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
