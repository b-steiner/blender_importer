#include "AIOMesh.hpp"

using namespace AssetIO;

AIOMesh::AIOMesh(const std::string& _name) : name(_name)
{ }
AIOMesh::~AIOMesh()
{ }

const std::string& AIOMesh::Name() const
{
	return name;
}

std::vector<AIOVector3>& AIOMesh::Positions()
{
	return positions;
}
std::vector<AIOVector3>& AIOMesh::Normals()
{
	return normals;
}
std::vector<std::vector<AIOVector4>>& AIOMesh::Tangents()
{
	return tangents;
}
std::vector<std::vector<AIOVector2>>& AIOMesh::TexCoords()
{
	return texCoords;
}

std::vector<unsigned int>& AIOMesh::Indices()
{
	return indices;
}