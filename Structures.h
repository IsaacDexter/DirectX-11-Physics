#pragma once
#include <cstring>
#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 TexC;

	bool operator<(const SimpleVertex other) const
	{
		return memcmp((void*)this, (void*)&other, sizeof(SimpleVertex)) > 0;
	};
};

struct MeshData
{
	/// <summary>A memory buffer that contains vertex data that can be rendered in the render pipleline.</summary>
	ID3D11Buffer* VertexBuffer;
	/// <summary>Used alongside the vertex buffer to structure them into the various shapes and primitives.
	/// <para>If vertex buffer holds the points in space, the index buffer stores how those points can be organised into shapes.</para></summary>
	ID3D11Buffer* IndexBuffer;
	/// <summary>Referes to the spacing in the buffer. Should be set to the size of each of the vertices.</summary>
	UINT VBStride;
	/// <summary>How much to offset the vertex buffer by.</summary>
	UINT VBOffset;
	/// <summary>The number of indices in the index buffer.</summary>
	UINT IndexCount;
};

struct SurfaceInfo
{
	XMFLOAT4 AmbientMtrl;
	XMFLOAT4 DiffuseMtrl;
	XMFLOAT4 SpecularMtrl;
};

struct Light
{
	XMFLOAT4 AmbientLight;
	XMFLOAT4 DiffuseLight;
	XMFLOAT4 SpecularLight;

	float SpecularPower;
	XMFLOAT3 LightVecW;
};

struct ConstantBuffer
{
	XMMATRIX World;
	XMMATRIX View;
	XMMATRIX Projection;

	SurfaceInfo surface;

	Light light;

	XMFLOAT3 EyePosW;
	float HasTexture;
};

struct Geometry
{
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int numberOfIndices;

	UINT vertexBufferStride;
	UINT vertexBufferOffset;
};

struct Material
{
	XMFLOAT4 diffuse;
	XMFLOAT4 ambient;
	XMFLOAT4 specular;
	float specularPower;
};