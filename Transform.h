#pragma once

#include <directxmath.h>
#include "Quaternion.h"

using namespace DirectX;

class GameObject;

class Transform
{
public:
	Transform();
	~Transform();

	// Setters and Getters for position/rotation/scale
	void SetPosition(Vector3 position) { m_position = position; }
	void SetPosition(float x, float y, float z) { m_position.x = x; m_position.y = y; m_position.z = z; }

	Vector3 GetPosition() const { return m_position; }
	XMMATRIX GetTranslationMatrix() const { return XMMatrixTranslation(m_position.x, m_position.y, m_position.z); }

	void SetScale(Vector3 scale) { m_scale = scale; }
	void SetScale(float x, float y, float z) { m_scale.x = x; m_scale.y = y; m_scale.z = z; }

	Vector3 GetScale() const { return m_scale; }
	XMMATRIX GetScaleMatrix() const { return XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z); }

	void SetRotation(Vector3 rotation) { m_orientation = MakeQFromEulerAngles(rotation.x, rotation.y, rotation.z); }
	void SetRotation(float x, float y, float z) { m_orientation = MakeQFromEulerAngles(x, y, z); }
	Vector3 GetRotation() const { return MakeEulerAnglesFromQ(m_orientation); }

	void SetOrientation(Quaternion orientation) { m_orientation = orientation; };
	Quaternion GetOrientation() const { return m_orientation; }

	XMMATRIX GetRotationMatrix() const { return XMMatrixRotationQuaternion(XMVectorSet(m_orientation.v.x, m_orientation.v.y, m_orientation.v.z, m_orientation.n)); }

	void CalculateWorldMatrix();

	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&m_world); }

	void SetParent(GameObject* parent) { parent = parent; }

private:
	/// <summary>The objects position relative to it's</summary>
	Vector3 m_position;
	Quaternion m_orientation;
	Vector3 m_scale;

	XMFLOAT4X4 m_world;

	GameObject* m_parent;
};

