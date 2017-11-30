#pragma once

#include "Maths.h"
#include "buffer.h"
#include "indexbuffer.h"
#include "vertexarray.h"
#include "Shader.h"

namespace Engine { namespace Graphics
	{
		class Renderable2D
		{
		protected:
			Maths::vec3 _position;
			Maths::vec2 _size;
			Maths::vec4 _color;

			VertexArray* _vertexArray;
			IndexBuffer* _indexBuffer;

			Shader& _shader;

		public:
			Renderable2D(Maths::vec3 position, Maths::vec2 size, Maths::vec4 color, Shader& shader)
				: _position(position), _size(size), _color(color), _shader(shader)
			{
				_vertexArray = new VertexArray();

				GLfloat vertices[] =
				{
						0,			0,			0,
						0,		 size.y,		0,
					  size.x,	 size.y,		0,
					  size.x,		0,			0
				};

				GLfloat colors[] =
				{
					color.x, color.y, color.z, color.w,
					color.x, color.y, color.z, color.w,
					color.x, color.y, color.z, color.w,
					color.x, color.y, color.z, color.w
				};

				_vertexArray->addBuffer(new Buffer(vertices, 4 * 3, 3), 0);
				_vertexArray->addBuffer(new Buffer(colors, 4 * 4, 4), 1);


				GLushort indices[] = { 0, 1, 2, 2, 3, 0};
				_indexBuffer = new IndexBuffer(indices, 6);
			}

			virtual ~Renderable2D()
			{
				delete _vertexArray;
				delete _indexBuffer;
			}

			inline const VertexArray* getVAO() const { return _vertexArray; }
			inline const IndexBuffer* getIBO() const { return _indexBuffer; }

			inline Shader& getShader() const { return _shader; }

			inline const Maths::vec3& getPosition() const { return _position; }
			inline const Maths::vec2& getSize() const { return _size; }
			inline const Maths::vec4& getColor() const { return _color; }

		};
}}