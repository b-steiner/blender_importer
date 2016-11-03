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

class BLI_EXPORT bli_vector3
{
	PROPERTY2(float, x, GET, SET);
	PROPERTY2(float, y, GET, SET);
	PROPERTY2(float, z, GET, SET);

public:
	bli_vector3();
	bli_vector3(float* values);
	bli_vector3(float x, float y, float z);
	bli_vector3(const bli_vector3& vector);

	bli_vector3& operator = (const bli_vector3& vector);
};

END_BDL_BLI