#include "sprite.h"



namespace Engine { namespace Graphics {

		Sprite::Sprite(float x, float y, float width, float height, const Maths::vec4& color)
			: Renderable2D(Maths::vec3(x, y, 0), Maths::vec2(width, height), color)
		{		
			
		}
}}