#include <BlubbEngine2_AssetIO\Importer\AIOBlenderImporter.hpp>

void main(int argc, char* argv[])
{
	AssetIO::AIOBlenderImporter importer;
	//importer.ToXML(argv[1], argv[2]);
	importer.Load(argv[1]);
	system("PAUSE");
}