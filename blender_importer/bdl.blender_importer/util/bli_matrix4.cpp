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
#include "bli_matrix4.hpp"

using namespace bdl::blender_importer;

bli_matrix4::bli_matrix4()
{
	memset(m_m, 0, 16);
	m_m[0] = 1;
	m_m[5] = 1;
	m_m[10] = 1;
	m_m[15] = 1;
}

bli_matrix4::bli_matrix4(float m[16])
{
	memcpy(m_m, m, 16 * sizeof(float));
}
bli_matrix4::bli_matrix4(const bli_matrix4& vector)
{
	memcpy(m_m, vector.m(), 16 * sizeof(float));
}

bli_matrix4& bli_matrix4::operator = (const bli_matrix4& vector)
{
	memcpy(m_m, vector.m(), 16 * sizeof(float));
	return *this;
}

float* bli_matrix4::m() { return m_m; }
const float* bli_matrix4::m() const { return m_m; }
