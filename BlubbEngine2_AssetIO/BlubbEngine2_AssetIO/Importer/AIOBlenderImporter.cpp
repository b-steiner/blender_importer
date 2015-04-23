#include "AIOBlenderImporter.hpp"

#include "../Model/AIOException.hpp"
#include <fstream>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#include <unordered_map>
#include <sstream>
#include <iomanip>

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

	if (sdnaIndex == 114)
		int asdf = 4;

	std::string intent = "\t\t";
	if (count > 1)
		intent += "\t";

	for (uint32_t i = 0; i < count; i++)
	{
		if (count > 1)
			_out << "\t\t" << "<Instance Index =\"" << i << "\">" << std::endl;

		uint64_t strcSize = SDNAStruct2XML(ptr, strc, _out, intent);
		ptr += strcSize;

		if (count > 1)
			_out << "\t\t" << "</Instance>" << std::endl;
	}


	_out << "\t" << "</FileBlock>" << std::endl;
}

uint64_t AIOBlenderImporter::SDNAStruct2XML(char* _ptr, SDNAStructure* _strc, std::ofstream& _out, std::string& _intent)
{
	uint64_t size = 0;

	for (auto field : _strc->Fields())
	{
		size += SDNAField2XML(_ptr, field.second, _out, _intent);
	}

	return size;
}

uint64_t AIOBlenderImporter::SDNAField2XML(char* _ptr, SDNAField* _field, std::ofstream& _out, std::string& _intent)
{
	uint64_t size = 0;

	std::string& name = sdnaNames[_field->NameIdx()];
	_out << _intent << "<" << name << " Type=\"" << sdnaTypes[_field->TypeIdx()] << "\" Size=\"" << sdnaTypeSizes[_field->TypeIdx()] << "\">";

	//Value
	if (sdnaArrayLength[_field->NameIdx()].size() > 0)
	{
		_out << std::endl;
		_out << _intent << "\t" << "<Array Length=\"";
		for (auto l : sdnaArrayLength[_field->NameIdx()])
			_out << l << ", ";
		_out << "\">";

		if (sdnaArrayLength[_field->NameIdx()].size() == 1)
		{
			uint64_t arraySize = 0;

			for (uint32_t arrayIdx = 0; arrayIdx < sdnaArrayLength[_field->NameIdx()][0]; arrayIdx++)
				arraySize += Value2XML(_ptr + _field->Offset() + arraySize, _field->TypeIdx(), sdnaTypeSizes[_field->TypeIdx()], sdnaIsPointer[_field->NameIdx()], _out, _intent);

			size += arraySize;
		}
		if (sdnaArrayLength[_field->NameIdx()].size() == 2)
		{
			uint64_t arraySize = sdnaTypeSizes[_field->TypeIdx()];
			for (auto l : sdnaArrayLength[_field->NameIdx()])
				arraySize *= l;
			size += arraySize;

			/*for (int arrayIdx1 = 0; arrayIdx1 < sdnaArrayLength[field->NameIdx()][0]; arrayIdx1++)
			{
			for (int arrayIdx2 = 0; arrayIdx2 < sdnaArrayLength[field->NameIdx()][0]; arrayIdx2++)
			{
			Value2XML(&ptr, sdnaTypes[field->TypeIdx()], sdnaIsPointer[field->NameIdx()]);
			}
			}*/
		}

		_out << "</Array>" << std::endl << _intent;
	}
	else
	{
		size += Value2XML(_ptr + _field->Offset(), _field->TypeIdx(), sdnaTypeSizes[_field->TypeIdx()], sdnaIsPointer[_field->NameIdx()], _out, _intent);
	}

	_out << "</" << name << ">" << std::endl;
	return size;
}

uint64_t AIOBlenderImporter::Value2XML(char* _ptr, uint16_t _typeIdx, uint16_t _size, bool _isPtr, std::ofstream& _out, std::string& _intent)
{
	std::string type = sdnaTypes[_typeIdx];

	if (_isPtr)
	{
		_out << RPtr(_ptr);
		return ptrSize;
	}

	if (type == "char")
		_out << R<char>(_ptr);
	else if (type == "short")
		_out << R<int16_t>(_ptr);
	else if (type == "int")
		_out << R<int32_t>(_ptr);
	else if (type == "float")
		_out << R<float>(_ptr);
	else if (type == "double")
		_out << R<double>(_ptr);
	else
	{
		//Subtype
		SDNAStructure* strc = sdna[nameToSDNAIdx[type]];

		_out << std::endl;

		for (auto field : strc->Fields())
		{
			SDNAField2XML(_ptr, field.second, _out, _intent + "\t");
		}

		_out << std::endl << _intent;
	}

	return _size;
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
	
	versionInt = 0;
	for (int i = 0; i < 3; i++)
		versionInt += versionNumber[i] * (int)powl(10, 2-i);

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
	char* idPtr = data + strc->Fields()["id"]->Offset();

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
		sqrtf(parentInv.M11() *parentInv.M11() + parentInv.M12() * parentInv.M12() + parentInv.M13() * parentInv.M13()),
		sqrtf(parentInv.M21() *parentInv.M21() + parentInv.M22() * parentInv.M22() + parentInv.M23() * parentInv.M23()),
		sqrtf(parentInv.M31() *parentInv.M31() + parentInv.M32() * parentInv.M32() + parentInv.M33() * parentInv.M33())
		);

	size = AIOVector3(size.X() * parentScale.X(), size.Y() * parentScale.Y(), size.Z() * parentScale.Z());

	AIOMatrix3 scaleMatrix = AIOMatrix3::Scale(parentScale);
	AIOMatrix3 rsMatrix = parentInv.GetMat3();
	AIOMatrix3 rMatrix = rsMatrix * scaleMatrix;
	rMatrix.Transpose();
	rsMatrix.Transpose();

	AIOVector3 ptranslation(parentInv.M14(), parentInv.M24(), parentInv.M34());
	node->Translation(rsMatrix * node->Translation() + ptranslation);

	AIOMatrix3 crmat = AIOMatrix3::RotateZ(node->Rotation().Z()) * AIOMatrix3::RotateY(node->Rotation().Y()) * AIOMatrix3::RotateX(node->Rotation().X());
	crmat = rMatrix * crmat;

	float theta = 0.0f;
	float psi = 0.0f;
	float phi = 0.0f;

	if (abs(crmat.M31()) > 0.999)
	{
		phi = 0.0f;

		if (crmat.M31() < 0.0f)
		{
			theta = (float)M_PI / 2.0f;
			psi = atan2f(crmat.M12(), crmat.M13());
		}
		else
		{
			theta = -(float)M_PI / 2.0f;
			psi = atan2f(-crmat.M12(), -crmat.M13());
		}
	}
	else
	{
		theta = -asinf(crmat.M31());
		psi = atan2f(crmat.M32(), crmat.M33());
		phi = atan2f(crmat.M21() / cosf(theta), crmat.M11() / cosf(theta));
	}

	node->Rotation(AIOVector3(psi, theta, phi));


	//Find out what type of data is attached to this node
	uint64_t dataPtr = R<uint64_t>(data + strc->Fields()["*data"]->Offset());
	if (dataPtr != 0)
	{
		uint16_t dataType = R<uint16_t>(data + strc->Fields()["type"]->Offset());

		switch (dataType)
		{
		case 0: //No data
			break;
		case 1: //Mesh
			node->Mesh(ParseMesh(dataPtr));
			node->Material(ParseMaterial(dataPtr));
			break;
		case 10: //Lamp
			break;
		}
	}

	return node;
}
AIOMesh* AIOBlenderImporter::ParseMesh(uint64_t _ptr)
{
	char* ptr = addressToFileblock[_ptr];
	SDNAStructure* strc = sdna[R<uint32_t>(ptr + 8 + ptrSize)];
	SDNAStructure* idstrc = sdna[nameToSDNAIdx["ID"]];

	ptr += 16 + ptrSize;

	std::string name = std::string(ptr + strc->Fields()["id"]->Offset() + idstrc->Fields()["name"]->Offset() + 2);

	auto meshIt = meshes.find(name);
	if (meshIt != meshes.end())
		return meshIt->second;

	//Not found -> load it
	AIOMesh* mesh = new AIOMesh(name);
	
	AIOVector3* positions;
	AIOVector3* normals;

	std::vector<unsigned int> indices;
	unsigned int* faceStart;
	unsigned int* faceLength;
	std::vector<std::vector<AIOVector2>> faceUvs;
	AIOVector3* faceNormals;
	char* faceFlags;

	int triangles = 0;
	int faceCount = 0;
	bool hasUvs = false;

	//Load data from block
	uint32_t vertexCount = R<uint32_t>(ptr + strc->Fields()["totvert"]->Offset());
	positions = new AIOVector3[vertexCount];
	normals = new AIOVector3[vertexCount];

	char* mvertBlock = addressToFileblock[RPtr(ptr + strc->Fields()["*mvert"]->Offset())] + 16 + ptrSize;
	uint32_t mvertSize = sdnaTypeSizes[strc->Fields()["*mvert"]->TypeIdx()];
	SDNAStructure* mvertstrc = sdna[nameToSDNAIdx["MVert"]];

	for (unsigned int i = 0; i < vertexCount; i++)
	{
		positions[i] = AIOVector3((float*)(ptr + mvertSize * i + mvertstrc->Fields()["co"]->Offset()));
		int16_t* noPtr = (int16_t*)(ptr + mvertSize * i + mvertstrc->Fields()["no"]->Offset());
		int32_t li = noPtr[0] * noPtr[0] + noPtr[1] * noPtr[1] + noPtr[2] * noPtr[2];
		double l = sqrt((double)li);
		normals[i] = AIOVector3((float)(((double)noPtr[0]) / l), (float)(((double)noPtr[1]) / l), (float)(((double)noPtr[2]) / l));
	}

	#pragma region Face Information < 236

	if (versionInt < 263)
	{
		faceCount = R<uint32_t>(ptr + strc->Fields()["totface"]->Offset());
		triangles = faceCount;

		char* mfacePtr = addressToFileblock[RPtr(ptr + strc->Fields()["*mface"]->Offset())] + 16 + ptrSize;
		char* mtfacePtr = nullptr;
		uint64_t mtfaceAddr = RPtr(ptr + strc->Fields()["*mtface"]->Offset());
		if (mtfaceAddr != 0)
		{
			mtfacePtr = addressToFileblock[mtfaceAddr] + 16 + ptrSize;
			hasUvs = true;
		}

		indices.reserve(faceCount * 4);
		faceNormals = new AIOVector3[faceCount];
		if (hasUvs)
			faceUvs.push_back(std::vector<AIOVector2>(faceCount * 4));
		faceFlags = new char[faceCount];
		faceStart = new unsigned int[faceCount];
		faceLength = new unsigned int[faceCount];

		//Get face blocks
		SDNAStructure* mfaceStrc = sdna[nameToSDNAIdx["MFace"]];
		SDNAStructure* mtfaceStrc = sdna[nameToSDNAIdx["MTFace"]];
		uint32_t mfaceSize = sdnaTypeSizes[strc->Fields()["*mface"]->TypeIdx()];
		uint32_t mtfaceSize = sdnaTypeSizes[strc->Fields()["*mtface"]->TypeIdx()];

		char* uvPtr = mtfacePtr + mtfaceStrc->Fields()["uv"]->Offset();
		
		unsigned int indexCount = 0;

		for (int i = 0; i < faceCount; i++)
		{
			faceStart[i] = indexCount;

			//Read indices
			char* indexPtr = mfacePtr + i * mfaceSize;
			indices.push_back(R<uint32_t>(indexPtr + mfaceStrc->Fields()["v1"]->Offset()));
			indices.push_back(R<uint32_t>(indexPtr + mfaceStrc->Fields()["v2"]->Offset()));
			indices.push_back(R<uint32_t>(indexPtr + mfaceStrc->Fields()["v3"]->Offset()));
			uint32_t v4 = R<uint32_t>(indexPtr + mfaceStrc->Fields()["v4"]->Offset());
			indexCount += 3;

			//If we have 4 indices -> 2 triangles
			if (v4 != 0)
			{
				indices.push_back(v4);
				indexCount++;
				faceLength[i] = 4;
			}
			else
			{
				faceLength[i] = 3;
			}


			if (hasUvs) // Load uv if necessary
			{
				float* uvPtr = (float*)(mtfacePtr + i * mtfaceSize + mtfaceStrc->Fields()["uv"]->Offset());

				for (unsigned int j = 0; j < faceLength[i]; j++)
					faceUvs[0].push_back(AIOVector2(uvPtr[2 * j], uvPtr[2 * j + 1]));
			}

			//Calculate face normal + read face flag
			faceNormals[i] = AIOVector3::Cross(AIOVector3::Normalize(positions[indices[i * 4 + 1]] - positions[indices[i * 4]]),
				AIOVector3::Normalize(positions[indices[i * 4 + 2]] - positions[indices[i * 4]]));
			faceFlags[i] = R<char>(mfacePtr + i * mfaceSize + mfaceStrc->Fields()["flag"]->Offset());
		}

	}

	#pragma endregion
	#pragma region Face Information >= 236

	if (versionInt >= 236)
	{
		faceCount = R<uint32_t>(ptr + strc->Fields()["totpoly"]->Offset());

		char* mpolyPtr = addressToFileblock[RPtr(ptr + strc->Fields()["mpoly"]->Offset())] + 16 + ptrSize;
		char* mloopPtr = addressToFileblock[RPtr(ptr + strc->Fields()["mloop"]->Offset())] + 16 + ptrSize;

		uint64_t mloopuvAddr = RPtr(ptr + strc->Fields()["mloopuv"]->Offset());
		char* mloopuvPtr = nullptr;
		if (mloopuvAddr != 0)
		{
			mloopuvPtr = addressToFileblock[mloopuvAddr] + 16 + ptrSize;
			hasUvs = true;
		}
		uint64_t mpolySize = sdnaTypeSizes[strc->Fields()["mpoly"]->TypeIdx()];
		uint64_t mloopSize = sdnaTypeSizes[strc->Fields()["mloop"]->TypeIdx()];
		uint64_t mloopuvSize = sdnaTypeSizes[strc->Fields()["mloopuv"]->TypeIdx()];

		indices.reserve(faceCount * 3);
		faceNormals = new AIOVector3[faceCount];
		faceFlags = new char[faceCount];
		if (hasUvs)
			faceUvs.push_back(std::vector<AIOVector2>(faceCount * 4));

		SDNAStructure* mpolyStrc = sdna[nameToSDNAIdx["MPoly"]];
		SDNAStructure* mloopStrc = sdna[nameToSDNAIdx["MLoop"]];
		SDNAStructure* mloopuvStrc = sdna[nameToSDNAIdx["MLoopUV"]];

		unsigned int indexCount = 0;

		for (int face = 0; face < faceCount; face++)
		{
			char* mpolyFaceStart = mpolyPtr + mpolySize * face;
			char* mloopFaceStart = mloopPtr + mloopSize * face;

			int loopStart = R<uint32_t>(mpolyFaceStart + mpolyStrc->Fields()["loopstart"]->Offset());
			int loopLength = R<uint32_t>(mpolyFaceStart + mpolyStrc->Fields()["totloop"]->Offset());

			triangles += (loopLength - 2);

			faceStart[face] = indexCount;
			faceLength[face] = loopLength;
			indexCount += loopLength;

			for (int i = 0; i < loopLength; i++)
			{
				indices.push_back(R<uint32_t>(mloopFaceStart + mloopStrc->Fields()["v"]->Offset()));

				if (hasUvs)
				{
					float* mloopuvFaceStart = (float*)(mloopuvPtr + mloopuvSize * face + mloopuvStrc->Fields()["uv"]->Offset());
					faceUvs[0].push_back(AIOVector2(mloopuvFaceStart[0], mloopuvFaceStart[1]));
				}
			}

			//Face normal
			int first = 0;
			faceNormals[face] = AIOVector3();
			while (faceNormals[face].Length() <= 0.0001 && first < loopLength - 2)
			{
				faceNormals[face] = AIOVector3::Cross(
					AIOVector3::Normalize(positions[indices[faceStart[face] + 1]] - positions[indices[faceStart[face]]]),
					AIOVector3::Normalize(positions[indices[faceStart[face] + 2]] - positions[indices[faceStart[face]]])
					);
				first++;
			}

			faceFlags[face] = R<char>(mpolyFaceStart + mpolyStrc->Fields()["flag"]->Offset());
		}		
	}

	#pragma endregion


	#pragma region Triangulation & Indices

	std::unordered_map < std::string, unsigned int > vertexDictionary;
	std::vector<AIOVector3> indPositions(triangles * 3);
	std::vector<AIOVector3> indNormals(triangles * 3);
	std::vector<AIOVector2> indUVs(triangles * 3);
	std::vector<unsigned int> indIndices(triangles * 3);
	std::vector<unsigned int> indexCache;

	for (int face = 0; face < faceCount; face++)
	{
		indexCache.clear();

		//Find unique vertices
		for (unsigned int findex = faceStart[face]; findex < faceStart[face] + faceLength[face]; findex++)
		{
			AIOVector3 p = positions[indices[findex]];
			AIOVector3 n = normals[indices[findex]];
			if (faceFlags[face] % 2 == 0)
				n = faceNormals[face];
			AIOVector2 uv;

			if (hasUvs)
				uv = faceUvs[0][indices[findex]];

			std::string key = KeyFromVertex(p, n, uv);
			auto vit = vertexDictionary.find(key);
			if (vit == vertexDictionary.end())
			{
				vertexDictionary[key] = (unsigned int)indPositions.size();
				indPositions.push_back(p);
				indNormals.push_back(n);
				indUVs.push_back(uv);
				vit = vertexDictionary.find(key);
			}

			indexCache.push_back(vit->second);
		}

		// Search for good triangulation
		bool isDegenerated = true;
		unsigned int startIndex = 0;
		while (isDegenerated && startIndex < faceLength[face]) 
		{
			isDegenerated = false;

			for (unsigned int findex = 1; findex < faceLength[face] - 1; findex++)
			{
				int i1 = (startIndex + findex) % faceLength[face];
				int i2 = (startIndex + findex + 2) % faceLength[face];

				AIOVector3 p1 = indPositions[indexCache[startIndex]];
				AIOVector3 p2 = indPositions[indexCache[i1]];
				AIOVector3 p3 = indPositions[indexCache[i2]];

				float mag = AIOVector3::Cross(p2 - p1, p3 - p1).Length();

				if (mag < 0.000001)
				{
					isDegenerated = true;
					break;
				}
			}
		}

		//Perform triangulation
		for (unsigned int findex = 1; findex < faceLength[face] - 1; findex++)
		{
			int i1 = (startIndex + findex) % faceLength[face];
			int i2 = (startIndex + findex + 2) % faceLength[face];

			indIndices.push_back(indexCache[startIndex]);
			indIndices.push_back(indexCache[i1]);
			indIndices.push_back(indexCache[i2]);
		}
	}


	#pragma endregion

	//Build mesh
	mesh->Indices() = indIndices;
	mesh->Positions() = indPositions;
	mesh->Normals() = indNormals;

	if (hasUvs)
	{
		mesh->TexCoords().push_back(indUVs);
		CalculateTangents(mesh);
	}

	meshes[mesh->Name()] = mesh;

	return mesh;
}
AIOMaterial* AIOBlenderImporter::ParseMaterial(uint64_t _ptr)
{
	return new AIOMaterial("default-material");
}

std::string AIOBlenderImporter::KeyFromVertex(AIOVector3 _position, AIOVector3 _normal, AIOVector2 _uv)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(4) << _position.X() << ";" << _position.Y() << ";" << _position.Z() << ";" << _normal.X() << ";" << _normal.Y() << ";" << _normal.Z()
		<< _uv.X() << ";" << _uv.Y();
	return ss.str();
}
void AIOBlenderImporter::CalculateTangents(AIOMesh* _mesh)
{
	for (std::vector<AIOVector2>& uvs : _mesh->TexCoords())
	{
		std::vector<AIOVector4> finalTangents(uvs.size());
		AIOVector3* vertexTangents = new AIOVector3[uvs.size()];
		AIOVector3* vertexBitangents = new AIOVector3[uvs.size()];

		for (int i = 0; i < _mesh->Indices().size(); i+=3)
		{
			unsigned int i1 = i;
			unsigned int i2 = i + 1;
			unsigned int i3 = i + 2;

			AIOVector2 uv1 = uvs[i1];
			AIOVector2 uv2 = uvs[i2];
			AIOVector2 uv3 = uvs[i3];

			AIOVector3 p1 = _mesh->Positions()[i1];
			AIOVector3 p2 = _mesh->Positions()[i2];
			AIOVector3 p3 = _mesh->Positions()[i3];

			AIOVector3 e1 = p2 - p1;
			AIOVector3 e2 = p3 - p1;

			AIOVector2 uve1 = uv2 - uv1;
			AIOVector2 uve2 = uv3 - uv1;

			float area = 1.0f / (uve1.X() * uve2.Y() + uve1.Y() * uve2.X());

			AIOVector3 svec(
				uve2.Y() * e1.X() - uve1.Y() * e2.X(),
				uve2.Y() * e1.Y() - uve1.Y() * e2.Y(),
				uve2.Y() * e1.Z() - uve1.Y() * e2.Z());

			AIOVector3 tvec(
				uve1.X() * e2.X() - uve2.X() * e1.X(),
				uve1.X() * e2.Y() - uve2.X() * e1.Y(),
				uve1.X() * e2.Z() - uve2.X() * e1.Z());

			vertexTangents[i1] += svec;
			vertexTangents[i2] += svec;
			vertexTangents[i3] += svec;
			vertexBitangents[i1] += tvec;
			vertexBitangents[i2] += tvec;
			vertexBitangents[i3] += tvec;
		}

		for (int i = 0; i < uvs.size(); i++)
		{
			float headness = 1.0f;
			if (AIOVector3::Dot(AIOVector3::Cross(_mesh->Normals()[i], vertexTangents[i]), vertexBitangents[i]) < 0.0f)
				headness = -1.0f;
			finalTangents[i] = AIOVector4(AIOVector3::Normalize(vertexTangents[i] - _mesh->Normals()[i] * AIOVector3::Dot(vertexTangents[i], _mesh->Normals()[i])), headness);
		}

		_mesh->Tangents().push_back(finalTangents);
	}
}


bool AIOBlenderImporter::CheckStructure(const std::string& _path)
{
	LoadFile(_path);
	bool result = true;

	std::cout << "Checking Version " << versionNumber << std::endl;

	result &= CheckStructure("Object", "id", "ID");
	result &= CheckStructure("Object", "loc", "float");
	result &= CheckStructure("Object", "rot", "float");
	result &= CheckStructure("Object", "size", "float");
	result &= CheckStructure("Object", "parentinv", "float");
	result &= CheckStructure("Object", "*data", "void");
	result &= CheckStructure("Object", "type", "short");

	result &= CheckStructure("Mesh", "id", "ID");
	result &= CheckStructure("Mesh", "totvert", "int");
	result &= CheckStructure("Mesh", "*mvert", "MVert");

	result &= CheckStructure("MVert", "co", "float");
	result &= CheckStructure("MVert", "no", "short");

	result &= CheckStructure("ID", "name", "char");

	return result;
}
bool AIOBlenderImporter::CheckStructure(const std::string& _strcName, const std::string& _fieldName, const std::string& _fieldType, int _minVersion, int _maxVersion)
{
	if (_minVersion <= versionInt && _maxVersion >= versionInt)
	{
		auto sdnaId = nameToSDNAIdx[_strcName];
		if (sdna.find(sdnaId) == sdna.end())
		{
			std::cout << "Unable to find structure " << _strcName << std::endl;
			return false;
		}

		SDNAStructure* strc = sdna[sdnaId];

		if (strc->Fields().find(_fieldName) == strc->Fields().end())
		{
			std::cout << "Unable to find field " << _strcName << "." << _fieldName << std::endl;
			return false;
		}

		SDNAField* field = strc->Fields()[_fieldName];

		if (sdnaTypes[field->TypeIdx()] != _fieldType)
		{
			std::cout << "Field " << _strcName << "." << _fieldName << " is " << sdnaTypes[field->TypeIdx()] << " but should be " << _fieldType << std::endl;
			return false;
		}
	}

	return true;
}