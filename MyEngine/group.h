#pragma once


#include "renderable2d.h"


namespace Engine { namespace Graphics {
	

	class Group : public Renderable2D
	{
	private:
		std::vector<Renderable2D*> _renderables;
		Maths::mat4 _transformationMatrix;
	public:
		Group(const Maths::mat4& tranform);
		~Group();
		void add(Renderable2D* renderable);
		void submit(Renderer2D* renderer) const override;
	};
	
}}