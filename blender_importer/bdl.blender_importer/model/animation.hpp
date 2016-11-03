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
#include "../util/bli_vector2.hpp"

BEGIN_BDL_BLI

	enum class interpolation_mode
	{
		linear, bezier
	};

	enum class interpolation_target
	{
		locX, locY, locZ,
		rotX, rotY, rotZ,
		sizeX, sizeY, sizeZ
	};

	class BLI_EXPORT animation
	{
		PROPERTY1(std::string, name, GET_CONST_REF);
		PROPERTY1(std::vector<std::vector<bli_vector2>>, points, GET_REF);
		PROPERTY1(std::vector<std::vector<bli_vector2>>, prev_handles, GET_REF);
		PROPERTY1(std::vector<std::vector<bli_vector2>>, next_handles, GET_REF);
		PROPERTY1(std::vector<interpolation_mode>, interpolation_mode, GET_REF);
		PROPERTY1(std::vector<interpolation_target>, target, GET_REF);

	public:
		animation(const std::string& name);
		~animation();
	};

END_BDL_BLI