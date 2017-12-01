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
#include <thread>

using namespace Engine;
using namespace Graphics;
using namespace Maths;

#define SCREENWIDTH 960
#define SCREENHEIGHT 540

class TestClass
{
public:
	TestClass(Window* WindowObject,Shader* ShaderObject)
	{
		this->ShaderObject = ShaderObject;
		this->WindowObject = WindowObject;
		this->CameraCoordinates = vec2(0.0f, 0.0f);
		DefineAspectRatio();
	}
	void new_square(vec2 left_corner, vec2 right_corner);
	vec2 get_relativeMSCoord(double x, double y);
	double get_relative_width(double x);
	double get_relavive_height(double y);
	double get_correct_width(double x);
	double get_correct_height(double y);
	void GetCameraMovement();
	void SetUniforMat4(std::string UniformName, mat4 Ortho) { ShaderObject->setUniformMat4(UniformName.c_str(), Ortho); }
	double GetCameraSpeed() { return CameraSpeed; }
	void DefineAspectRatio()
	{
		float ratio = (float)SCREENHEIGHT / (float)SCREENWIDTH;
		if (ratio == 0.5625f)
		{
			this->AspectRatio.x = 16;
			this->AspectRatio.y = 9;
		}
		std::cout << "\nAspectRatio is " << this->AspectRatio.x << ":" << this->AspectRatio.y;
	}
private:
	Shader* ShaderObject;
	Window* WindowObject;
	vec2 CameraCoordinates;
	double CameraSpeed = 1;
	vec2 AspectRatio;
};

//TestClassin funktiot m‰‰ritelty alapuolella
void TestClass::new_square(vec2 left_corner, vec2 right_corner) //voit antaa vasemman yl‰nurkan ja oikean alanurkan sijainnin niin tekee suoraan niihin neliˆn
{
	glBegin(GL_QUADS);
	glVertex2f(left_corner.x, left_corner.y);
	glVertex2f(left_corner.x, -left_corner.y);
	glVertex2f(right_corner.x, right_corner.y);
	glVertex2f(right_corner.x, -right_corner.y);
	glEnd();
}
vec2 TestClass::get_relativeMSCoord(double x, double y) //palauttaa vec2 objektin joka on -1/1 v‰lilt‰
{
	float relative_x = x - SCREENWIDTH / 2;
	relative_x = relative_x / (SCREENWIDTH / 2);
	float relative_y = y - SCREENHEIGHT / 2;
	relative_y = relative_y / (SCREENHEIGHT / 2);
	return vec2(relative_x, relative_y);
}
double TestClass::get_relative_width(double x) //osaatte varmaan p‰‰tell‰
{
	float relative_x = x - SCREENWIDTH / 2;
	relative_x = relative_x / (SCREENWIDTH / 2);
	return relative_x;
}
double TestClass::get_relavive_height(double y)//osaatte varmaan p‰‰tell‰ x2
{
	float relative_y = y - SCREENHEIGHT / 2;
	relative_y = relative_y / (SCREENHEIGHT / 2);
	return -relative_y;
}
double TestClass::get_correct_width(double x)
{
	float tmp = SCREENWIDTH / 2;
	tmp = x / tmp;
	return tmp / 2;
}
double TestClass::get_correct_height(double y)
{
	float tmp = SCREENHEIGHT / 2;
	tmp = y / tmp;
	return tmp / 2;
}
void TestClass::GetCameraMovement()
{
	for (int i = 60; i < 95; i++)
	{
		if (WindowObject->isKeyPressed(i))
		{
			switch (i)
			{
			case GLFW_KEY_A:
				CameraCoordinates.x -= 0.1*CameraSpeed;
				break;
			case GLFW_KEY_W:
				CameraCoordinates.y += 0.1*CameraSpeed;
				break;
			case GLFW_KEY_D:
				CameraCoordinates.x += 0.1*CameraSpeed;
				break;
			case GLFW_KEY_S:
				CameraCoordinates.y -= 0.1*CameraSpeed;
				break;
			case GLFW_KEY_Q:
				CameraSpeed += 0.1f;
				break;
			case GLFW_KEY_E:
				if (CameraSpeed > 0) { CameraSpeed -= 0.1f; }
				break;
			default:
				break;
			}
		}
	}
	mat4 ortho = mat4::orthographic(CameraCoordinates.x, AspectRatio.x+ CameraCoordinates.x, CameraCoordinates.y, AspectRatio.y + CameraCoordinates.y, -1.0f, 1.0f);
	SetUniforMat4("pr_matrix", ortho);
}