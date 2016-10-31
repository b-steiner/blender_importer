#pragma once

#include "../blender_importer.hpp"

BEGIN_BDL_BLI

class BLI_EXPORT bli_matrix4
{
	float m_m[16];

public:
	bli_matrix4();
	bli_matrix4(float m[16]);
	bli_matrix4(const bli_matrix4& vector);

	bli_matrix4& operator = (const bli_matrix4& vector);

	float* m();
	const float* m() const;
};

END_BDL_BLI
