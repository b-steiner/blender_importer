#include <bdl.blender_importer/blender_importer.hpp>
#include "light_source.hpp"

using namespace bdl::blender_importer;

light_source::light_source(const std::string& name) : m_name(name), m_intensity(0), m_angle(0), m_angular_attenuation(0), m_color(0, 0, 0), m_attenuation(0, 0, 0),
m_type(light_source_type::directionallight), m_distance(0), m_has_clipped_sphere(false) { }

light_source::~light_source() { }