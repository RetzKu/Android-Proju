#include <core/Filemanager.h>

Filemanager::~Filemanager()
{

}
#ifdef defined(_WIN32)


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

	return texture;
}
#endif // defined(_WIN32)