#pragma once
#include <iostream>
#include <string>
#include <SDL\SDL.h>
#include <SDL\SDL_ttf.h>
#include "Renderer.h"

class Input
{
public:
	//Input(MainGame Game) { _Game = Game; }
	Input(SDL_Window* WindowObject, Renderer* RenderClass) { _WindowObject = WindowObject; _RenderClass = RenderClass;}
	~Input();

	/*Checks all functions inside this class. Put this in main gameloop*/
	void RunInput(SDL_Event* Event);
	void ProcessClick();
	SDL_Rect* MouseLoc();

	void SetMouseLoc(SDL_Rect MouseLoc) { _MouseLoc.x = MouseLoc.x; _MouseLoc.y = MouseLoc.y; }
	SDL_Rect GetMouseLoc() { return _MouseLoc; }
	void CoutCorrectKey(std::string t);
	std::string KeypadTranslate(std::string key);
	bool CheckIfCompleted();
	bool TaskFilled;
	
	Texture* TextureInfo()
	{
		if (sizeof(_RenderClass->TextureList) != 0)
		{
			for each(Texture* var in _RenderClass->TextureList)
			{
				if (var->Rect.x <= _MouseLoc.x && var->Rect.x + var->Rect.w >= _MouseLoc.x)
				{
					if (var->Rect.y <= _MouseLoc.y && var->Rect.y + var->Rect.h >= _MouseLoc.y)
					{
						std::cout << "\nFound at: x:" << var->Rect.x << " y:" << var->Rect.y;
						return var;
					}
				}
			}
			return nullptr;
		}
	}

	Texture* SelectedTexture;

private:
	SDL_Window* _WindowObject;
	SDL_Rect _MouseLoc;
	Renderer* _RenderClass;
	
};

