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
	void SetPosition(Vector3 position) { _position = position; }
	void SetPosition(float x, float y, float z) { _position.x = x; _position.y = y; _position.z = z; }

	Vector3 GetPosition() const { return _position; }
	XMMATRIX GetTranslationMatrix() const { return XMMatrixTranslation(_position.x, _position.y, _position.z); }

	void SetScale(Vector3 scale) { _scale = scale; }
	void SetScale(float x, float y, float z) { _scale.x = x; _scale.y = y; _scale.z = z; }

	Vector3 GetScale() const { return _scale; }
	XMMATRIX GetScaleMatrix() const { return XMMatrixScaling(_scale.x, _scale.y, _scale.z); }

	void SetRotation(Vector3 rotation) { _rotation = rotation; }
	void SetRotation(float x, float y, float z) { _rotation.x = x; _rotation.y = y; _rotation.z = z; }

	Vector3 GetRotation() const { return _rotation; }
	XMMATRIX GetRotationMatrix() const { return XMMatrixRotationX(_rotation.x) * XMMatrixRotationY(_rotation.y) * XMMatrixRotationZ(_rotation.z); }

	void CalculateWorldMatrix();

	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

	void SetParent(GameObject* parent) { _parent = parent; }

private:
	Vector3 _position;
	Vector3 _rotation;
	Vector3 _scale;

	XMFLOAT4X4 _world;

	GameObject* _parent;
};

