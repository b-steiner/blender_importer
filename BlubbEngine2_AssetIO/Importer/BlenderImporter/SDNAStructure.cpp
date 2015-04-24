#include "SDNAStructure.hpp"

using namespace AssetIO;

SDNAField::SDNAField(uint16_t _nameIdx, uint16_t _typeIdx, uint16_t _offset) : nameIdx(_nameIdx), typeIdx(_typeIdx), offset(_offset)
{ }

uint16_t SDNAField::NameIdx() const
{
	return nameIdx;
}
uint16_t SDNAField::TypeIdx() const
{
	return typeIdx;
}
uint16_t SDNAField::Offset() const
{
	return offset;
}


SDNAStructure::SDNAStructure(uint16_t _nameIdx) : nameIdx(_nameIdx)
{ }
SDNAStructure::~SDNAStructure()
{
	for (auto it : fields)
		delete it.second;
}

uint16_t SDNAStructure::NameIdx() const
{
	return nameIdx;
}
std::unordered_map<std::string, SDNAField*>& SDNAStructure::Fields()
{
	return fields;
}