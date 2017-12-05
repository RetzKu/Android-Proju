#include "sprite.h"



namespace Engine { namespace Graphics {

		Sprite::Sprite(float x, float y, float width, float height, const Maths::vec4& color)
			: Renderable2D(Maths::vec3(x, y, 0), Maths::vec2(width, height), color)
		{		
			
		}

		Sprite::Sprite(float x, float y, float width, float height, Texture* texture)
			: Renderable2D(Maths::vec3(x, y, 0), Maths::vec2(width, height), Maths::vec4(1, 0, 1, 1))
		{
			_texture = texture;
		}

}}