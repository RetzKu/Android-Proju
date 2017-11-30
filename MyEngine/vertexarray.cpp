#include "vertexarray.h"


namespace Engine { namespace Graphics {
	
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &_arrayID);
	}
	
	VertexArray::~VertexArray()
	{
		for (int x = 0; x < _buffers.size(); x++)
		{
			delete _buffers[x];
		}
		glDeleteVertexArrays(1, &_arrayID);
	}
	
	void VertexArray::addBuffer(Buffer* buffer, GLuint index)
	{
		bind();
		buffer->bind();
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, buffer->getComponentCount(), GL_FLOAT, GL_FALSE, 0, 0);

		buffer->unbind();
		unbind();
	}

	void VertexArray::bind() const
	{
		glBindVertexArray(_arrayID);
	}

	void VertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

}}