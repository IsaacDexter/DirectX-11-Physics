#include "GameObject.h"

GameObject::GameObject(string type, Geometry geometry, Material material, Transform* transform) : _geometry(geometry), _type(type), _material(material), _pTransform(transform)
{
	_textureRV = nullptr;
}

GameObject::~GameObject()
{
	_textureRV = nullptr;
	delete _pTransform;
	_pTransform = nullptr;
}

void GameObject::Update(float t)
{
	_pTransform->CalculateWorldMatrix();
}

void GameObject::Draw(ID3D11DeviceContext * pImmediateContext)
{
	// NOTE: We are assuming that the constant buffers and all other draw setup has already taken place

	// Set vertex and index buffers
	pImmediateContext->IASetVertexBuffers(0, 1, &_geometry.vertexBuffer, &_geometry.vertexBufferStride, &_geometry.vertexBufferOffset);
	pImmediateContext->IASetIndexBuffer(_geometry.indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	pImmediateContext->DrawIndexed(_geometry.numberOfIndices, 0, 0);
}
