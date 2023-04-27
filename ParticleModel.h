#ifndef PARTICLE_MODEL_H
#define PARTICLE_MODEL_H

#include "PhysicsModel.h"
class ParticleModel :
    public PhysicsModel
{
public:
    ParticleModel(Transform* transform, float mass = 1.0f, float restitution = 0.5f);
    virtual void Update(float dt) override;
    //As particles do not really have points as they are only one point, simply apply the force
    virtual void AddRelativeForce(Vector3 force, Vector3 point);
};

#endif // !PARTICLE_MODEL_H