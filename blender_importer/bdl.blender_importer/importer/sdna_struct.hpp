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

	/*! \brief Represents a data field in a sdna_struct
	 *
	 * \author bdl
	 */
	class sdna_field
	{
		//! Stores the index of the type
		PROPERTY1(uint16_t, type_idx, GET);
		//! Stores the index of the name
		PROPERTY1(uint16_t, name_idx, GET);
		//! Stores the offset to the begin of the sdna_struct
		PROPERTY1(uint16_t, offset, GET);

	public:
		/*! \brief Initializes a new instance of the sdna_field class
		 *
		 * \param name_idx The index of the name
		 * \param type_idx The index of the datatype
		 * \param offset The offset to the begin of the structure
		 */
		sdna_field(uint16_t name_idx, uint16_t type_idx, uint16_t offset);
	};

	/*! \brief Contains information about a blender data structure
	 *
	 * \author bdl
	 */
	class sdna_struct
	{
		//! Stores the index to the name of the structure
		PROPERTY1(uint16_t, name_idx, GET);
		
		typedef std::unordered_map<std::string, sdna_field*> string_field_map;
		//! Stores a number of fields addressed by the field name
		PROPERTY1(string_field_map, fields, GET_REF);

	public:
		/*! \brief Initializes a new instance of the sdna_field class
		 *
		 * \param name_idx The index of the name
		 */
		sdna_struct(uint16_t name_idx);
		/*! \brief Releases all data associated with an instance of the sdna_struct class
		 */
		~sdna_struct();
	};

END_BDL_BLI