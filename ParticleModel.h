#ifndef PARTICLE_MODEL_H
#define PARTICLE_MODEL_H

#include "PhysicsModel.h"
class ParticleModel :
    public PhysicsModel
{
public:
    ParticleModel(Transform* transform, float mass);
    virtual void Update(float dt) override;
};

#endif // !PARTICLE_MODEL_H