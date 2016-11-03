#pragma once

#include <string>
#include <vector>

#include "../blender_importer.hpp"
#include "../util/bli_vector2.hpp"
#include "../util/bli_vector3.hpp"
#include "../util/bli_vector4.hpp"

BEGIN_BDL_BLI

	class BLI_EXPORT mesh
	{
		PROPERTY1(std::string, name, GET_CONST_REF);
		PROPERTY1(std::vector<bli_vector3>, positions, GET_REF);
		PROPERTY1(std::vector<bli_vector3>, normals, GET_REF);
		PROPERTY1(std::vector<std::vector<bli_vector4>>, tangents, GET_REF);
		PROPERTY1(std::vector<unsigned int>, indices, GET_REF);
		PROPERTY1(std::vector<std::vector<bli_vector2>>, tex_coords, GET_REF);

	public:
		mesh(const std::string& name);
		~mesh();
	};

END_BDL_BLI