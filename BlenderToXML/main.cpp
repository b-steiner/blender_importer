#include <BlubbEngine2_AssetIO\AssetIO.hpp>

#include <Windows.h>
#include <vector>
#include <string>

using namespace std;
using namespace AssetIO;

vector<string> get_all_files_names_within_folder(const string& folder);
bool hasEnding(std::string const &fullString, std::string const &ending);

void main(int argc, char* argv[])
{
	AIOBlenderImporter importer;
	//importer.ToXML(argv[1], argv[2]);
	//importer.CheckStructure(argv[1]);
	auto asset = importer.Load(argv[1]);

	/*vector<string> pathes = get_all_files_names_within_folder("..\\TestData\\");

	for (auto file : pathes)
	{
		if (hasEnding(file, ".blend"))
		{
			try
			{
				AIOBlenderImporter importer;
				if (importer.CheckStructure(std::string("..\\TestData\\") + file))
				{
					auto asset = importer.Load(std::string("..\\TestData\\") + file);
					delete asset;
				}
			}
			catch (AIOException& excpt)
			{
				std::cout << excpt.Message() << std::endl;
			}
		}
	}

	system("PAUSE");*/
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