#include <bdl.blender_importer/blender_importer.hpp>
#include "node.hpp"

using namespace bdl::blender_importer;

node::node(const std::string& name) : m_name(name), m_lightsource(nullptr), m_material(nullptr), m_mesh(nullptr), m_animation(nullptr)
{ }
node::~node()
{
	if (m_lightsource != nullptr)
		delete m_lightsource;

	for (auto node : m_nodes)
		delete node;
}