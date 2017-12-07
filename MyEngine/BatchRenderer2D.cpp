#include "BatchRenderer2D.h"
#include <../Dependencies/FreeType/freetype-gl/freetype-gl.h>

// T‰ss‰ filussa oleviin kommentteihin ‰lk‰‰ kajotko voi k‰ytt‰‰ myˆhemmin hyv‰ks et kattoo jos on PC nii k‰ytt‰‰ sit VAO:ta

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
		//glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
		//glEnableVertexAttribArray(SHADER_COLOR_INDEX);
		//glVertexAttribPointer(SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*) 0);
		//glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::color)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// oli GLushort, mutta ku pit‰‰ saada paljo neliˆit‰ ruutuun nii v‰liaikasesti int

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
		m_FTAtlas = ftgl::texture_atlas_new(512,512,1);
		m_FTFont = ftgl::texture_font_new_from_file(m_FTAtlas, 20, "arial.ttf");
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
		const std::vector<Maths::vec2>& uv = renderable->getUV();
		const GLuint tid = renderable->getTID();

		unsigned int c = 0;

		// ts = Texture slot
		float ts = 0.0f;
		// tid = Texture ID
		if(tid > 0)
		{
			// Defaulttina ei lˆytynyt
			bool found = false;
			// K‰yd‰‰n _textureSlotsit l‰pi jos lˆytyisi oikea tekstuuri
			for (int i = 0; i < _textureSlots.size(); i++)
			{
				// Jos tekstuuri lˆytyy
				if(_textureSlots[i] == tid)
				{
					// Texture slot talteen
					ts = (float)(i + 1);
					// Lˆydetty
					found = true;
					break;
				}
			}

			// Jos ei lˆytynyt, pit‰‰ ladata tekstuuri tekstuurislotteihin
			if(!found)
			{
				// Jos meill‰ loppuu tekstuurislotit kesken
				if(_textureSlots.size() >= 32)
				{
					// Piirret‰‰n kaikki mit‰ pystyttiin, nyt ei en‰‰ pystyt‰ koska
					// puuttuu tekstuuri, joten piirret‰‰n kaikki mit‰ oli bufferissa
					end();
					// Flush updatee texture slotit
					flush();
					begin();
				}

				// Laitetaan puuttuva tekstuuri viimesimp‰‰n slottiin
				_textureSlots.push_back(tid);
				// Ja p‰ivitet‰‰n ts uuteen slotti m‰‰r‰‰n
				ts = (float)_textureSlots.size();
			}
		}
		else
		{
			int r = color.x * 255.0f;
			int g = color.y * 255.0f;
			int b = color.z * 255.0f;
			int a = color.w * 255.0f;			

			c = a << 24 | b << 16 | g << 8 | r;
		}

		
		_buffer->vertex = *_transformationBack * position;
		_buffer->uv = uv[0];
		_buffer->tid = ts;
		_buffer->color = c;
		_buffer++;	

		_buffer->vertex = *_transformationBack * Maths::vec3(position.x, position.y + size.y, position.z);
		_buffer->uv = uv[1];
		_buffer->tid = ts;
		_buffer->color = c;
		_buffer++;

		_buffer->vertex = *_transformationBack * Maths::vec3(position.x + size.x, position.y + size.y, position.z);
		_buffer->uv = uv[2];
		_buffer->tid = ts;
		_buffer->color = c;
		_buffer++;

		_buffer->vertex = *_transformationBack * Maths::vec3(position.x + size.x, position.y, position.z);
		_buffer->uv = uv[3];
		_buffer->tid = ts;
		_buffer->color = c;
		_buffer++;

		_indexCount += 6;
	}

	void BatchRenderer2D::drawString(const std::string& text,const Maths::vec3& position,const Maths::vec4& color)
	{
		using namespace ftgl;

		float ts = 0.0f;
		bool found = false;
		for (int i = 0; i < _textureSlots.size(); i++)
		{
			if (_textureSlots[i] == m_FTAtlas->id)
			{
				ts = (float)(i + 1);
				found = true;
				break;
			}
		}
		if (!found)
		{
			if (_textureSlots.size() >= 32)
			{
				end();
				flush();
				begin();
			}
			_textureSlots.push_back(m_FTAtlas->id);
			ts = (float)(_textureSlots.size());
		}

		_buffer->vertex = Maths::vec3(-8,-8,0);
		_buffer->uv = Maths::vec2(0,1);
		_buffer->tid = ts;
		_buffer++;

		_buffer->vertex = Maths::vec3(-8, 8, 0);
		_buffer->uv = Maths::vec2(0,1);
		_buffer->tid = ts;
		_buffer++;

		_buffer->vertex = Maths::vec3(8, 8, 0);
		_buffer->uv = Maths::vec2(0,1);
		_buffer->tid = ts;
		_buffer++;

		_buffer->vertex = Maths::vec3(8, -8, 0);
		_buffer->uv = Maths::vec2(0, 1);
		_buffer->tid = ts;
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
		// Bindataan tekstuurit
		for(int i = 0; i < _textureSlots.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, _textureSlots[i]);
		}

		//glBindVertexArray(_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, _VBO);

		glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
		glEnableVertexAttribArray(SHADER_UV_INDEX);
		glEnableVertexAttribArray(SHADER_TID_INDEX);
		glEnableVertexAttribArray(SHADER_COLOR_INDEX);

		glVertexAttribPointer(SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)0);
		glVertexAttribPointer(SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::uv)));
		glVertexAttribPointer(SHADER_TID_INDEX, 1, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::tid)));
		glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::color)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		_IBO->bind();
		// oli GL_UNSIGNED_SHORT, mutta ku pit‰‰ saada paljo neliˆit‰ ruutuun nii v‰liaikasesti int
		glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, NULL);

		_IBO->unbind();

		//glBindVertexArray(0);

		_indexCount = 0;
	}
	

}}