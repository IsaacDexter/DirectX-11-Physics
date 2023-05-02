#include "SphereCollider.h"
#include "AABBCollider.h"
#include "PlaneCollider.h"

#include "Debug.h"


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
            collision.contacts.push_back(new Contact(point, normal, penetration));

            //DebugPrintF("contact.point = (%f, %f, %f)\n", contact.point.x, contact.point.y, contact.point.z);
            //DebugPrintF("contact.normal = (%f, %f, %f)\n", contact.normal.x, contact.normal.y, contact.normal.z);
            //DebugPrintF("contact.penetration = %f\n", contact.penetration);
        }
    }
    //DebugPrintF("Plane/AABB: collision.contacts.size->(%i)\n", collision.contacts.size());


    return collision;
}

Collision PlaneCollider::CollidesWith(SphereCollider& other)
{
    Collision collision;
    Vector3 positionOther = other.GetPosition();
    float radiusOther = other.GetRadius();
    //Find the distance between the centre of the sphere and the plane with (s.c dot n) and subtracting m_distance
    float distance = (positionOther * m_normal) - m_distance;
    //if this distance is within +- a radius' distance from the plane, the two intersect
    if (abs(distance) <= radiusOther)
    {
        //if theres been a collision,
        collision.collided = true;
        //Calculate contact points:
        Contact contact = Contact();
        //the plane only has one face so the normal will always be the plane's
        contact.normal = m_normal;
        //The sphere has definitely intersected, so the distance from the plane will be the amount intersected, as its distance from the other side.
        contact.penetration = -distance;
        //Get the point of collision along the line of the normal from the position
        contact.point = positionOther - m_normal * (distance + radiusOther);
        //write this contact to the collision
        collision.contacts.push_back(&contact);
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
        //Calculate contact points:

    }
    return collision;
}

float PlaneCollider::CalculateDistanceFromPoint(Vector3 point) const
{
    return m_normal * point;
}
