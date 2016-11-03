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
#include "sdna_struct.hpp"

using namespace bdl::blender_importer;

sdna_field::sdna_field(uint16_t name_idx, uint16_t type_idx, uint16_t offset) : m_name_idx(name_idx), m_type_idx(type_idx), m_offset(offset) { }



sdna_struct::sdna_struct(uint16_t name_idx) : m_name_idx(name_idx)
{ }
sdna_struct::~sdna_struct()
{
	for (auto it : m_fields)
		delete it.second;
}