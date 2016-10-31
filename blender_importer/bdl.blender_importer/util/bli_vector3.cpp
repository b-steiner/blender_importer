#include <bdl.blender_importer/blender_importer.hpp>
#include "bli_vector3.hpp"

using namespace bdl::blender_importer;

bli_vector3::bli_vector3() : m_x(0), m_y(0), m_z(0) { }

bli_vector3::bli_vector3(float x, float y, float z) : m_x(x), m_y(y), m_z(z) { }

bli_vector3::bli_vector3(const bli_vector3 & vector) : m_x(vector.x()), m_y(vector.y()), m_z(vector.z()) { }

bli_vector3 & bli_vector3::operator=(const bli_vector3 & vector)
{
	m_x = vector.x();
	m_y = vector.y();
	m_z = vector.z();
	return *this;
}
