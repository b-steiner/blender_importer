#pragma once

#include "../blender_importer.hpp"
#include "../util/bli_vector2.hpp"

BEGIN_BDL_BLI

	enum class interpolation_mode
	{
		linear, bezier
	};

	enum class interpolation_target
	{
		locX, locY, locZ,
		rotX, rotY, rotZ,
		sizeX, sizeY, sizeZ
	};

	class BLI_EXPORT animation
	{
		PROPERTY1(std::string, name, GET_CONST_REF);
		PROPERTY1(std::vector<std::vector<bli_vector2>>, points, GET_REF);
		PROPERTY1(std::vector<std::vector<bli_vector2>>, prev_handles, GET_REF);
		PROPERTY1(std::vector<std::vector<bli_vector2>>, next_handles, GET_REF);
		PROPERTY1(std::vector<interpolation_mode>, interpolation_mode, GET_REF);
		PROPERTY1(std::vector<interpolation_target>, target, GET_REF);

	public:
		animation(const std::string& name);
		~animation();
	};

END_BDL_BLI