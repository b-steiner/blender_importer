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
#include "../util/bli_vector3.hpp"

BEGIN_BDL_BLI

	enum class light_source_type
	{
		pointlight,
		spotlight,
		directionallight
	};

	class BLI_EXPORT light_source
	{
		PROPERTY2(float, intensity, GET, SET);
		PROPERTY2(float, angle, GET, SET);
		PROPERTY2(float, angular_attenuation, GET, SET);
		PROPERTY2(float, distance, GET, SET);
		PROPERTY1(std::string, name, GET_CONST_REF);

		PROPERTY2(bool, has_clipped_sphere, GET, SET);

		PROPERTY2(bli_vector3, color, GET_CONST_REF, SET);
		PROPERTY2(bli_vector3, attenuation, GET_CONST_REF, SET);

		PROPERTY2(light_source_type, type, GET, SET);

	public:
		light_source(const std::string& name);
		~light_source();
	};

END_BDL_BLI