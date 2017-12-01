#define STB_IMAGE_IMPLEMENTATION
#include "fileutils.h"

using namespace Engine;

unsigned char* FileUtils::LoadTextureFromFile(const std::string FileName)
{
		int x, y;
		int n = 3;
		std::string FileDir = "../Assets/" + FileName;
		unsigned char *data = stbi_load(FileDir.c_str(), &x, &y, &n, 3);

		if (data == nullptr)
		{
			std::cout << "\nKuvan lataus epäonnistui <NULLPTR>";
			return nullptr;
		}
		else return data;
}