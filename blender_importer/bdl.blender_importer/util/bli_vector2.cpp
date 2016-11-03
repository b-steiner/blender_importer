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
#include "bli_vector2.hpp"

using namespace bdl::blender_importer;

bli_vector2::bli_vector2() : m_x(0), m_y(0) { }

bli_vector2::bli_vector2(float x, float y) : m_x(x), m_y(y) { }

bli_vector2::bli_vector2(const bli_vector2 & vector) : m_x(vector.x()), m_y(vector.y()) { }

bli_vector2 & bli_vector2::operator=(const bli_vector2 & vector)
{
	m_x = vector.x();
	m_y = vector.y();
	return *this;
}
