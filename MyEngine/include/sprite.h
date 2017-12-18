#pragma once

#include "renderable2d.h"

namespace Engine { namespace Graphics {

	class Sprite : public Renderable2D
	{
	private:

	public:
		Sprite(float x, float y, float width, float height, const Maths::vec4& color);
		Sprite(float x, float y, float width, float height, Texture* texture);
	
	};
}}