#pragma once

#include <string>
#include <vector>

#include "../dllconfig.hpp"
#include "AIOMatrix4.hpp"
#include "AIOVector3.hpp"
#include "AIOMesh.hpp"
#include "AIOMaterial.hpp"
#include "AIOLightSource.hpp"
#include "AIOAnimation.hpp"

namespace AssetIO
{
	class _AIO_DECLSPEC AIONode
	{
	public:
		AIONode(const std::string& _name);
		~AIONode();

		const std::string& Name() const;
		std::vector<AIONode*>& Nodes();

		AIOMesh* Mesh() const;
		void Mesh(AIOMesh* _value);
		AIOMaterial* Material() const;
		void Material(AIOMaterial* _value);
		AIOAnimation* Animation() const;
		void Animation(AIOAnimation* _value);
		AIOLightSource* LightSource() const;
		void LightSource(AIOLightSource* _value);

		const AIOMatrix4& Matrix() const;
		void Matrix(const AIOMatrix4& _value);
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
		AIOMatrix4 matrix;
		AIOVector3 translation;
		AIOVector3 scale;
		AIOVector3 rotation;
	};
}