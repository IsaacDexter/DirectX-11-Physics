#include "Appearance.h"

Appearance::Appearance(Geometry geometry, Material material) : m_geometry(geometry), m_material(material)
{
	m_textureResourceView = nullptr;
}

Appearance::~Appearance()
{
	m_textureResourceView = nullptr;
}
