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

#define ASSERT(eq, error) if (!(eq)) { std::cout << error << std::endl; return false; }
#define EPS 0.0001

#include <bdl.blender_importer/blender_importer.hpp>

#include <Windows.h>
#include <vector>
#include <string>

using namespace std;
using namespace bdl;
using namespace bdl::blender_importer;

vector<string> get_all_files_names_within_folder(const string& folder);
bool hasEnding(std::string const &fullString, std::string const &ending);
bool check_assets(const std::string& path);
bool check_pointlight(node* lnode);
bool check_spotlight(node* lnode);
bool check_directionallight(node* lnode);
bool check_matrices(node* parent_node, node* child_node, node* point_node, node* spot_node, node* directional_node);
bool check_mesh(node* mnode);
bool check_color_material(node* mnode);
bool check_texture_material(node* mnode);

void main(int argc, char* argv[])
{
	vector<string> pathes = get_all_files_names_within_folder("..\\test_data\\");

	for (auto file : pathes)
	{
		if (hasEnding(file, ".blend"))
		{
			try
			{
				blender_importer::importer importer;
				if (importer.check_structure(std::string("..\\test_data\\") + file))
				{
					if (!check_assets(std::string("..\\test_data\\") + file));
						break;
				}
			}
			catch (blender_importer::bli_exception& excpt)
			{
				std::cout << excpt.message() << std::endl;
			}
		}
	}

	system("PAUSE");
}

wchar_t *convertCharArrayToLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}

vector<string> get_all_files_names_within_folder(const string& folder)
{
	vector<string> names;
	char search_path[200];
	sprintf(search_path, "%s*.*", folder.c_str());
	WIN32_FIND_DATA fd;
	wchar_t* wstr = convertCharArrayToLPCWSTR(search_path);

	HANDLE hFind = ::FindFirstFile(wstr, &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

				char buffer[256];
				char DefChar = ' ';
				WideCharToMultiByte(CP_ACP, 0, fd.cFileName, -1, buffer, 260, &DefChar, NULL);

				names.push_back(std::string(buffer));
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}

	delete[] wstr;

	return names;
}

bool hasEnding(std::string const &fullString, std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else {
		return false;
	}
}






node* node_by_name(const std::string& name, std::vector<node*> nodes)
{
	for (auto it = nodes.begin(); it != nodes.end(); it++)
	{
		if ((*it)->name() == name)
			return *it;
	}

	return nullptr;
}

bool eq(bli_vector4 vec1, bli_vector4 vec2)
{
	return (abs(vec1.x() - vec2.x()) <= EPS &&
			abs(vec1.y() - vec2.y()) <= EPS &&
			abs(vec1.z() - vec2.z()) <= EPS &&
			abs(vec1.w() - vec2.w()) <= EPS);
}
bool eq(bli_vector3 vec1, bli_vector3 vec2)
{
	return (abs(vec1.x() - vec2.x()) <= EPS &&
			abs(vec1.y() - vec2.y()) <= EPS &&
			abs(vec1.z() - vec2.z()) <= EPS);
}
bool eq(bli_vector2 vec1, bli_vector2 vec2)
{
	return (abs(vec1.x() - vec2.x()) <= EPS &&
			abs(vec1.y() - vec2.y()) <= EPS);
}
bool eq(bli_matrix4 m1, float m2[16])
{
	for (int i = 0; i < 16; i++)
		if (abs(m1.m()[i] - m2[i]) > EPS)
			return false;

	return true;
}
bool eq(float f1, float f2)
{
	return (abs(f1 - f2) < EPS);
}

bool check_assets(const std::string& path)
{
	bool succeed = true;

	blender_importer::importer importer;
	auto asset = importer.load(path);

	//Nodes
	auto node_parent_cube = node_by_name("parent_cube", asset->nodes());
	auto node_spotlight = node_by_name("spotlight", asset->nodes());
	auto node_pointlight = node_by_name("pointlight", asset->nodes());
	auto node_directional_light = node_by_name("directional_light", asset->nodes());

	ASSERT(node_parent_cube != nullptr, "Node parent_cube not found");
	ASSERT(node_spotlight != nullptr, "Node spotlight not found");
	ASSERT(node_pointlight != nullptr, "Node pointlight not found");
	ASSERT(node_directional_light != nullptr, "Node directional_light not found");
	ASSERT(asset->nodes().size() == 4, "Asset contains additional nodes");

	auto node_child_cube = node_by_name("child_cube", node_parent_cube->nodes());
	ASSERT(node_child_cube != nullptr, "Node child_cube not found");
	ASSERT(node_parent_cube->nodes().size() == 1, "node_parent_cube contains additional nodes");

	succeed &= check_pointlight(node_pointlight);
	succeed &= check_spotlight(node_spotlight);
	succeed &= check_directionallight(node_directional_light);

	succeed &= check_matrices(node_parent_cube, node_child_cube, node_pointlight, node_spotlight, node_directional_light);
	succeed &= check_mesh(node_parent_cube);

	succeed &= check_color_material(node_parent_cube);
	succeed &= check_texture_material(node_child_cube);

	delete asset;
	return succeed;
}
bool check_pointlight(node* lnode)
{
	ASSERT(lnode->lightsource() != nullptr, "Pointlight not present in node");

	auto l = lnode->lightsource();
	ASSERT(l->type() == light_source_type::pointlight, "Pointlight has wrong type");
	ASSERT(eq(l->attenuation(), bli_vector3(1.0f, 0.0f, 1.0f / 25.0f)), "Pointlight attenuation is wrong");
	ASSERT(eq(l->color(), bli_vector3(0.5f, 0.6f, 0.7f)), "Pointlight color is wrong");
	ASSERT(eq(l->distance(), 5.0f), "Pointlight distance is wrong");
	ASSERT(l->has_clipped_sphere() == true, "Pointlight has_clipped_sphere is wrong");
	ASSERT(eq(l->intensity(), 0.8f), "Pointlight intensity is wrong");
	ASSERT(l->name() == "Point", "Pointlight name is wrong");

	return true;
}
bool check_spotlight(node* lnode)
{
	ASSERT(lnode->lightsource() != nullptr, "Spotlight not present in node");

	auto l = lnode->lightsource();
	ASSERT(l->type() == light_source_type::spotlight, "Spotlight has wrong type");
	ASSERT(eq(l->attenuation(), bli_vector3(1.0f, 1.0f / 20.0f, 0.0f)), "Spotlight attenuation is wrong");
	ASSERT(eq(l->color(), bli_vector3(1.0f, 1.0f, 1.0f)), "Spotlight color is wrong");
	ASSERT(eq(l->distance() , 20.0f), "Spotlight distance is wrong");
	ASSERT(l->has_clipped_sphere() == false, "Spotlight distance is wrong");
	ASSERT(eq(l->intensity() , 1.0f), "Spotlight intensity is wrong");
	ASSERT(l->name() == "Spot", "Spotlight name is wrong");
	ASSERT(eq(l->angle() , 20.0f), "Spotlight angle is wrong");
	ASSERT(eq(l->angular_attenuation() , 0.15), "Spotlight angular_attenuation is wrong");
}
bool check_directionallight(node* lnode)
{
	ASSERT(lnode->lightsource() != nullptr, "Directional light not present in node");

	auto l = lnode->lightsource();
	ASSERT(l->type() == light_source_type::directionallight, "Directional light has wrong type");
	ASSERT(eq(l->color(), bli_vector3(1.0f, 0.2f, 1.0f)), "Directional light color is wrong");
	ASSERT(eq(l->intensity() , 0.8f), "Directional light intensity is wrong");
	ASSERT(l->name() == "Sun", "Directional light name is wrong");
}
bool check_matrices(node* parent_node, node* child_node, node* point_node, node* spot_node, node* directional_node)
{
	ASSERT(eq(parent_node->translation(), bli_vector3(0, 0, 0)), "parent_node translation is wrong");
	ASSERT(eq(parent_node->rotation(), bli_vector3(0, 0.78539f, 0)), "parent_node rotation is wrong");
	ASSERT(eq(parent_node->scale(), bli_vector3(1, 1, 1)), "parent_node scale is wrong");

	ASSERT(eq(child_node->translation(), bli_vector3(5, 0, 0)), "child_node translation is wrong");
	ASSERT(eq(child_node->rotation(), bli_vector3(0, 0, -0.78539f)), "child_node rotation is wrong");
	ASSERT(eq(child_node->scale(), bli_vector3(1, 2, 1)), "child_node scale is wrong");

	ASSERT(eq(point_node->translation(), bli_vector3(0, 0, 3)), "point_node translation is wrong");
	ASSERT(eq(point_node->rotation(), bli_vector3(0, 0, 0)), "point_node rotation is wrong");
	ASSERT(eq(point_node->scale(), bli_vector3(1, 1, 1)), "point_node scale is wrong");

	ASSERT(eq(spot_node->translation(), bli_vector3(-5, 0, -5)), "spot_node translation is wrong");
	ASSERT(eq(spot_node->rotation(), bli_vector3(-2.35619f, 0, 0)), "spot_node rotation is wrong");
	ASSERT(eq(spot_node->scale(), bli_vector3(1, 1, 1)), "spot_node scale is wrong");

	ASSERT(eq(directional_node->translation(), bli_vector3(0, 0, 10)), "directional_node translation is wrong");
	ASSERT(eq(directional_node->rotation(), bli_vector3(0, -0.78539f, 0)), "directional_node rotation is wrong");
	ASSERT(eq(directional_node->scale(), bli_vector3(1, 1, 1)), "directional_node scale is wrong");

	return true;
}
bool check_mesh(node* mnode)
{
	ASSERT(mnode->mesh() != nullptr, "Mesh not present in node");

	ASSERT(mnode->mesh()->positions().size() == 8, "Mesh contains wrong number of positions");
}

bool check_color_material(node* mnode)
{
	ASSERT(mnode->material() != nullptr, "Material not found");

	auto m = mnode->material();

	ASSERT(eq(m->ambient_intensity(), 1.0f), "Material: ambient_intensity is wrong");
	ASSERT(eq(m->diffuse_color(), bli_vector3(1, 0.5f, 0)), "Material: diffuse_color is wrong");

	ASSERT(eq(m->diffuse_intensity(), 0.8f), "Material: diffuse_color is wrong");
	ASSERT(eq(m->emitting_intensity(), 0.2f), "Material: emitting_intensity is wrong");
	ASSERT(m->hardness() == 50, "Material: hardness is wrong");
	ASSERT(m->mode() == material_mode::shadeless, "Material: mode is wrong");
	ASSERT(m->name() == "Material_Color", "Material: name is wrong");
	ASSERT(eq(m->opacity(), 1.0f), "Material: opacity is wrong");
	ASSERT(eq(m->specular_color(), bli_vector3(0, 1.0f, 0.5f)), "Material: specular_color is wrong");
	ASSERT(eq(m->specular_intensity(), 0.5f), "Material: specular_intensity is wrong");
	ASSERT(m->textures().size() == 0, "Material: textures is wrong");
}
bool check_texture_material(node* mnode)
{
	ASSERT(mnode->material() != nullptr, "Texture Material not found");

	auto m = mnode->material();

	ASSERT(eq(m->ambient_intensity(), 1.0f), "Texture Material: ambient_intensity is wrong");
	ASSERT(eq(m->diffuse_color(), bli_vector3(0.8f, 0.8f, 0.8f)), "Texture Material: diffuse_color is wrong");

	ASSERT(eq(m->diffuse_intensity(), 0.8f), "Texture Material: diffuse_color is wrong");
	ASSERT(eq(m->emitting_intensity(), 0.0f), "Texture Material: emitting_intensity is wrong");
	ASSERT(m->hardness() == 50, "Texture Material: hardness is wrong");
	ASSERT(m->mode() == material_mode::transparent, "Texture Material: mode is wrong");
	ASSERT(m->name() == "Material_Texture", "Texture Material: name is wrong");
	ASSERT(eq(m->opacity(), 0.5f), "Texture Material: opacity is wrong");
	ASSERT(eq(m->specular_color(), bli_vector3(1, 1, 1)), "Texture Material: specular_color is wrong");
	ASSERT(eq(m->specular_intensity(), 0.5f), "vMaterial: specular_intensity is wrong");

	ASSERT(m->textures().size() == 2, "Texture Material: textures is wrong");
	auto dt = m->textures()[mapping_target::diffuse];
	ASSERT(dt->color_space() == color_space::SRGB, "Diffuse texture colorspace is wrong");
	ASSERT(dt->path() == "/texture.png", "Diffuse texture path is wrong");

	auto nt = m->textures()[mapping_target::normals];
	ASSERT(nt->color_space() == color_space::SRGB, "Normal texture colorspace is wrong");
	ASSERT(nt->path() == "/OriginalFiles/normals.png", "Normal texture path is wrong");
}