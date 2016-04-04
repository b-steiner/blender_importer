#include "AIOLightSource.hpp"

using namespace AssetIO;

AIOLightSource::AIOLightSource() : intensity(0), angle(0), angularAttenuation(0), color(0, 0, 0), attenuation(0, 0, 0),
type(AIOLightSourceType::DirectionalLight), distance(0), hasClippingSphere(false)
{ }
AIOLightSource::~AIOLightSource()
{ }

inline float AIOLightSource::Intensity() const
{
	return intensity;
}
inline void AIOLightSource::Intensity(float _value)
{
	intensity = _value;
}
inline float AIOLightSource::Angle() const
{
	return angle;
}
inline void AIOLightSource::Angle(float _value)
{
	angle = _value;
}
inline float AIOLightSource::AngularAttenuation() const
{
	return angularAttenuation;
}
inline void AIOLightSource::AngularAttenuation(float _value)
{
	angularAttenuation = _value;
}

inline const AIOVector3& AIOLightSource::Color() const
{
	return color;
}
inline void AIOLightSource::Color(const AIOVector3& _value)
{
	color = _value;
}
inline const AIOVector3& AIOLightSource::Attenuation() const
{
	return attenuation;
}
inline void AIOLightSource::Attenuation(const AIOVector3& _value)
{
	attenuation = _value;
}

inline AIOLightSourceType AIOLightSource::Type() const
{
	return type;
}
inline void AIOLightSource::Type(AIOLightSourceType _value)
{
	type = _value;
}

inline float AIOLightSource::Distance() const
{
	return distance;
}
inline void AIOLightSource::Distance(float _value)
{
	distance = _value;
}
bool AIOLightSource::HasClippingSphere()
{
	return hasClippingSphere;
}
void AIOLightSource::HasClippingSphere(bool _value)
{
	hasClippingSphere = _value;
}