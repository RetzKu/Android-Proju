#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <GLES2/gl2.h>

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


class Texture
{
	Texture(){}
	~Texture(){}


#if defined(_WIN32)
private:

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


