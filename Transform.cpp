#include "Transform.h"

#include "GameObject.h"

Transform::Transform()
{
	m_position = Vector3();
	m_rotation = Vector3();
	m_scale = Vector3(1.0f, 1.0f, 1.0f);

	m_parent = nullptr;
}

Transform::~Transform()
{
	m_parent = nullptr;
}

void Transform::CalculateWorldMatrix()
{
	// Calculate world matrix
	XMMATRIX scale = GetScaleMatrix();
	XMMATRIX rotation = GetRotationMatrix();
	XMMATRIX translation = GetTranslationMatrix();

	XMStoreFloat4x4(&m_world, scale * rotation * translation);

	if (m_parent != nullptr)
	{
		XMStoreFloat4x4(&m_world, this->GetWorldMatrix() * m_parent->GetTransform()->GetWorldMatrix());
	}
}
