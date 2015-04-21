#pragma once

#include "../dllconfig.hpp"

namespace AssetIO
{
	class _AIO_DECLSPEC AIOVector2
	{
	public:
		AIOVector2(float _x, float _y);
		AIOVector2(const AIOVector2& _value);
		~AIOVector2();

		float* Values() const;

		AIOVector2& operator=(const AIOVector2& _other);

	private:
		float values[2];
	};
}