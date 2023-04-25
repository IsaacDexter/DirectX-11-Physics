#pragma once
#include "Collider.h"
class PlaneCollider :
    public Collider
{
public:
    /// <summary>A plane represents an infinite plane using a normal and a distance to the origin from that normal. To represent a non-infinite plane, use a flattened bounding box.</summary>
    /// <param name="transform">The transform component used for position and orientation</param>
    /// <param name="normal">The normal vector of the plane, representing the direction perpendicular to it</param>
    /// <param name="distance">The distance from the origin, in m, of the plane along the normal</param>
    PlaneCollider(Transform* transform, Vector3 normal, float distance);
    /// <summary>A plane represents an infinite plane using a normal and a distance to the origin from that normal. To represent a non-infinite plane, use a flattened bounding box.</summary>
    /// <param name="transform">The transform component used for position and orientation</param>
    /// <param name="normal">The normal vector of the plane, representing the direction perpendicular to it</param>
    /// <param name="point">A point, X, lying on the plane</param>
    PlaneCollider(Transform* transform, Vector3 normal, Vector3 point);

    /// <summary>For other collider types, simply call a collision check from that collider</summary>
    /// <param name="other">A collider of unspecified type</param>
    /// <returns>Whether or not the two colliders are overlapping</returns>
    virtual Collision CollidesWith(Collider& other) override;
    /// <summary>Find the distance between the sphere's centre and the plane, and if its less than a radius away, the two collided.</summary>
    /// <param name="other">The other object's AABB collider</param>
    /// <returns>Whether or not the two colliders are overlapping</returns>
    virtual Collision CollidesWith(AABBCollider& other) override;
    /// <summary></summary>
    /// <param name="other">The other object's sphere collider</param>
    /// <returns>whether or not there was a collision</returns>
    virtual Collision CollidesWith(SphereCollider& other) override;
    /// <summary>Checks to see if two planes' normals (and by extension, themselves) are parralell. If they aren't, the two planes overlap at somepoint, as they are infinite.</summary>
    /// <param name="other">The other object's plane collider</param>
    /// <returns>whether or not there was a collision</returns>
    virtual Collision CollidesWith(PlaneCollider& other) override;

    /// <returns>The normal vector of the plane, representing the direction perpendicular to it</returns>
    Vector3 GetNormal() const { return m_normal; };
    /// <param name="normal">The normal vector of the plane, representing the direction perpendicular to it</param>
    void SetNormal(Vector3 normal) { m_normal = normal.Normalized(); };

    /// <returns>A point, X, lying on the plane</returns>
    float GetDistance() const { return m_distance; };
    /// <param name="point">A point, X, lying on the plane</param>
    void SetDistance(float distance) { m_distance = distance; };
    /// <param name="point">A point, X, lying on the plane</param>
    void SetDistance(Vector3 point) { m_distance = CalculateDistanceFromPoint(point); };

protected:
    Vector3 m_normal;
    float m_distance;

    /// <summary>Finds the distance along the normal given a point using (n dot x) = d. Used in the constructor and certain set statements</summary>
    /// <param name="point">A point, X, lying on the plane</param>
    /// <returns>The distance from the origin, in m, of the plane along the normal</returns>
    float CalculateDistanceFromPoint(Vector3 point) const;
};

