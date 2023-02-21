#pragma once

#include <directxmath.h>
#include <d3d11_4.h>
#include <string>
#include "Vector3.h"

#include "Structures.h"
#include "Transform.h"
#include "Appearance.h"

using namespace DirectX;
using namespace std;

class GameObject
{
public:
	GameObject(string type, Appearance* appearance, Transform* transform);
	~GameObject();

	Appearance* GetAppearance() const { return _appearance; }
	Transform* GetTransform() const { return _transform; }

	string GetType() const { return _type; }

	void Update(float t);
	void Draw(ID3D11DeviceContext * pImmediateContext);

private:
	Transform* _transform;
	Appearance* _appearance;
	
	string _type;
};

