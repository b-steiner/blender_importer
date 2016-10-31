#pragma once

#include "../blender_importer.hpp"
#include "texture.hpp"
#include "../util/bli_vector3.hpp"

BEGIN_BDL_BLI

	enum class material_mode
	{
		none,
		transparent,
		shadeless
	};

	class BLI_EXPORT material
	{
		PROPERTY1(std::string, name, GET_CONST_REF);
		PROPERTY2(bli_vector3, diffuse_color, GET, SET);
		PROPERTY2(bli_vector3, specular_color, GET, SET);
		PROPERTY2(float, diffuse_intensity, GET, SET);
		PROPERTY2(float, specular_intensity, GET, SET);
		PROPERTY2(float, ambient_intensity, GET, SET);
		PROPERTY2(float, emitting_intensity, GET, SET);
		PROPERTY2(short, hardness, GET, SET);
		PROPERTY2(float, opacity, GET, SET);
		PROPERTY2(material_mode, mode, GET, SET);

		typedef std::unordered_map<mapping_target, texture*> mapping_texture_map;
		PROPERTY1(mapping_texture_map, textures, GET_REF);

	public:
		material(const std::string& name);
		~material();
	};

END_BDL_BLI

inline bdl::blender_importer::material_mode operator| (bdl::blender_importer::material_mode a, bdl::blender_importer::material_mode b)
{
	return (bdl::blender_importer::material_mode)((int)a | (int)b);
}
inline bdl::blender_importer::material_mode operator& (bdl::blender_importer::material_mode a, bdl::blender_importer::material_mode b)
{
	return (bdl::blender_importer::material_mode)((int)a & (int)b);
}