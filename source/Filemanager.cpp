#include <core/Filemanager.h>

#if defined(ANDROID)
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <cstdlib>
#endif

Filemanager::~Filemanager()
{

}


#if defined(_WIN32)


GLuint Filemanager::GetTexture(std::string FileName)
{
	GLuint texture;
	glGenTextures(1, &texture);
	int x, y;
	int n = 3;
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	unsigned char *data = stbi_load(FileName.c_str(), &x, &y, &n, 3);
	
	if (data != NULL)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
		//glUniform1i(glGetUniformLocation(shaderProgram, "texKitten"), 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//std::cout << "Kuvan lataus onnistui!" << std::endl;
	}
	else
	std::cout << FileName << " Kuvan lataaminen ei onnistunut!" << std::endl;
	auto tmp = std::make_pair(FileName, texture);
	Textures.push_back(tmp);
	return texture;
}
#endif // _WIN32

#if defined(ANDROID)
AAsset* Filemanager::GetTexture(std::string Filename, int type)
{
    AAsset* ASpointer = AAssetManager_open(_ASmanager,Filename.c_str(),AASSET_MODE_STREAMING);
    size_t lenght = AAsset_getLength(ASpointer);
    char* buffer = (char*)malloc(lenght+1);
    AAsset_read(ASpointer,buffer,lenght);
    if(ASpointer == 0) {
        return nullptr;
    }
    else
    {
        return ASpointer;
    }
}
#endif


