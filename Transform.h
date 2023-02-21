#pragma once

#include <directxmath.h>
#include "Vector3.h"

using namespace DirectX;

class Transform
{
public:
	Transform();
	~Transform();

	// Setters and Getters for position/rotation/scale
	void SetPosition(Vector3 position) { _position = position; }
	void SetPosition(float x, float y, float z) { _position.x = x; _position.y = y; _position.z = z; }

	Vector3 GetPosition() const { return _position; }
	XMMATRIX GetTranslationMatrix() { return XMMatrixTranslation(_position.x, _position.y, _position.z); }

	void SetScale(Vector3 scale) { _scale = scale; }
	void SetScale(float x, float y, float z) { _scale.x = x; _scale.y = y; _scale.z = z; }

	Vector3 GetScale() const { return _scale; }
	XMMATRIX GetScaleMatrix() { return XMMatrixScaling(_scale.x, _scale.y, _scale.z); }

	void SetRotation(Vector3 rotation) { _rotation = rotation; }
	void SetRotation(float x, float y, float z) { _rotation.x = x; _rotation.y = y; _rotation.z = z; }

	Vector3 GetRotation() const { return _rotation; }
	XMMATRIX GetRotationMatrix() { return XMMatrixRotationX(_rotation.x) * XMMatrixRotationY(_rotation.y) * XMMatrixRotationZ(_rotation.z); }

private:
	Vector3 _position;
	Vector3 _rotation;
	Vector3 _scale;
};

