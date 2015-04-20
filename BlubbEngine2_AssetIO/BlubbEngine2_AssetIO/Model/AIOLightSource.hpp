#pragma once

#include "AIOVector3.hpp"

namespace AssetIO
{
	enum class AIOLightSourceType
	{
		PointLight,
		SpotLight,
		DirectionalLight
	};

	class AIOLightSource
	{
	public:
		AIOLightSource();
		~AIOLightSource();

		inline float Intensity() const;
		inline void Intensity(float _value);
		inline float Angle() const;
		inline void Angle(float _value);
		inline float AngularAttenuation() const;
		inline void AngularAttenuation(float _value);

		inline const AIOVector3& Color() const;
		inline void Color(const AIOVector3& _value);
		inline const AIOVector3& Attenuation() const;
		inline void Attenuation(const AIOVector3& _value);

		inline AIOLightSourceType Type() const;
		inline void Type(AIOLightSourceType _value);

	private:
		float intensity;
		float angle;
		float angularAttenuation;

		AIOVector3 color;
		AIOVector3 attenuation;

		AIOLightSourceType type;
	};
}