#pragma once

#include "../blender_importer.hpp"
#include "../model/asset.hpp"
#include "sdna_struct.hpp"

BEGIN_BDL_BLI

	class BLI_EXPORT importer
	{
	public:
		importer();
		~importer();

		asset* load(const std::string& path);
		void to_xml(const std::string& input_path, const std::string& output_path);
		bool check_structure(const std::string& path);

	private:
		void load_file(const std::string& path);
		void parse_sdna_block(char* ptr);

		void fileblock_to_xml(char* ptr, std::ofstream& out);
		uint64_t sdna_struct_to_xml(char* ptr, sdna_struct* strc, std::ofstream& out, std::string& intent);
		uint64_t sdna_field_to_xml(char* ptr, sdna_field* field, std::ofstream& out, std::string& intent);
		uint64_t value_to_xml(char* ptr, uint16_t type_idx, uint16_t size, bool is_ptr, std::ofstream& out, std::string& intent);
		
		asset* parse_asset(const std::string& path);
		node* parse_node(char* ptr);
		mesh* parse_mesh(uint64_t ptr);
		material* parse_material(uint64_t ptr);
		light_source* parse_light_source(uint64_t ptr);

		std::string key_from_vertex(bli_vector3 position, bli_vector3 normal, bli_vector2 uv);
		void calculate_tangents(mesh* mesh);


		bool check_structure(const std::string& strc_name, const std::string& field_name, const std::string& field_type, int min_version = 0, int max_version = 99999);



		template <typename T> T R(char* ptr)
		{
			return (T)*((T*)ptr);
		}
		template <> bli_vector3 R<bli_vector3>(char* ptr)
		{
			return bli_vector3(R<float>(ptr), R<float>(ptr + 4), R<float>(ptr + 8));
		}
		template <> bli_matrix4 R<bli_matrix4>(char* ptr)
		{
			return bli_matrix4((float*)ptr);
		}
		uint64_t RPtr(char* ptr)
		{
			if (ptrSize == 4)
				return (uint64_t)(*((uint32_t*)ptr));
			else
				return *((uint64_t*)ptr);
		}

		char* m_data;
		size_t m_data_size;
		char m_ident[8];
		char m_ptr_char;
		int m_ptr_size;
		char m_endianness;
		char m_version_number[4];
		int m_version_int;

		std::unordered_map<uint64_t, char*> m_address_to_fileblock;
		std::unordered_map<std::string, uint32_t> m_name_to_sdna_idx;
		std::unordered_map<uint32_t, sdna_struct*> m_sdna;

		std::vector<std::string> m_sdna_names;
		std::vector<std::string> m_sdna_types;
		std::vector<bool> m_sdna_is_pointer;
		std::vector<std::vector<uint32_t>> m_sdna_array_length;
		std::vector<uint16_t> m_sdna_type_sizes;

		std::vector<char*> m_object_blocks;
		std::vector<uint64_t> m_object_parent_addr;

		std::string m_file_path;
		asset* m_asset;
	};

END_BDL_BLI