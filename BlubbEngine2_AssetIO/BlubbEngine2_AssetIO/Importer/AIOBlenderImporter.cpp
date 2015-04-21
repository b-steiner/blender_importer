#include "AIOBlenderImporter.hpp"

#include "../Model/AIOException.hpp"
#include <fstream>
#include <iostream>

using namespace AssetIO;

AIOBlenderImporter::AIOBlenderImporter()
{ }
AIOBlenderImporter::~AIOBlenderImporter()
{
	for (auto str : sdna)
		delete str.second;
}

AIOAsset* AIOBlenderImporter::Load(const std::string& _path)
{
	LoadFile(_path);
	return ParseAsset(_path);
}


void AIOBlenderImporter::ToXML(const std::string& _inputPath, const std::string& _outputPath)
{
	LoadFile(_inputPath);

	std::ofstream out;
	out.open(_outputPath);

	out << "<Blender Version=\"" << versionNumber << "\" PointerSize=\"" << ptrSize << "\" Endianness=\"" << endianness << "\">" << std::endl;

	out << "\t" << "<SDNABlock>" << std::endl;

	for (auto str : sdna)
	{
		out << "\t\t" << "<SDNAStructure SDNAIdx=\"" << str.first << "\" Name=\"" << sdnaTypes[str.second->NameIdx()] << "\">" << std::endl;

		for (auto field : str.second->Fields())
		{
			out << "\t\t\t" << "<SDNAField Name=\"" << sdnaNames[field.second->NameIdx()] << "\" Type=\"" << sdnaTypes[field.second->TypeIdx()] << "\" Size=\"" <<
				sdnaTypeSizes[field.second->TypeIdx()] << "\"";
			out << " IsPointer=\"" << sdnaIsPointer[field.second->NameIdx()] << "\"";

			if (sdnaArrayLength[field.second->NameIdx()].size() > 0)
			{
				out << " ArrayLength=\"";
				for (auto arr : sdnaArrayLength[field.second->NameIdx()])
					out << arr << ", ";
				out << "\"";
			}
			
			out << "/>" << std::endl;
		}

		out << "\t\t" << "</SDNAStructure>" << std::endl;
	}
	out << "\t" << "</SDNABlock>" << std::endl;

	for (auto fb : addressToFileblock)
		Fileblock2XML(fb.second, out);


	out << "</Blender>" << std::endl;

	out.close();
}
void AIOBlenderImporter::Fileblock2XML(char* _ptr, std::ofstream& _out)
{
	char* ptr = _ptr;
	char identifier[5];
	identifier[4] = '\0';

	memcpy(identifier, ptr, 4);
	ptr += 4;

	uint32_t size = R<uint32_t>(ptr);
	ptr += 4;

	uint64_t addr = RPtr(ptr);
	ptr += ptrSize;

	uint32_t sdnaIndex = R<uint32_t>(ptr);
	ptr += 4;
	uint32_t count = R<uint32_t>(ptr);
	ptr += 4;

	SDNAStructure* strc = sdna[sdnaIndex];

	_out << "\t" << "<FileBlock Identifier=\"" << identifier << "\" Size=\"" << size << "\" OriginalAddr=\"" << addr << "\" SDNAIndex=\"" << sdnaIndex
		 << "\" SDNATypeName=\"" << sdnaTypes[strc->NameIdx()]
		 << "\" Count=\"" << count << "\">" << std::endl;

	std::string intent = "\t\t";
	if (count > 1)
		intent += "\t";

	for (uint32_t i = 0; i < count; i++)
	{
		if (count > 1)
			_out << "\t\t" << "<Instance Index =\"" << i << "\">" << std::endl;

		for (auto field : strc->Fields())
		{
			_out << intent << "<" << sdnaNames[field.second->NameIdx()] << " Type=\"" << sdnaTypes[field.second->TypeIdx()] << "\" Size=\"" << sdnaTypeSizes[field.second->TypeIdx()] << "\">";

			//Value
			if (sdnaArrayLength[field.second->NameIdx()].size() > 0)
			{
				_out << std::endl;
				_out << intent << "\t" << "<Array Length=\"";
				for (auto l : sdnaArrayLength[field.second->NameIdx()])
					_out << l << ", ";
				_out << "\">";

				if (sdnaArrayLength[field.second->NameIdx()].size() == 1)
				{
					for (uint32_t arrayIdx = 0; arrayIdx < sdnaArrayLength[field.second->NameIdx()][0]; arrayIdx++)
						Value2XML(&ptr, sdnaTypes[field.second->TypeIdx()], sdnaTypeSizes[field.second->TypeIdx()], sdnaIsPointer[field.second->NameIdx()], _out);
				}
				if (sdnaArrayLength[field.second->NameIdx()].size() == 2)
				{
					/*for (int arrayIdx1 = 0; arrayIdx1 < sdnaArrayLength[field->NameIdx()][0]; arrayIdx1++)
					{
						for (int arrayIdx2 = 0; arrayIdx2 < sdnaArrayLength[field->NameIdx()][0]; arrayIdx2++)
						{
							Value2XML(&ptr, sdnaTypes[field->TypeIdx()], sdnaIsPointer[field->NameIdx()]);
						}
					}*/
					std::cout << "FOUND 2D ARRAY" << std::endl;
				}

				_out << "</Array>" << std::endl << intent;
			}
			else
			{
				Value2XML(&ptr, sdnaTypes[field.second->TypeIdx()], sdnaTypeSizes[field.second->TypeIdx()], sdnaIsPointer[field.second->NameIdx()], _out);
			}

			_out << "</" << sdnaNames[field.second->NameIdx()] << ">" << std::endl;
		}

		if (count > 1)
			_out << "\t\t" << "</Instance>" << std::endl;
	}


	_out << "\t" << "</FileBlock>" << std::endl;
}
void AIOBlenderImporter::Value2XML(char** _ptr, const std::string& _type, uint16_t _size, bool _isPtr, std::ofstream& _out)
{
	if (_isPtr)
	{
		_out << RPtr(*_ptr);
		(*_ptr) += ptrSize;
		return;
	}

	if (_type == "char")
		_out << R<char>(*_ptr);
	else if (_type == "short")
		_out << R<int16_t>(*_ptr);
	else if (_type == "int")
		_out << R<int32_t>(*_ptr);
	else if (_type == "float")
		_out << R<float>(*_ptr);
	else if (_type == "double")
		_out << R<double>(*_ptr);

	(*_ptr) += _size;
}



void AIOBlenderImporter::LoadFile(const std::string& _path)
{
	FILE* file = fopen(_path.c_str(), "rb");
	if (file == nullptr)
		throw AIOException("Unable to open file");

	fseek(file, 0, SEEK_END);
	dataSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	data = new char[dataSize];
	fread(data, dataSize, 1, file);
	fclose(file);
	file = nullptr;

	//Parse header

	//Ident
	memcpy(ident, data, 7);
	ident[7] = '\0';
	if (strcmp(ident, "BLENDER") != 0)
		throw AIOException(std::string("File Identifier has to be \"BLENDER\", but was ") + std::string(ident));

	//Ptr Size
	ptrChar = data[7];
	if (ptrChar == '_')
		ptrSize = 4;
	else if (ptrChar == '-')
		ptrSize = 8;
	else
		throw AIOException("PointerSize was not \'-\' or \'_\'");

	endianness = data[8];
	memcpy(versionNumber, data + 9, 3);
	versionNumber[3] = '\0';

	//Go over all fileblocks
	char* ptr = data + 12; // Start of first fileblock

	while (ptr < data + dataSize)
	{
		//Always start at 4-byte alignment
		if ((uint64_t)ptr % 4 != 0)
			ptr += 4 - ((uint64_t)ptr % 4);

		//Read header (everything we need at the moment
		char code[5];
		code[4] = '\0';
		memcpy(code, ptr, 4);
		
		uint32_t size = R<uint32_t>(ptr + 4);
		uint64_t oldAddress = RPtr(ptr + 8);
		uint32_t count = R<uint32_t>(ptr + 12 + ptrSize);

		if (strcmp(code, "DNA1") == 0)
			ParseSDNABlock(ptr);
		else
		{
			addressToFileblock[oldAddress] = ptr;
		}

		//To next block
		ptr += (16 + ptrSize);
		ptr += size;
	}
}
void AIOBlenderImporter::ParseSDNABlock(char* _ptr)
{
	char* ptr = _ptr + 16 + ptrSize;

	char code[5];
	code[4] = '\0';
	
	memcpy(code, ptr, 4);
	if (strcmp(code, "SDNA") != 0)
		throw new AIOException(std::string("SDNA Block identifier hast to be SDNA, but is ") + std::string(code));
	ptr += 4;


	// NAMES
	if ((uint64_t)ptr % 4 != 0)
		ptr += 4 - ((uint64_t)ptr % 4);
	memcpy(code, ptr, 4);
	if (strcmp(code, "NAME") != 0)
		throw new AIOException(std::string("SDNA Subblock identifier hast to be NAME, but is ") + std::string(code));
	ptr += 4;

	uint32_t nameCount = R<uint32_t>(ptr);
	ptr += 4;

	char buffer[256];
	for (uint32_t i = 0; i < nameCount; i++)
	{
		int idx = 0;
		bool isPtr = false;

		while (*ptr != '\0' && *ptr != '[')
		{
			buffer[idx] = *ptr;
			idx++;
			if (*ptr == '*')
				isPtr = true;
			ptr++;
		}
		buffer[idx] = '\0';

		sdnaIsPointer.push_back(isPtr);
		sdnaNames.push_back(std::string(buffer));
		sdnaArrayLength.push_back(std::vector<uint32_t>());

		while (*ptr == '[') // Array
		{
			ptr++;
			idx = 0;
			while (*ptr != ']')
			{
				buffer[idx] = *ptr;
				idx++;
				ptr++;
			}

			buffer[idx] = '\0';
			int length = atoi(buffer);
			sdnaArrayLength.back().push_back((uint32_t)length);

			ptr++;
		}

		ptr++;
	}

	//TYPES
	if ((uint64_t)ptr % 4 != 0)
		ptr += 4 - ((uint64_t)ptr % 4);
	memcpy(code, ptr, 4);
	if (strcmp(code, "TYPE") != 0)
		throw new AIOException(std::string("SDNA Subblock identifier hast to be TYPE, but is ") + std::string(code));
	ptr += 4;

	uint32_t typeCount = R<uint32_t>(ptr);
	ptr += 4;

	for (uint32_t i = 0; i < typeCount; i++)
	{
		int idx = 0;
		while (*ptr != '\0')
		{
			buffer[idx] = *ptr;
			idx++;
			ptr++;
		}
		buffer[idx] = '\0';
		ptr++;

		sdnaTypes.push_back(std::string(buffer));
	}

	if ((uint64_t)ptr % 4 != 0)
		ptr += 4 - ((uint64_t)ptr % 4);
	memcpy(code, ptr, 4);
	if (strcmp(code, "TLEN") != 0)
		throw new AIOException(std::string("SDNA Subblock identifier hast to be TLEN, but is ") + std::string(code));
	ptr += 4;

	for (uint32_t i = 0; i < typeCount; i++)
	{
		sdnaTypeSizes.push_back(R<uint16_t>(ptr));
		ptr += 2;
	}


	// STRUCTURES
	if ((uint64_t)ptr % 4 != 0)
		ptr += 4 - ((uint64_t)ptr % 4);
	memcpy(code, ptr, 4);
	if (strcmp(code, "STRC") != 0)
		throw new AIOException(std::string("SDNA Subblock identifier hast to be STRC, but is ") + std::string(code));
	ptr += 4;

	uint32_t structureCount = R<uint32_t>(ptr);
	ptr += 4;

	for (uint32_t i = 0; i < structureCount; i++)
	{
		uint16_t nameIdx = R<uint16_t>(ptr);
		ptr += 2;

		SDNAStructure* str = new SDNAStructure(nameIdx);

		uint16_t fieldCount = R<uint16_t>(ptr);
		ptr += 2;

		uint16_t offset = 0;

		for (int j = 0; j < fieldCount; j++)
		{
			uint16_t fieldType = R<uint16_t>(ptr);
			ptr += 2;
			uint16_t fieldName = R<uint16_t>(ptr);
			ptr += 2;

			SDNAField* field = new SDNAField(fieldName, fieldType, offset);
			str->Fields()[sdnaNames[fieldName]] = field;

			if (sdnaIsPointer[fieldName])
				offset += ptrSize;
			else
			{
				if (sdnaArrayLength[fieldName].size() == 0)
					offset += sdnaTypeSizes[fieldType];
				else
				{
					uint16_t multiply = 1;
					for (auto l : sdnaArrayLength[fieldName])
						multiply *= l;
					offset += sdnaTypeSizes[fieldType] * multiply;
				}
			}
		}

		auto name = sdnaTypes[nameIdx];
		nameToSDNAIdx[name] = i;
		sdna[i] = str;
	}
}


AIOAsset* AIOBlenderImporter::ParseAsset(const std::string& _path)
{
	AIOAsset* asset = new AIOAsset(_path);

	auto objectId = nameToSDNAIdx["Object"];

	//Find all nodes
	for (auto fb : addressToFileblock)
	{
		uint32_t sdnaIdx = R<uint32_t>(fb.second + 8 + ptrSize);

		if (sdnaIdx == objectId)
			asset->Nodes().push_back(ParseNode(fb.second));
	}

	return asset;
}
AIONode* AIOBlenderImporter::ParseNode(char* _ptr)
{
	char* data = _ptr + 16 + ptrSize;

	auto sdnaIdx = nameToSDNAIdx["Object"];
	SDNAStructure* strc = sdna[sdnaIdx];
	SDNAStructure* idStrc = sdna[nameToSDNAIdx["ID"]];

	auto o = strc->Fields()["id"]->Offset();
	char* idPtr = data + o;
	o = idStrc->Fields()["name"]->Offset();

	std::string name = std::string(idPtr + idStrc->Fields()["name"]->Offset() + 2);
	AIONode* node = new AIONode(name);

	AIOVector3 loc = R<AIOVector3>(data + strc->Fields()["loc"]->Offset());
	AIOVector3 rot = R<AIOVector3>(data + strc->Fields()["rot"]->Offset());
	AIOVector3 size = R<AIOVector3>(data + strc->Fields()["size"]->Offset());

	//Correct parent-child hirarchical transformations
	//Blender stores transformations relative to the parent, but with an additional "offset" stored in the parentinv matrix.
	//So we have to get the parameters of this matrix and correct our transformations.
	AIOMatrix4 parentInv = R<AIOMatrix4>(data + strc->Fields()["parentinv"]->Offset());
	AIOVector3 parentScale(
		sqrtf(parentInv.Values()[IDX2(0, 0)] * parentInv.Values()[IDX2(0, 0)] + parentInv.Values()[IDX2(0, 1)] * parentInv.Values()[IDX2(0, 1)] + parentInv.Values()[IDX2(0, 2)] * parentInv.Values()[IDX2(0, 2)]),
		sqrtf(parentInv.Values()[IDX2(1, 0)] * parentInv.Values()[IDX2(1, 0)] + parentInv.Values()[IDX2(1, 1)] * parentInv.Values()[IDX2(1, 1)] + parentInv.Values()[IDX2(1, 2)] * parentInv.Values()[IDX2(1, 2)]),
		sqrtf(parentInv.Values()[IDX2(2, 0)] * parentInv.Values()[IDX2(2, 0)] + parentInv.Values()[IDX2(2, 1)] * parentInv.Values()[IDX2(2, 1)] + parentInv.Values()[IDX2(2, 2)] * parentInv.Values()[IDX2(2, 2)])
		);

	size.Values()[0] *= parentScale.Values()[0];
	size.Values()[1] *= parentScale.Values()[1];
	size.Values()[2] *= parentScale.Values()[2];

	return node;
}

bool AIOBlenderImporter::CheckStructure()
{
	return true;
}