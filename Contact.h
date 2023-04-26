#pragma once
#include "Vector3.h"
#include <vector>

/// <summary><para>Represents two bodies in contact.</para>
/// <para>Resolving removes its interpenetration, and applies sufficient impulse to keep them apart</para>
/// <para>Colliding bodies may also rebound</para></summary>
struct Contact
{
	/// <summary>The world position of the contact.</summary>
	Vector3 point;
	/// <summary>The direction of the contact in world coordinates.</summary>
	Vector3 normal;
	/// <summary>The depth of penetration at the contact point. If both bodies are specifeid then the contact point should be midway between the interpenetrating points</summary>
	float penetration;
	Contact()
	{
		point = Vector3();
		normal = Vector3();
		penetration = 0.0f;
	}
	/// <summary><para>point-face: Point–face contacts are the most common and important type of contact. 
	/// Whether the face is flat or curved, the contact properties are generated in the same way.</para>
	/// <para>edge-edge: Edge–edge contacts are the second most important type of contact and are critical for resting contacts between objects with flat or concave sides. </para></summary>
	/// <param name="contactPoint"><para>point-face: the point involved in the contact.</para>
	/// <para>edge-edge: closest point on one edge to the other</para></param>
	/// <param name="contactNormal"><para>point-face: The contact normal is given by the normal of the surface at the point of contact.
	/// If the object point(i.e., the point that is in contact with the face) is penetrated into the surface, 
	/// then it is usually projected back onto the surface in order to determine where the contact is measured from.</para>
	/// <para>edge-edge: The contact normal is at right angles to the tangents of both edges. 
	/// The vector product is used to calculate this.</para></param>
	/// <param name="penetration"><para>point-face: The penetration depth is calculated as the distance between the object point and the projected point.</para>
	/// <para>edge-edge: The penetration depth is the distance between the two edges.</para></param>
	Contact(Vector3 contactPoint, Vector3 contactNormal, float penetration)
	{
		this->point = contactPoint;
		this->normal = contactPoint;
		this->penetration = penetration;
	}
};

/// <summary>Holds data relavent to a collision such as contact points, whether there was a collision, the bodies involved</summary>
struct Collision
{
	bool collided;
	/// <summary><para>To represent a face-face collision, rather than storing a great many points, simply store each of the four corners of collision.</para>
	/// <para>To represent an edge collision, store the points on either side of the colliding edge.</para></summary>
	std::vector<Contact*> contacts;
	Collision()
	{
		collided = false;
		contacts = std::vector<Contact*>();
	}
	Collision(bool collision)
	{
		this->collided = collided;
		contacts = std::vector<Contact*>();
	}
	Collision(bool collision, std::vector<Contact*> contacts)
	{
		this->collided = collided;
		this->contacts = contacts;
	}
};