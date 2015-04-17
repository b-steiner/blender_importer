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

		AIOVector3* Positions() const;
		void Positions(AIOVector3* _value);
		AIOVector3* Normals() const;
		void Normals(AIOVector3* _value);
		AIOVector4* Tangents() const;
		void Tangents(AIOVector4* _value);
		const std::vector<AIOVector2>& TexCoords() const;
		void TexCoords(std::vector<AIOVector2> _value) const;

		unsigned int* Indices() const;
		void Indices(unsigned int* _value);

	private:
		std::string name;
	};
}