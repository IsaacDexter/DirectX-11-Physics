#ifndef _COLLIDER_H
#define _COLLIDER_H

#include "Transform.h"
#include "Contact.h"

class SphereCollider;
class AABBCollider;
class PlaneCollider;


class Collider abstract
{
protected:
	Transform* m_transform;
public:
	Collider(Transform* transform);
	
	virtual Collision CollidesWith(Collider& other) = 0;
	virtual Collision CollidesWith(SphereCollider& other) = 0;
	virtual Collision CollidesWith(AABBCollider& other) = 0;
	virtual Collision CollidesWith(PlaneCollider& other) = 0;

	Vector3 GetPosition() const { return m_transform->GetPosition(); };

};

#endif // !_COLLIDER_H