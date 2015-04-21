#pragma once

#include <string>
#include <vector>
#include <stdint.h>
#include <unordered_map>

namespace AssetIO
{
	enum class SDNAType
	{
	};

	class SDNAField
	{
	public:
		SDNAField(uint16_t _nameIdx, uint16_t _typeIdx, uint16_t _offset);

		uint16_t NameIdx() const;
		uint16_t TypeIdx() const;
		uint16_t Offset() const;

	private:
		uint16_t typeIdx, nameIdx, offset;
	};

	class SDNAStructure
	{
	public:
		SDNAStructure(uint16_t _nameIdx);
		~SDNAStructure();
		
		uint16_t NameIdx() const;
		std::unordered_map<std::string, SDNAField*>& Fields();

	private:
		uint16_t nameIdx;
		std::unordered_map<std::string, SDNAField*> fields;
	};
};