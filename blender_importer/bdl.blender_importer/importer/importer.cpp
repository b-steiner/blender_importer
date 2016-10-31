#include <bdl.blender_importer/blender_importer.hpp>
#include "importer.hpp"

#include "../util/bli_exception.hpp"

//This has to contain the correct gamma value for the system. For windows this is 1/2.2 = 0.45454545
#define ONE_OVER_GAMMA 0.4545454545f


using namespace bdl::blender_importer;

importer::importer() : m_data(nullptr)
{ }
importer::~importer()
{
	for (auto str : m_sdna)
		delete str.second;
	if (m_data != nullptr)
		delete m_data;
}

asset* importer::load(const std::string& path)
{
	load_file(path);
	return parse_asset(path);
}


void importer::to_xml(const std::string& input_path, const std::string& output_path)
{
	load_file(input_path);

	std::ofstream out;
	out.open(output_path);

	out << "<Blender Version=\"" << m_version_number << "\" PointerSize=\"" << m_ptr_size << "\" Endianness=\"" << m_endianness << "\">" << std::endl;

	out << "\t" << "<SDNABlock>" << std::endl;

	for (auto str : m_sdna)
	{
		out << "\t\t" << "<sdna_struct SDNAIdx=\"" << str.first << "\" Name=\"" << m_sdna_types[str.second->name_idx()] << "\">" << std::endl;

		for (auto field : str.second->fields())
		{
			out << "\t\t\t" << "<sdna_field Name=\"" << m_sdna_names[field.second->name_idx()] << "\" Type=\"" << m_sdna_types[field.second->type_idx()] << "\" Size=\"" <<
				m_sdna_type_sizes[field.second->type_idx()] << "\"";
			out << " IsPointer=\"" << m_sdna_is_pointer[field.second->name_idx()] << "\"";

			if (m_sdna_array_length[field.second->name_idx()].size() > 0)
			{
				out << " ArrayLength=\"";
				for (auto arr : m_sdna_array_length[field.second->name_idx()])
					out << arr << ", ";
				out << "\"";
			}
			
			out << "/>" << std::endl;
		}

		out << "\t\t" << "</sdna_struct>" << std::endl;
	}
	out << "\t" << "</SDNABlock>" << std::endl;

	for (auto fb : m_address_to_fileblock)
		fileblock_to_xml(fb.second, out);


	out << "</Blender>" << std::endl;

	out.close();
}
void importer::fileblock_to_xml(char* ptr, std::ofstream& out)
{
	char* ptr = ptr;
	char identifier[5];
	identifier[4] = '\0';

	memcpy(identifier, ptr, 4);
	ptr += 4;

	uint32_t size = R<uint32_t>(ptr);
	ptr += 4;

	uint64_t addr = RPtr(ptr);
	ptr += m_ptr_size;

	uint32_t sdnaIndex = R<uint32_t>(ptr);
	ptr += 4;
	uint32_t count = R<uint32_t>(ptr);
	ptr += 4;

	sdna_struct* strc = m_sdna[sdnaIndex];

	out << "\t" << "<FileBlock Identifier=\"" << identifier << "\" Size=\"" << size << "\" OriginalAddr=\"" << addr << "\" SDNAIndex=\"" << sdnaIndex
		 << "\" SDNATypeName=\"" << m_sdna_types[strc->name_idx()]
		 << "\" Count=\"" << count << "\">" << std::endl;

	if (sdnaIndex == 114)
		int asdf = 4;

	std::string intent = "\t\t";
	if (count > 1)
		intent += "\t";

	for (uint32_t i = 0; i < count; i++)
	{
		if (count > 1)
			out << "\t\t" << "<Instance Index =\"" << i << "\">" << std::endl;

		uint64_t strcSize = sdna_struct_to_xml(ptr, strc, out, intent);
		ptr += strcSize;

		if (count > 1)
			out << "\t\t" << "</Instance>" << std::endl;
	}


	out << "\t" << "</FileBlock>" << std::endl;
}

uint64_t importer::sdna_struct_to_xml(char* ptr, sdna_struct* strc, std::ofstream& out, std::string& intent)
{
	uint64_t size = 0;

	for (auto field : strc->fields())
	{
		size += sdna_field_to_xml(ptr, field.second, out, intent);
	}

	return size;
}

uint64_t importer::sdna_field_to_xml(char* ptr, sdna_field* field, std::ofstream& out, std::string& intent)
{
	uint64_t size = 0;

	std::string& name = m_sdna_names[field->name_idx()];
	out << intent << "<" << name << " Type=\"" << m_sdna_types[field->type_idx()] << "\" Size=\"" << m_sdna_type_sizes[field->type_idx()] << "\">";

	//Value
	if (m_sdna_array_length[field->name_idx()].size() > 0)
	{
		out << std::endl;
		out << intent << "\t" << "<Array Length=\"";
		for (auto l : m_sdna_array_length[field->name_idx()])
			out << l << ", ";
		out << "\">";

		if (m_sdna_array_length[field->name_idx()].size() == 1)
		{
			uint64_t arraySize = 0;

			for (uint32_t arrayIdx = 0; arrayIdx < m_sdna_array_length[field->name_idx()][0]; arrayIdx++)
				arraySize += value_to_xml(ptr + field->offset() + arraySize, field->type_idx(), m_sdna_type_sizes[field->type_idx()], m_sdna_is_pointer[field->name_idx()], out, intent);

			size += arraySize;
		}
		if (m_sdna_array_length[field->name_idx()].size() == 2)
		{
			uint64_t arraySize = m_sdna_type_sizes[field->type_idx()];
			for (auto l : m_sdna_array_length[field->name_idx()])
				arraySize *= l;
			size += arraySize;
		}

		out << "</Array>" << std::endl << intent;
	}
	else
	{
		size += Value2XML(ptr + field->offset(), field->type_idx(), m_sdna_type_sizes[field->type_idx()], m_sdna_is_pointer[field->name_idx()], out, intent);
	}

	out << "</" << name << ">" << std::endl;
	return size;
}

uint64_t importer::value_to_xml(char* ptr, uint16_t type_idx, uint16_t size, bool is_ptr, std::ofstream& out, std::string& intent)
{
	std::string type = m_sdna_types[type_idx];

	if (is_ptr)
	{
		out << RPtr(ptr);
		return m_ptr_size;
	}

	if (type == "char")
		out << R<char>(ptr);
	else if (type == "short")
		out << R<int16_t>(ptr);
	else if (type == "int")
		out << R<int32_t>(ptr);
	else if (type == "float")
		out << R<float>(ptr);
	else if (type == "double")
		out << R<double>(ptr);
	else
	{
		//Subtype
		sdna_struct* strc = m_sdna[m_name_to_sdna_idx[type]];

		out << std::endl;

		for (auto field : strc->fields())
		{
			sdna_field_to_xml(ptr, field.second, out, intent + "\t");
		}

		out << std::endl << intent;
	}

	return size;
}



void importer::load_file(const std::string& path)
{
	if (path != m_file_path)
	{
		m_sdna.clear();
		m_sdna_types.clear();
		m_sdna_is_pointer.clear();
		m_sdna_array_length.clear();
		m_sdna_type_sizes.clear();
		m_address_to_fileblock.clear();
		m_name_to_sdna_idx.clear();

		FILE* file = fopen(path.c_str(), "rb");
		if (file == nullptr)
			throw bli_exception("Unable to open file");

		fseek(file, 0, SEEK_END);
		m_data_size = ftell(file);
		fseek(file, 0, SEEK_SET);

		if (m_data != nullptr)
			delete m_data;
		m_data = new char[m_data_size];
		fread(m_data, m_data_size, 1, file);
		fclose(file);
		file = nullptr;

		//Parse header

		//Ident
		memcpy(m_ident, m_data, 7);
		m_ident[7] = '\0';
		if (strcmp(m_ident, "BLENDER") != 0)
			throw bli_exception(std::string("File Identifier has to be \"BLENDER\", but was ") + std::string(m_ident));

		//Ptr Size
		m_ptr_char = m_data[7];
		if (m_ptr_char == '_')
			m_ptr_size = 4;
		else if (m_ptr_char == '-')
			m_ptr_size = 8;
		else
			throw bli_exception("PointerSize was not \'-\' or \'_\'");

		m_endianness = m_data[8];
		memcpy(m_version_number, m_data + 9, 3);
		m_version_number[3] = '\0';

		m_version_int = 0;
		for (int i = 0; i < 3; i++)
			m_version_int += (m_version_number[i] - '0') * (int)powl(10, 2 - i);

		//Go over all fileblocks
		char* ptr = m_data + 12; // Start of first fileblock

		while (ptr < m_data + m_data_size)
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
			uint32_t count = R<uint32_t>(ptr + 12 + m_ptr_size);

			if (strcmp(code, "DNA1") == 0)
				parse_sdna_block(ptr);
			else
			{
				m_address_to_fileblock[oldAddress] = ptr;
			}

			//To next block
			ptr += (16 + m_ptr_size);
			ptr += size;
		}
	}
}
void importer::parse_sdna_block(char* ptr)
{
	char* ptr = ptr + 16 + m_ptr_size;

	char code[5];
	code[4] = '\0';
	
	memcpy(code, ptr, 4);
	if (strcmp(code, "SDNA") != 0)
		throw new bli_exception(std::string("SDNA Block identifier hast to be SDNA, but is ") + std::string(code));
	ptr += 4;


	// NAMES
	if ((uint64_t)ptr % 4 != 0)
		ptr += 4 - ((uint64_t)ptr % 4);
	memcpy(code, ptr, 4);
	if (strcmp(code, "NAME") != 0)
		throw new bli_exception(std::string("SDNA Subblock identifier hast to be NAME, but is ") + std::string(code));
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

		m_sdna_is_pointer.push_back(isPtr);
		m_sdna_names.push_back(std::string(buffer));
		m_sdna_array_length.push_back(std::vector<uint32_t>());

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
			m_sdna_array_length.back().push_back((uint32_t)length);

			ptr++;
		}

		ptr++;
	}

	//TYPES
	if ((uint64_t)ptr % 4 != 0)
		ptr += 4 - ((uint64_t)ptr % 4);
	memcpy(code, ptr, 4);
	if (strcmp(code, "TYPE") != 0)
		throw new bli_exception(std::string("SDNA Subblock identifier hast to be TYPE, but is ") + std::string(code));
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

		m_sdna_types.push_back(std::string(buffer));
	}

	if ((uint64_t)ptr % 4 != 0)
		ptr += 4 - ((uint64_t)ptr % 4);
	memcpy(code, ptr, 4);
	if (strcmp(code, "TLEN") != 0)
		throw new bli_exception(std::string("SDNA Subblock identifier hast to be TLEN, but is ") + std::string(code));
	ptr += 4;

	for (uint32_t i = 0; i < typeCount; i++)
	{
		m_sdna_type_sizes.push_back(R<uint16_t>(ptr));
		ptr += 2;
	}


	// STRUCTURES
	if ((uint64_t)ptr % 4 != 0)
		ptr += 4 - ((uint64_t)ptr % 4);
	memcpy(code, ptr, 4);
	if (strcmp(code, "STRC") != 0)
		throw new bli_exception(std::string("SDNA Subblock identifier hast to be STRC, but is ") + std::string(code));
	ptr += 4;

	uint32_t structureCount = R<uint32_t>(ptr);
	ptr += 4;

	for (uint32_t i = 0; i < structureCount; i++)
	{
		uint16_t nameIdx = R<uint16_t>(ptr);
		ptr += 2;

		sdna_struct* str = new sdna_struct(nameIdx);

		uint16_t fieldCount = R<uint16_t>(ptr);
		ptr += 2;

		uint16_t offset = 0;

		for (int j = 0; j < fieldCount; j++)
		{
			uint16_t fieldType = R<uint16_t>(ptr);
			ptr += 2;
			uint16_t fieldName = R<uint16_t>(ptr);
			ptr += 2;

			sdna_field* field = new sdna_field(fieldName, fieldType, offset);
			str->fields()[m_sdna_names[fieldName]] = field;

			if (m_sdna_is_pointer[fieldName])
				offset += m_ptr_size;
			else
			{
				if (m_sdna_array_length[fieldName].size() == 0)
					offset += m_sdna_type_sizes[fieldType];
				else
				{
					uint16_t multiply = 1;
					for (auto l : m_sdna_array_length[fieldName])
						multiply *= l;
					offset += m_sdna_type_sizes[fieldType] * multiply;
				}
			}
		}

		auto name = m_sdna_types[nameIdx];
		m_name_to_sdna_idx[name] = i;
		m_sdna[i] = str;
	}
}


asset* importer::parse_asset(const std::string& _path)
{
	m_asset = new asset(_path);

	auto objectId = m_name_to_sdna_idx["Object"];
	sdna_struct* objectStrc = m_sdna[objectId];
	m_object_blocks.clear();
	m_object_parent_addr.clear();

	//Find all nodes
	for (auto fb : m_address_to_fileblock)
	{
		uint32_t sdnaIdx = R<uint32_t>(fb.second + 8 + m_ptr_size);

		if (sdnaIdx == objectId)
		{
			m_object_blocks.push_back(fb.second);
			m_object_parent_addr.push_back(RPtr(fb.second + 16 + m_ptr_size + objectStrc->fields()["*parent"]->offset()));
		}
	}

	for (int i = 0; i < m_object_blocks.size(); i++)
	{
		if (m_object_parent_addr[i] == 0)
			m_asset->nodes().push_back(parse_node(m_object_blocks[i]));
	}

	return asset;
}
node* importer::parse_node(char* ptr)
{
	uint64_t addr = RPtr(ptr + 8);
	char* m_data = ptr + 16 + m_ptr_size;

	auto sdnaIdx = m_name_to_sdna_idx["Object"];
	sdna_struct* strc = m_sdna[sdnaIdx];

	sdna_struct* idStrc = m_sdna[m_name_to_sdna_idx["ID"]];
	char* idPtr = m_data + strc->fields()["id"]->offset();

	std::string name = std::string(idPtr + idStrc->fields()["name"]->offset() + 2);
	node* result_node = new node(name);

	bli_vector3 loc = R<bli_vector3>(m_data + strc->fields()["loc"]->offset());
	bli_vector3 rot = R<bli_vector3>(m_data + strc->fields()["rot"]->offset());
	bli_vector3 size = R<bli_vector3>(m_data + strc->fields()["size"]->offset());

	//Correct parent-child hirarchical transformations
	//Blender stores transformations relative to the parent, but with an additional "offset" stored in the parentinv matrix.
	//So we have to get the parameters of this matrix and correct our transformations.
	bli_matrix4 parentInv = R<bli_matrix4>(m_data + strc->fields()["parentinv"]->offset());
	bli_vector3 parentScale(
		sqrtf(parentInv.M11() *parentInv.M11() + parentInv.M12() * parentInv.M12() + parentInv.M13() * parentInv.M13()),
		sqrtf(parentInv.M21() *parentInv.M21() + parentInv.M22() * parentInv.M22() + parentInv.M23() * parentInv.M23()),
		sqrtf(parentInv.M31() *parentInv.M31() + parentInv.M32() * parentInv.M32() + parentInv.M33() * parentInv.M33())
		);

	size = bli_vector3(size.X() * parentScale.X(), size.Y() * parentScale.Y(), size.Z() * parentScale.Z());
	node->Scale(size);

	AIOMatrix3 scaleMatrix = AIOMatrix3::Scale(parentScale);
	AIOMatrix3 rsMatrix = parentInv.GetMat3();
	AIOMatrix3 rMatrix = rsMatrix * scaleMatrix;
	rMatrix.Transpose();
	rsMatrix.Transpose();

	bli_vector3 ptranslation(parentInv.M14(), parentInv.M24(), parentInv.M34());
	node->Translation(rsMatrix * loc + ptranslation);

	AIOMatrix3 crmat = AIOMatrix3::RotateZ(rot.Z()) * AIOMatrix3::RotateY(rot.Y()) * AIOMatrix3::RotateX(rot.X());
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

	node->Rotation(bli_vector3(psi, theta, phi));


	//Find out what type of m_data is attached to this node
	uint64_t dataPtr = R<uint64_t>(m_data + strc->fields()["*m_data"]->offset());
	if (dataPtr != 0)
	{
		uint16_t dataType = R<uint16_t>(m_data + strc->fields()["type"]->offset());

		switch (dataType)
		{
		case 0: //No m_data
			break;
		case 1: //Mesh
			node->Mesh(ParseMesh(dataPtr));
			node->Material(ParseMaterial(dataPtr));
			break;
		case 10: //Lamp
			node->LightSource(ParseLightSource(dataPtr));
			break;
		}
	}

	for (int i = 0; i < objectBlocks.size(); i++)
	{
		if (objectParentAddr[i] == addr)
			node->Nodes().push_back(ParseNode(objectBlocks[i]));
	}

	return node;
}
AIOMesh* importer::ParseMesh(uint64_t ptr)
{
	char* ptr = m_address_to_fileblock[ptr];
	sdna_struct* strc = m_sdna[R<uint32_t>(ptr + 8 + m_ptr_size)];
	sdna_struct* idstrc = m_sdna[m_name_to_sdna_idx["ID"]];

	ptr += 16 + m_ptr_size;

	std::string name = std::string(ptr + strc->fields()["id"]->offset() + idstrc->fields()["name"]->offset() + 2);

	auto meshIt = asset->meshes.find(name);
	if (meshIt != asset->meshes.end())
		return meshIt->second;

	//Not found -> load it
	AIOMesh* mesh = new AIOMesh(name);
	
	bli_vector3* positions;
	bli_vector3* normals;

	std::vector<unsigned int> indices;
	unsigned int* faceStart;
	unsigned int* faceLength;
	std::vector<std::vector<bli_vector2>> faceUvs;
	bli_vector3* faceNormals;
	char* faceFlags;

	int triangles = 0;
	int faceCount = 0;
	bool hasUvs = false;

	//Load m_data from block
	uint32_t vertexCount = R<uint32_t>(ptr + strc->fields()["totvert"]->offset());
	positions = new bli_vector3[vertexCount];
	normals = new bli_vector3[vertexCount];

	char* mvertPtr = m_address_to_fileblock[RPtr(ptr + strc->fields()["*mvert"]->offset())] + 16 + m_ptr_size;
	uint32_t mvertSize = m_sdna_type_sizes[strc->fields()["*mvert"]->type_idx()];
	sdna_struct* mvertstrc = m_sdna[m_name_to_sdna_idx["MVert"]];

	for (unsigned int i = 0; i < vertexCount; i++)
	{
		positions[i] = bli_vector3((float*)(mvertPtr + mvertSize * i + mvertstrc->fields()["co"]->offset()));
		int16_t* noPtr = (int16_t*)(mvertPtr + mvertSize * i + mvertstrc->fields()["no"]->offset());
		int32_t li = noPtr[0] * noPtr[0] + noPtr[1] * noPtr[1] + noPtr[2] * noPtr[2];
		double l = sqrt((double)li);
		normals[i] = bli_vector3((float)(((double)noPtr[0]) / l), (float)(((double)noPtr[1]) / l), (float)(((double)noPtr[2]) / l));
	}

	#pragma region Face Information < 263

	if (versionInt < 263)
	{
		faceCount = R<uint32_t>(ptr + strc->fields()["totface"]->offset());
		triangles = faceCount;

		char* mfacePtr = m_address_to_fileblock[RPtr(ptr + strc->fields()["*mface"]->offset())] + 16 + m_ptr_size;
		char* mtfacePtr = nullptr;
		uint64_t mtfaceAddr = RPtr(ptr + strc->fields()["*mtface"]->offset());
		if (mtfaceAddr != 0)
		{
			mtfacePtr = m_address_to_fileblock[mtfaceAddr] + 16 + m_ptr_size;
			hasUvs = true;
		}

		indices.reserve(faceCount * 4);
		faceNormals = new bli_vector3[faceCount];
		if (hasUvs)
		{
			std::vector<bli_vector2> inituv;
			inituv.reserve(faceCount * 4);
			faceUvs.push_back(inituv);
		}
		faceFlags = new char[faceCount];
		faceStart = new unsigned int[faceCount];
		faceLength = new unsigned int[faceCount];

		//Get face blocks
		sdna_struct* mfaceStrc = m_sdna[m_name_to_sdna_idx["MFace"]];
		sdna_struct* mtfaceStrc = m_sdna[m_name_to_sdna_idx["MTFace"]];
		uint32_t mfaceSize = m_sdna_type_sizes[strc->fields()["*mface"]->type_idx()];
		uint32_t mtfaceSize = m_sdna_type_sizes[strc->fields()["*mtface"]->type_idx()];

		char* uvPtr = mtfacePtr + mtfaceStrc->fields()["uv"]->offset();
		
		unsigned int indexCount = 0;

		for (int i = 0; i < faceCount; i++)
		{
			faceStart[i] = indexCount;

			//Read indices
			char* indexPtr = mfacePtr + i * mfaceSize;
			indices.push_back(R<uint32_t>(indexPtr + mfaceStrc->fields()["v1"]->offset()));
			indices.push_back(R<uint32_t>(indexPtr + mfaceStrc->fields()["v2"]->offset()));
			indices.push_back(R<uint32_t>(indexPtr + mfaceStrc->fields()["v3"]->offset()));
			uint32_t v4 = R<uint32_t>(indexPtr + mfaceStrc->fields()["v4"]->offset());
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
				float* uvPtr = (float*)(mtfacePtr + i * mtfaceSize + mtfaceStrc->fields()["uv"]->offset());

				for (unsigned int j = 0; j < faceLength[i]; j++)
					faceUvs[0].push_back(bli_vector2(uvPtr[2 * j], uvPtr[2 * j + 1]));
			}

			//Calculate face normal + read face flag
			faceNormals[i] = bli_vector3::Normalize(bli_vector3::Cross(bli_vector3::Normalize(positions[indices[i * 4 + 1]] - positions[indices[i * 4]]),
				bli_vector3::Normalize(positions[indices[i * 4 + 2]] - positions[indices[i * 4]])));;
			faceFlags[i] = R<char>(mfacePtr + i * mfaceSize + mfaceStrc->fields()["flag"]->offset());
		}

	}

	#pragma endregion
	#pragma region Face Information >= 263

	else if (versionInt >= 263)
	{
		faceCount = R<uint32_t>(ptr + strc->fields()["totpoly"]->offset());

		char* mpolyPtr = m_address_to_fileblock[RPtr(ptr + strc->fields()["*mpoly"]->offset())] + 16 + m_ptr_size;
		char* mloopPtr = m_address_to_fileblock[RPtr(ptr + strc->fields()["*mloop"]->offset())] + 16 + m_ptr_size;

		uint64_t mloopuvAddr = RPtr(ptr + strc->fields()["*mloopuv"]->offset());
		char* mloopuvPtr = nullptr;
		if (mloopuvAddr != 0)
		{
			mloopuvPtr = m_address_to_fileblock[mloopuvAddr] + 16 + m_ptr_size;
			hasUvs = true;
		}
		uint64_t mpolySize = m_sdna_type_sizes[strc->fields()["*mpoly"]->type_idx()];
		uint64_t mloopSize = m_sdna_type_sizes[strc->fields()["*mloop"]->type_idx()];
		uint64_t mloopuvSize = m_sdna_type_sizes[strc->fields()["*mloopuv"]->type_idx()];

		indices.reserve(faceCount * 3);
		faceNormals = new bli_vector3[faceCount];
		faceFlags = new char[faceCount];
		faceStart = new unsigned int[faceCount];
		faceLength = new unsigned int[faceCount];
		if (hasUvs)
		{
			std::vector<bli_vector2> inituv;
			faceUvs.push_back(inituv);
			faceUvs.back().reserve(faceCount * 4);
		}

		sdna_struct* mpolyStrc = m_sdna[m_name_to_sdna_idx["MPoly"]];
		sdna_struct* mloopStrc = m_sdna[m_name_to_sdna_idx["MLoop"]];
		sdna_struct* mloopuvStrc = m_sdna[m_name_to_sdna_idx["MLoopUV"]];

		unsigned int indexCount = 0;

		for (int face = 0; face < faceCount; face++)
		{
			char* mpolyFaceStart = mpolyPtr + mpolySize * face;

			int loopStart = R<uint32_t>(mpolyFaceStart + mpolyStrc->fields()["loopstart"]->offset());
			int loopLength = R<uint32_t>(mpolyFaceStart + mpolyStrc->fields()["totloop"]->offset());

			char* mloopFaceStart = mloopPtr + mloopSize * loopStart;
			char* mloopuvFaceStart = mloopuvPtr + mloopuvSize * loopStart;

			triangles += (loopLength - 2);

			faceStart[face] = indexCount;
			faceLength[face] = loopLength;
			indexCount += loopLength;

			for (int i = 0; i < loopLength; i++)
			{
				indices.push_back(R<uint32_t>(mloopFaceStart + mloopStrc->fields()["v"]->offset()));

				if (hasUvs)
				{
					float* uv = (float*)(mloopuvFaceStart + mloopuvStrc->fields()["uv"]->offset());
					faceUvs[0].push_back(bli_vector2(uv[0], 1.0f - uv[1])); //Blender has the texture origin on top left, OpenGL uses bottom left
				}

				mloopFaceStart += mloopSize;
				mloopuvFaceStart += mloopuvSize;
			}

			//Face normal
			int first = 0;
			faceNormals[face] = bli_vector3();
			while (faceNormals[face].Length() <= 0.0001 && first < loopLength - 2)
			{
				faceNormals[face] = bli_vector3::Normalize(bli_vector3::Cross(
					bli_vector3::Normalize(positions[indices[faceStart[face] + 1]] - positions[indices[faceStart[face]]]),
					bli_vector3::Normalize(positions[indices[faceStart[face] + 2]] - positions[indices[faceStart[face]]])
					));
				first++;
			}

			faceFlags[face] = R<char>(mpolyFaceStart + mpolyStrc->fields()["flag"]->offset());
		}		
	}

	#pragma endregion


	#pragma region Triangulation & Indices

	std::unordered_map < std::string, unsigned int > vertexDictionary;
	std::vector<bli_vector3> indPositions;
	indPositions.reserve(triangles * 3);
	std::vector<bli_vector3> indNormals;
	indNormals.reserve(triangles * 3);
	std::vector<bli_vector2> indUVs;
	indUVs.reserve(triangles * 3);
	std::vector<unsigned int> indIndices;
	indIndices.reserve(triangles * 3);
	std::vector<unsigned int> indexCache;

	for (int face = 0; face < faceCount; face++)
	{
		indexCache.clear();

		//Find unique vertices
		for (unsigned int findex = faceStart[face]; findex < faceStart[face] + faceLength[face]; findex++)
		{
			bli_vector3 p = positions[indices[findex]];
			bli_vector3 n = normals[indices[findex]];
			if (faceFlags[face] % 2 == 0)
				n = faceNormals[face];
			bli_vector2 uv;

			if (hasUvs)
				uv = faceUvs[0][findex];

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
				int i2 = (startIndex + findex + 1) % faceLength[face];

				bli_vector3 p1 = indPositions[indexCache[startIndex]];
				bli_vector3 p2 = indPositions[indexCache[i1]];
				bli_vector3 p3 = indPositions[indexCache[i2]];

				float mag = bli_vector3::Cross(p2 - p1, p3 - p1).Length();

				if (mag < 0.000001)
				{
					isDegenerated = true;
					startIndex++;
					break;
				}
			}
		}

		//Perform triangulation
		for (unsigned int findex = 1; findex < faceLength[face] - 1; findex++)
		{
			int i1 = (startIndex + findex) % faceLength[face];
			int i2 = (startIndex + findex + 1) % faceLength[face];

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

	asset->meshes[mesh->Name()] = mesh;

	return mesh;
}
AIOMaterial* importer::ParseMaterial(uint64_t ptr)
{
	sdna_struct* matStrc = m_sdna[m_name_to_sdna_idx["Material"]];
	sdna_struct* idStrc = m_sdna[m_name_to_sdna_idx["ID"]];
	sdna_struct* meshStrc = m_sdna[m_name_to_sdna_idx["Mesh"]];
	sdna_struct* mtexStrc = m_sdna[m_name_to_sdna_idx["MTex"]];
	sdna_struct* texStrc = m_sdna[m_name_to_sdna_idx["Tex"]];
	sdna_struct* imaStrc = m_sdna[m_name_to_sdna_idx["Image"]];
	sdna_struct* linkStrc = m_sdna[m_name_to_sdna_idx["Link"]];

	char* meshPtr = m_address_to_fileblock[ptr] + 16 + m_ptr_size;
	uint64_t matLinkAddr = RPtr(meshPtr + meshStrc->fields()["**mat"]->offset());
	
	if (matLinkAddr == 0)
		return asset->materials["default-material"];

	char* matLinkPtr = m_address_to_fileblock[matLinkAddr] + 16 + m_ptr_size;
	uint64_t matAddr = RPtr(matLinkPtr + linkStrc->fields()["*next"]->offset());

	if (matAddr == 0)
		return asset->materials["default-material"];

	char* matPtr = m_address_to_fileblock[matAddr] + 16 + m_ptr_size;
	std::string name(matPtr + matStrc->fields()["id"]->offset() + idStrc->fields()["name"]->offset());

	auto matit = asset->materials.find(name);
	if (matit != asset->materials.end())
		return matit->second;

	//Not cached -> load
	AIOMaterial* material = new AIOMaterial(name);
	asset->materials[name] = material;

	material->DiffuseColor(bli_vector3(
		powf(R<float>(matPtr + matStrc->fields()["r"]->offset()), ONE_OVER_GAMMA),
		powf(R<float>(matPtr + matStrc->fields()["g"]->offset()), ONE_OVER_GAMMA),
		powf(R<float>(matPtr + matStrc->fields()["b"]->offset()), ONE_OVER_GAMMA)));
	material->SpecularColor(bli_vector3(
		powf(R<float>(matPtr + matStrc->fields()["specr"]->offset()), ONE_OVER_GAMMA),
		powf(R<float>(matPtr + matStrc->fields()["specg"]->offset()), ONE_OVER_GAMMA),
		powf(R<float>(matPtr + matStrc->fields()["specb"]->offset()), ONE_OVER_GAMMA)));
	material->DiffuseIntensity(R<float>(matPtr + matStrc->fields()["ref"]->offset()));
	material->SpecularIntensity(R<float>(matPtr + matStrc->fields()["spec"]->offset()));
	material->EmittingIntensity(R<float>(matPtr + matStrc->fields()["emit"]->offset()));
	material->Opacity(R<float>(matPtr + matStrc->fields()["alpha"]->offset()));
	material->Hardness(R<uint16_t>(matPtr + matStrc->fields()["har"]->offset()));

	int mode = R<uint32_t>(matPtr + matStrc->fields()["mode"]->offset());
	AIOMaterialMode matMode = AIOMaterialMode::None;
	if ((mode & 0x4) != 0)
		matMode = matMode | AIOMaterialMode::Shadeless;
	if ((mode & 0x4) != 0)
		matMode = matMode | AIOMaterialMode::Shadeless;
	material->Mode(matMode);

	uint32_t mtexLength = m_sdna_array_length[matStrc->fields()["*mtex"]->name_idx()][0];
	char* mtexStart = matPtr + matStrc->fields()["*mtex"]->offset();

	for (unsigned int mtexId = 0; mtexId < mtexLength; mtexId++)
	{
		uint64_t mtexAddr = RPtr(mtexStart);

		if (mtexAddr != 0)
		{
			char* mtexPtr = m_address_to_fileblock[mtexAddr] + 16 + m_ptr_size;
			uint64_t texAddr = RPtr(mtexPtr + mtexStrc->fields()["*tex"]->offset());

			if (texAddr != 0)
			{
				char* texPtr = m_address_to_fileblock[texAddr] + 16 + m_ptr_size;
				uint64_t imaAddr = RPtr(texPtr + texStrc->fields()["*ima"]->offset());

				if (imaAddr != 0)
				{
					//Image texture
					char* imaPtr = m_address_to_fileblock[imaAddr] + 16 + m_ptr_size;
					std::string path(imaPtr + imaStrc->fields()["name"]->offset() + 2);
					std::replace(path.begin(), path.end(), '\\', '/');

					if (path[0] != '/' && path.find(':') == path.npos) // add filename to path if not absolute
						path = filePath.substr(0, filePath.find_last_of('/')) + "/" + path;

					auto texit = asset->textures.find(path);
					AIOTexture* texture = nullptr;
					if (texit != asset->textures.end())
						texture = texit->second;
					else
					{
						texture = new AIOTexture();
						texture->Path(path);
						asset->textures[path] = texture;

						if (versionInt > 263)
						{
							sdna_struct* colorspaceStrc = m_sdna[m_name_to_sdna_idx["ColorManagedColorspaceSettings"]];
							std::string colorspace(imaPtr + imaStrc->fields()["colorspace_settings"]->offset() + colorspaceStrc->fields()["name"]->offset());

							if (colorspace == "sRGB")
								texture->ColorSpace(bli_color_space::SRGB);
							else if (colorspace == "Linear")
								texture->ColorSpace(bli_color_space::Linear);
							else
								texture->ColorSpace(bli_color_space::Other);
						}
						else
							texture->ColorSpace(bli_color_space::SRGB);
					}

					uint16_t mapTo = R<uint16_t>(mtexPtr + mtexStrc->fields()["mapto"]->offset());
					if ((mapTo & 0x01) != 0)
						material->Textures()[bli_mapping_target::Diffuse] = texture;
					if ((mapTo & 0x02) != 0)
						material->Textures()[bli_mapping_target::Normals] = texture;
				}
				else
				{
					std::cout << "Material " << name << " contains an unsupported texture, skipping it" << std::endl;
				}
			}
		}

		mtexStart += m_ptr_size;
	}


	return material;
}
AIOLightSource* importer::ParseLightSource(uint64_t ptr)
{
	sdna_struct* lampStrc = m_sdna[m_name_to_sdna_idx["Lamp"]];
	sdna_struct* idStrc = m_sdna[m_name_to_sdna_idx["ID"]];

	char* lampPtr = m_address_to_fileblock[ptr] + 16 + m_ptr_size;
	std::string name(lampPtr + lampStrc->fields()["id"]->offset() + idStrc->fields()["name"]->offset());

	AIOLightSource* light = new AIOLightSource(name);

	uint16_t type = R<uint16_t>(lampPtr + lampStrc->fields()["type"]->offset());

	switch (type)
	{
	case 0:
		light->Type(AIOLightSourceType::PointLight);
		break;
	case 1:
		light->Type(AIOLightSourceType::DirectionalLight);
		break;
	case 2:
		light->Type(AIOLightSourceType::SpotLight);
		break;
	}

	light->Color(bli_vector3(
		pow(R<float>(lampPtr + lampStrc->fields()["r"]->offset()), ONE_OVER_GAMMA),
		pow(R<float>(lampPtr + lampStrc->fields()["g"]->offset()), ONE_OVER_GAMMA),
		pow(R<float>(lampPtr + lampStrc->fields()["b"]->offset()), ONE_OVER_GAMMA)));
	light->Intensity(R<float>(lampPtr + lampStrc->fields()["energy"]->offset()));
	light->AngularAttenuation(R<float>(lampPtr + lampStrc->fields()["spotblend"]->offset()));
	light->Angle(R<float>(lampPtr + lampStrc->fields()["spotsize"]->offset()));

	float dist = R<float>(lampPtr + lampStrc->fields()["dist"]->offset());
	uint16_t falloffType = R<uint16_t>(lampPtr + lampStrc->fields()["falloff_type"]->offset());

	switch (falloffType)
	{
	case 1:
		light->Attenuation(bli_vector3(1.0f, 1.0f / dist, 0.0f));
		break;
	case 2:
		light->Attenuation(bli_vector3(1.0f, 0.0f, 1.0f / (dist * dist)));
		break;
	default:
		light->Attenuation(bli_vector3(1.0f, 0.0f, 0.0f));
		break;
	}

	light->Distance(dist);

	auto mode = R<uint32_t>(lampPtr + lampStrc->fields()["mode"]->offset());

	light->HasClippingSphere((mode & 0x40) == 1);

	return light;
}

std::string importer::KeyFromVertex(bli_vector3 _position, bli_vector3 _normal, bli_vector2 _uv)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(4) << _position.X() << ";" << _position.Y() << ";" << _position.Z() << ";" << _normal.X() << ";" << _normal.Y() << ";" << _normal.Z()
		<< ";" << _uv.X() << "; " << _uv.Y();
	return ss.str();
}
void importer::CalculateTangents(AIOMesh* _mesh)
{
	for (std::vector<bli_vector2>& uvs : _mesh->TexCoords())
	{
		std::vector<bli_vector4> finalTangents(uvs.size());
		bli_vector3* vertexTangents = new bli_vector3[uvs.size()];
		bli_vector3* vertexBitangents = new bli_vector3[uvs.size()];

		for (int i = 0; i < _mesh->Indices().size(); i+=3)
		{
			unsigned int i1 = _mesh->Indices()[i];
			unsigned int i2 = _mesh->Indices()[i+1];
			unsigned int i3 = _mesh->Indices()[i+2];

			bli_vector2 uv1 = uvs[i1];
			bli_vector2 uv2 = uvs[i2];
			bli_vector2 uv3 = uvs[i3];

			bli_vector3 p1 = _mesh->Positions()[i1];
			bli_vector3 p2 = _mesh->Positions()[i2];
			bli_vector3 p3 = _mesh->Positions()[i3];

			bli_vector3 e1 = p2 - p1;
			bli_vector3 e2 = p3 - p1;

			bli_vector2 uve1 = uv2 - uv1;
			bli_vector2 uve2 = uv3 - uv1;

			float area = 1.0f / (uve1.X() * uve2.Y() + uve1.Y() * uve2.X());

			bli_vector3 svec(
				uve2.Y() * e1.X() - uve1.Y() * e2.X(),
				uve2.Y() * e1.Y() - uve1.Y() * e2.Y(),
				uve2.Y() * e1.Z() - uve1.Y() * e2.Z());

			bli_vector3 tvec(
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
			if (bli_vector3::Dot(bli_vector3::Cross(_mesh->Normals()[i], vertexTangents[i]), vertexBitangents[i]) < 0.0f)
				headness = -1.0f;
			finalTangents[i] = bli_vector4(bli_vector3::Normalize(vertexTangents[i] - _mesh->Normals()[i] * bli_vector3::Dot(vertexTangents[i], _mesh->Normals()[i])), headness);
		}

		_mesh->Tangents().push_back(finalTangents);
	}
}


bool importer::CheckStructure(const std::string& _path)
{
	LoadFile(_path);
	bool result = true;

	std::cout << "Checking Version " << versionNumber << " (x" << m_ptr_size * 8 << ")";

	result &= CheckStructure("Object", "id", "ID");
	result &= CheckStructure("Object", "loc", "float");
	result &= CheckStructure("Object", "rot", "float");
	result &= CheckStructure("Object", "size", "float");
	result &= CheckStructure("Object", "parentinv", "float");
	result &= CheckStructure("Object", "*m_data", "void");
	result &= CheckStructure("Object", "type", "short");
	result &= CheckStructure("Object", "*parent", "Object");

	result &= CheckStructure("Lamp", "type", "short");
	result &= CheckStructure("Lamp", "r", "float");
	result &= CheckStructure("Lamp", "g", "float");
	result &= CheckStructure("Lamp", "b", "float");
	result &= CheckStructure("Lamp", "energy", "float");
	result &= CheckStructure("Lamp", "spotsize", "float");
	result &= CheckStructure("Lamp", "spotblend", "float");
	result &= CheckStructure("Lamp", "dist", "float");
	result &= CheckStructure("Lamp", "falloff_type", "short");

	result &= CheckStructure("Mesh", "id", "ID");
	result &= CheckStructure("Mesh", "totvert", "int");
	result &= CheckStructure("Mesh", "*mvert", "MVert");
	result &= CheckStructure("Mesh", "**mat", "Material");
	result &= CheckStructure("Mesh", "totface", "int", 0, 263);
	result &= CheckStructure("Mesh", "*mface", "MFace", 0, 263);
	result &= CheckStructure("Mesh", "*mtface", "MTFace", 0, 263);
	result &= CheckStructure("Mesh", "totpoly", "int", 264);
	result &= CheckStructure("Mesh", "*mpoly", "MPoly", 264);
	result &= CheckStructure("Mesh", "*mloop", "MLoop", 264);
	result &= CheckStructure("Mesh", "*mloopuv", "MLoopUV", 264);

	result &= CheckStructure("MPoly", "loopstart", "int", 264);
	result &= CheckStructure("MPoly", "totloop", "int", 264);
	result &= CheckStructure("MPoly", "flag", "char", 264);
	result &= CheckStructure("MLoop", "v", "int", 264);
	result &= CheckStructure("MLoopUV", "uv", "float", 264);

	result &= CheckStructure("MFace", "v1", "int", 0, 263);
	result &= CheckStructure("MFace", "v2", "int", 0, 263);
	result &= CheckStructure("MFace", "v3", "int", 0, 263);
	result &= CheckStructure("MFace", "v4", "int", 0, 263);
	result &= CheckStructure("MFace", "flag", "char", 0, 263);
	result &= CheckStructure("MTFace", "uv", "float", 0, 263);

	

	result &= CheckStructure("Material", "r", "float");
	result &= CheckStructure("Material", "g", "float");
	result &= CheckStructure("Material", "b", "float");
	result &= CheckStructure("Material", "specr", "float");
	result &= CheckStructure("Material", "specg", "float");
	result &= CheckStructure("Material", "specb", "float");
	result &= CheckStructure("Material", "ref", "float");
	result &= CheckStructure("Material", "spec", "float");
	result &= CheckStructure("Material", "emit", "float");
	result &= CheckStructure("Material", "alpha", "float");
	result &= CheckStructure("Material", "har", "short");
	result &= CheckStructure("Material", "mode", "int");
	result &= CheckStructure("Material", "*mtex", "MTex");

	result &= CheckStructure("MTex", "*tex", "Tex");
	result &= CheckStructure("MTex", "mapto", "short");
	result &= CheckStructure("Tex", "*ima", "Image");
	result &= CheckStructure("Image", "name", "char");
	result &= CheckStructure("Image", "colorspace_settings", "ColorManagedColorspaceSettings", 264);
	result &= CheckStructure("ColorManagedColorspaceSettings", "name", "char", 264);

	result &= CheckStructure("MVert", "co", "float");
	result &= CheckStructure("MVert", "no", "short");

	result &= CheckStructure("ID", "name", "char");
	result &= CheckStructure("Link", "*next", "Link");

	if (result)
		std::cout << "        ....    ok!" << std::endl;

	return result;
}
bool importer::CheckStructure(const std::string& _strcName, const std::string& _fieldName, const std::string& _fieldType, int _minVersion, int _maxVersion)
{
	if (_minVersion <= versionInt && _maxVersion >= versionInt)
	{
		auto sdnaId = m_name_to_sdna_idx[_strcName];
		if (m_sdna.find(sdnaId) == m_sdna.end())
		{
			std::cout << std::endl << "Unable to find structure " << _strcName << std::endl;
			return false;
		}

		sdna_struct* strc = m_sdna[sdnaId];

		if (strc->fields().find(_fieldName) == strc->fields().end())
		{
			std::cout << std::endl << "Unable to find field " << _strcName << "." << _fieldName << std::endl;
			return false;
		}

		sdna_field* field = strc->fields()[_fieldName];

		if (m_sdna_types[field->type_idx()] != _fieldType)
		{
			std::cout << std::endl << "Field " << _strcName << "." << _fieldName << " is " << m_sdna_types[field->type_idx()] << " but should be " << _fieldType << std::endl;
			return false;
		}
	}

	return true;
}