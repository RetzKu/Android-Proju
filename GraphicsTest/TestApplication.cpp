/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
/// "THE BEER-WARE LICENSE" (Revision 42):
/// <mikko.romppainen@kamk.fi> wrote this file.  As long as you retain this notice you
/// can do whatever you want with this stuff. If we meet some day, and you think
/// this stuff is worth it, you can buy me a beer in return. Mikko Romppainen.
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#include <TestApplication.h>

#include <graphics/GraphicsSystem.h>
#include <graphics/Window.h>
#include <math.h>
#include <GLES2/gl2.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <graphics\stb_image.h>

namespace engine
{

	TestApplication::TestApplication(Window* window, GraphicsSystem* graphics)
		: GraphicsApplication(window, graphics)
		, m_totalTime(0.0f)
	{
		
	}


	TestApplication::~TestApplication()
	{

	}


	bool TestApplication::update(float deltaTime)
	{
		m_totalTime += deltaTime;
		return true;
	}

	GLuint TestApplication::GetImage(std::string filename)
	{
		GLuint texture;
		glGenTextures(1, &texture);
		int x, y;
		int n = 3;
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		const char* FileName = filename.c_str();

		unsigned char *data = stbi_load(FileName, &x, &y, &n, 3);
		if (data != NULL)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			//glUniform1i(glGetUniformLocation(shaderProgram, "texKitten"), 0);



			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#if 0
			std::cout << "Kuvan lataus onnistui!" << std::endl;
#endif
		}
		else
#if 0
			std::cout << "Kuvan lataaminen ei onnistunut!" << std::endl;
#endif

		return texture;
	}

	void TestApplication::render(Window* window, GraphicsSystem* graphics)
	{
		(void)window;	
		float val = fabsf(sinf(2.0f*m_totalTime));
		// Clear screen with pulsating yellow
		
		//stbi_load_from_memory();

		graphics->clearScreen(val, val, 0.0f, true);

		// Tän pitäs saada se kuva ladattua, vielä pitäs miettiä et miten se piirretään ruudulle :D
		GetImage("Untitled.png");

		// Swap buffers
		graphics->swapBuffers();
	}

}

