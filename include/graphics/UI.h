#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <GLES2/gl2.h>
#include <graphics\stb_image.h>

enum UIEvents
{
	OK,
	CANCEL,
	TEXTFIELD,
	ESCAPE,
	YES,
	NO,
	CUSTOMEVENT,
};

typedef struct 
{
	int X;
	int Y;
}Rect;

class Texture
{
	Texture(std::string Filename);
	~Texture();
	void setsize(int x, int y) { _Rect.X = x; _Rect.Y = y; }
	void Bind(unsigned int unit);

#if defined(_WIN32)
private:
	std::string _Filename;
	GLuint _TextureID;
	Rect _Rect;
	int N;
#endif
};

struct UIObject
{
	UIObject(){}

#if defined(_WIN32)
private:
	Texture* _AttachedTexturePTR;
	std::vector<std::pair<UIEvents, std::string>> UIEventList;
#endif	
};

class UI
{
public:
	UI();
	~UI();

	GLuint CreateShaderProgram();

#if defined(_WIN32)
private:
	std::vector<UIObject> ActiveUIObjects;
	std::vector<UIObject> InactiveUIObjects;
#endif
};


