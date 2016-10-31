#include <bdl.blender_importer/blender_importer.hpp>
#include "asset.hpp"

using namespace bdl::blender_importer;

asset::asset(const std::string& path) : m_path(path)
{
	m_materials["default-material"] = new material("default-material");
}
asset::~asset()
{
	for(auto node : m_nodes)
		delete node;

	for (auto del : m_meshes)
		delete del.second;
	for (auto del : m_materials)
		delete del.second;
	for (auto del : m_textures)
		delete del.second;
}