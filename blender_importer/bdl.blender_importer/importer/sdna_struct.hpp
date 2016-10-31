#pragma once

#include "../blender_importer.hpp"

BEGIN_BDL_BLI

	class sdna_field
	{
		PROPERTY1(uint16_t, type_idx, GET);
		PROPERTY1(uint16_t, name_idx, GET);
		PROPERTY1(uint16_t, offset, GET);

	public:
		sdna_field(uint16_t name_idx, uint16_t type_idx, uint16_t offset);
	};

	class sdna_struct
	{
		PROPERTY1(uint16_t, name_idx, GET);
		
		typedef std::unordered_map<std::string, sdna_field*> string_field_map;
		PROPERTY1(string_field_map, fields, GET_REF);

	public:
		sdna_struct(uint16_t name_idx);
		~sdna_struct();
	};

END_BDL_BLI