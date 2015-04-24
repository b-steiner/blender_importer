#include "AIOException.hpp"

using namespace AssetIO;

AIOException::AIOException(const std::string& _message) : message(_message)
{ }

const std::string& AIOException::Message() const
{
	return message;
}