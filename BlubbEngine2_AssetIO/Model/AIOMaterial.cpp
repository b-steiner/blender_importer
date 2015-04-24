#include "AIOMaterial.hpp"

using namespace AssetIO;

AIOMaterial::AIOMaterial(const std::string& _name) : name(_name), diffuseColor(0.8f, 0.8f, 0.8f), diffuseIntensity(0.8f), specularColor(1.0f, 1.0f, 1.0f), specularIntensity(0.5f),
ambientIntensity(1.0f), emittingIntensity(0.0f), hardness(50), opacity(1.0f), mode(AIOMaterialMode::None)
{ }
AIOMaterial::~AIOMaterial()
{
}

inline const std::string& AIOMaterial::Name() const
{
	return name;
}

inline const AIOVector3& AIOMaterial::DiffuseColor() const
{
	return diffuseColor;
}
inline void AIOMaterial::DiffuseColor(const AIOVector3& _value)
{
	diffuseColor = _value;
}
inline float AIOMaterial::DiffuseIntensity() const
{
	return diffuseIntensity;
}
inline void AIOMaterial::DiffuseIntensity(float _value)
{
	diffuseIntensity = _value;
}

inline const AIOVector3& AIOMaterial::SpecularColor() const
{
	return specularColor;
}
inline void AIOMaterial::SpecularColor(const AIOVector3& _value)
{
	specularColor = _value;
}
inline float AIOMaterial::SpecularIntensity() const
{
	return specularIntensity;
}
inline void AIOMaterial::SpecularIntensity(float _value)
{
	specularIntensity = _value;
}

inline float AIOMaterial::AmbientIntensity() const
{
	return ambientIntensity;
}
inline void AIOMaterial::AmbientIntensity(float _value)
{
	ambientIntensity = _value;
}
inline float AIOMaterial::EmittingIntensity() const
{
	return emittingIntensity;
}
inline void AIOMaterial::EmittingIntensity(float _value)
{
	emittingIntensity = _value;
}

inline short AIOMaterial::Hardness() const
{
	return hardness;
}
inline void AIOMaterial::Hardness(short _value)
{
	hardness = _value;
}
inline float AIOMaterial::Opacity() const
{
	return opacity;
}
inline void AIOMaterial::Opacity(float _value)
{
	opacity = _value;
}

inline AIOMaterialMode AIOMaterial::Mode() const
{
	return mode;
}
inline void AIOMaterial::Mode(AIOMaterialMode _value)
{
	mode = _value;
}

std::unordered_map<AIOMappingTarget, AIOTexture*>& AIOMaterial::Textures()
{
	return textures;
}