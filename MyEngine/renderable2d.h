#pragma once

#include "Maths.h"
#include "buffer.h"
#include "indexbuffer.h"
#include "vertexarray.h"
#include "Shader.h"

namespace Engine { namespace Graphics
	{
		struct VertexData
		{
			Maths::vec3 vertices;
			//Maths::vec4 color;
			unsigned int color;
		};


		class Renderable2D
		{
		protected:
			Maths::vec3 _position;
			Maths::vec2 _size;
			Maths::vec4 _color;

			

		public:
			Renderable2D(Maths::vec3 position, Maths::vec2 size, Maths::vec4 color)
				: _position(position), _size(size), _color(color)
			{ }

			virtual ~Renderable2D(){ }

			inline const Maths::vec3& getPosition() const { return _position; }
			inline const Maths::vec2& getSize() const { return _size; }
			inline const Maths::vec4& getColor() const { return _color; }

		};
}}