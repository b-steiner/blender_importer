#pragma once

#include <string>
#include <unordered_map>

#include "../dllconfig.hpp"
#include "AIOTexture.hpp"
#include "AIOVector3.hpp"

namespace AssetIO
{
	enum class AIOMaterialMode
	{
		None,
		Transparent,
		Shadeless
	};

	class _AIO_DECLSPEC AIOMaterial
	{
	public:
		AIOMaterial(const std::string& _name);
		~AIOMaterial();

		inline const std::string& Name() const;

		inline const AIOVector3& DiffuseColor() const;
		inline void DiffuseColor(const AIOVector3& _value);
		inline float DiffuseIntensity() const;
		inline void DiffuseIntensity(float _value);

		inline const AIOVector3& SpecularColor() const;
		inline void SpecularColor(const AIOVector3& _value);
		inline float SpecularIntensity() const;
		inline void SpecularIntensity(float _value);

		inline float AmbientIntensity() const;
		inline void AmbientIntensity(float _value);
		inline float EmittingIntensity() const;
		inline void EmittingIntensity(float _value);

		inline short Hardness() const;
		inline void Hardness(short _value);
		inline float Opacity() const;
		inline void Opacity(float _value);

		inline AIOMaterialMode Mode() const;
		inline void Mode(AIOMaterialMode _value);

		std::unordered_map<AIOMappingTarget, AIOTexture*>& Textures();

	private:
		std::string name;
		AIOVector3 diffuseColor;
		AIOVector3 specularColor;
		float diffuseIntensity;
		float specularIntensity;
		float ambientIntensity;
		float emittingIntensity;
		short hardness;
		float opacity;
		AIOMaterialMode mode;
		std::unordered_map<AIOMappingTarget, AIOTexture*> textures;
	};
}