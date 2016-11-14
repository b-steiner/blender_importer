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

	//! Enumeration with different targets for textures
	enum class mapping_target
	{
		//! The texture contains diffuse colors
		diffuse,
		//! The texture contains normals
		normals, 
		//! The texture contains unknown data
		other
	};

	//! Enumeration with different color spaces
	enum class color_space
	{
		//! Colors are linearly stored
		linear,
		//! Colors are in sRGB space
		SRGB,
		//! The colorspace is unknown
		other
	};

	/*! \brief Stores information about a texture
	 *
	 * \author bdl
	 */
	class BLI_EXPORT texture
	{
		//! Stores the file path (relative to the .blend file)
		PROPERTY2(std::string, path, GET_CONST_REF, SET);
		//! Stores the colorspace in which the texture is stored
		PROPERTY2(blender_importer::color_space, color_space, GET_CONST_REF, SET);

	public:
		/*! \brief Initializes a new instance of the texture class
		 */
		texture();
		/*! \brief Releases all data associated with an instance of the texture
		 */
		~texture();		
	};

END_BDL_BLI