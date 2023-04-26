#pragma once
#include "PhysicsModel.h"
class StaticModel : public PhysicsModel
{
public:
	/// <summary>A physics model that does not move or update its forces. It will not move under any circumstances.</summary>
	/// <param name="transform">The a pointer to the transform object, which is used to apply these forces</param>
	/// <param name="mass">The object's mass, in kg. Defaults to 1 kg</param>
	/// <param name="restitution">The object's coefficient restitution, from 0 - 1, where 0 is an inelastic object and 1 is a perfectly elastic one</param>
	StaticModel(Transform* transform, float mass, float restitution);
	virtual void Update(float dt) override;
	/// <summary>Prevents the application of an impulse, so it can have no collision response
	virtual void ApplyImpulse(Vector3 impulse) override;
};

