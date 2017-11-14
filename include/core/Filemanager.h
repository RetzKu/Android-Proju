#pragma once
#include <iostream>
#include <string>
#include <GLES2/gl2.h>
#include <graphics\stb_image.h>
#include <vector>
#include <map>

#if defined(ANDROID)
#include <android/asset_manager.h>
#endif

class Filemanager
{
public:
	Filemanager(){}
	~Filemanager();
#if defined(_WIN32)


	GLuint GetTexture(std::string FileName);
	std::vector<std::pair<std::string, GLuint>> Textures;
#endif

#if defined(ANDROID)
	AAsset* GetTexture(std::string Filename, int type);
#endif // Win32
};
