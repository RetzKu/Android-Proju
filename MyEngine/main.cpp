#include "window.h"
#include <windows.h>
#include <iostream>
#include "Maths.h"

// Jos haluat printit päälle, tässä 1, jos et valitse 0
#define DEBUG 1
#if DEBUG
	#define CONSOLE(x) std::cout << x
	#define CONSOLEND(x) std::cout << x << std::endl
#else
	#define CONSOLE(x)
	#define CONSOLEND(x)
#endif

#define SCREENWIDTH 1024
#define SCREENHEIGHT 720

using namespace Engine;
using namespace Graphics;
using namespace Maths;

void new_square(vec2 left_corner, vec2 right_corner);
vec2 get_relativeMSCoord(double x, double y);
double get_relative_width(double x);
double get_relavive_height(double y);
double get_correct_width(double x);
double get_correct_height(double y);

class box
{
public:
	box(int x, int y, int width, int height) { this->x = x;this->y = y;this->height = height;this->width = width; }
	void set_new_pos(int x, int y) { this->x = x; this->y = y; }
	void draw_box()
	{
		glBegin(GL_QUADS);
		glVertex2f(get_relative_width(x) + get_correct_width(width), get_relavive_height(y) + get_correct_height(height)); // 1,1
		glVertex2f(get_relative_width(x) + get_correct_width(width), get_relavive_height(y) - get_correct_height(height)); //1,-1
		glVertex2f(get_relative_width(x) - get_correct_width(width), get_relavive_height(y) - get_correct_height(height)); //-1,-1
		glVertex2f(get_relative_width(x) - get_correct_width(width), get_relavive_height(y) + get_correct_height(height)); //-1,1
		glEnd();
	}
	void ConvertToVertices(GLfloat* Vertices)
	{
		Vertices[0] = get_relative_width(x) + get_correct_width(width);
		Vertices[1] = get_relavive_height(y) + get_correct_height(height);
		Vertices[2] = 0;
		Vertices[3] = get_relative_width(x) + get_correct_width(width);
		Vertices[4] = get_relavive_height(y) - get_correct_height(height);
		Vertices[5] = 0;
		Vertices[6] = get_relative_width(x) - get_correct_width(width);
		Vertices[7] = get_relavive_height(y) - get_correct_height(height);
		Vertices[8] = 0;
		Vertices[9] = get_relative_width(x) - get_correct_width(width);
		Vertices[10] = get_relavive_height(y) + get_correct_height(height);
		Vertices[11] = 0;
	}
private:
	int x; //sijainti
	int y; //sijainti
	int width; //leveys
	int height; //korkeus
};

int main()
{
	//asetetaan ikkunan parametrit
	Window window("Engine", SCREENWIDTH, SCREENHEIGHT);
	//clearataan näyttö mustalla
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//gluint = unsigned int mutta koska eri platformeilla se on erikokoinen (16bit tai 64bit) niin käytetään GL-u-int:tiä
	GLuint vao;
	//luo tietyn määrän tietyn nimisiä alustamattomia objekteja (1 vao objekti) 
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
	box* tmp = new box(100, 100, 20, 80); //luodaan testi boxi (x,y,leveys,korkeus)
	GLfloat Vertices[12] = { 0 }; //tehdään testi vertices taulukko
	tmp->ConvertToVertices(Vertices); //muutetaan boxiin annetut sijainnit vertices taulukoksi
	vec4 c = a + b;

	mat4 position = mat4::translation(vec3(2, 3, 4));
	position *= mat4::identity();

	while (!window.closed())
	{
		window.clear();
		
#if 1
		double x, y;
		if (window.isKeyPressed(GLFW_KEY_S))
		{
			CONSOLEND("'S' pressed!");// katellaan jos S-nappia painetaan
			Sleep(350);
		}
		if (window.isButtonPressed(GLFW_MOUSE_BUTTON_1))
		{
			CONSOLEND("Mouse pressed!"); // katsotaan jos Klikkejä tulee
			Sleep(350);
		}
		window.GetMousePosition(x, y);
			CONSOLE("X: "); // Tulostaa mouse positionia
			CONSOLE(x);
			CONSOLE(" Y: ");
			CONSOLEND(y);
			Sleep(5);
#endif
#if 1
		tmp->set_new_pos(x, y); //nopea funkki joka osaa siirtää neliön paikkaa atm hiiressä
		tmp->draw_box(); //normi glvertex2f funktio
		
			//openGL neliön teko
		//glBegin(GL_QUADS);
		//glVertex2f(-0.5f, -0.5f);
		//glVertex2f(-0.5f,  0.5f);
		//glVertex2f( 0.5f,  0.5f);
		//glVertex2f(	0.5f, -0.5f);
		//glEnd();
#endif
		glDrawArrays(GL_ARRAY_BUFFER, 0, 6);
		window.update();
	}

	return 0;

}

// TAVARAT ALAPUOLELLA ON MIKAN TESTI PIIRTO HELPOKKEITA
void new_square(vec2 left_corner, vec2 right_corner) //voit antaa vasemman ylänurkan ja oikean alanurkan sijainnin niin tekee suoraan niihin neliön
{
	glBegin(GL_QUADS);
	glVertex2f(left_corner.x, left_corner.y);
	glVertex2f(left_corner.x, -left_corner.y);
	glVertex2f(right_corner.x, right_corner.y);
	glVertex2f(right_corner.x, -right_corner.y);
	glEnd();
}

vec2 get_relativeMSCoord(double x, double y) //palauttaa vec2 objektin joka on -1/1 väliltä
{
	float relative_x = x - SCREENWIDTH / 2;
	relative_x = relative_x / (SCREENWIDTH / 2);
	float relative_y = y - SCREENHEIGHT / 2;
	relative_y = relative_y/ (SCREENHEIGHT/ 2);
	return vec2(relative_x, relative_y);
}
double get_relative_width(double x) //osaatte varmaan päätellä
{
	float relative_x = x - SCREENWIDTH / 2;
	relative_x = relative_x / (SCREENWIDTH / 2);
	return relative_x;
}
double get_relavive_height(double y)//osaatte varmaan päätellä x2
{
	float relative_y = y - SCREENHEIGHT / 2;
	relative_y = relative_y/ (SCREENHEIGHT/ 2);
	return -relative_y;
}
double get_correct_width(double x)
{
	float tmp = SCREENWIDTH / 2;
	tmp = x/tmp;
	return tmp/2;
}
double get_correct_height(double y)
{
	float tmp = SCREENHEIGHT / 2;
	tmp = y / tmp;
	return tmp / 2;
}