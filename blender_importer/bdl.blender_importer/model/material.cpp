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
#include "material.hpp"

using namespace bdl::blender_importer;

material::material(const std::string& name) : m_name(name), m_diffuse_color(0.8f, 0.8f, 0.8f), m_diffuse_intensity(0.8f), m_specular_color(1.0f, 1.0f, 1.0f), m_specular_intensity(0.5f),
m_ambient_intensity(1.0f), m_emitting_intensity(0.0f), m_hardness(50), m_opacity(1.0f), m_mode(material_mode::none)
{ }

material::~material() { }