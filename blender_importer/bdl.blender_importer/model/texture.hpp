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
#include <string>

BEGIN_BDL_BLI

	enum class mapping_target
	{
		diffuse, normals, other
	};

	enum class color_space
	{
		linear, SRGB, other
	};

	class BLI_EXPORT texture
	{
		PROPERTY2(std::string, path, GET_CONST_REF, SET);
		PROPERTY2(blender_importer::color_space, color_space, GET_CONST_REF, SET);

	public:
		texture();
		~texture();		
	};

END_BDL_BLI