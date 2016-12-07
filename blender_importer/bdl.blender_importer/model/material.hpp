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
#include "texture.hpp"
#include "../util/bli_vector3.hpp"

BEGIN_BDL_BLI

	//! Enumeration containing different special shading modes
	enum class material_mode
	{
		//! No special properties
		none = 0,
		//! Material is transparent
		transparent = 1,
		//! Material is shadeless
		shadeless = 2
	};

	/*! \brief Contains material properties
	 *
	 * \author bdl
	 */
	class BLI_EXPORT material
	{
		//! Stores the name of the material
		PROPERTY1(std::string, name, GET_CONST_REF);
		//! Stores the diffuse rgb color
		PROPERTY2(bli_vector3, diffuse_color, GET, SET);
		//! Stores the specular rgb color
		PROPERTY2(bli_vector3, specular_color, GET, SET);
		//! Stores the intensity of diffuse reflections
		PROPERTY2(float, diffuse_intensity, GET, SET);
		//! Stores the specular intensity
		PROPERTY2(float, specular_intensity, GET, SET);
		//! Stores the intensity of ambient illumination
		PROPERTY2(float, ambient_intensity, GET, SET);
		//! Stores how much light the material emits by itself
		PROPERTY2(float, emitting_intensity, GET, SET);
		//! Stores the specular exponent
		PROPERTY2(short, hardness, GET, SET);
		//! Stores the opacity of the material
		PROPERTY2(float, opacity, GET, SET);
		//! Stores a binary combination of special material flags
		PROPERTY2(material_mode, mode, GET, SET);

		typedef std::unordered_map<mapping_target, texture*> mapping_texture_map;
		typedef std::unordered_map<mapping_target, float> mapping_float_map;
		typedef std::unordered_map<mapping_target, std::string> mapping_string_map;

		//! Stores the textures used in this material
		PROPERTY1(mapping_texture_map, textures, GET_REF);
		//! Stores the influence factor for each texture target. This will have different meanings depending on the target
		PROPERTY1(mapping_float_map, texture_influence, GET_REF);
		//! Stores the name of the uv coordinate layer for each texture target. This is the same name present in mesh::tex_coord_names
		PROPERTY1(mapping_string_map, texture_uv_name, GET_REF);

	public:
		/*! \brief Initializes a new instance of the material class
		 *
		 * \param name The name of the material
		 */
		material(const std::string& name);
		/*! \brief Releases all data associated with an instance of the material class
		 */
		~material();
	};

END_BDL_BLI

/* \brief Binary or combination of multiple material mode flags
 *
 * \param a First mode
 * \param b Second mode
 * \returns The binary or of a and b
 */
inline bdl::blender_importer::material_mode operator| (bdl::blender_importer::material_mode a, bdl::blender_importer::material_mode b)
{
	return (bdl::blender_importer::material_mode)((int)a | (int)b);
}
/* \brief Binary and combination of multiple material mode flags
*
* \param a First mode
* \param b Second mode
* \returns The binary and of a and b
*/
inline bdl::blender_importer::material_mode operator& (bdl::blender_importer::material_mode a, bdl::blender_importer::material_mode b)
{
	return (bdl::blender_importer::material_mode)((int)a & (int)b);
}