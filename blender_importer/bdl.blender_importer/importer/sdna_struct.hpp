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