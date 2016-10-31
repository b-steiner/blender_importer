#include <bdl.blender_importer/blender_importer.hpp>
#include "material.hpp"

using namespace bdl::blender_importer;

material::material(const std::string& name) : m_name(name), m_diffuse_color(0.8f, 0.8f, 0.8f), m_diffuse_intensity(0.8f), m_specular_color(1.0f, 1.0f, 1.0f), m_specular_intensity(0.5f),
m_ambient_intensity(1.0f), m_emitting_intensity(0.0f), m_hardness(50), m_opacity(1.0f), m_mode(material_mode::none)
{ }

material::~material()
{
	for (auto& tex : m_textures)
		delete tex.second;

	m_textures.clear();
}