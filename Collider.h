#ifndef _COLLIDER_H
#define _COLLIDER_H

#include "Transform.h"

class SphereCollider;

class Collider abstract
{
protected:
	Transform* m_transform;
public:
	Collider(Transform* transform);
	
	virtual bool CollidesWith(Collider& other) = 0;
	virtual bool CollidesWith(SphereCollider& other) = 0;

	Vector3 GetPosition() const { return m_transform->GetPosition(); };
};

#endif // !_COLLIDER_H