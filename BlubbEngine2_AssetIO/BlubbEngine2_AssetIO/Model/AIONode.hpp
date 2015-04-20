#pragma once

#include <string>
#include <vector>

#include "AIOMatrix4.hpp"
#include "AIOVector3.hpp"
#include "AIOMesh.hpp"
#include "AIOMaterial.hpp"
#include "AIOLightSource.hpp"
#include "AIOAnimation.hpp"

namespace AssetIO
{
	class AIONode
	{
	public:
		AIONode(const std::string& _name);
		~AIONode();

		const std::string& Name() const;
		std::vector<AIONode*>& Nodes();

		const AIOMesh* Mesh() const;
		void Mesh(AIOMesh* _value);
		const AIOMaterial* Material() const;
		void Material(AIOMaterial* _value);
		const AIOAnimation* Animation() const;
		void Animation(AIOAnimation* _value);
		const AIOLightSource* LightSource() const;
		void LightSource(AIOLightSource* _value);

		const AIOMatrix4& Matrix() const;
		const AIOVector3 Translation() const;
		void Translation(const AIOVector3& _value);
		const AIOVector3 Scale() const;
		void Scale(const AIOVector3& _value);
		const AIOVector3 Rotation() const;
		void Rotation(const AIOVector3& _value);

	private:
		std::vector<AIONode*> nodes;
		std::string name;

		AIOMesh* mesh;
		AIOMaterial* material;
		AIOAnimation* animation;
		AIOLightSource* lightsource;
		AIOVector3 translation;
		AIOVector3 scale;
		AIOVector3 rotation;
	};
}