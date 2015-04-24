#include "AIOAsset.hpp"

using namespace AssetIO;

AIOAsset::AIOAsset(const std::string& _path) : path(_path)
{
	materials["default-material"] = new AIOMaterial("default-material");
}
AIOAsset::~AIOAsset()
{
	for(auto node : nodes)
		delete node;

	for (auto del : meshes)
		delete del.second;
	for (auto del : materials)
		delete del.second;
	for (auto del : textures)
		delete del.second;
}

const std::string& AIOAsset::Path() const
{
	return path;
}

std::vector<AIONode*>& AIOAsset::Nodes()
{
	return nodes;
}