#include "group.h"


namespace Engine { namespace Graphics {


	Group::Group(const Maths::mat4& transform)
		: _transformationMatrix(transform)
	{		
	}
	
	Group::~Group()
	{
		for (int i = 0; i < _renderables.size(); i++)
		{
			delete _renderables[i];
		}
	}

	void Group::add(Renderable2D* renderable)
	{
		_renderables.push_back(renderable);
	}

	void Group::submit(Renderer2D* renderer) const
	{
		renderer->push(_transformationMatrix);
		for(const Renderable2D* renderable : _renderables)
		{
			renderable->submit(renderer);
		}
		renderer->pop();
	}
}}