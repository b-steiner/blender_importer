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