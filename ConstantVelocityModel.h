#ifndef CONSTANT_VELOCITY_MODEL_H
#define CONSTANT_VELOCITY_MODEL_H

#include "PhysicsModel.h"
class ConstantVelocityModel :
    public PhysicsModel
{
    /// <summary>The constant velocity model is a physics model where acceleration and forces have no effect, it will move according to its velocity only</summary>
    /// <param name="transform">The transform component used to update position</param>
    /// <param name="mass">Unused in physics calculations</param>
    ConstantVelocityModel(Transform* transform, float mass, float restitution);
    virtual void Update(float dt) override;
};

#endif // !CONSTANT_VELOCITY_MODEL_H