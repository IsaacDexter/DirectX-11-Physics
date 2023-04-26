#pragma once
#include "Collider.h"
#include <array>

class AABBCollider :
    public Collider
{
public:
    /// <summary>An Axis Aligned Bounding Box Collider encapsules an object's collision in a single non-orientable box according to the object's length, height and width in the x, y, z directions. It's an efficient, but often inaccurate form of collision.</summary>
    /// <param name="transform">The object's transform component, used to get it's position</param>
    /// <param name="halfExtents">The object's half extents, or radii in the x, y, z directions. Use half the object's length, height and width respectively</param>
    AABBCollider(Transform* transform, Vector3 halfExtents);
    /// <summary>An Axis Aligned Bounding Box Collider encapsules an object's collision in a single non-orientable box according to the object's length, height and width in the x, y, z directions. It's an efficient, but often inaccurate form of collision.</summary>
    /// <param name="transform">The object's transform component, used to get it's position</param>
    /// <param name="rx">The object's half extents, or radii in the x direction. Use half the object's length</param>
    /// <param name="ry">The object's half extents, or radii in the y direction. Use half the object's height</param>
    /// <param name="rz">The object's half extents, or radii in the z direction. Use half the object's width</param>
    AABBCollider(Transform* transform, float rx, float ry, float rz);
    /// <summary>An Axis Aligned Bounding Box Collider encapsules an object's collision in a single non-orientable box according to the object's length, height and width in the x, y, z directions. It's an efficient, but often inaccurate form of collision.</summary>
    /// <param name="transform">The object's transform component, used to get it's position</param>
    /// <param name="halfExtents">The object's half extents, or radii in the x, y, z directions. Use half the object's length, height and width respectively</param>
    /// <param name="centre">The offset from the object's centre in the x, y, z directions to use as the new centre for this bounding box. Useful for objects that aren't uniform</param>
    AABBCollider(Transform* transform, Vector3 halfExtents, Vector3 centre);
    /// <summary>An Axis Aligned Bounding Box Collider encapsules an object's collision in a single non-orientable box according to the object's length, height and width in the x, y, z directions. It's an efficient, but often inaccurate form of collision.</summary>
    /// <param name="transform">The object's transform component, used to get it's position</param>
    /// <param name="rx">The object's half extents, or radii in the x direction. Use half the object's length</param>
    /// <param name="ry">The object's half extents, or radii in the y direction. Use half the object's height</param>
    /// <param name="rz">The object's half extents, or radii in the z direction. Use half the object's width</param>
    /// <param name="dx">The offset from the object's centre in the x direction to use as the new centre for this bounding box. Useful for objects that aren't uniform</param>
    /// <param name="dy">The offset from the object's centre in the y direction to use as the new centre for this bounding box. Useful for objects that aren't uniform</param>
    /// <param name="dz">The offset from the object's centre in the z direction to use as the new centre for this bounding box. Useful for objects that aren't uniform</param>
    AABBCollider(Transform* transform, float rx, float ry, float rz, float dx, float dy, float dz);

    /// <summary>For other collider types, simply call a collision check from that collider</summary>
    /// <param name="other">A collider of unspecified type</param>
    /// <returns>Whether or not the two colliders are overlapping</returns>
    virtual Collision CollidesWith(Collider& other) override;
    /// <summary>If the distance between the centres of the objects is greater than the sum of half extents in all directions, there has been a collision</summary>
    /// <param name="other">The other object's AABB collider</param>
    /// <returns>Whether or not the two colliders are overlapping</returns>
    virtual Collision CollidesWith(AABBCollider& other) override;
    /// <summary>Finds the distance (squared) between the closest edge of the bounding box and the centre of the circle. If this distance is less than the radius (squared) of the sphere, the objects have collided</summary>
    /// <param name="other">The other object's sphere collider</param>
    /// <returns>whether or not there was a collision</returns>
    virtual Collision CollidesWith(SphereCollider& other) override;
    /// <summary>Performs a closest point operation and returns it for the out vector</summary>
    /// <param name="other">The sphere collider to check for collision with</param>
    /// <param name="out">the closest point, to the collision, regardless of if there was a collision</param>
    /// <returns>whether or not there was a collision</returns>
    virtual Collision CollidesWith(SphereCollider& other, Vector3& out) ;

    virtual Collision CollidesWith(PlaneCollider& other) override;

    /// <returns>The object's centre (the objects position + it's centre offset)</returns>
    Vector3 GetCentre() const { return m_centre + GetPosition(); };
    /// <param name="centre">The object's local offset from it's local origin for it's centre</param>
    void SetCentre(Vector3 centre) { m_centre = centre; };
    /// <param name="centreX">The object's local offset in the x direction from it's local origin for it's centre</param>
    /// <param name="centreY">The object's local offset in the y direction from it's local origin for it's centre</param>
    /// <param name="centreZ">The object's local offset in the z direction from it's local origin for it's centre</param>
    void SetCentre(float centreX, float centreY, float centreZ) { m_centre.x = centreX; m_centre.y = centreY, m_centre.z = centreZ; };
    
    /// <returns>The object's length height and width, halved, or it's radii in the x, y and z directions, respectively</returns>
    Vector3 GetHalfExtents() const { return m_halfExtents; };
    /// <param name="halfExtents">The object's length height and width, halved, or it's radii in the x, y and z directions, respectively</param>
    void SetHalfExtents(Vector3 halfExtents) { m_halfExtents = halfExtents; };
    /// <param name="halfExtentsX">The object's length, halved, or it's radii in the x direction</param>
    /// <param name="halfExtentsY">The object's height, halved, or it's radii in the y direction</param>
    /// <param name="halfExtentsZ">The object's width, halved, or it's radii in the z direction</param>
    void SetHalfExtents(float halfExtentsX, float halfExtentsY, float halfExtentsZ) { m_halfExtents.x = halfExtentsX; m_halfExtents.y = halfExtentsY, m_halfExtents.z = halfExtentsZ; };

    std::array<Vector3, 8> GetVertices() const;

    /// <returns>The position - the half extents</returns>
    Vector3 GetMin() const { return m_transform->GetPosition() - m_halfExtents; };
    /// <returns>The position + the half extents</returns>
    Vector3 GetMax() const { return m_transform->GetPosition() + m_halfExtents; };

#pragma region Calculations

    /// <summary>Finds the squared distance between a point and this Axis-Aligned Bounding Box</summary>
    /// <param name="point">The point, p</param>
    /// <returns>The distance, squared, between the two.</returns>
    float DistanceSq(Vector3 point);

    /// <summary>Finds the distance between a point and this Axis-Aligned Bounding Box</summary>
    /// <param name="point">The point, p</param>
    /// <returns>The distance, between the two.</returns>
    float Distance(Vector3 point);

    /// <summary>Finds the closest point on this Axis-Aligned Bounding Box to a point</summary>
    /// <param name="point">The point, p</param>
    /// <returns>A point on this AABB that is closest to the p</returns>
    Vector3 ClosestPoint(Vector3 point);

    Vector3 GetXAxis() const { return Vector3(1.0f, 0.0f, 0.0f); };
    Vector3 GetYAxis() const { return Vector3(0.0f, 1.0f, 0.0f); };
    Vector3 GetZAxis() const { return Vector3(0.0f, 0.0f, 1.0f); };

    float TransformToAxis(const Vector3& axis);

    bool overlapOnAxis(AABBCollider& other, const Vector3& axis);

#pragma endregion


protected:
    /// <summary>The object's local offset from it's local origin for it's centre</summary>
    Vector3 m_centre;
    /// <summary>The object's length height and width, halved, or it's radii in the x, y and z directions, respectively</summary>
    Vector3 m_halfExtents;
};

