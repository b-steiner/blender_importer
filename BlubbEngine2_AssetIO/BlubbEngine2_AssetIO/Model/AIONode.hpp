#pragma once

#include <string>
#include <vector>

#include "AIOMatrix4.hpp"
#include "AIOVector3.hpp"
#include "AIOMesh.hpp"
#include "AIOMaterial.hpp"

namespace AssetIO
{
	class AIONode
	{
	public:
		AIONode(const std::string& _name);
		~AIONode();

		const std::string& Name() const;
		const std::vector<AIONode*>& Nodes() const;

		const AIOMesh* Mesh() const;
		const AIOMaterial* Material() const;
		const AIOAnimation* Animation() const;
		const AIOLightSource* LightSource() const;

		const AIOMatrix4 Matrix() const;
		const AIOVector3 Translation() const;
		const AIOVector3 Scale() const;
		const AIOVector3 Rotation() const;

	private:
		std::vector<AIONode*> nodes;
		std::string name;
	};
}