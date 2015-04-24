#include "AIOTexture.hpp"

using namespace AssetIO;

AIOTexture::AIOTexture() : path(""), colorSpace(AIOColorSpace::Other)
{ }
AIOTexture::~AIOTexture()
{ }

inline const std::string& AIOTexture::Path() const
{
	return path;
}
inline void AIOTexture::Path(const std::string& _value)
{
	path = _value;
}

inline AIOColorSpace AIOTexture::ColorSpace() const
{
	return colorSpace;
}
inline void AIOTexture::ColorSpace(AIOColorSpace _value)
{
	colorSpace = _value;
}