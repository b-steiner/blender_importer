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

#ifndef BLI_BUILD
#	if _DEBUG
#		pragma comment(lib, "blender_importerD.lib")
#	else
#		pragma comment(lib, "blender_importer.lib")
#	endif
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <stdint.h>

#define BEGIN_BDL_BLI namespace bdl { namespace blender_importer {
#define END_BDL_BLI }; };

#ifdef BLI_BUILD
#define BLI_EXPORT _declspec(dllexport)
#else
#define BLI_EXPORT _declspec(dllimport)
#endif

#include "util\properties.hpp"

#ifndef BLI_BUILD

#include "importer/importer.hpp"

#include "model/animation.hpp"
#include "model/asset.hpp"
#include "model/light_source.hpp"
#include "model/material.hpp"
#include "model/mesh.hpp"
#include "model/node.hpp"
#include "model/texture.hpp"

#include "util/bli_exception.hpp"
#include "util/bli_vector2.hpp"
#include "util/bli_vector3.hpp"
#include "util/bli_vector4.hpp"
#include "util/bli_matrix4.hpp"

#endif