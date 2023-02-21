#pragma once

#include <directxmath.h>
#include <d3d11_4.h>
#include <string>
#include "Structures.h"
#include "Vector3.h"
#include "Transform.h"

using namespace DirectX;
using namespace std;

class GameObject
{
public:
	GameObject(string type, Geometry geometry, Material material, Transform* transform);
	~GameObject();

	string GetType() const { return _type; }

	Geometry GetGeometryData() const { return _geometry; }

	Material GetMaterial() const { return _material; }

	Transform* GetTransform() const { return _pTransform; }

	void SetTextureRV(ID3D11ShaderResourceView * textureRV) { _textureRV = textureRV; }
	ID3D11ShaderResourceView * GetTextureRV() const { return _textureRV; }
	bool HasTexture() const { return _textureRV ? true : false; }

	void Update(float t);
	void Draw(ID3D11DeviceContext * pImmediateContext);

private:
	string _type;

	Geometry _geometry;
	Material _material;
	ID3D11ShaderResourceView * _textureRV;

	Transform* _pTransform;
};

