#ifndef PHYSICS_MODEL_H
#define PHYSICS_MODEL_H

#include "Transform.h"

class PhysicsModel
{
public:
	PhysicsModel(Transform* transform);

protected:
	Transform* m_transform;
	Vector3 m_velocity;
};

#endif // !PHYSICS_MODEL_H