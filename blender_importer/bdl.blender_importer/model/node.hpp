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
#include "../util/bli_matrix4.hpp"
#include "../util/bli_vector3.hpp"
#include "mesh.hpp"
#include "material.hpp"
#include "light_source.hpp"
#include "animation.hpp"

BEGIN_BDL_BLI

	/*! \brief Stores all data for a node in the scenegraph
	 *
	 * \author bdl
	 */
	class BLI_EXPORT node
	{
		//! Stores the mesh attached to the node, or nullptr when no mesh is present
		PROPERTY2(blender_importer::mesh*, mesh, GET, SET);
		//! Stores the material attached to the node, or nullptr when no mesh is present
		PROPERTY2(blender_importer::material*, material, GET, SET);
		//! Stores the animation attached to the node, or nullptr when no mesh is present
		PROPERTY2(blender_importer::animation*, animation, GET, SET);
		//! Stores the lightsource attached to the node, or nullptr when no mesh is present
		PROPERTY2(light_source*, lightsource, GET, SET);

		//! Stores a list of child nodes
		PROPERTY1(std::vector<node*>, nodes, GET_REF);
		//! Stores the name of the node
		PROPERTY1(std::string, name, GET_CONST_REF);

		//! Stores the node transformation
		PROPERTY2(bli_matrix4, matrix, GET_CONST_REF, SET);
		//! Stores the translation of the node in relation to the parent
		PROPERTY2(bli_vector3, translation, GET_CONST_REF, SET);
		//! Stores the scale of the node in relation to the parent
		PROPERTY2(bli_vector3, scale, GET_CONST_REF, SET);
		//! Stores the rotation of the node in relation to the parent
		PROPERTY2(bli_vector3, rotation, GET_CONST_REF, SET);

	public:
		/*! \brief Initializes a new instance of the node class
		 *
		 * \param name The name of the node
		 */
		node(const std::string& name);
		/*! \brief Releases all data associated with an instance of the node class
		 */
		~node();
	};

END_BDL_BLI