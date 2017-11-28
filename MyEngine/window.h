#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

namespace Engine 
{
	namespace Graphics
	{
#define MAX_KEYS 1024 // just in case some chinese has 1024 key keyboard
#define MAX_BUTTONS 32 // same for mouse. too big array but just in case.
		class Window
		{
		private:
			//osaa varmaan jokanen lukea.
			const char *priv_Title;
			int priv_Width, priv_Height;
			GLFWwindow *priv_Window;
			bool priv_Closed; // onko ikkuna auki vai ei

			static bool priv_Keys[MAX_KEYS];
			static bool priv_Buttons[MAX_BUTTONS];
			static double priv_MouseX, priv_MouseY;

			bool init();
			friend static void key_callback(GLFWwindow* Window, int key, int scancode, int action, int mods);
			friend static void button_callback(GLFWwindow* Window, int button, int action, int mods);
			friend static void cursor_callback(GLFWwindow* Window, double MouseX, double MouseY);

		public:
			Window(const char *title, int width, int height);
			~Window();
			void clear() const;
			void update();
			bool closed() const;

			inline int getWidth() const { return priv_Width; } //ei kutsuta mit‰‰n funktiota vaan tehd‰‰n jokin toimenpide. esim t‰ss‰ return width;
			inline int getHeight() const { return priv_Height; } // return height;

			static bool isKeyPressed(unsigned int keycode);
			static bool isButtonPressed(unsigned int button);
			static void GetMousePosition(double& x, double& y);


		};
	}
}

#endif