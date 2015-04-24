#pragma once

#include <string>
#include <vector>
#include "../dllconfig.hpp"
#include "AIONode.hpp"

namespace AssetIO
{
	class AIOBlenderImporter;

	class _AIO_DECLSPEC AIOAsset
	{
		friend AIOBlenderImporter;

	public:
		AIOAsset(const std::string& _path);
		~AIOAsset();

		const std::string& Path() const;

		std::vector<AIONode*>& Nodes();

	private:
		std::string path;
		std::vector<AIONode*> nodes;

		//Cache
		std::unordered_map<std::string, AIOMesh*> meshes;
		std::unordered_map<std::string, AIOMaterial*> materials;
		std::unordered_map<std::string, AIOTexture*> textures;
	};
}