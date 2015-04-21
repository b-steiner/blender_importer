#pragma once

#include <string>
#include <vector>
#include "../dllconfig.hpp"
#include "AIONode.hpp"

namespace AssetIO
{
	class _AIO_DECLSPEC AIOAsset
	{
	public:
		AIOAsset(const std::string& _path);
		~AIOAsset();

		const std::string& Path() const;

		std::vector<AIONode*>& Nodes();

	private:
		std::string path;
		std::vector<AIONode*> nodes;

	};
}