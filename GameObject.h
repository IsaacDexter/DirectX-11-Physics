#pragma once

#include <directxmath.h>
#include <d3d11_4.h>
#include <string>
#include "Structures.h"
#include "Vector3.h"
#include "Transform.h"
#include "Appearance.h"

using namespace DirectX;
using namespace std;

class GameObject
{
public:
	GameObject(string type, Appearance* appearance, Transform* transform);
	~GameObject();

	string GetType() const { return _type; }

	Appearance* GetAppearance() const { return _pAppearance; }
	Transform* GetTransform() const { return _pTransform; }

	void Update(float t);
	void Draw(ID3D11DeviceContext * pImmediateContext);

private:
	string _type;

	Appearance* _pAppearance;
	Transform* _pTransform;
};

