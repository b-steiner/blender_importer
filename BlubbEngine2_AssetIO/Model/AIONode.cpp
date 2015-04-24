#include "AIONode.hpp"

using namespace AssetIO;

AIONode::AIONode(const std::string& _name) : name(_name), lightsource(nullptr), material(nullptr), mesh(nullptr), animation(nullptr)
{ }
AIONode::~AIONode()
{
	if (lightsource != nullptr)
		delete lightsource;

	for (auto node : nodes)
		delete node;
}

const std::string& AIONode::Name() const
{
	return name;
}
std::vector<AIONode*>& AIONode::Nodes()
{
	return nodes;
}

const AIOMesh* AIONode::Mesh() const
{
	return mesh;
}
void AIONode::Mesh(AIOMesh* _value)
{
	mesh = _value;
}
const AIOMaterial* AIONode::Material() const
{
	return material;
}
void AIONode::Material(AIOMaterial* _value)
{
	material = _value;
}
const AIOAnimation* AIONode::Animation() const
{
	return animation;
}
void AIONode::Animation(AIOAnimation* _value)
{
	animation = _value;
}
const AIOLightSource* AIONode::LightSource() const
{
	return lightsource;
}
void AIONode::LightSource(AIOLightSource* _value)
{
	lightsource = _value;
}

const AIOMatrix4& AIONode::Matrix() const
{
	return matrix;
}
void AIONode::Matrix(const AIOMatrix4& _value)
{
	matrix = _value;
}
const AIOVector3 AIONode::Translation() const
{
	return translation;
}
void AIONode::Translation(const AIOVector3& _value)
{
	translation = _value;
}
const AIOVector3 AIONode::Scale() const
{
	return scale;
}
void AIONode::Scale(const AIOVector3& _value)
{
	scale = _value;
}
const AIOVector3 AIONode::Rotation() const
{
	return rotation;
}
void AIONode::Rotation(const AIOVector3& _value)
{
	rotation = _value;
}