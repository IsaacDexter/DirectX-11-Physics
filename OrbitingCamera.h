#pragma once
#include "Camera.h"
#include <algorithm>
#include "Vector3.h"

class OrbitingCamera :
    public Camera
{
public:
	OrbitingCamera(XMFLOAT3 position, XMFLOAT3 at, XMFLOAT3 up, FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);
	~OrbitingCamera();

	void Update(float dt) override;

	void ZoomIn();
	void ZoomOut();
	void OrbitLeft();
	void OrbitRight();

	float GetRadius() const { return m_radius; };
	void SetRadius(float radius) { m_radius = std::clamp(radius, m_radiusMin, m_radiusMax); };
	void SetRadius(float radius, float radiusMin, float radiusMax) { m_radius = 7.0f, m_radiusMin = radiusMin; m_radiusMax = radiusMax; };

	float GetAngleXZ() const { return m_angleXZ; };
	void SetAngleXZ(float angle) { m_angleXZ = angle; };

	float GetSpeed() const { return m_speed; };
	void SetSpeed(float speed) { m_speed = speed; };

private:
	Vector3 m_velocity;

	float m_radius = 7.0f;
	float m_radiusMin = 2.0f;
	float m_radiusMax = 50.0f;
	float m_angleXZ = -90.0f;
	float m_speed = 30.0f;
};

