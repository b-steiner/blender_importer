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

	private:
		void LoadFile(const std::string& _path);
		void ParseSDNABlock(char* _ptr);

		void Fileblock2XML(char* _ptr, std::ofstream& _out);
		void Value2XML(char** _ptr, const std::string& _type, uint16_t _size, bool _isPtr, std::ofstream& _out);
		
		AIOAsset* ParseAsset(const std::string& _path);
		AIONode* ParseNode(char* _ptr);

		bool CheckStructure();



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

		std::unordered_map<uint64_t, char*> addressToFileblock;
		std::unordered_map<std::string, uint32_t> nameToSDNAIdx;
		std::unordered_map<uint32_t, SDNAStructure*> sdna;

		std::vector<std::string> sdnaNames;
		std::vector<std::string> sdnaTypes;
		std::vector<bool> sdnaIsPointer;
		std::vector<std::vector<uint32_t>> sdnaArrayLength;
		std::vector<uint16_t> sdnaTypeSizes;
	};
}