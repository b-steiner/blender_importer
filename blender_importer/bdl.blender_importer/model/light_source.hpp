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

	//! Enumeration for the type of a light source
	enum class light_source_type
	{
		pointlight,
		spotlight,
		directionallight
	};

	/*! \brief Stores data for a light_source
	*
	* \author bdl
	*/
	class BLI_EXPORT light_source
	{
		//! Stores the intensity
		PROPERTY2(float, intensity, GET, SET);
		//! Stores the opening angle for spot lights
		PROPERTY2(float, angle, GET, SET);
		//! Stores the angular attenuation in [0, 1] range for spot lights
		PROPERTY2(float, angular_attenuation, GET, SET);
		//! Stores the maximum influence distance
		PROPERTY2(float, distance, GET, SET);
		//! Stores the name of the light source
		PROPERTY1(std::string, name, GET_CONST_REF);

		//! Stores whether the lightsource is clipped after a fixed distance
		PROPERTY2(bool, has_clipped_sphere, GET, SET);

		//! Stores the light's color
		PROPERTY2(bli_vector3, color, GET_CONST_REF, SET);
		//! Stores the attenuation. x = constant, y = linear, z = quadratic
		PROPERTY2(bli_vector3, attenuation, GET_CONST_REF, SET);

		//! Stores the type of the light source
		PROPERTY2(light_source_type, type, GET, SET);

	public:
		/*! \brief Initializes a new instance of the light_source class
		 *
		 * \param name The name of the light source
		 */
		light_source(const std::string& name);
		/*! \brief Releases all data associated with an instance of the light_source
		 */
		~light_source();
	};

END_BDL_BLI