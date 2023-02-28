#pragma once

#include <directxmath.h>
#include "Vector3.h"

using namespace DirectX;

class GameObject;

class Transform
{
public:
	Transform();
	~Transform();

	// Setters and Getters for position/rotation/scale
	void SetPosition(Vector3 position) { position = position; }
	void SetPosition(float x, float y, float z) { m_position.x = x; m_position.y = y; m_position.z = z; }

	Vector3 GetPosition() const { return m_position; }
	XMMATRIX GetTranslationMatrix() const { return XMMatrixTranslation(m_position.x, m_position.y, m_position.z); }

	void SetScale(Vector3 scale) { scale = scale; }
	void SetScale(float x, float y, float z) { m_scale.x = x; m_scale.y = y; m_scale.z = z; }

	Vector3 GetScale() const { return m_scale; }
	XMMATRIX GetScaleMatrix() const { return XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z); }

	void SetRotation(Vector3 rotation) { rotation = rotation; }
	void SetRotation(float x, float y, float z) { m_rotation.x = x; m_rotation.y = y; m_rotation.z = z; }

	Vector3 GetRotation() const { return m_rotation; }
	XMMATRIX GetRotationMatrix() const { return XMMatrixRotationX(m_rotation.x) * XMMatrixRotationY(m_rotation.y) * XMMatrixRotationZ(m_rotation.z); }

	void CalculateWorldMatrix();

	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&m_world); }

	void SetParent(GameObject* parent) { parent = parent; }

private:
	/// <summary>The objects position relative to it's</summary>
	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_scale;

	XMFLOAT4X4 m_world;

	GameObject* m_parent;
};

