#pragma once
#include <iostream>
#include <string>
#include <GLES2/gl2.h>
#include <graphics\stb_image.h>
#include <vector>
#include <map>

#if defined(ANDROID)
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif

class Filemanager
{
public:

#if defined(_WIN32)
    Filemanager(){}
	~Filemanager();

	GLuint GetTexture(std::string FileName);
	std::vector<std::pair<std::string, GLuint>> Textures;
#endif
#if defined(ANDROID)
    Filemanager(AAssetManager* ASmanager){_ASmanager = ASmanager;}
    ~Filemanager();
	AAsset* GetTexture(std::string Filename,int type);

private:
    AAssetManager* _ASmanager;
#endif // Win32
};
