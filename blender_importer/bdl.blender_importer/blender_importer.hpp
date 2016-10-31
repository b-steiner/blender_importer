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

#include "model/texture.hpp"
#include "model/animation.hpp"

#include "util/bli_exception.hpp"
#include "util/bli_vector2.hpp"

#endif