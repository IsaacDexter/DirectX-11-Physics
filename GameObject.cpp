#include "GameObject.h"

GameObject::GameObject(string type, Geometry geometry, Material material, Transform* transform) : _geometry(geometry), _type(type), _material(material), _pTransform(transform)
{
	_parent = nullptr;

	_textureRV = nullptr;
}

GameObject::~GameObject()
{
	_textureRV = nullptr;
	_parent = nullptr;
	delete _pTransform;
	_pTransform = nullptr;
}

void GameObject::Update(float t)
{
	// Calculate world matrix
	XMMATRIX scale = XMMatrixScaling(_pTransform->GetScale().x, _pTransform->GetScale().y, _pTransform->GetScale().z);
	XMMATRIX rotation = XMMatrixRotationX(_pTransform->GetRotation().x) * XMMatrixRotationY(_pTransform->GetRotation().y) * XMMatrixRotationZ(_pTransform->GetRotation().z);
	XMMATRIX translation = XMMatrixTranslation(_pTransform->GetPosition().x, _pTransform->GetPosition().y, _pTransform->GetPosition().z);

	XMStoreFloat4x4(&_world, scale * rotation * translation);

	if (_parent != nullptr)
	{
		XMStoreFloat4x4(&_world, this->GetWorldMatrix() * _parent->GetWorldMatrix());
	}
}

void GameObject::Draw(ID3D11DeviceContext * pImmediateContext)
{
	// NOTE: We are assuming that the constant buffers and all other draw setup has already taken place

	// Set vertex and index buffers
	pImmediateContext->IASetVertexBuffers(0, 1, &_geometry.vertexBuffer, &_geometry.vertexBufferStride, &_geometry.vertexBufferOffset);
	pImmediateContext->IASetIndexBuffer(_geometry.indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	pImmediateContext->DrawIndexed(_geometry.numberOfIndices, 0, 0);
}
