#pragma once

#include "../dllconfig.hpp"

#include <string>

namespace AssetIO
{
	class _AIO_DECLSPEC AIOException
	{
	public:
		AIOException(const std::string& _message);

		const std::string& Message() const;

	private:
		std::string message;
	};
}