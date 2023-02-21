#include "Transform.h"

#include "GameObject.h"

Transform::Transform()
{
	_position = Vector3();
	_rotation = Vector3();
	_scale = Vector3(1.0f, 1.0f, 1.0f);

	_parent = nullptr;
}

Transform::~Transform()
{
	_parent = nullptr;
}

void Transform::CalculateWorldMatrix()
{
	// Calculate world matrix
	XMMATRIX scale = GetScaleMatrix();
	XMMATRIX rotation = GetRotationMatrix();
	XMMATRIX translation = GetTranslationMatrix();

	XMStoreFloat4x4(&_world, scale * rotation * translation);

	if (_parent != nullptr)
	{
		XMStoreFloat4x4(&_world, this->GetWorldMatrix() * _parent->GetTransform()->GetWorldMatrix());
	}
}
