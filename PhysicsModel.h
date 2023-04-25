#ifndef PHYSICS_MODEL_H
#define PHYSICS_MODEL_H

#include "Transform.h"
#include "Constants.h"
#include "Collider.h"

class PhysicsModel
{
public:	//Public Methods
	/// <param name="transform">The a pointer to the transform object, which is used to apply these forces</param>
	/// <param name="mass">The object's mass, in kg. Defaults to 1 kg</param>
	/// <param name="restitution">The object's coefficient restitution, from 0 - 1, where 0 is an inelastic object and 1 is a perfectly elastic one</param>
	PhysicsModel(Transform* transform, float mass = 1.0f, float restitution = 0.5f);
	~PhysicsModel();

	/// <summary>Calculates the object's acceleration, velocity and displacement from the forces applied to it; Appliest the displacement; Resets acceleration and force</summary>
	/// <param name="dt">The change in time since last frame</param>
	virtual void Update(float dt);

	/// <param name="force">The force to be added, in N</param>
	void AddForce(Vector3 force) { m_netforce += force; };
	/// <param name="x">The force to be added in the x direction, in N</param>
	/// <param name="y">The force to be added in the y direction, in N</param>
	/// <param name="z">The force to be added in the z direction, in N</param>
	void AddForce(float x, float y, float z) { m_netforce += Vector3(x, y, z); };

#pragma region ImpulseForces

	/// <summary>Applys a force over a given time of one tick as a vector</summary>
	/// <param name="impulse">The force to apply, in Ns</param>
	void ApplyImpulse(Vector3 impulse);

#pragma endregion

#pragma region SettersAndGetters

	/// <returns>if this model has a collider set</returns>
	bool IsCollidable() const { return m_collider != nullptr; };
	/// <summary>Deletes the model's collider and replaces it</summary>
	/// <param name="collider">This model's collider, if it has one</param>
	void SetCollider(Collider* collider) { if (m_collider != nullptr) delete m_collider; m_collider = collider; };
	/// <returns>This model's collider, if it has one</returns>
	Collider* GetCollider() const { return m_collider; };

	/// <param name="velocity">The object's velocity in m/s</param>
	void SetVelocity(Vector3 velocity) { m_velocity = velocity; }
	/// <param name="x">The object's velocity in the x direction in m/s</param>
	/// <param name="y">The object's velocity in the y direction in m/s</param>
	/// <param name="z">The object's velocity in the z direction in m/s</param>
	void SetVelocity(float x, float y, float z) { m_velocity.x = x; m_velocity.y = y; m_velocity.z = z; }
	/// <returns>The object's velocity in m/s</returns>
	Vector3 GetVelocity() const { return m_velocity; }
	
	/// <param name="velocity">The object's acceleration in m/s^2</param>
	void SetAcceleration(Vector3 acceleration) { m_acceleration = acceleration; }
	/// <param name="x">The object's acceleration in the x direction in m/s^2</param>
	/// <param name="y">The object's acceleration in the y direction in m/s^2</param>
	/// <param name="z">The object's acceleration in the z direction in m/s^2</param>
	void SetAcceleration(float x, float y, float z) { m_acceleration.x = x; m_acceleration.y = y; m_acceleration.z = z; }
	/// <returns>The object's acceleration in m/s^2</returns>
	Vector3 GetAcceleration() const { return m_acceleration; }

	/// <param name="mass">The object's mass, in kg</param>
	void SetMass(float mass) { m_mass = mass; CalculateWeight(); };
	/// <returns>The object's mass, in kg</returns>
	float GetMass() const { return m_mass; };
	
	/// <param name="restitution">The object's coefficient restitution, from 0 - 1, where 0 is an inelastic object and 1 is a perfectly elastic one</param>
	void SetRestitution(float restitution) { m_restitution = restitution; };
	/// <returns>The object's coefficient restitution, from 0 - 1, where 0 is an inelastic object and 1 is a perfectly elastic one</returns>
	float GetRestitution() const { return m_restitution; };
	
	/// <param name="g">Acceleration due to gravity, in m/s. A negative value will act downwards.</param>
	void SetGravityAcceleration(float g) { m_gravityAcceleration = g; CalculateWeight(); };
	/// <returns>Acceleration due to gravity, in m/s. A negative value will act downwards.</returns>
	float GetGravityAcceleration() { return m_gravityAcceleration; };
	/// <param name="enableGravity">Whether or not to apply the force of weight each frame.</param>
	void EnableGravity(bool enableGravity) { m_enableGravity = enableGravity; };

	/// <param name="mass">The density, in kg/m^3, of the fluid the object is moving through.</param>
	void SetFluidDensity(float fluidDensity) { m_fluidDensity = fluidDensity; CalculateDragCoefficient(); };
	/// <returns>The density, in kg/m^3, of the fluid the object is moving through.</returns>
	float GetFluidDensity() const { return m_fluidDensity; };
	
	/// <param name="mass">The drag coefficent of the object from the drag coefficients dictionary. Disables dynamic drag coefficents</param>
	void SetDragCoefficient(float dragCoefficient) { m_dragCoefficient = dragCoefficient; m_enableDynamicDragCoefficient = false; };
	/// <returns>The drag coefficent of the object from the drag coefficients dictionary</returns>
	float GetDragCoefficient() const { return m_dragCoefficient; };

	/// <param name="mass">The cross sectional area of the object perpendicular the direction of motion relative to the fluid, in m^2</param>
	void SetReferenceArea(Vector3 referenceArea) { m_referenceArea = referenceArea; };
	/// <returns>The cross sectional area of the object perpendicular the direction of motion relative to the fluid, in m^2</returns>
	Vector3 GetReferenceArea() const { return m_referenceArea; };

#pragma endregion



protected:	//Protected Methods

#pragma region Calculations

	/// <summary>Acceleration = Force / Mass</summary>
	void CalculateAcceleration();
	/// <summary>Delta Velocity = Acceleration * Delta Time. Velocity += Delta Velocity</summary>
	/// <param name="dt">Delta Time</param>
	void CalculateVelocity(float dt);
	/// <summary>Displacement = Velocity * Delta Time. Position += Displacement</summary>
	/// <param name="dt">Delta Time</param>
	void CalculateDisplacement(float dt);
	/// <summary>Resets force and acceleration to their default Vector. Should be called at the end of the update call</summary>
	void ClearForceAndAcceleration();
	/// <summary>Recalculates weight, using Fg = g * m</summary>
	void CalculateWeight();
	/// <summary>If using dynamic drag coefficients, Recalculates the drag coefficent, according to cd = 2Fd / rho * u^2 * A</summary>
	void CalculateDragCoefficient();
	/// <returns>If y is less than or equal to 1.0f, and cancels movement in the negative y</returns>
	bool IsGrounded();



#pragma endregion

#pragma region FieldForces

	/// <summary>Adds the force of weight, if m_enableGravity == true.</summary>
	void ApplyGravity();
	/// <summary>Adds the force of drag and air resistance, if m_enableDrag == true;</summary>
	void ApplyDrag();
	/// <summary>If the object is grounded, adds the force of friction parallel and opposite to the direction of motion, according to the normal equal downward force of the object.</summary>
	void ApplyFriction();

#pragma endregion

protected:	//Protected Variables
	Transform* m_transform;
	Collider* m_collider;

	/// <summary>The objects velocity in m/s</summary>
	Vector3 m_velocity;
	/// <summary>The object's acceleration in m/s^2</summary>
	Vector3 m_acceleration;

	/// <summary>The sum of all forces acting on the object, in N</summary>
	Vector3 m_netforce;
	/// <summary>The object's mass, in kg</summary>
	float m_mass;
	/// <summary>The object's coefficient restitution, from 0 - 1, where 0 is an inelastic object and 1 is a perfectly elastic one</summary>
	float m_restitution;

	/// <summary>The acceleration due to gravity, in m/s. Default value from https://en.wikipedia.org/wiki/Gravity_of_Earth</summary>
	float m_gravityAcceleration = EARTH_GRAVITY_ACCELERATION;
	/// <summary>m_mass * m_gravityAcceleration in the negative y, as a force in N</summary>
	Vector3 m_weight = Vector3(0.0f, m_gravityAcceleration, 0.0f);
	bool m_enableGravity = true;

	/// <summary>Whether or not to recalculate drag coefficient according to the drag force and flow speed each frame. Defaults to off to remain performant.</summary>
	bool m_enableDynamicDragCoefficient = false;
	/// <summary>The mass density of the fluid the object is moving through in kg/m^3. Default value from https://en.wikipedia.org/wiki/Density_of_air</summary>
	float m_fluidDensity = AIR_FLUID_DENSITY;
	/// <summary>The drag coefficient of the the object. Default value from https://en.wikipedia.org/wiki/Drag_coefficient</summary>
	float m_dragCoefficient = CUBE_DRAG_COEFFICIENT;
	/// <summary>The magnitude of the drag force acting against an object in N, used in calculating drag coefficient.</summary>
	float m_dragForceMagnitude;
	/// <summary>The projected frontal area of the object, in regards to the direction its moving.</summary>
	Vector3 m_referenceArea = CUBE_REFERENCE_AREA;
};

#endif // !PHYSICS_MODEL_H