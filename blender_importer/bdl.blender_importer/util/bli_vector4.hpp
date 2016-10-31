#pragma once

#include "../blender_importer.hpp"

BEGIN_BDL_BLI

class BLI_EXPORT bli_vector4
{
	PROPERTY2(float, x, GET, SET);
	PROPERTY2(float, y, GET, SET);
	PROPERTY2(float, z, GET, SET);
	PROPERTY2(float, w, GET, SET);

public:
	bli_vector4(float x, float y, float z, float w);
	bli_vector4(const bli_vector4& vector);

	bli_vector4& operator = (const bli_vector4& vector);
};

END_BDL_BLI