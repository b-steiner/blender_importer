#pragma once

#include "../dllconfig.hpp"
#include "../Model/AIOAsset.hpp"
#include "BlenderImporter\SDNAStructure.hpp"

#include <unordered_map>
#include <stdint.h>
#include <fstream>

namespace AssetIO
{
	class _AIO_DECLSPEC AIOBlenderImporter
	{
	public:
		AIOBlenderImporter();
		~AIOBlenderImporter();

		AIOAsset* Load(const std::string& _path);
		void ToXML(const std::string& _inputPath, const std::string& _outputPath);
		bool CheckStructure(const std::string& _path);

	private:
		void LoadFile(const std::string& _path);
		void ParseSDNABlock(char* _ptr);

		void Fileblock2XML(char* _ptr, std::ofstream& _out);
		uint64_t SDNAStruct2XML(char* _ptr, SDNAStructure* _strc, std::ofstream& _out, std::string& _intent);
		uint64_t SDNAField2XML(char* _ptr, SDNAField* _field, std::ofstream& _out, std::string& _intent);
		uint64_t Value2XML(char* _ptr, uint16_t _typeIdx, uint16_t _size, bool _isPtr, std::ofstream& _out, std::string& _intent);
		
		AIOAsset* ParseAsset(const std::string& _path);
		AIONode* ParseNode(char* _ptr);
		AIOMesh* ParseMesh(uint64_t _ptr);
		AIOMaterial* ParseMaterial(uint64_t _ptr);
		AIOLightSource* ParseLightSource(uint64_t _ptr);

		std::string KeyFromVertex(AIOVector3 _position, AIOVector3 _normal, AIOVector2 _uv);
		void CalculateTangents(AIOMesh* _mesh);


		bool CheckStructure(const std::string& _strcName, const std::string& _fieldName, const std::string& _fieldType, int _minVersion = 0, int _maxVersion = 99999);



		template <typename T> T R(char* _ptr)
		{
			return (T)*((T*)_ptr);
		}
		template <> AIOVector3 R<AIOVector3>(char* _ptr)
		{
			return AIOVector3(R<float>(_ptr), R<float>(_ptr + 4), R<float>(_ptr + 8));
		}
		template <> AIOMatrix4 R<AIOMatrix4>(char* _ptr)
		{
			return AIOMatrix4((float*)_ptr);
		}
		uint64_t RPtr(char* _ptr)
		{
			if (ptrSize == 4)
				return (uint64_t)(*((uint32_t*)_ptr));
			else
				return *((uint64_t*)_ptr);
		}

		char* data;
		size_t dataSize;
		char ident[8];
		char ptrChar;
		int ptrSize;
		char endianness;
		char versionNumber[4];
		int versionInt;

		std::unordered_map<uint64_t, char*> addressToFileblock;
		std::unordered_map<std::string, uint32_t> nameToSDNAIdx;
		std::unordered_map<uint32_t, SDNAStructure*> sdna;

		std::vector<std::string> sdnaNames;
		std::vector<std::string> sdnaTypes;
		std::vector<bool> sdnaIsPointer;
		std::vector<std::vector<uint32_t>> sdnaArrayLength;
		std::vector<uint16_t> sdnaTypeSizes;

		std::vector<char*> objectBlocks;
		std::vector<uint64_t> objectParentAddr;

		std::string filePath;
		AIOAsset* asset;
	};
}