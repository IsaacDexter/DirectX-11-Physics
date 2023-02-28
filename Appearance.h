#pragma once
#include "Structures.h"

class Appearance
{
public:
	Appearance(Geometry geometry, Material material);
	~Appearance();

	Geometry GetGeometryData() const { return m_geometry; }

	Material GetMaterial() const { return m_material; }

	void SetTextureRV(ID3D11ShaderResourceView* textureResourceView) { m_textureResourceView = textureResourceView; }
	ID3D11ShaderResourceView* GetTextureRV() const { return m_textureResourceView; }
	bool HasTexture() const { return m_textureResourceView ? true : false; }

private:
	Geometry m_geometry;
	Material m_material;
	ID3D11ShaderResourceView* m_textureResourceView;
};

