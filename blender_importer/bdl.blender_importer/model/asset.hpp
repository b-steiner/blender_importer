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


#pragma once

#include "../blender_importer.hpp"
#include "node.hpp"

BEGIN_BDL_BLI

	class importer;

	class BLI_EXPORT asset
	{
		friend importer;

		PROPERTY1(std::string, path, GET_CONST_REF);
		PROPERTY1(std::vector<node*>, nodes, GET_REF);

	public:
		asset(const std::string& path);
		~asset();

	private:
		//Cache
		std::unordered_map<std::string, mesh*> m_meshes;
		std::unordered_map<std::string, material*> m_materials;
		std::unordered_map<std::string, texture*> m_textures;
	};

END_BDL_BLI