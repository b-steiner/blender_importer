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


#include <bdl.blender_importer/blender_importer.hpp>
#include "bli_vector3.hpp"

using namespace bdl::blender_importer;

bli_vector3::bli_vector3() : m_x(0), m_y(0), m_z(0) { }

bli_vector3::bli_vector3(float * values) : m_x(values[0]), m_y(values[1]), m_z(values[2]) { }

bli_vector3::bli_vector3(float x, float y, float z) : m_x(x), m_y(y), m_z(z) { }

bli_vector3::bli_vector3(const bli_vector3 & vector) : m_x(vector.x()), m_y(vector.y()), m_z(vector.z()) { }

bli_vector3 & bli_vector3::operator=(const bli_vector3 & vector)
{
	m_x = vector.x();
	m_y = vector.y();
	m_z = vector.z();
	return *this;
}
