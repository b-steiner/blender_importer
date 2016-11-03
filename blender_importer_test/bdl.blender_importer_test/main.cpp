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

#include <Windows.h>
#include <vector>
#include <string>

using namespace std;
using namespace bdl;

vector<string> get_all_files_names_within_folder(const string& folder);
bool hasEnding(std::string const &fullString, std::string const &ending);

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
					auto asset = importer.load(std::string("..\\test_data\\") + file);
					delete asset;
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