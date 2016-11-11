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

/*! \brief Stores a 4x4 matrix
 *
 * \author bdl
 */
class BLI_EXPORT bli_matrix4
{
	float m_m[16];

public:
	/*! \brief Initializes a new instance of the bli_matrix4 class
	*/
	bli_matrix4();
	/*! \brief Initializes a new instance of the bli_matrix4 class
	*
	* \param m A array containing 16 float values to initialize the matrix
	*/
	bli_matrix4(float m[16]);
	/*! \brief Initializes a new instance of the bli_matrix4 class
	*
	* \param matrix Another matrix to copy the data from
	*/
	bli_matrix4(const bli_matrix4& matrix);

	/*! \brief Copy assignment operator
	 *
	 * \param matrix Another matrix to copy the data from
	 * \returns A reference to the current object
	 */
	bli_matrix4& operator = (const bli_matrix4& matrix);

	/*! \brief Returns a pointer to the data array
	 *
	 * \returns A pointer to the data array
	 */
	float* m();
	/*! \brief Returns a pointer to the data array
	*
	* \returns A pointer to the data array
	*/
	const float* m() const;
};

END_BDL_BLI
