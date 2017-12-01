#pragma once

#include <string>
#include <fstream>
#include <stbi_image.h>
#include <iostream>

namespace Engine {

	// Filun luku

	class FileUtils
	{
	public:
		static std::string read_file(const char* filepath)
		{
			// Mode rt, read text
			FILE* file = fopen(filepath, "rt");
			// Mennään filun loppuun
			// TODO error checki polkuun
			fseek(file, 0, SEEK_END);
			// Otetaan ylös pituus
			unsigned long length = ftell(file);
			char* data = new char[length + 1];
			// Asetetaan data nollaksi
			memset(data, 0, length + 1);
			// Mennään takasi filun alkuun ja otetaan datat talteen
			fseek(file, 0, SEEK_SET);
			fread(data, 1, length, file);
			fclose(file);

			// Datat stringiksi ja poistoon, resultit ulos funktiosta
			std::string result(data);
			delete[] data;
			return result;
		}
		static unsigned char* LoadTextureFromFile(std::string Filename)
		{
			int x, y;
			int n = 3;
			char* FileDir = "Assets";
			unsigned char *data = stbi_load("assets/asdsada.png",&x, &y, &n, 3);

			if (data == nullptr)
			{
				std::cout << "\nKuvan lataus epäonnistui <NULLPTR>";
			}
			else
				return data;
		}
	};
}