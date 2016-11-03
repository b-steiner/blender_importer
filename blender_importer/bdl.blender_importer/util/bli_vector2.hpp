#pragma once

#include "../blender_importer.hpp"

BEGIN_BDL_BLI

class BLI_EXPORT bli_vector2
{
	PROPERTY2(float, x, GET, SET);
	PROPERTY2(float, y, GET, SET);

public:
	bli_vector2();
	bli_vector2(float x, float y);
	bli_vector2(const bli_vector2& vector);

	bli_vector2& operator = (const bli_vector2& vector);
};

END_BDL_BLI