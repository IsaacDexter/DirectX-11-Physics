#pragma once
#include "Collider.h"

class AABBCollider :
    public Collider
{
public:
    AABBCollider(Transform* transform, Vector3 centre, Vector3 halfExtents);
    AABBCollider(Transform* transform, float dx, float dy, float dz, float rx, float ry, float rz);

    virtual bool CollidesWith(Collider& other) override;
    virtual bool CollidesWith(AABBCollider& other);
    virtual bool CollidesWith(SphereCollider& other) override;
    /// <summary>Performs a closest point operation and returns it for the out vector</summary>
    /// <param name="other">The sphere collider to check for collision with</param>
    /// <param name="out">the closest point, to the collision, regardless of if there was a collision</param>
    /// <returns>whether or not there was a collision</returns>
    virtual bool CollidesWith(SphereCollider& other, Vector3& out) ;

    Vector3 GetCentre() const { return m_centre; };
    void SetCentre(Vector3 centre) { m_centre = centre; };
    void SetCentre(float centreX, float centreY, float centreZ) { m_centre.x = centreX; m_centre.y = centreY, m_centre.z = centreZ; };
    
    Vector3 GetHalfExtents() const { return m_halfExtents; };
    void SetHalfExtents(Vector3 halfExtents) { m_halfExtents = halfExtents; };
    void SetHalfExtents(float halfExtentsX, float halfExtentsY, float halfExtentsZ) { m_halfExtents.x = halfExtentsX; m_halfExtents.y = halfExtentsY, m_halfExtents.z = halfExtentsZ; };

    /// <returns>The position - the half extents</returns>
    Vector3 GetMin() const { return m_transform->GetPosition() - m_halfExtents; };
    /// <returns>The position + the half extents</returns>
    Vector3 GetMax() const { return m_transform->GetPosition() + m_halfExtents; };

protected:
    Vector3 m_centre;
    Vector3 m_halfExtents;
};

