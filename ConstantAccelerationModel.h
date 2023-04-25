#ifndef CONSTANT_ACCELERATION_MODEL_H
#define CONSTANT_ACCELERATION_MODEL_H

#include "PhysicsModel.h"
class ConstantAccelerationModel :
    public PhysicsModel
{
    /// <summary>The constant acceleration model is a physics model where forces have no effect, it will move according to its acceleration only</summary>
   /// <param name="transform">The transform component used to update position</param>
   /// <param name="mass">Unused in physics calculations</param>
    ConstantAccelerationModel(Transform* transform, float mass = 1.0f, float restitution = 0.5f);
    virtual void Update(float dt) override;
};

#endif // !CONSTANT_ACCELERATION_MODEL_H