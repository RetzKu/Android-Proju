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
#include <graphics/stb_image.h>

namespace engine
{

	TestApplication::TestApplication(Window* window, GraphicsSystem* graphics)
		: GraphicsApplication(window, graphics)
		, m_totalTime(0.0f)
	{

#ifdef defined(_WIN32)
		FileData = new Filemanager();
		GLuint boi = FileData->GetTexture("Untitled.png"); //tietokone version paikka textuuri haulle
#endif // defined(_WIN32)
	}


	TestApplication::~TestApplication()
	{
		delete FileData;
	}


	bool TestApplication::update(float deltaTime)
	{
		m_totalTime += deltaTime;
		return true;
	}

	void TestApplication::render(Window* window, GraphicsSystem* graphics)
	{
		(void)window;	
		float val = fabsf(sinf(2.0f*m_totalTime));
		// Clear screen with pulsating yellow
		
		//stbi_load_from_memory();

		graphics->clearScreen(val, val, 0.0f, true);
		// T�n pit�s saada se kuva ladattua, viel� pit�s mietti� et miten se piirret��n ruudulle :D

		// Swap buffers
		graphics->swapBuffers();
	}

}

