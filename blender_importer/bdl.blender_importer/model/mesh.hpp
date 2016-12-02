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

	/*! \brief Represents a triangle mesh
	 *
	 * \author bdl
	 */
	class BLI_EXPORT mesh
	{
		//! Stores the name of the mesh
		PROPERTY1(std::string, name, GET_CONST_REF);
		//! Stores a list of vertex positions
		PROPERTY1(std::vector<bli_vector3>, positions, GET_REF);
		//! Stores a list of vertex normals
		PROPERTY1(std::vector<bli_vector3>, normals, GET_REF);
		//! Stores a list of vertex tangents. The w component describes whether the binormal points to the positive or to the negative halfspace.
		PROPERTY1(std::vector<std::vector<bli_vector4>>, tangents, GET_REF);
		//! Stores a list of indices into the positons/normals/tangents list
		PROPERTY1(std::vector<unsigned int>, indices, GET_REF);
		//! Stores multiple sets of uv coordinates.
		PROPERTY1(std::vector<std::vector<bli_vector2>>, tex_coords, GET_REF);
		PROPERTY1(std::vector<std::string>, tex_coord_names, GET_REF);

	public:
		/*! \brief Initializes a new instance of the mesh class
		 *
		 * \param name The name of the mesh
		 */
		mesh(const std::string& name);
		/*! \brief Releases all data associated with an instance of the mesh
		 */
		~mesh();
	};

END_BDL_BLI