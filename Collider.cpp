#include "Collider.h"

#include "AABBCollider.h"


Collider::Collider(Transform* transform)
{
	m_transform = transform;
}

float Collider::DistanceSq(Vector3 p, AABBCollider b)
{
	float distanceSq = 0.0f;

	//For each axis, count any excess distance outside box extents
	//x:
	if (p.x < b.GetMin().x)	//if the point is below the minimum of this axis,
	{
		distanceSq += (b.GetMin().x - p.x); //add the distance, 
		distanceSq *= distanceSq;			//squared
	}
	if (p.x > b.GetMax().x)	//if the point is above the maxinum of this axis,
	{
		distanceSq += (p.x - b.GetMax().x);	//add the distance,
		distanceSq *= distanceSq;			//squared
	}
	//y:
	if (p.y < b.GetMin().y)	//if the point is below the minimum of this axis,
	{
		distanceSq += (b.GetMin().y - p.y); //add the distance, 
		distanceSq *= distanceSq;			//squared
	}
	if (p.y > b.GetMax().y)	//if the point is above the maxinum of this axis,
	{
		distanceSq += (p.y - b.GetMax().y);	//add the distance,
		distanceSq *= distanceSq;			//squared
	}
	//z:
	if (p.z < b.GetMin().z)	//if the point is below the minimum of this axis,
	{
		distanceSq += (b.GetMin().z - p.z); //add the distance, 
		distanceSq *= distanceSq;			//squared
	}
	if (p.z > b.GetMax().z)	//if the point is above the maxinum of this axis,
	{
		distanceSq += (p.z - b.GetMax().z);	//add the distance,
		distanceSq *= distanceSq;			//squared
	}
	
	return distanceSq;
}

Vector3 Collider::ClosestPoint(Vector3 p, AABBCollider b)
{
	Vector3 closestPoint = p;
	//For each axis, if the point is outside the box, clamp it to the box, otherwise keep it as is
	//x:
	closestPoint.Clamp(b.GetMin(), b.GetMax());
	return closestPoint;
}

//Vector3 Collider::ClosestPoint(Vector3 p, AABBCollider b)
//{
//	Vector3 closestPoint = p;
//	//For each axis, if the point is outside the box, clamp it to the box, otherwise keep it as is
//	//x:
//	if (p.x < b.GetMin().x)	//if the point is below the minimum of this axis,
//	{
//		closestPoint.x = b.GetMin().x;	//The closest point's value in this axis is the minimum
//	}
//	if (p.x > b.GetMax().x)	//if the point is above the maxinum of this axis,
//	{
//		closestPoint.x = b.GetMax().x;
//	}
//	//y:
//	if (p.y < b.GetMin().y)	//if the point is below the minimum of this axis,
//	{
//		closestPoint.y = b.GetMin().y;	//The closest point's value in this axis is the minimum
//	}
//	if (p.y > b.GetMax().y)	//if the point is above the maxinum of this axis,
//	{
//		closestPoint.y = b.GetMax().y;
//	}
//	//z:
//	if (p.z < b.GetMin().z)	//if the point is below the minimum of this axis,
//	{
//		closestPoint.z = b.GetMin().z;	//The closest point's value in this axis is the minimum
//	}
//	if (p.z > b.GetMax().z)	//if the point is above the maxinum of this axis,
//	{
//		closestPoint.z = b.GetMax().z;
//	}
//}

//float Collider::DistanceSq(Vector3 point, AABBCollider aabb)
//{
//	float distanceSq = 0.0f;
//	const float p[3] = { point.x, point.y, point.z };
//	const float bMin[3] = { aabb.GetMin().x, aabb.GetMin().y, aabb.GetMin().z };
//	const float bMax[3] = { aabb.GetMax().x, aabb.GetMax().y, aabb.GetMax().z };
//
//	//For each axis, count any excess distance outside box extents
//	for (unsigned int i = 0; i < 3; i++)
//	{
//		if (p[i] < bMin[i])	//if the point is below the minimum of this axis,
//		{
//			distanceSq += (bMin[i] - p[i]) * (bMin[i] - p[i]);	//add the distance, squared
//		}
//		if (p[i] < bMax[i])	//if the point is above the maxinum of this axis,
//		{
//			distanceSq += (p[i] - bMax[i]) * (p[i] - bMax[i]);	//add the distance, squared
//		}
//	}
//
//	return distanceSq;
//}
