#include "BatchRenderer2D.h"

// T�ss� filussa oleviin kommentteihin �lk�� kajotko voi k�ytt�� my�hemmin hyv�ks et kattoo jos on PC nii k�ytt�� sit VAO:ta


namespace Engine { namespace Graphics {

	BatchRenderer2D::BatchRenderer2D()
	{
		init();
	}

	BatchRenderer2D::~BatchRenderer2D()
	{
		delete _IBO;
		glDeleteBuffers(1, &_VBO);
	}

	void BatchRenderer2D::init()
	{
		//glGenVertexArrays(1, &_VAO);
		glGenBuffers(1, &_VBO);
		//glBindVertexArray(_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
		glEnableVertexAttribArray(SHADER_COLOR_INDEX);
		glVertexAttribPointer(SHADER_VERTEX_INDEX,	3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*) 0);
		glVertexAttribPointer(SHADER_COLOR_INDEX,	4, GL_UNSIGNED_BYTE, GL_TRUE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::color)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// oli GLushort, mutta ku pit�� saada paljo neli�it� ruutuun nii v�liaikasesti int

		GLuint* indices = new GLuint[RENDERER_INDICES_SIZE];

		int offset = 0;
		for(int i = 0; i < RENDERER_INDICES_SIZE;  i+= 6)
		{
			indices[  i  ] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		_IBO = new IndexBuffer(indices, RENDERER_INDICES_SIZE);
		//glBindVertexArray(0);

		_bufferStart = _buffer = new VertexData[RENDERER_MAX_SPRITES * 4];
	}

	void BatchRenderer2D::begin()
	{
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		//_buffer = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		_buffer = _bufferStart;
	}

	void BatchRenderer2D::submit(const Renderable2D* renderable)
	{
		const Maths::vec3& position = renderable->getPosition();
		const Maths::vec2& size = renderable->getSize();
		const Maths::vec4& color = renderable->getColor();

		int r = color.x * 255.0f;
		int g = color.y * 255.0f;
		int b = color.z * 255.0f;
		int a = color.w * 255.0f;

		unsigned int c = a << 24 | b << 16 | g << 8 | r;
		
		_buffer->vertices = position;
		_buffer->color = c;
		_buffer++;	

		_buffer->vertices = Maths::vec3(position.x, position.y + size.y, position.z);
		_buffer->color = c;
		_buffer++;

		_buffer->vertices = Maths::vec3(position.x + size.x, position.y + size.y, position.z);
		_buffer->color = c;
		_buffer++;

		_buffer->vertices = Maths::vec3(position.x + size.x, position.y, position.z);
		_buffer->color = c;
		_buffer++;

		_indexCount += 6;
	}
	
	void BatchRenderer2D::end()
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, RENDERER_MAX_SPRITES * RENDERER_SPRITE_SIZE, _bufferStart);
		//glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void BatchRenderer2D::flush()
	{
		//glBindVertexArray(_VAO);

		_IBO->bind();
		// oli GL_UNSIGNED_SHORT, mutta ku pit�� saada paljo neli�it� ruutuun nii v�liaikasesti int
		glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, NULL);

		_IBO->unbind();
		glBindVertexArray(0);

		_indexCount = 0;
	}
	

}}