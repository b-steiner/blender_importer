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

#include <string>
#include <vector>

#include "../blender_importer.hpp"
#include "../util/bli_vector2.hpp"
#include "../util/bli_vector3.hpp"
#include "../util/bli_vector4.hpp"

BEGIN_BDL_BLI

	class BLI_EXPORT mesh
	{
		PROPERTY1(std::string, name, GET_CONST_REF);
		PROPERTY1(std::vector<bli_vector3>, positions, GET_REF);
		PROPERTY1(std::vector<bli_vector3>, normals, GET_REF);
		PROPERTY1(std::vector<std::vector<bli_vector4>>, tangents, GET_REF);
		PROPERTY1(std::vector<unsigned int>, indices, GET_REF);
		PROPERTY1(std::vector<std::vector<bli_vector2>>, tex_coords, GET_REF);

	public:
		mesh(const std::string& name);
		~mesh();
	};

END_BDL_BLI