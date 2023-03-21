#pragma once
#include "PhysicsModel.h"
class ParticleModel :
    public PhysicsModel
{
public:
    ParticleModel(Transform* transform, float mass);
    virtual void Update(float dt) override;
};

