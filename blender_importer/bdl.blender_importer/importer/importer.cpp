/**************************************************************************************
**
** Copyright (C) 2016 Bernhard Steiner
**
** This file is part of the blender_importer library
**
** This product is licensed under the GNU General Public License version 3.
** The license is as published by the Free Software Foundation published at
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** Alternatively, this product is licensed under the GNU Lesser General Public
** License version 3 for non-commercial use. The license is as published by the
** Free Software Foundation published at https://www.gnu.org/licenses/lgpl-3.0.html.
**
** Feel free to contact us if you have any questions about licensing or want
** to use the library in a commercial closed source product.
**
**************************************************************************************/


#include <bdl.blender_importer/blender_importer.hpp>
#include "importer.hpp"

#include "../util/bli_exception.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//This has to contain the correct gamma value for the system.
#define ONE_OVER_GAMMA 1

using namespace bdl::blender_importer;


#pragma region Helper Methods

glm::vec2 to_glm(const bli_vector2& v)
{
	return glm::vec2(v.x(), v.y());
}

glm::vec3 to_glm(const bli_vector3& v)
{
	return glm::vec3(v.x(), v.y(), v.z());
}

#pragma endregion



importer::importer() : m_data(nullptr)
{ 

	auto m = glm::rotate(glm::mat4(1.0f), 45.0f, glm::vec3(0, 0, 1)) * glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 5));

}
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
void importer::fileblock_to_xml(char* pptr, std::ofstream& out)
{
	char* ptr = pptr;
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
			{
				arraySize += value_to_xml(ptr + field->offset() + arraySize, field->type_idx(), m_sdna_type_sizes[field->type_idx()], m_sdna_is_pointer[field->name_idx()], out, intent);
				if (m_sdna_types[field->type_idx()] != "char")
					out << " ";
			}

			size += arraySize;
		}
		if (m_sdna_array_length[field->name_idx()].size() == 2)
		{
			uint64_t arraySize = 0;

			out << "\n" << intent << "\t";
			
			for (uint32_t arrayIdx1 = 0; arrayIdx1 < m_sdna_array_length[field->name_idx()][0]; arrayIdx1++)
			{
				for (uint32_t arrayIdx2 = 0; arrayIdx2 < m_sdna_array_length[field->name_idx()][1]; arrayIdx2++)
				{
					arraySize += value_to_xml(ptr + field->offset() + arraySize, field->type_idx(), m_sdna_type_sizes[field->type_idx()], m_sdna_is_pointer[field->name_idx()], out, intent);
					if (m_sdna_types[field->type_idx()] != "char")
						out << " ";
				}

				out << "\n" << intent << "\t";
			}

			size += arraySize;
			out << "\n" << intent;

			/*uint64_t arraySize = m_sdna_type_sizes[field->type_idx()];
			for (auto l : m_sdna_array_length[field->name_idx()])
				arraySize *= l;
			size += arraySize;*/
		}

		out << "</Array>" << std::endl << intent;
	}
	else
	{
		size += value_to_xml(ptr + field->offset(), field->type_idx(), m_sdna_type_sizes[field->type_idx()], m_sdna_is_pointer[field->name_idx()], out, intent);
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
void importer::parse_sdna_block(char* pptr)
{
	char* ptr = pptr + 16 + m_ptr_size;

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

	return m_asset;
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

	glm::vec3 loc = glm::make_vec3((float*)(m_data + strc->fields()["loc"]->offset()));
	glm::vec3 rot = glm::make_vec3((float*)(m_data + strc->fields()["rot"]->offset()));
	glm::vec3 size = glm::make_vec3((float*)(m_data + strc->fields()["size"]->offset()));

	//Correct parent-child hirarchical transformations
	//Blender stores transformations relative to the parent, but with an additional "offset" stored in the parentinv matrix.
	//So we have to get the parameters of this matrix and correct our transformations.
	//bli_matrix4 parentInv = R<bli_matrix4>(m_data + strc->fields()["parentinv"]->offset());
	glm::mat4 parentInv = glm::make_mat4((float*)(m_data + strc->fields()["parentinv"]->offset()));

	glm::vec3 parentScale(
		glm::length(parentInv[0]),
		glm::length(parentInv[1]),
		glm::length(parentInv[2])
		);

	result_node->scale(bli_vector3(size.x * parentScale.x, size.y * parentScale.y, size.z * parentScale.z));

	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), parentScale);
	glm::mat4 rsMatrix = parentInv;
	glm::mat4 rMatrix = rsMatrix * scaleMatrix;
	rMatrix = glm::transpose(rMatrix);
	rsMatrix = glm::transpose(rsMatrix);

	glm::vec3 ptranslation(parentInv[0][3], parentInv[1][3], parentInv[2][3]);
	glm::vec3 t = glm::vec3(rsMatrix * glm::vec4(loc, 0)) + ptranslation;
	result_node->translation(bli_vector3(t.x, t.y, t.z));

	glm::mat4 crmat = glm::rotate(glm::mat4(1.0f), rot.x, glm::vec3(1, 0, 0)) *
		glm::rotate(glm::mat4(1.0f), rot.y, glm::vec3(0, 1, 0)) *
		glm::rotate(glm::mat4(1.0f), rot.z, glm::vec3(0, 0, 1));
		//* AIOMatrix3::RotateY(rot.y) * AIOMatrix3::RotateX(rot.x);
	crmat = rMatrix * crmat;

	float theta = 0.0f;
	float psi = 0.0f;
	float phi = 0.0f;

	if (abs(crmat[2][0]) > 0.999)
	{
		phi = 0.0f;

		if (crmat[2][0] < 0.0f)
		{
			theta = (float)M_PI / 2.0f;
			psi = atan2f(crmat[0][1], crmat[0][2]);
		}
		else
		{
			theta = -(float)M_PI / 2.0f;
			psi = atan2f(-crmat[0][1], -crmat[0][2]);
		}
	}
	else
	{
		theta = -asinf(crmat[2][0]);
		psi = atan2f(crmat[2][1], crmat[2][2]);
		phi = atan2f(crmat[1][0] / cosf(theta), crmat[0][0] / cosf(theta));
	}

	result_node->rotation(bli_vector3(psi, theta, phi));


	//Find out what type of m_data is attached to this node
	uint64_t dataPtr = R<uint64_t>(m_data + strc->fields()["*data"]->offset());
	if (dataPtr != 0)
	{
		uint16_t dataType = R<uint16_t>(m_data + strc->fields()["type"]->offset());

		switch (dataType)
		{
		case 0: //No m_data
			break;
		case 1: //Mesh
			result_node->mesh(parse_mesh(dataPtr));
			result_node->material(parse_material(dataPtr));
			break;
		case 10: //Lamp
			result_node->lightsource(parse_light_source(dataPtr));
			break;
		}
	}

	//Check if there is an animation present
	auto anim_data_ptr = RPtr(m_data + strc->fields()["*adt"]->offset());
	if (anim_data_ptr != 0)
		result_node->animation(parse_animation(anim_data_ptr));

	for (int i = 0; i < m_object_blocks.size(); i++)
	{
		if (m_object_parent_addr[i] == addr)
			result_node->nodes().push_back(parse_node(m_object_blocks[i]));
	}

	return result_node;
}
mesh* importer::parse_mesh(uint64_t pptr)
{
	char* ptr = m_address_to_fileblock[pptr];
	sdna_struct* strc = m_sdna[R<uint32_t>(ptr + 8 + m_ptr_size)];
	sdna_struct* idstrc = m_sdna[m_name_to_sdna_idx["ID"]];
	auto custom_data_strc = m_sdna[m_name_to_sdna_idx["CustomData"]];
	auto custom_data_layer_strc = m_sdna[m_name_to_sdna_idx["CustomDataLayer"]];
	uint64_t custom_data_layer_size = m_sdna_type_sizes[custom_data_strc->fields()["*layers"]->type_idx()];

	ptr += 16 + m_ptr_size;

	std::string name = std::string(ptr + strc->fields()["id"]->offset() + idstrc->fields()["name"]->offset() + 2);

	auto meshIt = m_asset->m_meshes.find(name);
	if (meshIt != m_asset->m_meshes.end())
		return meshIt->second;

	//Not found -> load it
	mesh* result_mesh = new mesh(name);
	
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

	if (m_version_int < 263)
	{
		faceCount = R<uint32_t>(ptr + strc->fields()["totface"]->offset());
		triangles = faceCount;

		char* mfacePtr = m_address_to_fileblock[RPtr(ptr + strc->fields()["*mface"]->offset())] + 16 + m_ptr_size;
		char* mtfacePtr = nullptr;
		uint64_t mtfaceAddr = RPtr(ptr + strc->fields()["*mtface"]->offset());
		if (mtfaceAddr != 0)
		{
			mtfacePtr = m_address_to_fileblock[mtfaceAddr] + 16 + m_ptr_size;
		}

		indices.reserve(faceCount * 4);
		faceNormals = new bli_vector3[faceCount];
		if (mtfacePtr != nullptr)
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


			if (mtfacePtr != nullptr) // Load uv if necessary
			{
				float* uvPtr = (float*)(mtfacePtr + i * mtfaceSize + mtfaceStrc->fields()["uv"]->offset());

				for (unsigned int j = 0; j < faceLength[i]; j++)
					faceUvs[0].push_back(bli_vector2(uvPtr[2 * j], uvPtr[2 * j + 1]));
			}

			//Calculate face normal + read face flag
			//faceNormals[i] = bli_vector3::Normalize(bli_vector3::Cross(bli_vector3::Normalize(positions[indices[i * 4 + 1]] - positions[indices[i * 4]]),
			//	bli_vector3::Normalize(positions[indices[i * 4 + 2]] - positions[indices[i * 4]])));;
			glm::vec3 fnorm = glm::cross(
				glm::normalize(to_glm(positions[indices[i * 4 + 1]]) - to_glm(positions[indices[i * 4]])),
				glm::normalize(to_glm(positions[indices[i * 4 + 2]]) - to_glm(positions[indices[i * 4]]))
				);
			faceNormals[i] = bli_vector3(fnorm.x, fnorm.y, fnorm.z);

			faceFlags[i] = R<char>(mfacePtr + i * mfaceSize + mfaceStrc->fields()["flag"]->offset());
		}

	}

	#pragma endregion
	#pragma region Face Information >= 263

	else if (m_version_int >= 263)
	{
		faceCount = R<uint32_t>(ptr + strc->fields()["totpoly"]->offset());

		char* mpolyPtr = m_address_to_fileblock[RPtr(ptr + strc->fields()["*mpoly"]->offset())] + 16 + m_ptr_size;
		char* mloopPtr = m_address_to_fileblock[RPtr(ptr + strc->fields()["*mloop"]->offset())] + 16 + m_ptr_size;

		//UV Layers
		std::vector<char*> uv_layer_ptr; //= RPtr(ptr + strc->fields()["*mloopuv"]->offset());
		auto custom_data_layer_addr = RPtr(ptr + strc->fields()["ldata"]->offset() + custom_data_strc->fields()["*layers"]->offset());
		//char* mloopuvPtr = nullptr;
		if (custom_data_layer_addr != 0)
		{
			char* custom_data_layer_ptr = m_address_to_fileblock[custom_data_layer_addr];
			//Check number of uv layers
			uint32_t layer_count = *((uint32_t*)(custom_data_layer_ptr + 12 + m_ptr_size));
			custom_data_layer_ptr += 16 + m_ptr_size;

			for (int i = 0; i < layer_count; i++)
			{
				int32_t type = R<int32_t>(custom_data_layer_ptr + custom_data_layer_strc->fields()["type"]->offset());
				if (type == 16) //UV
				{
					auto uv_layer_addr = RPtr(custom_data_layer_ptr + custom_data_layer_strc->fields()["*data"]->offset());
					uv_layer_ptr.push_back(m_address_to_fileblock[uv_layer_addr] + 16 + m_ptr_size);
					result_mesh->tex_coord_names().push_back(std::string(custom_data_layer_ptr + custom_data_layer_strc->fields()["name"]->offset()));
				}

				custom_data_layer_ptr += custom_data_layer_size;
			}

			//mloopuvPtr = m_address_to_fileblock[mloopuvAddr] + 16 + m_ptr_size;
			//hasUvs = true;
		}

		uint64_t mpolySize = m_sdna_type_sizes[strc->fields()["*mpoly"]->type_idx()];
		uint64_t mloopSize = m_sdna_type_sizes[strc->fields()["*mloop"]->type_idx()];
		uint64_t mloopuvSize = m_sdna_type_sizes[strc->fields()["*mloopuv"]->type_idx()];

		indices.reserve(faceCount * 3);
		faceNormals = new bli_vector3[faceCount];
		faceFlags = new char[faceCount];
		faceStart = new unsigned int[faceCount];
		faceLength = new unsigned int[faceCount];

		faceUvs.reserve(uv_layer_ptr.size());
		for (int i = 0; i < uv_layer_ptr.size(); i++)
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
			//char* mloopuvFaceStart = mloopuvPtr + mloopuvSize * loopStart;

			triangles += (loopLength - 2);

			faceStart[face] = indexCount;
			faceLength[face] = loopLength;
			indexCount += loopLength;

			for (int i = 0; i < loopLength; i++)
			{
				indices.push_back(R<uint32_t>(mloopFaceStart + mloopStrc->fields()["v"]->offset()));

				for (int uvi = 0; uvi < uv_layer_ptr.size(); uvi++)
				{
					float* uv = (float*)(uv_layer_ptr[uvi] + i * mloopuvSize + mloopuvStrc->fields()["uv"]->offset());
					//float* uv = (float*)(mloopuvFaceStart + mloopuvStrc->fields()["uv"]->offset());
					faceUvs[uvi].push_back(bli_vector2(uv[0], 1.0f - uv[1])); //Blender has the texture origin on top left, OpenGL uses bottom left
				}

				mloopFaceStart += mloopSize;
				//mloopuvFaceStart += mloopuvSize;
			}

			//Face normal
			int first = 0;
			glm::vec3 fnorm = glm::vec3(0, 0, 0);
			while (glm::length(fnorm) <= 0.0001 && first < loopLength - 2)
			{
				glm::vec3 p0 = glm::vec3(positions[indices[faceStart[face]]].x(), positions[indices[faceStart[face]]].y(), positions[indices[faceStart[face]]].z());
				glm::vec3 p1 = glm::vec3(positions[indices[faceStart[face] + 1]].x(), positions[indices[faceStart[face] + 1]].y(), positions[indices[faceStart[face] + 1]].z());
				glm::vec3 p2 = glm::vec3(positions[indices[faceStart[face] + 2]].x(), positions[indices[faceStart[face] + 2]].y(), positions[indices[faceStart[face] + 2]].z());

				fnorm = glm::cross(glm::normalize(p1 - p0), glm::normalize(p2 - p0));
				first++;
			}

			faceNormals[face] = bli_vector3(fnorm.x, fnorm.y, fnorm.z);
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
	std::vector<unsigned int> indIndices;
	indIndices.reserve(triangles * 3);
	std::vector<unsigned int> indexCache;

	std::vector<std::vector<bli_vector2>> indUVs;
	indUVs.reserve(faceUvs.size());
	for (int i = 0; i < faceUvs.size(); i++)
	{
		std::vector<bli_vector2> init;
		init.reserve(triangles * 3);
		indUVs.push_back(init);
	}

	std::vector<bli_vector2> uv;
	uv.reserve(faceUvs.size());

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

			uv.clear();
			for (int uvi = 0; uvi < faceUvs.size(); uvi++)
				uv.push_back(faceUvs[uvi][findex]);

			std::string key = key_from_vertex(p, n, uv);
			auto vit = vertexDictionary.find(key);
			if (vit == vertexDictionary.end())
			{
				vertexDictionary[key] = (unsigned int)indPositions.size();
				indPositions.push_back(p);
				indNormals.push_back(n);
				for (int uvi = 0; uvi < faceUvs.size(); uvi++)
					indUVs[uvi].push_back(uv[uvi]);
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

				glm::vec3 p1 = to_glm(indPositions[indexCache[startIndex]]);
				glm::vec3 p2 = to_glm(indPositions[indexCache[i1]]);
				glm::vec3 p3 = to_glm(indPositions[indexCache[i2]]);

				float mag = glm::length(glm::cross(p2 - p1, p3 - p1));

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
	result_mesh->indices() = indIndices;
	result_mesh->positions() = indPositions;
	result_mesh->normals() = indNormals;

	for (int i = 0; i < indUVs.size(); i++)
	{
		result_mesh->tex_coords().push_back(indUVs[i]);
		calculate_tangents(result_mesh);
	}

	m_asset->m_meshes[result_mesh->name()] = result_mesh;

	return result_mesh;
}
material* importer::parse_material(uint64_t pptr)
{
	sdna_struct* matStrc = m_sdna[m_name_to_sdna_idx["Material"]];
	sdna_struct* idStrc = m_sdna[m_name_to_sdna_idx["ID"]];
	sdna_struct* meshStrc = m_sdna[m_name_to_sdna_idx["Mesh"]];
	sdna_struct* mtexStrc = m_sdna[m_name_to_sdna_idx["MTex"]];
	sdna_struct* texStrc = m_sdna[m_name_to_sdna_idx["Tex"]];
	sdna_struct* imaStrc = m_sdna[m_name_to_sdna_idx["Image"]];
	sdna_struct* linkStrc = m_sdna[m_name_to_sdna_idx["Link"]];

	char* meshPtr = m_address_to_fileblock[pptr] + 16 + m_ptr_size;
	uint64_t matLinkAddr = RPtr(meshPtr + meshStrc->fields()["**mat"]->offset());
	
	if (matLinkAddr == 0)
		return m_asset->m_materials["default-material"];

	char* matLinkPtr = m_address_to_fileblock[matLinkAddr] + 16 + m_ptr_size;
	uint64_t matAddr = RPtr(matLinkPtr + linkStrc->fields()["*next"]->offset());

	if (matAddr == 0)
		return m_asset->m_materials["default-material"];

	char* matPtr = m_address_to_fileblock[matAddr] + 16 + m_ptr_size;
	std::string name(matPtr + matStrc->fields()["id"]->offset() + idStrc->fields()["name"]->offset() + 2);

	auto matit = m_asset->m_materials.find(name);
	if (matit != m_asset->m_materials.end())
		return matit->second;

	//Not cached -> load
	material* result_material = new material(name);
	m_asset->m_materials[name] = result_material;

	result_material->diffuse_color(bli_vector3(
		powf(R<float>(matPtr + matStrc->fields()["r"]->offset()), ONE_OVER_GAMMA),
		powf(R<float>(matPtr + matStrc->fields()["g"]->offset()), ONE_OVER_GAMMA),
		powf(R<float>(matPtr + matStrc->fields()["b"]->offset()), ONE_OVER_GAMMA)));
	result_material->specular_color(bli_vector3(
		powf(R<float>(matPtr + matStrc->fields()["specr"]->offset()), ONE_OVER_GAMMA),
		powf(R<float>(matPtr + matStrc->fields()["specg"]->offset()), ONE_OVER_GAMMA),
		powf(R<float>(matPtr + matStrc->fields()["specb"]->offset()), ONE_OVER_GAMMA)));
	result_material->diffuse_intensity(R<float>(matPtr + matStrc->fields()["ref"]->offset()));
	result_material->specular_intensity(R<float>(matPtr + matStrc->fields()["spec"]->offset()));
	result_material->emitting_intensity(R<float>(matPtr + matStrc->fields()["emit"]->offset()));
	result_material->opacity(R<float>(matPtr + matStrc->fields()["alpha"]->offset()));
	result_material->hardness(R<uint16_t>(matPtr + matStrc->fields()["har"]->offset()));

	int mode = R<uint32_t>(matPtr + matStrc->fields()["mode"]->offset());
	material_mode matMode = material_mode::none;
	if ((mode & 0x4) != 0)
		matMode = matMode | material_mode::shadeless;
	if ((mode & 0x10000) != 0)
		matMode = matMode | material_mode::transparent;
	result_material->mode(matMode);

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
						path = m_file_path.substr(0, m_file_path.find_last_of('/')) + "/" + path;

					auto texit = m_asset->m_textures.find(path);
					texture* result_texture = nullptr;
					if (texit != m_asset->m_textures.end())
						result_texture = texit->second;
					else
					{
						result_texture = new texture();
						result_texture->path(path);
						m_asset->m_textures[path] = result_texture;

						if (m_version_int > 263)
						{
							sdna_struct* colorspaceStrc = m_sdna[m_name_to_sdna_idx["ColorManagedColorspaceSettings"]];
							std::string colorspace(imaPtr + imaStrc->fields()["colorspace_settings"]->offset() + colorspaceStrc->fields()["name"]->offset());

							if (colorspace == "sRGB")
								result_texture->color_space(color_space::SRGB);
							else if (colorspace == "Linear")
								result_texture->color_space(color_space::linear);
							else
								result_texture->color_space(color_space::other);
						}
						else
							result_texture->color_space(color_space::SRGB);
					}

					uint16_t mapTo = R<uint16_t>(mtexPtr + mtexStrc->fields()["mapto"]->offset());
					if ((mapTo & 0x01) != 0)
					{
						result_material->textures()[mapping_target::diffuse] = result_texture;
						result_material->texture_influence()[mapping_target::diffuse] = R<float>(mtexPtr + mtexStrc->fields()["colfac"]->offset());
					}
					if ((mapTo & 0x02) != 0)
					{
						result_material->textures()[mapping_target::normals] = result_texture;
						result_material->texture_influence()[mapping_target::normals] = R<float>(mtexPtr + mtexStrc->fields()["norfac"]->offset());
					}
				}
				else
				{
					std::cout << "Material " << name << " contains an unsupported texture, skipping it" << std::endl;
				}
			}
		}

		mtexStart += m_ptr_size;
	}


	return result_material;
}
light_source* importer::parse_light_source(uint64_t ptr)
{
	sdna_struct* lampStrc = m_sdna[m_name_to_sdna_idx["Lamp"]];
	sdna_struct* idStrc = m_sdna[m_name_to_sdna_idx["ID"]];

	char* lampPtr = m_address_to_fileblock[ptr] + 16 + m_ptr_size;
	std::string name(lampPtr + lampStrc->fields()["id"]->offset() + idStrc->fields()["name"]->offset() + 2);

	light_source* light = new light_source(name);

	uint16_t type = R<uint16_t>(lampPtr + lampStrc->fields()["type"]->offset());

	switch (type)
	{
	case 0:
		light->type(light_source_type::pointlight);
		break;
	case 1:
		light->type(light_source_type::directionallight);
		break;
	case 2:
		light->type(light_source_type::spotlight);
		break;
	}

	light->color(bli_vector3(
		pow(R<float>(lampPtr + lampStrc->fields()["r"]->offset()), ONE_OVER_GAMMA),
		pow(R<float>(lampPtr + lampStrc->fields()["g"]->offset()), ONE_OVER_GAMMA),
		pow(R<float>(lampPtr + lampStrc->fields()["b"]->offset()), ONE_OVER_GAMMA)));
	light->intensity(R<float>(lampPtr + lampStrc->fields()["energy"]->offset()));
	light->angular_attenuation(R<float>(lampPtr + lampStrc->fields()["spotblend"]->offset()));
	float angle = R<float>(lampPtr + lampStrc->fields()["spotsize"]->offset());

	if (m_version_int < 270)
		angle = angle / 180.0f * M_PI;

	light->angle(angle);

	float dist = R<float>(lampPtr + lampStrc->fields()["dist"]->offset());
	uint16_t falloffType = R<uint16_t>(lampPtr + lampStrc->fields()["falloff_type"]->offset());

	switch (falloffType)
	{
	case 1:
		light->attenuation(bli_vector3(1.0f, 1.0f / dist, 0.0f));
		break;
	case 2:
		light->attenuation(bli_vector3(1.0f, 0.0f, 1.0f / (dist * dist)));
		break;
	default:
		light->attenuation(bli_vector3(1.0f, 0.0f, 0.0f));
		break;
	}

	light->distance(dist);

	auto mode = R<uint32_t>(lampPtr + lampStrc->fields()["mode"]->offset());

	light->has_clipped_sphere((mode & 0x40) != 0);

	return light;
}
animation* importer::parse_animation(uint64_t ptr)
{
	auto anim_data_strc = m_sdna[m_name_to_sdna_idx["AnimData"]];
	auto baction_data_strc = m_sdna[m_name_to_sdna_idx["bAction"]];
	auto listbase_data_strc = m_sdna[m_name_to_sdna_idx["ListBase"]];
	auto fcurve_data_strc = m_sdna[m_name_to_sdna_idx["FCurve"]];
	auto link_data_strc = m_sdna[m_name_to_sdna_idx["Link"]];
	auto bezt_data_strc = m_sdna[m_name_to_sdna_idx["BezTriple"]];
	sdna_struct* idStrc = m_sdna[m_name_to_sdna_idx["ID"]];

	auto anim_data = m_address_to_fileblock[ptr] + 16 + m_ptr_size;
	auto action_ptr = RPtr(anim_data + anim_data_strc->fields()["*action"]->offset());

	if (action_ptr == 0)
		return nullptr;

	auto action_data = m_address_to_fileblock[action_ptr] + 16 + m_ptr_size;
	auto curve_ptr = RPtr(action_data + baction_data_strc->fields()["curves"]->offset() + listbase_data_strc->fields()["*first"]->offset());
	
	if (curve_ptr == 0)
		return nullptr;

	animation* anim = new animation(std::string(action_data + baction_data_strc->fields()["id"]->offset() + idStrc->fields()["name"]->offset() + 2));

	while (curve_ptr != 0)
	{
		auto curve_data = m_address_to_fileblock[curve_ptr] + 16 + m_ptr_size;

		//Mapping target
		auto rna_link_ptr = RPtr(curve_data + fcurve_data_strc->fields()["*rna_path"]->offset());
		auto rna_data_str = m_address_to_fileblock[rna_link_ptr] + 16 + m_ptr_size;
		auto array_idx = R<int32_t>(curve_data + fcurve_data_strc->fields()["array_index"]->offset());

		int base_target = -1;
		if (strcmp(rna_data_str, "location") == 0)
			base_target = (int)animation_target::locX;
		else if (strcmp(rna_data_str, "rotation_euler") == 0)
			base_target = (int)animation_target::rotX;
		else if (strcmp(rna_data_str, "scale") == 0)
			base_target = (int)animation_target::sizeX;
		else
		{
			std::cout << "Animation to unsupported target found: " << rna_data_str;
			continue;
		}

		anim->target().push_back((animation_target)(base_target + array_idx));

		//Control points
		auto bezt_ptr = RPtr(curve_data + fcurve_data_strc->fields()["*bezt"]->offset());
		auto bezt_data = m_address_to_fileblock[bezt_ptr];
		auto bezt_count = R<uint32_t>(bezt_data + 12 + m_ptr_size);
		bezt_data += 16 + m_ptr_size;
		auto bezt_size = m_sdna_type_sizes[fcurve_data_strc->fields()["*bezt"]->type_idx()];

		std::vector<bli_vector2> points;
		points.reserve(bezt_count);
		std::vector<bli_vector2> prev_vec;
		prev_vec.reserve(bezt_count);
		std::vector<bli_vector2> next_vec;
		next_vec.reserve(bezt_count);
		std::vector<interpolation_mode> interp;
		interp.reserve(bezt_count);

		for (int bezi = 0; bezi < bezt_count; bezi++)
		{
			auto current_bezt = bezt_data + bezi * bezt_size;

			auto vec_ptr = current_bezt + bezt_data_strc->fields()["vec"]->offset();
			bli_vector2 prev(*((float*)vec_ptr), *((float*)(vec_ptr + 4)));
			vec_ptr += 12;
			bli_vector2 pt(*((float*)vec_ptr), *((float*)(vec_ptr + 4)));
			vec_ptr += 12;
			bli_vector2 next(*((float*)vec_ptr), *((float*)(vec_ptr + 4)));

			if (m_version_int >= 271)
			{
				char ipo = R<char>(current_bezt + bezt_data_strc->fields()["ipo"]->offset());
				interp.push_back((interpolation_mode)ipo);
			}
			else
			{
				short ipo = R<short>(current_bezt + bezt_data_strc->fields()["ipo"]->offset());
				interp.push_back((interpolation_mode)ipo);
			}

			points.push_back(pt);
			prev_vec.push_back(prev);
			next_vec.push_back(next);
		}

		anim->points().push_back(points);
		anim->prev_handles().push_back(prev_vec);
		anim->next_handles().push_back(next_vec);
		anim->interpolation_mode().push_back(interp);

		curve_ptr = RPtr(curve_data + fcurve_data_strc->fields()["*next"]->offset());
	}

	return anim;
}

std::string importer::key_from_vertex(bli_vector3 _position, bli_vector3 _normal, std::vector<bli_vector2>& uvs)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(4) << _position.x() << ";" << _position.y() << ";" << _position.z() << ";" << _normal.x() << ";" << _normal.y() << ";" << _normal.z()
		<< ";";
	for (auto& uv : uvs)
		ss << uv.x() << ";" << uv.y() << ";";
	return ss.str();
}
void importer::calculate_tangents(mesh* _mesh)
{
	for (std::vector<bli_vector2>& uvs : _mesh->tex_coords())
	{
		std::vector<bli_vector4> finalTangents(uvs.size());
		glm::vec3* vertexTangents = new glm::vec3[uvs.size()];
		glm::vec3* vertexBitangents = new glm::vec3[uvs.size()];

		for (int i = 0; i < _mesh->indices().size(); i+=3)
		{
			unsigned int i1 = _mesh->indices()[i];
			unsigned int i2 = _mesh->indices()[i+1];
			unsigned int i3 = _mesh->indices()[i+2];

			auto uv1 = to_glm(uvs[i1]);
			auto uv2 = to_glm(uvs[i2]);
			auto uv3 = to_glm(uvs[i3]);

			auto p1 = to_glm(_mesh->positions()[i1]);
			auto p2 = to_glm(_mesh->positions()[i2]);
			auto p3 = to_glm(_mesh->positions()[i3]);

			auto e1 = p2 - p1;
			auto e2 = p3 - p1;

			auto uve1 = uv2 - uv1;
			auto uve2 = uv3 - uv1;

			float area = 1.0f / (uve1.x * uve2.y + uve1.y * uve2.x);

			glm::vec3 svec(
				uve2.y * e1.x - uve1.y * e2.x,
				uve2.y * e1.y - uve1.y * e2.y,
				uve2.y * e1.z - uve1.y * e2.z);

			glm::vec3 tvec(
				uve1.x * e2.x - uve2.x * e1.x,
				uve1.x * e2.y - uve2.x * e1.y,
				uve1.x * e2.z - uve2.x * e1.z);

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
			if (glm::dot(glm::cross(to_glm(_mesh->normals()[i]), vertexTangents[i]), vertexBitangents[i]) < 0.0f)
				headness = -1.0f;
			glm::vec4 ftang = glm::vec4(glm::normalize(vertexTangents[i] - to_glm(_mesh->normals()[i]) * glm::dot(vertexTangents[i], to_glm(_mesh->normals()[i]))), headness);
			finalTangents[i] = bli_vector4(ftang.x, ftang.y, ftang.z, ftang.w);
		}

		_mesh->tangents().push_back(finalTangents);
	}
}


bool importer::check_structure(const std::string& _path)
{
	load_file(_path);
	bool result = true;

	std::cout << "Checking Version " << m_version_number << " (x" << m_ptr_size * 8 << ")";

	result &= check_structure("Object", "id", "ID");
	result &= check_structure("Object", "loc", "float");
	result &= check_structure("Object", "rot", "float");
	result &= check_structure("Object", "size", "float");
	result &= check_structure("Object", "parentinv", "float");
	result &= check_structure("Object", "*data", "void");
	result &= check_structure("Object", "type", "short");
	result &= check_structure("Object", "*parent", "Object");
	result &= check_structure("Object", "*adt", "AnimData");

	result &= check_structure("AnimData", "*action", "bAction");

	result &= check_structure("bAction", "id", "ID");
	result &= check_structure("bAction", "curves", "ListBase");

	result &= check_structure("ListBase", "*first", "void");

	result &= check_structure("FCurve", "*rna_path", "char");
	result &= check_structure("FCurve", "array_index", "int");
	result &= check_structure("FCurve", "*bezt", "BezTriple");

	result &= check_structure("BezTriple", "vec", "float");

	if (m_version_int <= 270)
	{
		result &= check_structure("BezTriple", "ipo", "short");
	}
	else
	{
		result &= check_structure("BezTriple", "ipo", "char");
	}

	result &= check_structure("Lamp", "type", "short");
	result &= check_structure("Lamp", "r", "float");
	result &= check_structure("Lamp", "g", "float");
	result &= check_structure("Lamp", "b", "float");
	result &= check_structure("Lamp", "energy", "float");
	result &= check_structure("Lamp", "spotsize", "float");
	result &= check_structure("Lamp", "spotblend", "float");
	result &= check_structure("Lamp", "dist", "float");
	result &= check_structure("Lamp", "falloff_type", "short");

	result &= check_structure("Mesh", "id", "ID");
	result &= check_structure("Mesh", "totvert", "int");
	result &= check_structure("Mesh", "*mvert", "MVert");
	result &= check_structure("Mesh", "**mat", "Material");
	result &= check_structure("Mesh", "totface", "int", 0, 263);
	result &= check_structure("Mesh", "*mface", "MFace", 0, 263);
	result &= check_structure("Mesh", "*mtface", "MTFace", 0, 263);
	result &= check_structure("Mesh", "totpoly", "int", 264);
	result &= check_structure("Mesh", "*mpoly", "MPoly", 264);
	result &= check_structure("Mesh", "*mloop", "MLoop", 264);
	result &= check_structure("Mesh", "*mloopuv", "MLoopUV", 264);

	result &= check_structure("MPoly", "loopstart", "int", 264);
	result &= check_structure("MPoly", "totloop", "int", 264);
	result &= check_structure("MPoly", "flag", "char", 264);
	result &= check_structure("MLoop", "v", "int", 264);
	result &= check_structure("MLoopUV", "uv", "float", 264);

	result &= check_structure("MFace", "v1", "int", 0, 263);
	result &= check_structure("MFace", "v2", "int", 0, 263);
	result &= check_structure("MFace", "v3", "int", 0, 263);
	result &= check_structure("MFace", "v4", "int", 0, 263);
	result &= check_structure("MFace", "flag", "char", 0, 263);
	result &= check_structure("MTFace", "uv", "float", 0, 263);

	

	result &= check_structure("Material", "r", "float");
	result &= check_structure("Material", "g", "float");
	result &= check_structure("Material", "b", "float");
	result &= check_structure("Material", "specr", "float");
	result &= check_structure("Material", "specg", "float");
	result &= check_structure("Material", "specb", "float");
	result &= check_structure("Material", "ref", "float");
	result &= check_structure("Material", "spec", "float");
	result &= check_structure("Material", "emit", "float");
	result &= check_structure("Material", "alpha", "float");
	result &= check_structure("Material", "har", "short");
	result &= check_structure("Material", "mode", "int");
	result &= check_structure("Material", "*mtex", "MTex");

	result &= check_structure("MTex", "*tex", "Tex");
	result &= check_structure("MTex", "mapto", "short");
	result &= check_structure("Tex", "*ima", "Image");
	result &= check_structure("Image", "name", "char");
	result &= check_structure("Image", "colorspace_settings", "ColorManagedColorspaceSettings", 264);
	result &= check_structure("ColorManagedColorspaceSettings", "name", "char", 264);

	result &= check_structure("MVert", "co", "float");
	result &= check_structure("MVert", "no", "short");

	result &= check_structure("ID", "name", "char");
	result &= check_structure("Link", "*next", "Link");

	if (result)
		std::cout << "        ....   structure ok!" << std::endl;

	return result;
}
int importer::version_int() const
{
	return m_version_int;
}
bool importer::check_structure(const std::string& strcName, const std::string& fieldName, const std::string& fieldType, int minVersion, int maxVersion)
{
	if (minVersion <= m_version_int && maxVersion >= m_version_int)
	{
		auto sdnaId = m_name_to_sdna_idx[strcName];
		if (m_sdna.find(sdnaId) == m_sdna.end())
		{
			std::cout << std::endl << "Unable to find structure " << strcName << std::endl;
			return false;
		}

		sdna_struct* strc = m_sdna[sdnaId];

		if (strc->fields().find(fieldName) == strc->fields().end())
		{
			std::cout << std::endl << "Unable to find field " << strcName << "." << fieldName << std::endl;
			return false;
		}

		sdna_field* field = strc->fields()[fieldName];

		if (m_sdna_types[field->type_idx()] != fieldType)
		{
			std::cout << std::endl << "Field " << strcName << "." << fieldName << " is " << m_sdna_types[field->type_idx()] << " but should be " << fieldType << std::endl;
			return false;
		}
	}

	return true;
}