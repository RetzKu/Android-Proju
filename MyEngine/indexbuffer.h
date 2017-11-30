#pragma once

#include <GL\glew.h>

namespace Engine { namespace Graphics {

		class IndexBuffer
		{
		private:
			GLuint _bufferID;
			GLuint _count;

		public:
			IndexBuffer(GLushort* data, GLsizei count);
			// oli GLushort, mutta ku pit�� saada paljo neli�it� ruutuun nii v�liaikasesti int
			IndexBuffer(GLuint* data, GLsizei count);
			~IndexBuffer();
			void bind() const;
			void unbind() const;

			inline GLuint getCount() const { return _count; }
		};


}}