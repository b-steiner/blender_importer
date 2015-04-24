#pragma once

#include "../dllconfig.hpp"
#include <string>

namespace AssetIO
{
	enum class AIOMappingTarget
	{
		Diffuse, Normals, Other
	};

	enum class AIOColorSpace
	{
		Linear, SRGB, Other
	};

	class _AIO_DECLSPEC AIOTexture
	{
	public:
		AIOTexture();
		~AIOTexture();

		inline const std::string& Path() const;
		inline void Path(const std::string& _value);

		inline AIOColorSpace ColorSpace() const;
		inline void ColorSpace(AIOColorSpace _value);

	private:
		std::string path;
		AIOColorSpace colorSpace;
	};
}