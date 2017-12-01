#pragma once

#include <string>
#include <fstream>
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
			// Menn��n filun loppuun
			// TODO error checki polkuun
			fseek(file, 0, SEEK_END);
			// Otetaan yl�s pituus
			unsigned long length = ftell(file);
			char* data = new char[length + 1];
			// Asetetaan data nollaksi
			memset(data, 0, length + 1);
			// Menn��n takasi filun alkuun ja otetaan datat talteen
			fseek(file, 0, SEEK_SET);
			fread(data, 1, length, file);
			fclose(file);

			// Datat stringiksi ja poistoon, resultit ulos funktiosta
			std::string result(data);
			delete[] data;
			return result;
		}
		static unsigned char* LoadTextureFromFile(const std::string Filename);

	};
}