#pragma once

#include "Maths.h"
#include "indexbuffer.h"
#include "vertexarray.h"
#include "renderer2d.h"
#include "Shader.h"
#include "texture.h"

namespace Engine { namespace Graphics
	{
		// Kama mitä nakataan shadereille
		struct VertexData
		{
			// Tekstuurin positio
			Maths::vec3 vertex;
			// Tekstuuri koordinaatit
			Maths::vec2 uv;
			// Tekstuuri ID
			float tid;
			// Tekstuurin väri
			unsigned int color;
		};


		class Renderable2D
		{
		protected:
			Maths::vec3 _position;
			Maths::vec2 _size;
			Maths::vec4 _color;
			std::vector<Maths::vec2> _UV;
			Texture* _texture;
		protected:
			Renderable2D() { setUVDefaults(); }
		public:
			Renderable2D(Maths::vec3 position, Maths::vec2 size, Maths::vec4 color)
				: _position(position), _size(size), _color(color)
			{ setUVDefaults(); }

			virtual ~Renderable2D(){ }

			virtual void submit(Renderer2D* renderer) const
			{
				renderer->submit(this);
			}

			inline const Maths::vec3& getPosition() const { return _position; }
			inline const Maths::vec2& getSize() const { return _size; }
			inline const Maths::vec4& getColor() const { return _color; }
			inline const std::vector<Maths::vec2>& getUV() const { return _UV; }

			inline const GLuint getTID() const { return _texture == nullptr ? 0 : _texture->getID(); }
		private:
			void setUVDefaults()
			{
				_UV.push_back(Maths::vec2(0, 0));
				_UV.push_back(Maths::vec2(0, 1));
				_UV.push_back(Maths::vec2(1, 1));
				_UV.push_back(Maths::vec2(1, 0));
			}
		};
}}