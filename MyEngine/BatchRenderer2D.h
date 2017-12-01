#pragma once

#include "renderer2d.h"
#include "indexbuffer.h"

namespace Engine { namespace Graphics {

#define RENDERER_MAX_SPRITES	60000
#define RENDERER_VERTEX_SIZE	sizeof(VertexData)
#define RENDERER_SPRITE_SIZE	RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE	RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE	RENDERER_MAX_SPRITES * 6
#define SHADER_VERTEX_INDEX		0
#define SHADER_COLOR_INDEX		1

	class BatchRenderer2D : public Renderer2D
	{
	private:
		GLuint _VAO;
		GLuint _VBO;
		IndexBuffer* _IBO;
		GLsizei _indexCount;
		VertexData* _buffer;
	public:
		BatchRenderer2D();
		~BatchRenderer2D();
		void begin();
		void submit(const Renderable2D* renderable);
		void end();
		void flush();
	private:
		void init();
	};

}}