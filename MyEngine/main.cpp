#include "window.h"
#include <windows.h>
#include <iostream>
#include "Maths.h"

// Jos haluat printit p‰‰lle, t‰ss‰ 1, jos et valitse 0
#define DEBUG 1
#if DEBUG
	#define CONSOLE(x) std::cout << x
	#define CONSOLEND(x) std::cout << x << std::endl
#else
	#define CONSOLE(x)
	#define CONSOLEND(x)
#endif

int main()
{
	using namespace Engine;
	using namespace Graphics;
	using namespace Maths;
	//asetetaan ikkunan parametrit
	Window window("Engine", 1024, 720);
	//clearataan n‰yttˆ mustalla
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//gluint = unsigned int mutta koska eri platformeilla se on erikokoinen (16bit tai 64bit) niin k‰ytet‰‰n GL-u-int:ti‰
	GLuint vao;
	//luo tietyn m‰‰r‰n tietyn nimisi‰ alustamattomia objekteja (1 vao objekti) 
	glGenVertexArrays(1, &vao);
	//OpenGL sivuilta sitaatti:
	/*glBindVertexArray binds the vertex array object with name array.
	array is the name of a vertex array object previously returned from a call to glGenVertexArrays, or zero to break the existing vertex array object binding.
	If no vertex array object with name array exists, one is created when array is first bound.
	If the bind is successful no change is made to the state of the vertex array object, and any previous vertex array object binding is broken.
	*/
	glBindVertexArray(vao);

	//testattu vektoreiden toimintaa
	vec4 a(0.2f, 0.3f, 0.8f, 1.0f);
	vec4 b(0.5f, 0.2f, 0.1f, 1.0f);

	vec4 c = a + b;

	while (!window.closed())
	{
		window.clear();

		double x, y;
		if (window.isKeyPressed(GLFW_KEY_S))
		{
			CONSOLEND("'S' pressed!");// katellaan jos S-nappia painetaan
			Sleep(350);
		}
		if (window.isButtonPressed(GLFW_MOUSE_BUTTON_1))
		{
			CONSOLEND("Mouse pressed!"); // katsotaan jos Klikkej‰ tulee
			Sleep(350);
		}
		window.GetMousePosition(x, y);
			CONSOLE("X: "); // Tulostaa mouse positionia
			CONSOLE(x);
			CONSOLE(" Y: ");
			CONSOLEND(y);
			Sleep(50);

#if 1
			//openGL neliˆn teko
		glBegin(GL_QUADS);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(-0.5f,  0.5f);
		glVertex2f( 0.5f,  0.5f);
		glVertex2f(	0.5f, -0.5f);
		glEnd();
#endif
		glDrawArrays(GL_ARRAY_BUFFER, 0, 6);
		window.update();
	}

	return 0;

}