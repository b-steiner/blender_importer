#include <bdl.blender_importer/blender_importer.hpp>
#include "bli_vector4.hpp"

using namespace bdl::blender_importer;

bli_vector4::bli_vector4() : m_x(0), m_y(0), m_z(0), m_w(0) { }

bli_vector4::bli_vector4(float x, float y, float z, float w) : m_x(x), m_y(y), m_z(z), m_w(w) { }

bli_vector4::bli_vector4(const bli_vector4 & vector) : m_x(vector.x()), m_y(vector.y()), m_z(vector.z()), m_w(vector.w()) { }

bli_vector4 & bli_vector4::operator=(const bli_vector4 & vector)
{
	m_x = vector.x();
	m_y = vector.y();
	m_z = vector.z();
	m_w = vector.w();
	return *this;
}
