#pragma once
#include "PhysicsModel.h"
class ParticleModel :
    public PhysicsModel
{
public:
    ParticleModel(Transform* transform);
    virtual void Update(float dt) override;
};

