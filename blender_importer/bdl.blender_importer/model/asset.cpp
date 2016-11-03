/**************************************************************************************
**
** Copyright (C) 2016 Bernhard Steiner
**
** This file is part of the blender_importer library
**
** This product is licensed under the GNU General Public License version 3.
** The license is as published by the Free Software Foundation published at
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** Alternatively, this product is licensed under the GNU Lesser General Public
** License version 3 for non-commercial use. The license is as published by the
** Free Software Foundation published at https://www.gnu.org/licenses/lgpl-3.0.html.
**
** Feel free to contact us if you have any questions about licensing or want
** to use the library in a commercial closed source product.
**
**************************************************************************************/


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