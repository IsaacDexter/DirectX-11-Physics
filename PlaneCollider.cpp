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

Collision PlaneCollider::CollidesWith(Collider& other)
{
    return other.CollidesWith(*this);
}

Collision PlaneCollider::CollidesWith(AABBCollider& other)
{
    Collision collision;
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
    if (abs(centreDistance) <= projectionIntervalRadius)
    {
        //if theres been a collision,
        collision.collided = true;
        //Calculate contact points:
        //We can find the set of contacts by simply checking each vertex of the box one ny one and generating a contact if it lies below the plane
        //This check for each vertex is similar to the one made in the sphere plane detector: d = p dot l - t,
        //But as the the vertices have no radii, simply check if the sign of d is positive or negative. A collision has occured if p dot l < l

        //otherwise, there has been a collision with this vertex
        //for each vertex 
        std::array<Vector3, 8> vertices = other.GetVertices();
        std::array<Vector3, 8>::iterator it;
        for (it = vertices.begin(); it != vertices.end(); ++it)
        {
            //calculate the distance of this vertex from the plane
            float distance = ((* it) * m_normal);
            //if it's greater than the plane's distance, this vertex hasn't collided, skip it
            if (distance > m_distance)
            {
                continue;
            }
            //The contact point, halfway between the vertex and the plane, can be found by:
            //plane's normal * (speraration distance)/2 + vertex's location
            Vector3 point = m_normal * (distance - m_distance) + (*it);
            //The normal is the plane's normal, always
            Vector3 normal = m_normal;
            //penetration is how the plane's distance - the distance to the vertex
            float penetration = m_distance - distance;
            //write this vertex
            collision.contacts.push_back(Contact(point, normal, penetration));
        }
    }

    return collision;
}

Collision PlaneCollider::CollidesWith(SphereCollider& other)
{
    Collision collision;

    //Cache sphere attributes
    Vector3 spherePosition = other.GetPosition();
    float sphereRadius = other.GetRadius();

    //Cache plane attributes
    Vector3 planeNormal = GetNormal();
    float planeDistance = GetDistance();


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
        //Get the point of collision along the line of the normal from the position
        //write this contact to the collision
        collision.contacts.push_back(Contact(point, normal, penetration));
    }
    return collision;
}

Collision PlaneCollider::CollidesWith(PlaneCollider& other)
{
    Collision collision;
    //Find the cross product between the two normals. If it's not the zero vector, they are not parralel and will collide
    if((m_normal ^ other.GetNormal()) != Vector3())
    {
        //if theres been a collision,
        collision.collided = true;
        //Do not Calculate contact points:
        //plane colliders are infinitely large and will overlap and an infinite number of points
        //as such, leave blank - when will planes need to collide that fine grain with eachother anyway.

    }
    return collision;
}

float PlaneCollider::CalculateDistanceFromPoint(Vector3 point) const
{
    return m_normal * point;
}
