#include "GameObject.h"

GameObject::GameObject(string type, Appearance* appearance, Transform* transform) : m_type(type), m_appearance(appearance), m_transform(transform)
{
	m_physicsModel = new RigidBodyModel(m_transform, 1.0f);
}

GameObject::~GameObject()
{
	delete m_transform;
	m_transform = nullptr;
	delete m_appearance;
	m_appearance = nullptr;
	delete m_physicsModel;
	m_physicsModel = nullptr;
}

void GameObject::Update(float dt)
{
	GetPhysicsModel()->Update(dt);
	GetTransform()->CalculateWorldMatrix();
}

void GameObject::Draw(ID3D11DeviceContext * pImmediateContext)
{
	// NOTE: We are assuming that the constant buffers and all other draw setup has already taken place

	// Set vertex and index buffers
	Geometry geometry = GetAppearance()->GetGeometryData();
	pImmediateContext->IASetVertexBuffers(0, 1, &geometry.vertexBuffer, &geometry.vertexBufferStride, &geometry.vertexBufferOffset);
	pImmediateContext->IASetIndexBuffer(geometry.indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	pImmediateContext->DrawIndexed(geometry.numberOfIndices, 0, 0);
}
