#include "OrbitingCamera.h"

OrbitingCamera::OrbitingCamera(XMFLOAT3 position, XMFLOAT3 at, XMFLOAT3 up, FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth) : Camera(position, at, up, windowWidth, windowHeight, nearDepth, farDepth)
{
	m_velocity = Vector3(0.0f, 0.0f, 0.0f);
}

OrbitingCamera::~OrbitingCamera()
{
}

void OrbitingCamera::Update(float dt)
{
	SetRadius(m_radius + (m_speed * dt * m_velocity.z));
	m_angleXZ += m_speed * m_velocity.x * dt;
	Camera::Update(dt);
	m_velocity = Vector3(0.0f, 0.0f, 0.0f);
}

void OrbitingCamera::ZoomIn()
{
	m_velocity.z = -0.2f;
}

void OrbitingCamera::ZoomOut()
{
	m_velocity.z = 0.2f;
}

void OrbitingCamera::OrbitLeft()
{
	m_velocity.x = -1;
}

void OrbitingCamera::OrbitRight()
{
	m_velocity.x = 1;
}
