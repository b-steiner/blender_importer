#pragma once

#include "../blender_importer.hpp"
#include "node.hpp"

BEGIN_BDL_BLI

	//class AIOBlenderImporter;

	class BLI_EXPORT asset
	{
		//friend AIOBlenderImporter;

		PROPERTY1(std::string, path, GET_CONST_REF);
		PROPERTY1(std::vector<node*>, nodes, GET_REF);

	public:
		asset(const std::string& path);
		~asset();

	private:
		//Cache
		std::unordered_map<std::string, mesh*> m_meshes;
		std::unordered_map<std::string, material*> m_materials;
		std::unordered_map<std::string, texture*> m_textures;
	};

END_BDL_BLI