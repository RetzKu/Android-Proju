#include "window.h"
#include <iostream>

namespace Engine 
{
	namespace Graphics
	{
		//array johon voi laittaa nappeja
		bool Window::priv_Keys[MAX_KEYS];
		//array johon tulee hiiren napit &/ joystick
		bool Window::priv_Buttons[MAX_BUTTONS];
		//mouse pos
		double Window::priv_MouseX;
		double Window::priv_MouseY;

		void WindowResize(GLFWwindow *window, int width, int height);

		//ikkunan konstruktori
		Window::Window(const char *title, int width, int height)
		{
			priv_Title = title;
			priv_Width = width;
			priv_Height = height;
			if (!init())
			{
				glfwTerminate(); // tuhoaa ikkunan / dekonstruktori
			}
			for (int i = 0; i < MAX_KEYS; i++)
			{
				priv_Keys[1] = false;
			}
			for (int i = 0; i < MAX_BUTTONS; i++)
			{
				priv_Buttons[1] = false;
			}
		}
		// -||- dekonstruktori
		Window::~Window()
		{
			glfwTerminate();
		}
		// varsinainen ikkunan teko
		bool Window::init()
		{
			if (!glfwInit()) // GLFW init check
			{
				std::cout << "GLFW Init Error!" << std::endl;
				return false;
			}
			//kutsutaan ikkunaa
			priv_Window = glfwCreateWindow(priv_Width, priv_Height, priv_Title, NULL, NULL);
			//ikkuna check,
			if (!priv_Window)
			{
				std::cout << "Failed to create GLFW window!" << std::endl;
				return false;
			}
			//
			glfwMakeContextCurrent(priv_Window);
			glfwSetWindowUserPointer(priv_Window, this);
			glfwSetWindowSizeCallback(priv_Window, WindowResize);
			//input hommia
			glfwSetKeyCallback(priv_Window, key_callback);
			glfwSetMouseButtonCallback(priv_Window, button_callback);
			glfwSetCursorPosCallback(priv_Window, cursor_callback);
			
			//glew (OpenGL Extension Wrangler Library) init check
			//GLEW on libi joka mahdollistaa cross-platform extensionit CPP filuihin
				if (glewInit() != GLEW_OK)
				{
						std::cout << "Could not initialize GLEW!" << std::endl;
						return false;
				}
				//tulostaa openGL version, pitäisi tulla "4.5.0" jos on GLEW init onnistui
				std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
			return true;

		}
		//näppäimet
		bool Window::isKeyPressed(unsigned int keycode)
		{
			if (keycode >= MAX_KEYS)
			{
				return false;
				std::cout << "Keycode is out of range!" << std::endl;
			}
			return priv_Keys[keycode];
		}
		//napit -> hiiri/joystick
		bool Window::isButtonPressed(unsigned int button)
		{
			if (button >= MAX_BUTTONS)
			{
				return false;
				std::cout << "Buttons are out of range!" << std::endl;
			}
			return priv_Buttons[button];
		}

		//mouse pos
		void Window::GetMousePosition(double& x, double& y)
		{
			x = priv_MouseX;
			y = priv_MouseY;
		}

		// clear metodi
		void Window::clear() const
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		//update metodi
		void Window::update()
		{
			// Error check
			GLenum error = glGetError();
			if(error != GL_NO_ERROR)
			{
				std::cout << "OpenGL Error: " << error << std::endl;
			}

			glfwPollEvents();
			//glfwGetFramebufferSize(priv_Window, &priv_Width, &priv_Height);
			glfwSwapBuffers(priv_Window);
		}
		//window up check
		bool Window::closed() const
		{
			return glfwWindowShouldClose(priv_Window);
		}
		//keskittää objektit ja skaalaa niitä (tässä tapauksessa neliö)
		void WindowResize(GLFWwindow *window, int width, int height)
		{
			glViewport(0, 0, width, height); // (resize function) ** kokeile kommentoida rivi ja resize ikkunaa **
		}
		//Näppäimistö -> openGL:n callback funktio (osa inputtia)
		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Window* win = (Window*) glfwGetWindowUserPointer(window);
			win->priv_Keys[key] = action != GLFW_RELEASE;

		}	
		//Hiirennapit -> openGL:n callback funktio (osa inputtia)
		void button_callback(GLFWwindow* window, int button, int action, int mods)
		{
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			win->priv_Buttons[button] = action != GLFW_RELEASE;
		}
		//Cursor position -> openGL:n callback funktio (osa inputtia)
		void cursor_callback(GLFWwindow* window, double MouseX, double MouseY)
		{
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			win->priv_MouseX = MouseX;
			win->priv_MouseY = MouseY;
		}
	}
}