#include <bdl.blender_importer/blender_importer.hpp>
#include "texture.hpp"

using namespace bdl::blender_importer;

texture::texture() : m_path(""), m_color_space(color_space::other)
{ }
texture::~texture() { }