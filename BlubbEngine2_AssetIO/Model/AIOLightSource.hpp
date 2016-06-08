#pragma once

#include "../dllconfig.hpp"
#include "AIOVector3.hpp"

namespace AssetIO
{
	enum class AIOLightSourceType
	{
		PointLight,
		SpotLight,
		DirectionalLight
	};

	class _AIO_DECLSPEC AIOLightSource
	{
	public:
		AIOLightSource(const std::string& name);
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
		inline float Distance() const;
		inline void Distance(float _value);
		bool HasClippingSphere();
		void HasClippingSphere(bool _value);

		inline AIOLightSourceType Type() const;
		inline void Type(AIOLightSourceType _value);

		inline const std::string& Name() const;

	private:
		float intensity;
		float angle;
		float angularAttenuation;
		float distance;
		std::string name;
		
		bool hasClippingSphere;

		AIOVector3 color;
		AIOVector3 attenuation;

		AIOLightSourceType type;
	};
}