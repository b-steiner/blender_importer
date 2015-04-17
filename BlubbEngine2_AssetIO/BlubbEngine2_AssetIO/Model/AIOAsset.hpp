#pragma once

#include <string>

namespace AssetIO
{
	class AIOAsset
	{
	public:
		AIOAsset(const std::string& _path);
		~AIOAsset();

		const std::string& Path() const;

	private:
		std::string path;

	};
}