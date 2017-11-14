#pragma once
#include <iostream>
#include <string>
#include <GLES2/gl2.h>
//include <android_native_app_glue.h>
//include <androidassetmanager.h>
#include <graphics\stb_image.h>

class Filemanager
{
public:
	Filemanager(){}
	~Filemanager();

	GLuint GetTexture(std::string FileName);
private:
	
};
