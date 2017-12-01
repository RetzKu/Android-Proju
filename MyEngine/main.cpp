#include "window.h"
#include <windows.h>
#include <iostream>
#include "Maths.h"
#include "fileutils.h"
#include "Shader.h"
#include "buffer.h"
#include "indexbuffer.h"
#include "vertexarray.h"
#include "renderer2d.h"
#include "renderable2d.h"
#include "simple2drenderer.h"
#include "static_sprite.h"
#include "BatchRenderer2D.h"
#include "sprite.h"
#include <time.h>
#include "timer.h"

#define SCREENWIDTH 960
#define SCREENHEIGHT 540

using namespace Engine;
using namespace Graphics;
using namespace Maths;

// Jos haluat printit päälle, tässä 1, jos et valitse 0
#define DEBUG 1
#if DEBUG
	#define CONSOLE(x) std::cout << x
	#define CONSOLEND(x) std::cout << x << std::endl
#else
	#define CONSOLE(x)
	#define CONSOLEND(x)
#endif

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
	// Asetetaan ikkunan parametrit
	Window window("Engine", SCREENWIDTH, SCREENHEIGHT);
	// Tausta väri
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Luodaan matriisi
	mat4 ortho = mat4::orthographic(0.0f, 16.0f, 0.0f, 9.0f, -1.0f, 1.0f);

	Shader shader("basic.vert", "basic.frag");
	shader.enable();
	// Heitetään matriisi shaderille
	shader.setUniformMat4("pr_matrix", ortho);
	// Vaihdetaan kuvion paikkaa, 0 0 0 vasen alareuna
	//shader.setUniformMat4("ml_matrix", mat4::translation(vec3(4, 3, 0)));

	// Esimerkki rotatesta, 45 astetta z suunnassa
	//shader.setUniformMat4("ml_matrix", mat4::rotation(45.0f, vec3(0, 0, 1)));
	
	// Luodaan uusia spritejä
	// Määritellään paikka, koko, väri ja shader
	//shader.setUniformMat4("ml_matrix", mat4::rotation(15.0f, vec3(0, 0, 1)));

	std::vector<Renderable2D*> sprites;

	srand(time(NULL));


	Sprite sprite(5, 5, 4, 4, Maths::vec4(1, 0, 1, 1));
	Sprite sprite3(7, 1, 7, 2, Maths::vec4(0.2f, 0, 1, 1));
	BatchRenderer2D renderer;

	for (float y = 0; y < 9.0f; y += 0.05f)
	{
		for(float x = 0; x < 16.0f; x += 0.05f)
		{
			sprites.push_back(new Sprite(x, y, 0.04f, 0.04f, Maths::vec4(rand() % 1000 / 1000.0f, 0, 1, 1)));
		}
	}


	// Valot shaderiin, 0.0f 0.0f on vasen alareuna
	// Jos objekti on vaikka 4.0f 2.0f kokoinen, luonnollisesti valo keskellä 2.0f 1.0f
	//shader.setUniformMat2f("light_pos", vec2(4.0f, 2.0f));
	// Shaderin värin vaihto
	//shader.setUniformMat4f("colour", vec4(0.2f, 0.3f, 0.8f, 1.0f));

	// Jatkuva aika
	Timer time;
	float timer = 0.0f;
	unsigned int frames = 0;

	printf("Sprites: %d\n", sprites.size());


	while (!window.closed())
	{
		window.clear();
		double x, y;
		window.GetMousePosition(x, y);
		shader.setUniformMat2f("light_pos", vec2((float)(x * 16.0f / 960.0f), (float)(9.0f - y * 9.0f / 540.0f)));
		// Sanotaan renderille että alkaa töihi
		// Submit laittaa ne spritet jonoon ja flush sitte tyhjentää jonon samalla ku nakkaa kamaa ruudulle
		renderer.begin();
		for(int i = 0; i < sprites.size(); i++)
		{
			renderer.submit(sprites[i]);
		}
		renderer.end();
		renderer.flush();


		window.update();
		frames++;
		if(time.elapsed() - timer > 1.0f)
		{
			timer += 1.0f;
			printf("%d FPS\n", frames);
			frames = 0;
		}
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

// Piti siivota mainia :)

// OpenGL piirtoa, mutta meh, halutaan omat bufferit
//#if 0
//
//GLfloat vertices[] =
//{
//	0,  0,  0,
//	8,  0,  0,
//	0,  3,  0,
//	0,  3,  0,
//	8,  3,  0,
//	8,  0,  0
//};
//
//GLuint vbo;
//glGenBuffers(1, &vbo);
//glBindBuffer(GL_ARRAY_BUFFER, vbo);
//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//glEnableVertexAttribArray(0);
//
//#else // Custom bufferihommia
//
//// Pisteet mitä piirretään
//GLfloat vertices[] =
//{
//	0,  0,  0,
//	0,  3,  0,
//	8,  3,  0,
//	8,  0,  0
//};
//
//// Piirretään neliö kellon suuntaan
//GLushort indices[] =
//{
//	0, 1, 2,
//	2, 3, 0
//};
//
//// Neliön väri
//GLfloat colorsA[] =
//{
//	1, 0, 1, 1,
//	1, 0, 1, 1,
//	1, 0, 1, 1,
//	1, 0, 1, 1
//};
//
//// Neliön väri
//GLfloat colorsB[] =
//{
//	0.2f, 0.3f, 0.8f, 1,
//	0.2f, 0.3f, 0.8f, 1,
//	0.2f, 0.3f, 0.8f, 1,
//	0.2f, 0.3f, 0.8f, 1
//};
//
//VertexArray sprite1;
//VertexArray sprite2;
//IndexBuffer ibo(indices, 6);
//
//// Kaikille pitää tehä omat bufferit koska muuten ne poistaa toisilta bufferit
//sprite1.addBuffer(new Buffer(vertices, 4 * 3, 3), 0);
//sprite1.addBuffer(new Buffer(colorsA, 4 * 4, 4), 1);
//sprite2.addBuffer(new Buffer(vertices, 4 * 3, 3), 0);
//sprite2.addBuffer(new Buffer(colorsB, 4 * 4, 4), 1);
//
//#endif

// EP 7 koodia poistoon alta pois
//#if 0
//glDrawArrays(GL_TRIANGLES, 0, 6);
//#else
//// Bindataan sprite ja index bufferi
//sprite1.bind();
//ibo.bind();
//// Siirretään neliötä
//shader.setUniformMat4("ml_matrix", mat4::translation(vec3(4, 3, 0)));
//// Piirretään neliö
//glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_SHORT, 0);
//// Unbindataan molemmat
//ibo.unbind();
//sprite1.unbind();
//
//// Samat jutut toiselle neliölle
//sprite2.bind();
//ibo.bind();
//shader.setUniformMat4("ml_matrix", mat4::translation(vec3(0, 0, 0)));
//glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_SHORT, 0);
//ibo.unbind();
//sprite2.unbind();
//#endif

//gluint = unsigned int mutta koska eri platformeilla se on erikokoinen (16bit tai 64bit) niin käytetään GL-u-int:tiä
//GLuint vao;
//luo tietyn määrän tietyn nimisiä alustamattomia objekteja (1 vao objekti) 
//glGenVertexArrays(1, &vao);
//OpenGL sivuilta sitaatti:
/*glBindVertexArray binds the vertex array object with name array.
array is the name of a vertex array object previously returned from a call to glGenVertexArrays, or zero to break the existing vertex array object binding.
If no vertex array object with name array exists, one is created when array is first bound.
If the bind is successful no change is made to the state of the vertex array object, and any previous vertex array object binding is broken.
*/
//glBindVertexArray(vao);

//////// INPUT
/*

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
Sleep(50);

*/


////////// NELIÖ
/*


#if 1
//openGL neliön teko
glBegin(GL_QUADS);
glVertex2f(-0.5f, -0.5f);
glVertex2f(-0.5f,  0.5f);
glVertex2f( 0.5f,  0.5f);
glVertex2f(	0.5f, -0.5f);
glEnd();
#endif


*/