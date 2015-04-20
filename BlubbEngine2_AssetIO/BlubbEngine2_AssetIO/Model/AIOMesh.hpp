#pragma once

#include <string>
#include <vector>

#include "AIOVector2.hpp"
#include "AIOVector3.hpp"
#include "AIOVector4.hpp"

namespace AssetIO
{
	class AIOMesh
	{
	public:
		AIOMesh(const std::string& _name);
		~AIOMesh();

		const std::string& Name() const;

		std::vector<AIOVector3>& Positions();
		std::vector<AIOVector3>& Normals();
		std::vector<AIOVector4>& Tangents();
		std::vector<std::vector<AIOVector2>>& TexCoords();

		std::vector<unsigned int>& Indices();

	private:
		std::string name;
		std::vector<AIOVector3> positions;
		std::vector<AIOVector3> normals;
		std::vector<AIOVector4> tangents;
		std::vector<unsigned int> indices;
		std::vector<std::vector<AIOVector2>> texCoords;
	};
}