#include "AIOAsset.hpp"

using namespace AssetIO;

AIOAsset::AIOAsset(const std::string& _path) : path(_path)
{ }
AIOAsset::~AIOAsset()
{ }

const std::string& AIOAsset::Path() const
{
	return path;
}

std::vector<AIONode*>& AIOAsset::Nodes()
{
	return nodes;
}