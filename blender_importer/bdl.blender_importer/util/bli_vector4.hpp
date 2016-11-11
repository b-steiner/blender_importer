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

BEGIN_BDL_BLI

/*! \brief Stores a 4 element vector
*
* \author bdl
*/
class BLI_EXPORT bli_vector4
{
	//! Stores the x coordinate
	PROPERTY2(float, x, GET, SET);
	//! Stores the y coordinate
	PROPERTY2(float, y, GET, SET);
	//! Stores the z coordinate
	PROPERTY2(float, z, GET, SET);
	//! Stores the w coordinate
	PROPERTY2(float, w, GET, SET);

public:
	/*! \brief Initializes a new instance of the bli_vector4 class
	*/
	bli_vector4();
	/*! \brief Initializes a new instance of the bli_vector4 class
	 *
	 * \param x The x value
	 * \param y The y value
	 * \param z The z value
	 * \param w The w value
	 */
	bli_vector4(float x, float y, float z, float w);
	/*! \brief Initializes a new instance of the bli_vector4 class
	*
	* \param vector Another vector to copy the data from
	*/
	bli_vector4(const bli_vector4& vector);

	/*! \brief Copy assignment operator
	*
	* \param vector Another vector to copy the data from
	* \returns A reference to the current object
	*/
	bli_vector4& operator = (const bli_vector4& vector);
};

END_BDL_BLI