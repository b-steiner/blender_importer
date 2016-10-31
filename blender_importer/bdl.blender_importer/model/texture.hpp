#pragma once

#include "../blender_importer.hpp"
#include <string>

BEGIN_BDL_BLI

	enum class mapping_target
	{
		diffuse, normals, other
	};

	enum class color_space
	{
		linear, SRGB, other
	};

	class BLI_EXPORT texture
	{
		PROPERTY2(std::string, path, GET_CONST_REF, SET);
		PROPERTY2(blender_importer::color_space, color_space, GET_CONST_REF, SET);

	public:
		texture();
		~texture();		
	};

END_BDL_BLI