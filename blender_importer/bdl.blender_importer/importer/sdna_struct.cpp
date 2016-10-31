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