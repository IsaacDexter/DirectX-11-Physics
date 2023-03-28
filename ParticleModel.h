#ifndef PARTICLE_MODEL_H
#define PARTICLE_MODEL_H

#include "PhysicsModel.h"
class ParticleModel :
    public PhysicsModel
{
public:
    /// <summary>Initialise the particle with endless lifespan and no perturbation, meaning it acts not unlike other physics objects</summary>
    /// <param name="transform">The transform component for the physics component to affect</param>
    /// <param name="mass">The mass of the object in kg</param>
    ParticleModel(Transform* transform, float mass);
    /// <summary>Initialise the particle with a fixed lifespan and a starting force, perturbation. It will expire after its lifespan is up</summary>
    /// <param name="transform">The transform component for the physics component to affect</param>
    /// <param name="mass">The mass of the particle in kg</param>
    /// <param name="lifetime">How long, in s, for the particle to exist before it expires</param>
    /// <param name="perturbation">The initial force of the particle, in N</param>
    ParticleModel(Transform* transform, float mass, float lifetime, Vector3 perturbation);

    /// <summary>On top of the usual physics calculations, delete this particle once its lifetime is up</summary>
    /// <param name="dt">The time passed each frame</param>
    virtual void Update(float dt) override;

    /// <summary>Called when the particles lifetime has exceeded its maximum. Deletes the particle</summary>
    virtual void Expire();
private:
    /// <summary>When lifetime exceeds this, expire</summary>
    float m_maxLifetime;
    /// <summary>How long the particle has been alive</summary>
    float m_lifetime = 0.0f;
};

#endif // !PARTICLE_MODEL_H