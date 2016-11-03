#include <bdl.blender_importer/blender_importer.hpp>
#include "bli_vector2.hpp"

using namespace bdl::blender_importer;

bli_vector2::bli_vector2() : m_x(0), m_y(0) { }

bli_vector2::bli_vector2(float x, float y) : m_x(x), m_y(y) { }

bli_vector2::bli_vector2(const bli_vector2 & vector) : m_x(vector.x()), m_y(vector.y()) { }

bli_vector2 & bli_vector2::operator=(const bli_vector2 & vector)
{
	m_x = vector.x();
	m_y = vector.y();
	return *this;
}
